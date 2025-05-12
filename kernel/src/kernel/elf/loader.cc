#include "elf/loader.h"
#include "cpu/cpu.h"
#include "elf/execute.h"
#include "elf/header.h"
#include "elf/parser.h"
#include "memory/memory.h"
#include "process/process.h"

#include <fstream>
#include <string>


namespace kernel {
namespace elf {


bool kernel::elf::ELF_Loader::load_elf(kernel::proc::Process& process, const std::string& filename) {
    // Open the ELF file
    std::ifstream elf_file(filename, std::ios::binary);
    if (!elf_file.is_open())
    {
        std::cerr << "Error: Could not open ELF file." << std::endl;
        return false;
    }

    // Read and validate ELF header
    kernel::elf::Elf32_Ehdr elf_header;
    elf_file.read(reinterpret_cast<char*>(&elf_header), sizeof(elf_header));
    if (elf_header.e_ident[0] != 0x7F || elf_header.e_ident[1] != 'E' || elf_header.e_ident[2] != 'L'
        || elf_header.e_ident[3] != 'F')
    {
        std::cerr << "Invalid ELF header!" << std::endl;
        return false;
    }

    // Read program header table
    elf_file.seekg(elf_header.e_phoff, std::ios::beg);
    for (int i = 0; i < elf_header.e_phnum; ++i)
    {
        kernel::elf::Elf32_Phdr phdr;
        elf_file.read(reinterpret_cast<char*>(&phdr), sizeof(phdr));

        if (phdr.p_type == PT_LOAD)
        {
            kernel::elf::ELF_Parser parser(filename);
            if (!parser.is_valid())
            {
                std::cerr << "Invalid ELF format\n";
                return false;
            }

            // Load segments
            this->load_segments(parser, process);
            // Load segments

            // Setup stack
            setup_stack(process);

            // Initialize registers with entry point
            kernel::cpu::CPU cpu;  // TOTO : this  is clearly wrong and should replaced
            initialize_registers(process, parser.get_entry_point(), cpu);
        }
    }

    // Set up the program's entry point and stack pointer
    process._process_control_block->setPC(elf_header.e_entry);
    process._process_control_block->setSP(kernel::memo::STACK_START_ADDRESS);  // Set to appropriate value

    // Start the execution of the program (run the instructions)
    // start_execution();

    return true;
}

void kernel::elf::ELF_Loader::load_segments(const kernel::elf::ELF_Parser& elf_parser, kernel::proc::Process& process) {
    for (const auto& ph : elf_parser.get_program_headers())
    {
        if (ph.p_type != PT_LOAD)
            continue;

        uint32_t    vaddr        = ph.p_vaddr;
        uint32_t    memsz        = ph.p_memsz;
        uint32_t    filesz       = ph.p_filesz;
        const auto& segment_data = elf_parser.get_segment_data(ph);

        // Write file contents into memory
        for (uint32_t i = 0; i < filesz; ++i)
            process.write_to_virtual_page(vaddr, i, segment_data[i]);

        // Zero out the remaining (BSS)
        for (uint32_t i = filesz; i < memsz; ++i)
            process.write_to_virtual_page(vaddr, i, 0);
    }
}

bool kernel::elf::ELF_Loader::is_valid_elf(const kernel::elf::Elf32_Ehdr& header) {
    // Check magic number: 0x7F, 'E', 'L', 'F'
    return header.e_ident[0] == 0x7F && header.e_ident[1] == 'E' && header.e_ident[2] == 'L'
        && header.e_ident[3] == 'F';
}


}  // namespace elf
}  // namespace kernel