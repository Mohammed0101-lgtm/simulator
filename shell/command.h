#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>
#include <vector>


class Command_Impl
{
   public:
    Command_Impl() = default;

    int execute(const std::string& command, const std::vector<std::string>& args);
    int execute_command(const std::string& command, const std::vector<std::string>& args);

   private:
    int recdir(const std::string& path);
    int pwd(const std::vector<std::string>& args);
    int cd(const std::vector<std::string>& args);
    int ls(const std::vector<std::string>& args);
    int mkdir(const std::vector<std::string>& args);
    int rmdir(const std::vector<std::string>& args);
    int rm(const std::vector<std::string>& args);
    int cp(const std::vector<std::string>& args);
    int mv(const std::vector<std::string>& args);
    int touch(const std::vector<std::string>& args);
    int cat(const std::vector<std::string>& args);
    int echo(const std::vector<std::string>& args);
    int clear(const std::vector<std::string>& args);

    int _open(const std::vector<std::string>& args);
    int run(const std::vector<std::string>& args);
    int head(const std::vector<std::string>& args);
};


#endif  // __COMMAND_H__