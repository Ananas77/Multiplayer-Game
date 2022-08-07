# Which compilers to use
# Will work with clang and clang++
CXX=g++
CC=gcc

# Flags
# Flags which are the same for C and C++ code
#      Give warnings about performance and compliance to standards, optimize, Add libraries
COMMON=-Wall -Wextra -Wpedantic                                     -O2       `pkg-config --cflags glfw3`
# These two lines just define which stdandards the compiler should enforce, 2011 standards are chosen here
CFLAGS=--std=c11 $(COMMON)
CXXFLAGS= -D_DEBUG --std=c++11 $(COMMON)
# Linker flags
#       Just libraries
LDFLAGS=`pkg-config --libs glfw3`

# Names of files
# Name of client executable
CLIENT=mpg-client
# Object files required to link the client executable
CLIENT-OBJS=obj/main.cpp.o obj/shader.cpp.o obj/camera.cpp.o obj/entities.cpp.o obj/glad.c.o

# Targets for a full build
all: client

# Client target, requires client executable to be completed
client: $(CLIENT)

# Client executable, required client objects to be compiled to be completed
$(CLIENT): $(CLIENT-OBJS)
	$(CXX) -g -o  $(CLIENT) $(CLIENT-OBJS) $(LDFLAGS)

# Client objects, require client source code
obj/%.cpp.o : %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# GLAD obejct, requires GLAD
obj/glad.c.o : lib/glad/glad.c
	$(CC) -c -o $@ $<

# Cleanup targets
clean: clean-client

clean-client:
	rm $(CLIENT-OBJS) $(CLIENT)
