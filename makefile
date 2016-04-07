UNAME_S := $(shell uname -s)

CC = g++
EXEC = exec
CCFLAGS = -std=c++11 -DGL_GLEXT_PROTOTYPES
INC =

ifeq ($(UNAME_S),Linux)
	LDFLAGS = -lGLEW -lGL -lSDL2 -lSDL2_image -lassimp
endif

ifeq ($(UNAME_S),Darwin)
	INC += -F/Library/Frameworks/ -I/usr/local/include -L/usr/local/lib -I/opt/local/include
	LDFLAGS = -lassimp -framework OpenGL -framework Cocoa -lSDL2 -lSDL2_image
endif

SRC_FILES = $(wildcard src/*.cpp)
H_FILES = $(wildcard src/*.hpp)
OBJS = $(SRC_FILES:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(INC) -o $(EXEC) $(LDFLAGS)

%.o: %.cpp %.hpp
	$(CC) $(CCFLAGS) $(INC) -c -o $@ $<

clean:
	rm -f $(EXEC) $(OBJS)
