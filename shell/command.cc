#include "command.h"
#include "config.h"

#include <algorithm>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>


namespace fs = std::filesystem;


int Command_Impl::execute_command(const std::string& command, const std::vector<std::string>& args) {
    if (command == "pwd")
        return this->pwd(args);
    else if (command == "cd")
        return this->cd(args);
    else if (command == "ls")
        return this->ls(args);
    else if (command == "mkdir")
        return this->mkdir(args);
    else if (command == "rmdir")
        return this->rmdir(args);
    else if (command == "rm")
        return this->rm(args);
    else if (command == "cp")
        return this->cp(args);
    else if (command == "mv")
        return this->mv(args);
    else if (command == "touch")
        return this->touch(args);
    else if (command == "cat")
        return this->cat(args);
    else if (command == "echo")
        return this->echo(args);
    else if (command == "clear")
        return this->clear(args);
    else if (command == "open")
        return this->_open(args);
    else if (command == "run")
        return this->run(args);
    else if (command == "head")
        return this->head(args);
    else
    {
        std::cerr << "Error: Command not found." << std::endl;
        return _NOT_SUPPORTED;
    }
}

int Command_Impl::execute(const std::string& command, const std::vector<std::string>& args) {
    pid_t pid = fork();
    int   status;

    if (pid == 0)
    {
        int result = this->execute_command(command, args);
        if (result == _ERROR)
        {
            std::cerr << "Error: Command execution failed." << std::endl;
            _exit(_ERROR);
        }
        else if (result == _NOT_SUPPORTED)
        {
            std::cerr << "Error: Command not supported." << std::endl;
            _exit(_NOT_SUPPORTED);
        }
        else
        {
            _exit(_SUCCESS);
        }
    }
    else if (pid < 0)
    {
        std::cerr << "Error: Fork failed." << std::endl;
        return _ERROR;
    }
    else
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else
            return _ERROR;
    }

    return WEXITSTATUS(status);
}

inline int getTerminalWidth() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
        return 80;  // fallback to default width

    return w.ws_col;
}

int Command_Impl::recdir(const std::string& path) {
    if (path.empty())
        return _ERROR;

    if (!fs::exists(path))
    {
        std::cerr << "Error: Path does not exist." << std::endl;
        return _ERROR;
    }

    if (!fs::is_directory(path))
    {
        std::cerr << "Error: Path is not a directory." << std::endl;
        return _ERROR;
    }

    std::vector<std::string> files;
    std::size_t              max_len = 0;

    for (const auto& entry : fs::directory_iterator(path))
    {
        if (entry.is_regular_file() || entry.is_directory())
        {
            files.push_back(entry.path().filename().string());
            max_len = std::max(max_len, entry.path().filename().string().length());
        }
    }

    std::sort(files.begin(), files.end());

    int separation     = max_len + 4;
    int terminal_width = getTerminalWidth();
    int columns        = terminal_width / separation;
    int count          = 0;

    for (const auto& file : files)
    {
        std::cout << std::left << std::setw(separation) << file;
        ++count;
        if (count % columns == 0)
            std::cout << '\n';
    }

    std::cout << std::endl;
    return _SUCCESS;
}

// print the current working directory
inline int Command_Impl::pwd(const std::vector<std::string>& args) {
    // no arguments are required for this command
    if (args.size() != 0)
    {
        std::cerr << "Error: Too many arguments." << std::endl;
        return _ERROR;
    }

    // get the current working directory path and print it right away
    std::cout << fs::current_path() << std::endl;

    return _SUCCESS;
}

int Command_Impl::cd(const std::vector<std::string>& args) {
    if (args.size() > 1)
    {
        std::cerr << "Error: Too many arguments." << std::endl;
        return _ERROR;
    }

    if (args.empty())
    {
        std::cerr << "Error: No arguments provided." << std::endl;
        return _ERROR;
    }

    const std::string& path = args[0];

    if (!fs::exists(path))
    {
        std::cerr << "Error: Path does not exist." << std::endl;
        return _ERROR;
    }

    if (!fs::is_directory(path))
    {
        std::cerr << "Error: Path is not a directory." << std::endl;
        return _ERROR;
    }

    fs::current_path(path);
    return _SUCCESS;
}

