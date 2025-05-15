#ifndef __FILE_H__
#define __FILE_H__

#include <iostream>


namespace kernel {
namespace io {

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

}  // namespace io
}  // namespace kernel

#endif  // __FILE_H__