#ifndef __SHELL_H__
#define __SHELL_H__

#include <string>
#include <vector>


class Shell
{
   public:
    // Constructor and Destructor
    Shell()  = default;
    ~Shell() = default;

    // Shell functions
    void run();

   private:
    // Helper function to display the shell prompt
    void display_prompt() const;
};

#endif  // __SHELL_H__