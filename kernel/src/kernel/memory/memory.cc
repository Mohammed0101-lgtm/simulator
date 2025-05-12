#include "memory/memory.h"
#include "error.h"

#include <iostream>


namespace kernel {
namespace memo {

kernel::memo::PageID kernel::memo::MemoryManager::allocate_physical_page() {
    kernel::memo::PageID new_page_id = this->_next_page_id;
    this->_next_page_id++;                                              // Increment the page ID for the next allocation
    this->_physical_pages[new_page_id] = kernel::memo::PhysicalPage();  // Create a new physical page
    return new_page_id;
}

void kernel::memo::MemoryManager::increment_ref(kernel::memo::PageID page) {
    if (this->_physical_pages.find(page) != this->_physical_pages.end())
        this->_physical_pages[page]._ref_count++;

    // Handle the case where the page does not exist
    // This could be an error or a no-op depending on your design
    // For example, you might want to throw an exception or log an error
    // std::cerr << "Page not found: " << page << std::endl;
    // Or you could just do nothing
    // std::cerr << "Page not found: " << page << std::endl;
}

void kernel::memo::MemoryManager::decrement_ref(kernel::memo::PageID page) {
    if (this->_physical_pages.find(page) != this->_physical_pages.end())
    {
        this->_physical_pages[page]._ref_count--;
        if (this->_physical_pages[page]._ref_count == 0)
            this->_physical_pages.erase(page);
    }
}

kernel::memo::PhysicalPage& kernel::memo::MemoryManager::get_page(kernel::memo::PageID id) {
    if (this->_physical_pages.find(id) != this->_physical_pages.end())
        return this->_physical_pages[id];

    throw kernel::error::Page_fault("Page not found");
}

void kernel::memo::MemoryManager::write_u8(kernel::memo::PageID page_id, uint32_t offset, uint8_t value) {
    if (this->_physical_pages.find(page_id) == this->_physical_pages.end())
        throw kernel::error::Page_fault("Physical page not found");

    kernel::memo::PhysicalPage& page = this->_physical_pages.at(page_id);
    if (offset >= sizeof(page._data))
        throw kernel::error::Segmentation_fault("Offset exceeds page size");

    page._data[offset] = value;
}

uint32_t kernel::memo::MemoryManager::read_u32(kernel::memo::PageID page_id, uint32_t offset) const {
    if (this->_physical_pages.find(page_id) == this->_physical_pages.end())
        throw kernel::error::Page_fault("Physical page not found");

    const kernel::memo::PhysicalPage& page = this->_physical_pages.at(page_id);
    if (offset + 3 >= sizeof(page._data))
        throw kernel::error::Segmentation_fault("Offset exceeds page size for 32-bit read");

    return (page._data[offset] << 24) | (page._data[offset + 1] << 16) | (page._data[offset + 2] << 8)
         | (page._data[offset + 3]);
}

uint8_t kernel::memo::MemoryManager::read_u8(kernel::memo::PageID page_id, uint32_t offset) const {
    if (this->_physical_pages.find(page_id) == this->_physical_pages.end())
        throw kernel::error::Page_fault("Physical page not found");

    const kernel::memo::PhysicalPage& page = this->_physical_pages.at(page_id);
    if (offset >= sizeof(page._data))
        throw kernel::error::Segmentation_fault("Offset exceeds page size for 8-bit read");

    return page._data[offset];
}

void kernel::memo::MemoryManager::write_u32(kernel::memo::PageID page_id, uint32_t offset, uint32_t value) {
    if (this->_physical_pages.find(page_id) == this->_physical_pages.end())
        throw kernel::error::Page_fault("Physical page not found");

    kernel::memo::PhysicalPage& page = this->_physical_pages.at(page_id);
    if (offset + 3 >= sizeof(page._data))
        throw kernel::error::Segmentation_fault("Offset exceeds page size for 32-bit write");

    page._data[offset]     = (value >> 24) & 0xFF;
    page._data[offset + 1] = (value >> 16) & 0xFF;
    page._data[offset + 2] = (value >> 8) & 0xFF;
    page._data[offset + 3] = value & 0xFF;
}

}  // namespace memo
}  // namespace kernel