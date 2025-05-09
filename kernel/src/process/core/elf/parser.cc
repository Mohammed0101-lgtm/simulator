#include "parser.h"
#include <cstring>
#include <fstream>
#include <iostream>


namespace kernel {
namespace core {
namespace elf {

kernel::core::elf::ELF_Parser::ELF_Parser(const std::string& elf_path) {
    std::ifstream file(elf_path, std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to open ELF file: " << elf_path << "\n";
        return;
    }

    // Read entire file into buffer
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    _elf_data.resize(size);
    if (!file.read(reinterpret_cast<char*>(&_elf_data[0]), size))
    {
        std::cerr << "Failed to read ELF file\n";
        return;
    }

    parse();
}

bool kernel::core::elf::ELF_Parser::is_valid() const { return _valid; }

uint32_t kernel::core::elf::ELF_Parser::get_entry_point() const { return _header.e_entry; }

const std::vector<kernel::core::elf::Elf32_Phdr>& kernel::core::elf::ELF_Parser::get_program_headers() const {
    return _program_headers;
}

const std::vector<uint8_t>&
kernel::core::elf::ELF_Parser::get_segment_data(const kernel::core::elf::Elf32_Phdr& ph) const {
    static std::vector<uint8_t> segment;
    if (ph.p_offset + ph.p_filesz > _elf_data.size())
        throw std::runtime_error("Segment data out of bounds");

    segment.assign(_elf_data.begin() + ph.p_offset, _elf_data.begin() + ph.p_offset + ph.p_filesz);
    return segment;
}

void kernel::core::elf::ELF_Parser::parse() {
    // Check file size
    if (_elf_data.size() < sizeof(kernel::core::elf::Elf32_Ehdr))
    {
        std::cerr << "ELF file too small\n";
        return;
    }

    // Copy ELF header
    std::memcpy(&_header, &_elf_data[0], sizeof(kernel::core::elf::Elf32_Ehdr));

    // Check magic number
    if (_header.e_ident[0] != 0x7F || _header.e_ident[1] != 'E' || _header.e_ident[2] != 'L'
        || _header.e_ident[3] != 'F')
    {
        std::cerr << "Invalid ELF magic\n";
        return;
    }

    // Check class (32-bit)
    if (_header.e_ident[4] != 1)
    {
        std::cerr << "Unsupported ELF class (only 32-bit supported)\n";
        return;
    }

    // Parse program headers
    const uint8_t* ph_table = &_elf_data[0] + _header.e_phoff;
    for (int i = 0; i < _header.e_phnum; ++i)
    {
        kernel::core::elf::Elf32_Phdr ph;
        std::memcpy(&ph, ph_table + i * _header.e_phentsize, sizeof(kernel::core::elf::Elf32_Phdr));
        _program_headers.push_back(ph);
    }

    _valid = true;
}


}  // namespace elf
}  // namespace core
}  // namespace kernel