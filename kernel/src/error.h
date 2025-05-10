#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdexcept>
#include <string>


namespace kernel {
namespace error {


/// Custom exception class representing a segmentation fault (e.g., invalid memory access).
class Segmentation_fault: public std::exception
{
   private:
    std::string imp_;  ///< Internal message providing extra context (e.g., address or reason).

   public:
    /// Construct with a detailed string message.
    explicit Segmentation_fault(const std::string& msg) noexcept :
        imp_(msg) {}

    /// Construct with a C-style string message.
    explicit Segmentation_fault(const char* msg) noexcept :
        imp_(msg) {}

    /// Copy constructor.
    Segmentation_fault(const Segmentation_fault& other) noexcept :
        imp_(other.imp()) {}

    /// Copy assignment operator.
    Segmentation_fault& operator=(const Segmentation_fault& other) noexcept {
        if (this != &other)
            imp_ = other.imp();
        return *this;
    }

    /// Destructor.
    ~Segmentation_fault() = default;

    /// Returns a short description of the error (used by std::exception).
    const char* what() const noexcept override { return "Segmentation fault occurred"; }

    /// Returns the detailed internal message.
    const std::string& imp() const { return imp_; }
};


/// Custom exception class representing a page fault (e.g., accessing an unmapped virtual page).
class Page_fault: public std::exception
{
   private:
    std::string imp_;  ///< Internal message providing additional details (e.g., virtual address).

   public:
    /// Construct with a detailed string message.
    explicit Page_fault(const std::string& msg) noexcept :
        imp_(msg) {}

    /// Construct with a C-style string message.
    explicit Page_fault(const char* msg) noexcept :
        imp_(msg) {}

    /// Copy constructor.
    Page_fault(const Page_fault& other) noexcept :
        imp_(other.imp()) {}

    /// Copy assignment operator.
    Page_fault& operator=(const Page_fault& other) noexcept {
        if (this != &other)
            imp_ = other.imp();
        return *this;
    }

    /// Destructor.
    ~Page_fault() = default;

    /// Returns a short description of the error (used by std::exception).
    const char* what() const noexcept override { return "Page fault occurred"; }

    /// Returns the detailed internal message.
    const std::string& imp() const { return imp_; }
};

/// Custom exception class representing a page fault (e.g., accessing an unmapped virtual page).
class Unknown_instruction: public std::exception
{
   private:
    std::string imp_;  ///< Internal message providing additional details (e.g., virtual address).

   public:
    /// Construct with a detailed string message.
    explicit Unknown_instruction(const std::string& msg) noexcept :
        imp_(msg) {}

    /// Construct with a C-style string message.
    explicit Unknown_instruction(const char* msg) noexcept :
        imp_(msg) {}

    /// Copy constructor.
    Unknown_instruction(const Unknown_instruction& other) noexcept :
        imp_(other.imp()) {}

    /// Copy assignment operator.
    Unknown_instruction& operator=(const Unknown_instruction& other) noexcept {
        if (this != &other)
            imp_ = other.imp();
        return *this;
    }

    /// Destructor.
    ~Unknown_instruction() = default;

    /// Returns a short description of the error (used by std::exception).
    const char* what() const noexcept override { return "Unknown instruction is not allowed."; }

    /// Returns the detailed internal message.
    const std::string& imp() const { return imp_; }
};

class Unsupported_template: public std::exception
{
   private:
    std::string imp_;  ///< Internal message providing additional details (e.g., virtual address).

   public:
    /// Construct with a detailed string message.
    explicit Unsupported_template(const std::string& msg) noexcept :
        imp_(msg) {}

    /// Construct with a C-style string message.
    explicit Unsupported_template(const char* msg) noexcept :
        imp_(msg) {}

    /// Copy constructor.
    Unsupported_template(const Unsupported_template& other) noexcept :
        imp_(other.imp()) {}

    /// Copy assignment operator.
    Unsupported_template& operator=(const Unsupported_template& other) noexcept {
        if (this != &other)
            imp_ = other.imp();
        return *this;
    }

    /// Destructor.
    ~Unsupported_template() = default;

    /// Returns a short description of the error (used by std::exception).
    const char* what() const noexcept override { return "Unsupported template error."; }

    /// Returns the detailed internal message.
    const std::string& imp() const { return imp_; }
};

class Panic_kernel: public std::exception
{
   private:
    std::string imp_;  ///< Internal message providing additional details (e.g., virtual address).

   public:
    /// Construct with a detailed string message.
    explicit Panic_kernel(const std::string& msg) noexcept :
        imp_(msg) {}

    /// Construct with a C-style string message.
    explicit Panic_kernel(const char* msg) noexcept :
        imp_(msg) {}

    /// Copy constructor.
    Panic_kernel(const Panic_kernel& other) noexcept :
        imp_(other.imp()) {}

    /// Copy assignment operator.
    Panic_kernel& operator=(const Panic_kernel& other) noexcept {
        if (this != &other)
            imp_ = other.imp();
        return *this;
    }

    /// Destructor.
    ~Panic_kernel() = default;

    /// Returns a short description of the error (used by std::exception).
    const char* what() const noexcept override { return "!!! Kernel Panic !!!"; }

    /// Returns the detailed internal message.
    const std::string& imp() const { return imp_; }
};


}  // namespace error
}  // namespace kernel

#endif  // __ERROR_H__
