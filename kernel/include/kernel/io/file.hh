#ifndef __FILE_H__
#define __FILE_H__

#include <iostream>


namespace kernel {
namespace io {

using FileDescriptor = unsigned int;


enum class FileType {
    REGULAR,
    DIRECTORY,
    CHAR,
    BLOCK,
    FIFO,
    SOCKET
};

enum class FileMode {
    READ,
    WRITE,
    APPEND,
    READ_WRITE
};

enum class FilePermission {
    READ,
    WRITE,
    EXECUTE
};

enum class FileStatus {
    OPEN,
    CLOSED,
    ERROR
};

enum class FileFlag {
    O_RDONLY,
    O_WRONLY,
    O_RDWR,
    O_APPEND,
    O_CREAT,
    O_TRUNC,
    O_EXCL
};

class FileBase
{
   private:
    kernel::io::FileFlag       _flag;
    kernel::io::FileStatus     _status;
    kernel::io::FileMode       _mode;
    kernel::io::FileType       _type;
    kernel::io::FileDescriptor _file_descriptor;

   public:
    ~FileBase() = delete;
    FileBase()  = delete;  // cannot use the default constructor
    explicit FileBase(kernel::io::FileType type, kernel::io::FileMode mode) noexcept;
};

class File
{
   public:
    virtual ssize_t read(char* buffer, size_t size);
    virtual ssize_t write(const char* buffer, size_t size);
    virtual void    close();
    virtual ~File() = default;
};

class ConsoleFile: public File
{
   public:
    ssize_t read(char* buffer, size_t size) override;
    ssize_t write(const char* buffer, size_t size) override;
    void    close() override;
};

class TextFile: public File
{
   public:
    ssize_t read(char* buffer, size_t size) override;
    ssize_t write(const char* buffer, size_t size) override;
    void    close() override;
};

}  // namespace io
}  // namespace kernel

#endif  // __FILE_H__