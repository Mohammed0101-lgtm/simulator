#include "io/file.hh"


namespace kernel {
namespace io {

ssize_t File::read(char* buffer, size_t size) {
    // Default implementation: do nothing
    return 0;
}

ssize_t File::write(const char* buffer, size_t size) {
    // Default implementation: do nothing
    return 0;
}

void File::close() {
    // Default implementation: do nothing
}

ssize_t ConsoleFile::read(char* buffer, size_t size) {
    // Read from console (stub implementation)
    std::cin.read(buffer, size);
    return std::cin.gcount();
}

ssize_t ConsoleFile::write(const char* buffer, size_t size) {
    // Write to console (stub implementation)
    std::cout.write(buffer, size);
    return size;
}

void ConsoleFile::close() {
    // Close console (stub implementation)
    std::cout.flush();
}

ssize_t TextFile::read(char* buffer, size_t size) {
    // read from text file (stub implementation)
}

ssize_t TextFile::write(const char* buffer, size_t size) {
    // write to text file (stub implementation)
}

void TextFile::close() {
    // close text file
}

}  // namespace io
}  // namespace kernel::io