inline int Command_Impl::ls(const std::vector<std::string>& args) {
    if (args.empty())
        return this->recdir(fs::current_path().string());

    if (args.size() > 1)
    {
        std::cerr << "Error: Too many arguments." << std::endl;
        return _ERROR;
    }

    return this->recdir(args[0]);
}

int Command_Impl::mkdir(const std::vector<std::string>& args) {
    if (args.empty())
    {
        std::cerr << "Error: No arguments provided." << std::endl;
        return _ERROR;
    }

    for (const auto& dir : args)
    {
        if (!fs::create_directory(dir))
        {
            std::cerr << "Error: Unable to create directory " << dir << "." << std::endl;
            return _ERROR;
        }
    }

    return _SUCCESS;
}

inline int Command_Impl::rmdir(const std::vector<std::string>& args) {
    if (args.empty())
    {
        std::cerr << "Error: No arguments provided." << std::endl;
        return _ERROR;
    }

    for (const auto& dir : args)
    {
        if (!fs::remove(dir))
        {
            std::cerr << "Error: Unable to remove directory " << dir << "." << std::endl;
            return _ERROR;
        }
    }

    return _SUCCESS;
}

int Command_Impl::rm(const std::vector<std::string>& args) {
    if (args.empty())
    {
        std::cerr << "Error: No arguments provided." << std::endl;
        return _ERROR;
    }

    for (const auto& file : args)
    {
        if (!fs::is_regular_file(file))
        {
            std::cerr << "Error: " << file << " is not a regular file." << std::endl;
            return _ERROR;
        }

        if (!fs::remove(file))
        {
            std::cerr << "Error: Unable to remove file " << file << "." << std::endl;
            return _ERROR;
        }
    }

    return _SUCCESS;
}

int Command_Impl::cp(const std::vector<std::string>& args) {
    if (args.size() != 2)
    {
        std::cerr << "Error: Invalid number of arguments." << std::endl;
        return _ERROR;
    }

    const std::string& source = args[0];
    const std::string& dest   = args[1];

    if (!fs::exists(source))
    {
        std::cerr << "Error: Source file does not exist." << std::endl;
        return _ERROR;
    }

    if (fs::is_directory(dest))
        fs::copy(source, dest / fs::path(source).filename());
    else
        fs::copy(source, dest);

    return _SUCCESS;
}

int Command_Impl::mv(const std::vector<std::string>& args) {
    if (args.size() != 2)
    {
        std::cerr << "Error: Invalid number of arguments." << std::endl;
        return _ERROR;
    }

    const std::string& source = args[0];
    const std::string& dest   = args[1];

    if (!fs::exists(source))
    {
        std::cerr << "Error: Source file does not exist." << std::endl;
        return _ERROR;
    }

    fs::path destPath(dest);
    if (fs::exists(destPath) && fs::is_directory(destPath))
        destPath /= fs::path(source).filename();

    try
    { fs::rename(source, destPath); } catch (const fs::filesystem_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return _ERROR;
    }

    return _SUCCESS;
}

// creates a new empty file
int Command_Impl::touch(const std::vector<std::string>& args) {
    // requires at least one file name
    if (args.empty())
    {
        std::cerr << "Error: No arguments provided." << std::endl;
        return _ERROR;
    }

    for (const auto& file : args)
    {
        std::ofstream outfile(file, std::ios::app | std::ios::out);
        if (!outfile)
        {
            std::cerr << "Error: Unable to create file " << file << "." << std::endl;
            return _ERROR;
        }
        outfile.close();
    }

    return _SUCCESS;
}

// I have no idea why they named it 'cat' (other than the fact that they love cats)
// prints the content of a file (this one is constrained by the buffer size)
// the precedure is too obvious to explain hehehe
int Command_Impl::cat(const std::vector<std::string>& args) {
    if (args.empty())
    {
        std::cerr << "Error: No arguments provided." << std::endl;
        return _ERROR;
    }

    for (const auto& file : args)
    {
        if (!fs::exists(file))
        {
            std::cerr << "Error: File " << file << " does not exist." << std::endl;
            return _ERROR;
        }

        std::ifstream infile(file);
        if (!infile)
        {
            std::cerr << "Error: Unable to open file " << file << "." << std::endl;
            return _ERROR;
        }

        std::cout << infile.rdbuf();
        infile.close();
    }

    return _SUCCESS;
}

