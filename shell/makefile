# Compiler and flags
CXX     := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -pedantic -g

# Sources and headers
SRCS := main.cc command.cc shell.cc input.cc
OBJS := $(SRCS:.cc=.o)
DEPS := command.h shell.h input.h

# Output executable
TARGET := app

# Default target
all: $(TARGET)

# Link object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile .cc files to .o
%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

test: 
	clang++ -std=c++20 \
		-I /opt/homebrew/Cellar/googletest/1.17.0/include \
		-L /opt/homebrew/Cellar/googletest/1.17.0/lib \
		tests/test_cmd.cc command.cc input.cc shell.cc \
		-lgtest -lgtest_main -pthread -o test_shell
	./test_shell