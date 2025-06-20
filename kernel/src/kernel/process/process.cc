#include "process/process.hh"
#include "error.h"

#include <memory>


namespace kernel {
namespace proc {


kernel::proc::Process::Process(int pid, int priority, int pc, int sp, int base, int limit, std::size_t stackSize) {
    this->_process_control_block =
      std::make_unique<kernel::proc::PCB>(kernel::proc::PCB(pid, priority, pc, sp, base, limit, stackSize));
    this->_page_table._entries.clear();  // Initialize the page table
}

kernel::proc::Process::Process(kernel::proc::Process&& process) :
    _process_control_block(std::move(process._process_control_block)),
    _page_table(std::move(process._page_table)) {}

kernel::proc::Process::Process(const kernel::proc::Process& process) {
    if (process._process_control_block)
        this->_process_control_block = std::make_unique<kernel::proc::PCB>(*process._process_control_block);

    this->_page_table = process._page_table;  // Assumes PageTable and its entries support deep copy
}

kernel::proc::Process& kernel::proc::Process::operator=(const kernel::proc::Process& other) {
    if (this == &other)
        return *this;

    // Deep copy PCB
    if (other._process_control_block)
        this->_process_control_block = std::make_unique<kernel::proc::PCB>(*other._process_control_block);
    else
        this->_process_control_block.reset();

    // Deep copy page table
    this->_page_table = other._page_table;

    return *this;
}

void kernel::proc::Process::write_to_virtual_page(kernel::memo::PageID vpage, int offset, char value) {
    // check if the virtual page is mapped
    if (this->_page_table._entries.find(vpage) == this->_page_table._entries.end())
        throw kernel::error::Page_fault("virtual page not mapped");

    // copy the page table entry metadata
    kernel::memo::PageTableEntry& entry     = this->_page_table._entries[vpage];
    kernel::memo::PageID          phpage_id = entry._physical_page;

    // Check if page is writable
    bool is_writable = entry._flag & kernel::memo::PageFlags::WRITE;
    bool is_cow      = entry._flag & kernel::memo::PageFlags::COW;

    if (is_writable == false)
    {
        if (is_cow == true)
        {
            // Perform Copy-On-Write
            kernel::memo::PhysicalPage& shared_page = kernel::memo::memory_manager.get_page(phpage_id);

            // Allocate new page and copy data
            kernel::memo::PageID        new_phpage_id = kernel::memo::memory_manager.allocate_physical_page();
            kernel::memo::PhysicalPage& new_page      = kernel::memo::memory_manager.get_page(new_phpage_id);

            // Copy data from shared page to new page copy
            kernel::memo::memory_manager.increment_ref(new_phpage_id);
            new_page._data = std::make_unique<char[]>(kernel::memo::PAGE_SIZE);
            std::memcpy(new_page._data.get(), shared_page._data.get(), kernel::memo::PAGE_SIZE);
            new_page._data = std::move(new_page._data);

            // Update ref counts
            kernel::memo::memory_manager.decrement_ref(phpage_id);
            entry._physical_page = new_phpage_id;

            // Update flags
            entry._flag &= ~kernel::memo::PageFlags::COW;
            entry._flag |= kernel::memo::PageFlags::WRITE;

            std::cout << "COW triggered: Process " << this->_process_control_block->getPID()._value << " now owns page "
                      << new_phpage_id << "\n";
        }
        else
        {
            throw kernel::error::Segmentation_fault("Write access denied on page");
        }
    }

    // Write to physical memory
    kernel::memo::PhysicalPage& page = kernel::memo::memory_manager.get_page(entry._physical_page);
    if (offset < 0 || offset >= kernel::memo::PAGE_SIZE)
        throw kernel::error::Segmentation_fault("Offset out of bounds");

    page._data[offset] = value;
}

char kernel::proc::Process::read_from_virtual_page(kernel::memo::PageID vpage, int offset) {
    if (this->_page_table._entries.find(vpage) == this->_page_table._entries.end())
        throw kernel::error::Page_fault("virtual page not mapped");

    kernel::memo::PageTableEntry& entry = this->_page_table._entries[vpage];
    kernel::memo::PhysicalPage&   page  = kernel::memo::memory_manager.get_page(entry._physical_page);

    if (!(entry._flag & memo::PageFlags::READ))
        throw kernel::error::Page_fault("virtual page not readable");

    if (offset < 0 || offset >= memo::PAGE_SIZE)
        throw kernel::error::Segmentation_fault("Offset out of bounds");

    return page._data[offset];
}

std::unique_ptr<kernel::proc::Process> kernel::proc::Process::fork_process(PID child_pid) {
    auto child =
      std::make_unique<kernel::proc::Process>(/* possibly pass child PID and metadata */ kernel::proc::Process());

    // Manually copy deep data from `*this` into *child
    child->_process_control_block = std::make_unique<kernel::proc::PCB>(*this->_process_control_block);

    for (const auto& [vpage, entry] : this->_page_table._entries)
    {
        kernel::memo::memory_manager.increment_ref(entry._physical_page);
        kernel::memo::PageTableEntry new_entry = entry;
        new_entry._flag &= ~kernel::memo::PageFlags::WRITE;
        new_entry._flag |= kernel::memo::PageFlags::COW;
        child->_page_table._entries[vpage] = new_entry;
    }

    return child;
}

void kernel::proc::Process::map_virtual_page(kernel::memo::PageID vpage, kernel::memo::PageID php_id) {
    if (this->_page_table._entries.find(vpage) != this->_page_table._entries.end())
        throw kernel::error::Page_fault("virtual page already mapped");

    kernel::memo::PageTableEntry entry;
    entry._physical_page = php_id;
    entry._flag          = kernel::memo::PageFlags::READ | kernel::memo::PageFlags::WRITE;

    this->_page_table._entries[vpage] = entry;
}


}  // namespace proc
}  // namespace kernel