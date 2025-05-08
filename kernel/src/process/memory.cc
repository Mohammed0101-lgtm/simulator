#include "memory.h"
#include "error.h"

#include <iostream>


namespace kernel {
namespace memo {

PageID MemoryManager::allocate_physical_page() {
    PageID new_page_id = next_page_id;
    next_page_id++;                                // Increment the page ID for the next allocation
    physical_pages[new_page_id] = PhysicalPage();  // Create a new physical page
    return new_page_id;
}

void MemoryManager::increment_ref(PageID page) {
    if (physical_pages.find(page) != physical_pages.end())
        physical_pages[page].ref_count++;

    // Handle the case where the page does not exist
    // This could be an error or a no-op depending on your design
    // For example, you might want to throw an exception or log an error
    // std::cerr << "Page not found: " << page << std::endl;
    // Or you could just do nothing
    // std::cerr << "Page not found: " << page << std::endl;
}

void MemoryManager::decrement_ref(PageID page) {
    if (physical_pages.find(page) != physical_pages.end())
    {
        physical_pages[page].ref_count--;
        if (physical_pages[page].ref_count == 0)
            physical_pages.erase(page);
    }
}

PhysicalPage& MemoryManager::get_page(PageID id) {
    if (physical_pages.find(id) != physical_pages.end())
        return physical_pages[id];

    throw error::page_fault("Page not found");
}

};  // namespace memo
};  // namespace kernel