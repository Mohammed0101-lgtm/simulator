#ifndef __SHELL_ERROR_H__
#define __SHELL_ERROR_H__


#include <stdexcept>

class Command_error: public std::exception
{
   private:
    std::string imp_;

   public:
    explicit Command_error(const std::string& msg) noexcept :
        imp_(msg) {}
    explicit Command_error(const char* msg) noexcept :
        imp_(msg) {}

    Command_error(const Command_error& other) noexcept :
        imp_(other.imp()) {}

    Command_error& operator=(const Command_error& other) noexcept {
        if (this != &other)
            imp_ = other.imp();

        return *this;
    }

    ~Command_error() = default;

    const char* what() const noexcept override { return "Command error occurred."; }

    const std::string& imp() const { return imp_; }
};

class Unsupported_command: public std::exception
{
   private:
    std::string imp_;

   public:
    explicit Unsupported_command(const std::string& msg) noexcept :
        imp_(msg) {}
    explicit Unsupported_command(const char* msg) noexcept :
        imp_(msg) {}

    Unsupported_command(const Unsupported_command& other) noexcept :
        imp_(other.imp()) {}

    Unsupported_command operator=(const Unsupported_command& other) noexcept {
        if (this != &other)
            imp_ = other.imp();

        return *this;
    }

    ~Unsupported_command() = default;

    const char* what() const noexcept override { return "Unsupported Command detected."; }

    const std::string& imp() const { return imp_; }
};


#endif  // __SHELL_ERROR_H__
