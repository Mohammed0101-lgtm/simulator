#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdexcept>
#include <string>


namespace kernel {
namespace error {


/// Custom exception class representing a segmentation fault (e.g., invalid memory access).
class segmentation_fault: public std::exception
{
   private:
    std::string imp_;  ///< Internal message providing extra context (e.g., address or reason).

   public:
    /// Construct with a detailed string message.
    explicit segmentation_fault(const std::string& msg) noexcept :
        imp_(msg) {}

    /// Construct with a C-style string message.
    explicit segmentation_fault(const char* msg) noexcept :
        imp_(msg) {}

    /// Copy constructor.
    segmentation_fault(const segmentation_fault& other) noexcept :
        imp_(other.imp()) {}

    /// Copy assignment operator.
    segmentation_fault& operator=(const segmentation_fault& other) noexcept {
        if (this != &other)
            imp_ = other.imp();
        return *this;
    }

    /// Destructor.
    ~segmentation_fault() = default;

    /// Returns a short description of the error (used by std::exception).
    const char* what() const noexcept override { return "Segmentation fault occurred"; }

    /// Returns the detailed internal message.
    const std::string& imp() const { return imp_; }
};


/// Custom exception class representing a page fault (e.g., accessing an unmapped virtual page).
class page_fault: public std::exception
{
   private:
    std::string imp_;  ///< Internal message providing additional details (e.g., virtual address).

   public:
    /// Construct with a detailed string message.
    explicit page_fault(const std::string& msg) noexcept :
        imp_(msg) {}

    /// Construct with a C-style string message.
    explicit page_fault(const char* msg) noexcept :
        imp_(msg) {}

    /// Copy constructor.
    page_fault(const page_fault& other) noexcept :
        imp_(other.imp()) {}

    /// Copy assignment operator.
    page_fault& operator=(const page_fault& other) noexcept {
        if (this != &other)
            imp_ = other.imp();
        return *this;
    }

    /// Destructor.
    ~page_fault() = default;

    /// Returns a short description of the error (used by std::exception).
    const char* what() const noexcept override { return "Page fault occurred"; }

    /// Returns the detailed internal message.
    const std::string& imp() const { return imp_; }
};

/// Custom exception class representing a page fault (e.g., accessing an unmapped virtual page).
class unknown_instruction: public std::exception
{
   private:
    std::string imp_;  ///< Internal message providing additional details (e.g., virtual address).

   public:
    /// Construct with a detailed string message.
    explicit unknown_instruction(const std::string& msg) noexcept :
        imp_(msg) {}

    /// Construct with a C-style string message.
    explicit unknown_instruction(const char* msg) noexcept :
        imp_(msg) {}

    /// Copy constructor.
    unknown_instruction(const unknown_instruction& other) noexcept :
        imp_(other.imp()) {}

    /// Copy assignment operator.
    unknown_instruction& operator=(const unknown_instruction& other) noexcept {
        if (this != &other)
            imp_ = other.imp();
        return *this;
    }

    /// Destructor.
    ~unknown_instruction() = default;

    /// Returns a short description of the error (used by std::exception).
    const char* what() const noexcept override { return "Unknown instruction is not allowed."; }

    /// Returns the detailed internal message.
    const std::string& imp() const { return imp_; }
};

class unsupported_template: public std::exception
{
   private:
    std::string imp_;  ///< Internal message providing additional details (e.g., virtual address).

   public:
    /// Construct with a detailed string message.
    explicit unsupported_template(const std::string& msg) noexcept :
        imp_(msg) {}

    /// Construct with a C-style string message.
    explicit unsupported_template(const char* msg) noexcept :
        imp_(msg) {}

    /// Copy constructor.
    unsupported_template(const unsupported_template& other) noexcept :
        imp_(other.imp()) {}

    /// Copy assignment operator.
    unsupported_template& operator=(const unsupported_template& other) noexcept {
        if (this != &other)
            imp_ = other.imp();
        return *this;
    }

    /// Destructor.
    ~unsupported_template() = default;

    /// Returns a short description of the error (used by std::exception).
    const char* what() const noexcept override { return "Unsupported template error."; }

    /// Returns the detailed internal message.
    const std::string& imp() const { return imp_; }
};


}  // namespace error
}  // namespace kernel

#endif  // __ERROR_H__
