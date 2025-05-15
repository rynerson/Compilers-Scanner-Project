# Compiler
CXX = g++
CXXFLAGS = -Wall -g -std=c++20

# Source files
SRCS = main.cpp scanner.cpp
# Object files
OBJS = $(SRCS:.cpp=.o)
# Executable name
EXEC = scanner.exe
# Input file
IN = test.b

# Default target
all: $(EXEC)

# Link the object files to create the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS)

# Compile each .cpp file to a .o file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean up generated files
clean:
	rm -f $(OBJS) $(EXEC)

# Run the program with the input file
run: $(EXEC)
	./$(EXEC) $(IN)

.PHONY: all clean run