// print text to a file or to stdout
int Command_Impl::echo(const std::vector<std::string>& args) {
    // requires at least one argument
    if (args.size() == 0)
    {
        std::cerr << "Error: Invalid arguments." << std::endl;
        return _ERROR;
    }

    // will check whether to print to file or to stdout
    bool        found_file = false;
    std::string filename;  // self explanatory

    for (const std::string& arg : args)
    {
        // operators to print to a file
        if (arg == ">" || arg == ">>")
        {
            found_file = true;  // toggle the switch

            // if no filepath provided
            if (arg == args[args.size() - 1])
            {
                std::cerr << "No filepath  provided" << std::endl;
                return _ERROR;
            }
        }
        else
        {
            // if a filepath is provided
            if (found_file && arg == args[args.size() - 1])
            {
                filename = arg;
                break;  // search ended
            }
            // many files are not allowed
            else if (found_file)
            {
                std::cerr << "Error: Too many arguments." << std::endl;
                return _ERROR;
            }
        }
    }

    // print to stdout
    if (!found_file)
    {
        for (const std::string& arg : std::vector(args.begin(), args.end()))
            std::cout << arg << " ";
        std::cout << std::endl;
    }
    // print to a file
    else
    {
        std::ofstream out_file(filename, std::ios::out | std::ios::app);
        if (out_file.is_open())
        {
            for (const std::string& arg : std::vector(args.begin(), args.end() - 2))
                out_file << arg << " ";
        }
        else
        {
            std::cerr << "Error: Unable to open file for writing." << std::endl;
            return _ERROR;
        }
    }

    return _SUCCESS;
}

inline int Command_Impl::clear(const std::vector<std::string>& args) {
    if (args.size() != 0)
    {
        std::cerr << "Error: Too many arguments." << std::endl;
        return _ERROR;
    }

    std::cout << _CLEAR;
    return _SUCCESS;
}

// open a file or directory
int Command_Impl::_open(const std::vector<std::string>& args) {
    if (args.empty())
    {
        std::cerr << "Error: not enough arguments." << std::endl;
        return _ERROR;
    }

    if (!fs::exists(args[0]))
    {
        std::cerr << "Error: path does not exist." << std::endl;
        return _ERROR;
    }

    int dev_null = open("/dev/null", O_RDWR);

    if (dev_null == -1)
    {
        perror("open");
        _exit(EXIT_FAILURE);
    }

    dup2(dev_null, STDIN_FILENO);
    dup2(dev_null, STDOUT_FILENO);
    dup2(dev_null, STDERR_FILENO);

    close(dev_null);

    if (execlp("open", "open", args[0].c_str(), NULL) == -1)
    {
        perror("execvp");
        _exit(EXIT_FAILURE);
    }

    return _SUCCESS;
}

int Command_Impl::run(const std::vector<std::string>& args) {
    if (args.empty())
    {
        std::cerr << "Error: not enough arguments." << std::endl;
        return _ERROR;
    }

    if (!fs::exists(args[0]))
    {
        std::cerr << "Error: path does not exist." << std::endl;
        return _ERROR;
    }

    // assert file is an executable
    if (access(args[0].c_str(), X_OK) != 0)
    {
        std::cerr << "Could not execute file : " << args[0] << std::endl;
        return _ERROR;
    }

    // construct the executable command
    // for the system call - exporting
    // adding the './' to the command
    // this discards the fact that you can in normal
    // shells execute a file that is not in the cwd
    std::string exec_cmd = "./" + args[0];

    // do the syscall
    execl(exec_cmd.c_str(), args[0].c_str(), (char*) NULL);

    return _SUCCESS;
}

int Command_Impl::head(const std::vector<std::string>& args) {}