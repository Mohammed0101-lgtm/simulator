// elf.h
#ifndef __ELFHEADER_H__
#define __ELFHEADER_H__

#include <cstdint>

namespace kernel {
namespace core {
namespace elf {

constexpr uint8_t ELF_MAGIC[] = {0x7F, 'E', 'L', 'F'};

struct Elf32_Ehdr
{
    uint8_t  e_ident[16];  // Magic number and other info
    uint16_t e_type;       // Object file type
    uint16_t e_machine;    // Architecture
    uint32_t e_version;    // Object file version
    uint32_t e_entry;      // Entry point virtual address
    uint32_t e_phoff;      // Program header table file offset
    uint32_t e_shoff;      // Section header table file offset
    uint32_t e_flags;      // Processor-specific flags
    uint16_t e_ehsize;     // ELF header size in bytes
    uint16_t e_phentsize;  // Program header table entry size
    uint16_t e_phnum;      // Program header table entry count
    uint16_t e_shentsize;  // Section header table entry size
    uint16_t e_shnum;      // Section header table entry count
    uint16_t e_shstrndx;   // Section header string table index
};

struct Elf32_Phdr
{
    uint32_t p_type;    // Segment type
    uint32_t p_offset;  // Segment file offset
    uint32_t p_vaddr;   // Virtual address in memory
    uint32_t p_paddr;   // Reserved
    uint32_t p_filesz;  // Size of segment in file
    uint32_t p_memsz;   // Size of segment in memory
    uint32_t p_flags;   // Segment flags
    uint32_t p_align;   // Alignment
};

enum Elf_ProgramType : int {
    PT_NULL,
    PT_LOAD,
    PT_DYNAMIC,
    PT_INTERP,
    PT_NOTE,
    PT_SHLIB,
    PT_PHDR,
    PT_TLS
};

}  // namespace elf
}  // namespace core
}  // namespace elf

#endif  // __ELFHEADER_H__