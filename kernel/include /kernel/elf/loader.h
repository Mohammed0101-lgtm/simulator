#ifndef __ELFLOADER_H__
#define __ELFLOADER_H__

#include "../../memory.h"
#include "../../process.h"
#include "header.h"
#include <string>


namespace kernel {
namespace core {
namespace elf {

class ELF_Loader
{
   public:
    bool ELF_Loader::load_elf(kernel::proc::Process& process, const std::string& filename);
    bool is_valid_elf(const Elf32_Ehdr& header);
    void load_segments(const ELF_Parser& elf_parser, kernel::proc::Process& process);
};

}  // namespace elf
}  // namespace core
}  // namespace kernel


#endif  // __ELFLOADER_H__