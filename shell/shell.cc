#include "shell.hh"
#include "command.hh"
#include "config.hh"
#include "input.hh"

#include <filesystem>
#include <iostream>
#include <limits.h>
#include <pwd.h>
#include <unistd.h>


namespace fs = std::filesystem;


void Shell::run() {
    Input_Handler input_handler;
    int           status = 0;
    std::string   line;

    do
    {
        display_prompt();
        line = input_handler.read_line();

        std::vector<std::string> args = input_handler.tokenize(line);

        if (args.empty())
            continue;

        Command_Impl command_impl;
        std::string  command = args[0];

        if (command == "exit")
            exit(_SUCCESS);  // exit the shell

        args.erase(args.begin());
        status = command_impl.execute(command, args);
    } while (status != _ERROR && status != _NOT_SUPPORTED);
}

void Shell::display_prompt() const {
    char username[_SC_LOGIN_NAME_MAX];
    char hostname[_SC_HOST_NAME_MAX];

    if (getlogin_r(username, sizeof(username)) != 0)
    {
        std::cerr << "Error getting username : %s\n" << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        std::cerr << "Error getting hostname : %s\n" << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string cwd = fs::current_path().string();

    std::cout << "\033[1;32m" << username << "@" << hostname << "\033[0m"
              << "\033[1;34m" << cwd << "\033[0m"
              << "\033[1;36m" << "$ " << "\033[0m" << " ";
}