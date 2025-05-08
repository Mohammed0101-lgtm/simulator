//
// input.h
//

#ifndef __INPUT_H__
#define __INPUT_H__

#include <string>
#include <vector>

class Input_Handler
{
   public:
    // Constructor and Destructor
    Input_Handler()  = default;
    ~Input_Handler() = default;

    // reading functions
    std::string read_line() const;
    // Helper function to tokenize the input string
    // based on whitespace and special characters
    std::vector<std::string> tokenize(const std::string& str) const;
};

#endif  // __INPUT_H__