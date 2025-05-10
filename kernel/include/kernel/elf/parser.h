#ifndef __PARSER_H__
#define __PARSER_H__

#include "header.h"
#include <vector>


namespace kernel {
namespace elf {


class ELF_Parser
{
   public:
    ELF_Parser(const std::string& elf_path);

    bool                           is_valid() const;
    uint32_t                       get_entry_point() const;
    const std::vector<Elf32_Phdr>& get_program_headers() const;
    const std::vector<uint8_t>&    get_segment_data(const Elf32_Phdr& ph) const;

   private:
    Elf32_Ehdr              _header;
    std::vector<Elf32_Phdr> _program_headers;
    std::vector<uint8_t>    _elf_data;
    bool                    _valid = false;

    void parse();
};


}  // kernel
}  // elf


#endif  // __PARSER_H__