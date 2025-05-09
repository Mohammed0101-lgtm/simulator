#include "process.h"
#include "../error.h"

#include <memory>


namespace kernel {
namespace proc {


explicit kernel::proc::Process::Process(
  int pid, int priority, int pc, int sp, int base, int limit, std::size_t stackSize) {
    process_control_block =
      std::make_unique<kernel::proc::PCB>(kernel::proc::PCB(pid, priority, pc, sp, base, limit, stackSize));
    page_table.entries.clear();  // Initialize the page table
}

explicit kernel::proc::Process::Process(kernel::proc::Process&& process) :
    process_control_block(std::move(process.process_control_block)),
    page_table(std::move(process.page_table)) {}

explicit kernel::proc::Process::Process(const kernel::proc::Process& process) {
    if (process.process_control_block)
        process_control_block = std::make_unique<kernel::proc::PCB>(*process.process_control_block);

    page_table = process.page_table;  // Assumes PageTable and its entries support deep copy
}

kernel::proc::Process& kernel::proc::Process::operator=(const kernel::proc::Process& other) {
    if (this == &other)
        return *this;

    // Deep copy PCB
    if (other.process_control_block)
        process_control_block = std::make_unique<kernel::proc::PCB>(*other.process_control_block);
    else
        process_control_block.reset();

    // Deep copy page table
    page_table = other.page_table;

    return *this;
}


void kernel::proc::Process::write_to_virtual_page(kernel::memo::PageID vpage, int offset, char value) {
    // check if the virtual page is mapped
    if (this->page_table.entries.find(vpage) == this->page_table.entries.end())
        throw kernel::error::page_fault("virtual page not mapped");

    // copy the page table entry metadata
    kernel::memo::PageTableEntry& entry     = this->page_table.entries[vpage];
    kernel::memo::PageID          phpage_id = entry.physical_page;

    // Check if page is writable
    bool is_writable = entry.flag & kernel::memo::PageFlags::WRITE;
    bool is_cow      = entry.flag & kernel::memo::PageFlags::COW;

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
            new_page.data = std::make_unique<char[]>(kernel::memo::PAGE_SIZE);
            std::memcpy(new_page.data.get(), shared_page.data.get(), kernel::memo::PAGE_SIZE);
            new_page.data = std::move(new_page.data);

            // Update ref counts
            kernel::memo::memory_manager.decrement_ref(phpage_id);
            entry.physical_page = new_phpage_id;

            // Update flags
            entry.flag &= ~kernel::memo::PageFlags::COW;
            entry.flag |= kernel::memo::PageFlags::WRITE;

            std::cout << "COW triggered: Process " << this->process_control_block->getPID().value << " now owns page "
                      << new_phpage_id << "\n";
        }
        else
        {
            throw kernel::error::segmentation_fault("Write access denied on page");
        }
    }

    // Write to physical memory
    kernel::memo::PhysicalPage& page = kernel::memo::memory_manager.get_page(entry.physical_page);
    if (offset < 0 || offset >= kernel::memo::PAGE_SIZE)
        throw kernel::error::segmentation_fault("Offset out of bounds");

    page.data[offset] = value;
}

char kernel::proc::Process::read_from_virtual_page(kernel::memo::PageID vpage, int offset) {
    if (this->page_table.entries.find(vpage) == this->page_table.entries.end())
        throw kernel::error::page_fault("virtual page not mapped");

    kernel::memo::PageTableEntry& entry = this->page_table.entries[vpage];
    kernel::memo::PhysicalPage&   page  = kernel::memo::memory_manager.get_page(entry.physical_page);

    if (!(entry.flag & memo::PageFlags::READ))
        throw kernel::error::page_fault("virtual page not readable");

    if (offset < 0 || offset >= memo::PAGE_SIZE)
        throw kernel::error::segmentation_fault("Offset out of bounds");

    return page.data[offset];
}

std::unique_ptr<kernel::proc::Process> kernel::proc::Process::fork_process(PID child_pid) {
    auto child =
      std::make_unique<kernel::proc::Process>(/* possibly pass child PID and metadata */ kernel::proc::Process());

    // Manually copy deep data from `*this` into *child
    child->process_control_block = std::make_unique<kernel::proc::PCB>(*this->process_control_block);

    for (const auto& [vpage, entry] : this->page_table.entries)
    {
        kernel::memo::memory_manager.increment_ref(entry.physical_page);
        kernel::memo::PageTableEntry new_entry = entry;
        new_entry.flag &= ~kernel::memo::PageFlags::WRITE;
        new_entry.flag |= kernel::memo::PageFlags::COW;
        child->page_table.entries[vpage] = new_entry;
    }

    return child;
}


}  // namespace proc
}  // namespace kernel