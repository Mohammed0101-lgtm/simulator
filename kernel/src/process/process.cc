#include "process.h"
#include "error.h"

#include <memory>


namespace kernel {
namespace proc {

void Process::write_to_virtual_page(memo::MemoryManager& mm, memo::PageID vpage, int offset, char value) {
    // check if the virtual page is mapped
    if (this->page_table.entries.find(vpage) == this->page_table.entries.end())
        throw error::page_fault("virtual page not mapped");

    // copy the page table entry metadata
    memo::PageTableEntry& entry     = this->page_table.entries[vpage];
    memo::PageID          phpage_id = entry.physical_page;

    // Check if page is writable
    bool is_writable = entry.flag & memo::PageFlags::WRITE;
    bool is_cow      = entry.flag & memo::PageFlags::COW;

    if (is_writable == false)
    {
        if (is_cow == true)
        {
            // Perform Copy-On-Write
            memo::PhysicalPage& shared_page = mm.get_page(phpage_id);

            // Allocate new page and copy data
            memo::PageID        new_phpage_id = mm.allocate_physical_page();
            memo::PhysicalPage& new_page      = mm.get_page(new_phpage_id);

            // Copy data from shared page to new page copy
            mm.increment_ref(new_phpage_id);
            new_page.data = std::make_unique<char[]>(memo::PAGE_SIZE);
            std::memcpy(new_page.data.get(), shared_page.data.get(), memo::PAGE_SIZE);
            new_page.data = std::move(new_page.data);

            // Update ref counts
            mm.decrement_ref(phpage_id);
            entry.physical_page = new_phpage_id;

            // Update flags
            entry.flag &= ~memo::PageFlags::COW;
            entry.flag |= memo::PageFlags::WRITE;

            std::cout << "COW triggered: Process " << this->process_control_block->getPID() << " now owns page "
                      << new_phpage_id << "\n";
        }
        else
        {
            throw error::segmentation_fault("Write access denied on page");
        }
    }

    // Write to physical memory
    memo::PhysicalPage& page = mm.get_page(entry.physical_page);
    if (offset < 0 || offset >= memo::PAGE_SIZE)
        throw error::segmentation_fault("Offset out of bounds");

    page.data[offset] = value;
}

char Process::read_from_virtual_page(memo::MemoryManager& mm, memo::PageID vpage, int offset) {
    if (this->page_table.entries.find(vpage) == this->page_table.entries.end())
        throw error::page_fault("virtual page not mapped");

    memo::PageTableEntry& entry = this->page_table.entries[vpage];
    memo::PhysicalPage&   page  = mm.get_page(entry.physical_page);

    if (!(entry.flag & memo::PageFlags::READ))
        throw error::page_fault("virtual page not readable");

    if (offset < 0 || offset >= memo::PAGE_SIZE)
        throw error::segmentation_fault("Offset out of bounds");

    return page.data[offset];
}

std::unique_ptr<Process> Process::fork_process(memo::MemoryManager& mm, PID child_pid) {
    auto child = std::make_unique<Process>(/* possibly pass child PID and metadata */ Process());

    // Manually copy deep data from `*this` into *child
    child->process_control_block = std::make_unique<proc::PCB>(*this->process_control_block);

    for (const auto& [vpage, entry] : this->page_table.entries)
    {
        mm.increment_ref(entry.physical_page);
        memo::PageTableEntry new_entry = entry;
        new_entry.flag &= ~memo::PageFlags::WRITE;
        new_entry.flag |= memo::PageFlags::COW;
        child->page_table.entries[vpage] = new_entry;
    }

    return child;
}


}  // namespace proc
}  // namespace kernel