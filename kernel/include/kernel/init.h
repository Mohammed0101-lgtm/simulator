#include "cpu/cpu.hh"
#include "elf/loader.hh"
#include "elf/parser.hh"
#include "memory/memory.hh"
#include "process/process.hh"
#include "process/scheduler.hh"

#include <iostream>
#include <string>


namespace kernel {
namespace core {

void init(memo::MemoryManager& memory_manager, cpu::CPU& cpu) {
    const std::string elf_path = "bin/init.elf";

    elf::ELF_Parser parser(elf_path);
    if (!parser.is_valid())
    {
        std::cerr << "[init] Invalid ELF file: " << elf_path << "\n";
        return;
    }

    proc::Process   user_process;
    elf::ELF_Loader loader;

    if (!loader.load_elf(user_process, elf_path))
    {
        std::cerr << "[init] Failed to load ELF file: " << elf_path << "\n";
        return;
    }

    loader.load_segments(parser, user_process);

    uint32_t entry_point = parser.get_entry_point();
    user_process._process_control_block->setPC(entry_point);
    user_process._process_control_block->setSP(memo::STACK_START_ADDRESS);

    std::cout << "[init] ELF loaded. Entry Point: 0x" << std::hex << entry_point << "\n";

    for (int i = 0; i < cpu._registers.size(); ++i)
        cpu._registers[i].setInstruction(cpu::instr::Instruction(cpu::instr::VALUE::NOP));


    kernel::proc::Scheduler scheduler;
    scheduler.add_process(std::make_unique<kernel::proc::Process>(std::move(user_process)));
    scheduler.run();
}

}  // namespace core
}  // namespace kernel
