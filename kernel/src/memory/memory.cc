#include "memory.h"
#include "../error.h"

#include <iostream>


namespace kernel {
namespace memo {

kernel::memo::PageID kernel::memo::MemoryManager::allocate_physical_page() {
    kernel::memo::PageID new_page_id = next_page_id;
    next_page_id++;                                              // Increment the page ID for the next allocation
    physical_pages[new_page_id] = kernel::memo::PhysicalPage();  // Create a new physical page
    return new_page_id;
}

void kernel::memo::MemoryManager::increment_ref(kernel::memo::PageID page) {
    if (physical_pages.find(page) != physical_pages.end())
        physical_pages[page].ref_count++;

    // Handle the case where the page does not exist
    // This could be an error or a no-op depending on your design
    // For example, you might want to throw an exception or log an error
    // std::cerr << "Page not found: " << page << std::endl;
    // Or you could just do nothing
    // std::cerr << "Page not found: " << page << std::endl;
}

void kernel::memo::MemoryManager::decrement_ref(kernel::memo::PageID page) {
    if (physical_pages.find(page) != physical_pages.end())
    {
        physical_pages[page].ref_count--;
        if (physical_pages[page].ref_count == 0)
            physical_pages.erase(page);
    }
}

kernel::memo::PhysicalPage& kernel::memo::MemoryManager::get_page(kernel::memo::PageID id) {
    if (physical_pages.find(id) != physical_pages.end())
        return physical_pages[id];

    throw kernel::error::page_fault("Page not found");
}

void kernel::memo::MemoryManager::write_u8(kernel::memo::PageID page_id, uint32_t offset, uint8_t value) {
    if (physical_pages.find(page_id) == physical_pages.end())
        throw kernel::error::page_fault("Physical page not found");

    kernel::memo::PhysicalPage& page = physical_pages.at(page_id);
    if (offset >= sizeof(page.data))
        throw kernel::error::segmentation_fault("Offset exceeds page size");

    page.data[offset] = value;
}

uint32_t kernel::memo::MemoryManager::read_u32(kernel::memo::PageID page_id, uint32_t offset) const {
    if (physical_pages.find(page_id) == physical_pages.end())
        throw kernel::error::page_fault("Physical page not found");

    const kernel::memo::PhysicalPage& page = physical_pages.at(page_id);
    if (offset + 3 >= sizeof(page.data))
        throw kernel::error::segmentation_fault("Offset exceeds page size for 32-bit read");

    return (page.data[offset] << 24) | (page.data[offset + 1] << 16) | (page.data[offset + 2] << 8)
         | (page.data[offset + 3]);
}

uint8_t kernel::memo::MemoryManager::read_u8(kernel::memo::PageID page_id, uint32_t offset) const {
    if (physical_pages.find(page_id) == physical_pages.end())
        throw kernel::error::page_fault("Physical page not found");

    const kernel::memo::PhysicalPage& page = physical_pages.at(page_id);
    if (offset >= sizeof(page.data))
        throw kernel::error::segmentation_fault("Offset exceeds page size for 8-bit read");

    return page.data[offset];
}

void kernel::memo::MemoryManager::write_u32(kernel::memo::PageID page_id, uint32_t offset, uint32_t value) {
    if (physical_pages.find(page_id) == physical_pages.end())
        throw kernel::error::page_fault("Physical page not found");

    kernel::memo::PhysicalPage& page = physical_pages.at(page_id);
    if (offset + 3 >= sizeof(page.data))
        throw kernel::error::segmentation_fault("Offset exceeds page size for 32-bit write");

    page.data[offset]     = (value >> 24) & 0xFF;
    page.data[offset + 1] = (value >> 16) & 0xFF;
    page.data[offset + 2] = (value >> 8) & 0xFF;
    page.data[offset + 3] = value & 0xFF;
}

}  // namespace memo
}  // namespace kernel