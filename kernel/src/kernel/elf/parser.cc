#include "elf/parser.hh"

#include <cstring>
#include <fstream>
#include <iostream>


namespace kernel {
namespace elf {

kernel::elf::ELF_Parser::ELF_Parser(const std::string& elf_path) {
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

bool kernel::elf::ELF_Parser::is_valid() const { return this->_valid; }

uint32_t kernel::elf::ELF_Parser::get_entry_point() const { return this->_header.e_entry; }

const std::vector<kernel::elf::Elf32_Phdr>& kernel::elf::ELF_Parser::get_program_headers() const {
    return this->_program_headers;
}

const std::vector<uint8_t>& kernel::elf::ELF_Parser::get_segment_data(const kernel::elf::Elf32_Phdr& ph) const {
    static std::vector<uint8_t> segment;
    if (ph.p_offset + ph.p_filesz > _elf_data.size())
        throw std::runtime_error("Segment data out of bounds");

    segment.assign(_elf_data.begin() + ph.p_offset, _elf_data.begin() + ph.p_offset + ph.p_filesz);
    return segment;
}

void kernel::elf::ELF_Parser::parse() {
    // Check file size
    if (this->_elf_data.size() < sizeof(kernel::elf::Elf32_Ehdr))
    {
        std::cerr << "ELF file too small\n";
        return;
    }

    // Copy ELF header
    std::memcpy(&this->_header, &this->_elf_data[0], sizeof(kernel::elf::Elf32_Ehdr));

    // Check magic number
    if (this->_header.e_ident[0] != 0x7F || this->_header.e_ident[1] != 'E' || this->_header.e_ident[2] != 'L'
        || this->_header.e_ident[3] != 'F')
    {
        std::cerr << "Invalid ELF magic\n";
        return;
    }

    // Check class (32-bit)
    if (this->_header.e_ident[4] != 1)
    {
        std::cerr << "Unsupported ELF class (only 32-bit supported)\n";
        return;
    }

    // Parse program headers
    const uint8_t* ph_table = &this->_elf_data[0] + this->_header.e_phoff;
    for (int i = 0; i < this->_header.e_phnum; ++i)
    {
        kernel::elf::Elf32_Phdr ph;
        std::memcpy(&ph, ph_table + i * this->_header.e_phentsize, sizeof(kernel::elf::Elf32_Phdr));
        this->_program_headers.push_back(ph);
    }

    this->_valid = true;
}


}  // namespace elf
}  // namespace kernel