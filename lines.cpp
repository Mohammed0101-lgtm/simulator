#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>


int main() {
    std::string directory = "/Users/mohamedrabbit/simulator/";  // Ensure this is correct

    try
    {
        if (!std::filesystem::is_directory(directory))
        {
            std::cerr << "Error: " << directory << " is not a valid directory." << std::endl;
            return 1;
        }

        std::size_t count        = 0;
        std::size_t num_of_files = 0;

        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
        {
            if (entry.is_regular_file())
            {  // Process only files, not directories
                if (not(entry.path().extension() == ".mm" or entry.path().extension() == ".cc"
                        or entry.path().extension() == ".cpp" or entry.path().extension() == ".c"
                        or entry.path().extension() == ".h" or entry.path().extension() == ".hpp"))
                {
                    continue;
                }

                std::ifstream file(entry.path());  // Use entry.path() to get the correct path

                if (!file.is_open())
                {
                    std::cerr << "Warning: Could not open file: " << entry.path() << std::endl;
                    continue;  // Skip unreadable files instead of throwing
                }

                std::string line;
                std::size_t sub_count = 0;

                while (getline(file, line))
                {
                    sub_count++;
                }

                count += sub_count;
                std::cout << "File processed : " << entry.path() << " | lines = " << sub_count << '\n';
                num_of_files++;
            }
        }

        std::cout << "Total lines: " << count << std::endl;
        std::cout << "Total files: " << num_of_files << std::endl;
    } catch (const std::filesystem::filesystem_error& e)
    {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}