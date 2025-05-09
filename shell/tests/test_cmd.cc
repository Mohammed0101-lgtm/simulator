#include "../command.h"
#include "../config.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>


namespace fs = std::filesystem;


class ShellCommandTest: public ::testing::Test
{
   protected:
    Command_Impl cmd;

    void SetUp() override {
        // Clean slate for each test
        if (fs::exists("testdir"))
            fs::remove_all("testdir");
        if (fs::exists("file1.txt"))
            fs::remove("file1.txt");
        if (fs::exists("file2.txt"))
            fs::remove("file2.txt");
    }

    void TearDown() override {
        SetUp();  // Reuse cleanup
    }
};

TEST_F(ShellCommandTest, MkdirCreatesDirectory) {
    std::vector<std::string> args = {"testdir"};
    EXPECT_EQ(cmd.execute_command("mkdir", args), 0);
    EXPECT_TRUE(fs::exists("testdir"));
}

TEST_F(ShellCommandTest, RmdirRemovesDirectory) {
    fs::create_directory("testdir");
    std::vector<std::string> args = {"testdir"};
    EXPECT_EQ(cmd.execute_command("rmdir", args), 0);
    EXPECT_FALSE(fs::exists("testdir"));
}

TEST_F(ShellCommandTest, TouchCreatesFile) {
    std::vector<std::string> args = {"file1.txt"};
    EXPECT_EQ(cmd.execute_command("touch", args), 0);
    EXPECT_TRUE(fs::exists("file1.txt"));
}

TEST_F(ShellCommandTest, RmRemovesFile) {
    std::ofstream("file1.txt").put('x');
    std::vector<std::string> args = {"file1.txt"};
    EXPECT_EQ(cmd.execute_command("rm", args), 0);
    EXPECT_FALSE(fs::exists("file1.txt"));
}

TEST_F(ShellCommandTest, CpCopiesFile) {
    std::ofstream("file1.txt") << "data";
    std::vector<std::string> args = {"file1.txt", "file2.txt"};
    EXPECT_EQ(cmd.execute_command("cp", args), 0);
    EXPECT_TRUE(fs::exists("file2.txt"));
}

TEST_F(ShellCommandTest, MvMovesFile) {
    std::ofstream("file1.txt") << "move";
    std::vector<std::string> args = {"file1.txt", "file2.txt"};
    EXPECT_EQ(cmd.execute_command("mv", args), 0);
    EXPECT_FALSE(fs::exists("file1.txt"));
    EXPECT_TRUE(fs::exists("file2.txt"));
}

TEST_F(ShellCommandTest, PwdPrintsCurrentPath) {
    std::vector<std::string> args;
    testing::internal::CaptureStdout();
    EXPECT_EQ(cmd.execute_command("pwd", args), 0);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find(fs::current_path().string()), std::string::npos);
}

TEST_F(ShellCommandTest, CatPrintsFileContents) {
    std::ofstream("file1.txt") << "hello cat";
    std::vector<std::string> args = {"file1.txt"};
    testing::internal::CaptureStdout();
    EXPECT_EQ(cmd.execute_command("cat", args), 0);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "hello cat");
}

TEST_F(ShellCommandTest, ClearProducesNewline) {
    std::vector<std::string> args;
    testing::internal::CaptureStdout();
    EXPECT_EQ(cmd.execute_command("clear", args), 0);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());
}