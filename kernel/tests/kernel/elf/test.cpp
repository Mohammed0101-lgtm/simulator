#include "elf/execute.hh"
#include "elf/header.hh"
#include "elf/loader.hh"
#include "elf/parser.hh"

#include <gtest/gtest.h>


TEST(ELF, ELFParser) {
    kernel::elf::ELF_Parser parser("test.elf");
    EXPECT_TRUE(parser.is_valid());
    EXPECT_EQ(parser.get_entry_point(), 0x08048000);  // Example entry point
}


TEST(ELF, ELFLoader) {
    kernel::proc::Process   process;
    kernel::elf::ELF_Loader loader;
    EXPECT_TRUE(loader.load_elf(process, "test.elf"));
}