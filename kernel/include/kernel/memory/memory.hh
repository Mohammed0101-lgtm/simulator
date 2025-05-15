#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "process/PCB.hh"
#include <memory>
#include <unordered_map>


namespace kernel {
namespace memo {


using PageID                           = int;
constexpr int      PAGE_SIZE           = 4096;
constexpr uint32_t STACK_START_ADDRESS = 0xC0000000;  // Top of user space
constexpr uint32_t STACK_SIZE          = 0x00100000;  // 1 MB


struct PageTableEntry
{
    PageID _physical_page;
    int    _flag;
};

struct PageTable
{
    std::unordered_map<PageID, PageTableEntry> _entries;
};

struct PhysicalPage
{
    int                     _ref_count = 0;
    std::unique_ptr<char[]> _data;

    PhysicalPage() :
        _data(new char[PAGE_SIZE]) {}  // Assuming page size is 4096 bytes
};

struct VirtualPage
{
    PageID            _page_id;
    kernel::proc::PID _pid;
    int               _offset;

    VirtualPage(PageID id, kernel::proc::PID process_id, int off) :
        _page_id(id),
        _pid(process_id),
        _offset(off) {}
};

enum PageFlags {
    READ  = 1 << 0,
    WRITE = 1 << 1,
    COW   = 1 << 2
};

struct MemoryManager
{
    std::unordered_map<PageID, PhysicalPage> _physical_pages;
    PageID                                   _next_page_id = 0;
    uint8_t                                  _simulated_memory[65536];  // 64KB

    PageID        allocate_physical_page();
    void          increment_ref(PageID page);
    void          decrement_ref(PageID page);
    PhysicalPage& get_page(PageID id);
    void          write_u8(PageID page_id, uint32_t offset, uint8_t value);
    uint32_t      read_u32(PageID page_id, uint32_t offset) const;
    uint8_t       read_u8(PageID, uint32_t offset) const;
    void          write_u32(PageID page_id, uint32_t offset, uint32_t value);
};

extern MemoryManager memory_manager;

}  // namespace memo
}  // namespace kernel


#endif  // __MEMORY_H__