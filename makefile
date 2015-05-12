#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = -IC:/Development/SDL/SDL2-2.0.3/i686-w64-mingw32/include/SDL2 -IC:/Development/SDL/SDL2_image-2.0.0/i686-w64-mingw32/include/SDL2 -IC:/Development/SDL/SDL2_ttf-2.0.12/i686-w64-mingw32/include/SDL2 -IC:/Development/SDL/SDL2_mixer-2.0.0/i686-w64-mingw32/include/SDL2 -IC:/Development/SDL/SDL2_net-2.0.0/i686-w64-mingw32/include/SDL2
CFLAGS = -std=c++11 -fexceptions
RESINC = 
LIBDIR = -LC:/Development/SDL/SDL2-2.0.3/i686-w64-mingw32/lib -LC:/Development/SDL/SDL2_image-2.0.0/i686-w64-mingw32/lib -LC:/Development/SDL/SDL2_ttf-2.0.12/i686-w64-mingw32/lib -LC:/Development/SDL/SDL2_mixer-2.0.0/i686-w64-mingw32/lib -LC:/Development/SDL/SDL2_net-2.0.0/i686-w64-mingw32/lib
LIB = 
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net

INC_DEBUG = $(INC) -Iinclude
CFLAGS_DEBUG = $(CFLAGS) -Wnon-virtual-dtor -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -pg -g
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS) -pg -lgmon
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/Test

INC_RELEASE = $(INC) -Iinclude
CFLAGS_RELEASE = $(CFLAGS) -O2 -Wfatal-errors
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/Test

OBJ_DEBUG = $(OBJDIR_DEBUG)/main.o $(OBJDIR_DEBUG)/src/sdltexture.o $(OBJDIR_DEBUG)/src/sdlimgerror.o $(OBJDIR_DEBUG)/src/sdlerror.o $(OBJDIR_DEBUG)/src/sdl.o $(OBJDIR_DEBUG)/src/renderer.o $(OBJDIR_DEBUG)/src/liquidsim.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/main.o $(OBJDIR_RELEASE)/src/sdltexture.o $(OBJDIR_RELEASE)/src/sdlimgerror.o $(OBJDIR_RELEASE)/src/sdlerror.o $(OBJDIR_RELEASE)/src/sdl.o $(OBJDIR_RELEASE)/src/renderer.o $(OBJDIR_RELEASE)/src/liquidsim.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)
	test -d $(OBJDIR_DEBUG)/src || mkdir -p $(OBJDIR_DEBUG)/src

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/main.o: main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.cpp -o $(OBJDIR_DEBUG)/main.o

$(OBJDIR_DEBUG)/src/sdltexture.o: src/sdltexture.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/sdltexture.cpp -o $(OBJDIR_DEBUG)/src/sdltexture.o

$(OBJDIR_DEBUG)/src/sdlimgerror.o: src/sdlimgerror.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/sdlimgerror.cpp -o $(OBJDIR_DEBUG)/src/sdlimgerror.o

$(OBJDIR_DEBUG)/src/sdlerror.o: src/sdlerror.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/sdlerror.cpp -o $(OBJDIR_DEBUG)/src/sdlerror.o

$(OBJDIR_DEBUG)/src/sdl.o: src/sdl.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/sdl.cpp -o $(OBJDIR_DEBUG)/src/sdl.o

$(OBJDIR_DEBUG)/src/renderer.o: src/renderer.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/renderer.c -o $(OBJDIR_DEBUG)/src/renderer.o

$(OBJDIR_DEBUG)/src/liquidsim.o: src/liquidsim.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/liquidsim.cpp -o $(OBJDIR_DEBUG)/src/liquidsim.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)
	rm -rf $(OBJDIR_DEBUG)/src

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)
	test -d $(OBJDIR_RELEASE)/src || mkdir -p $(OBJDIR_RELEASE)/src

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/main.o: main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.cpp -o $(OBJDIR_RELEASE)/main.o

$(OBJDIR_RELEASE)/src/sdltexture.o: src/sdltexture.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/sdltexture.cpp -o $(OBJDIR_RELEASE)/src/sdltexture.o

$(OBJDIR_RELEASE)/src/sdlimgerror.o: src/sdlimgerror.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/sdlimgerror.cpp -o $(OBJDIR_RELEASE)/src/sdlimgerror.o

$(OBJDIR_RELEASE)/src/sdlerror.o: src/sdlerror.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/sdlerror.cpp -o $(OBJDIR_RELEASE)/src/sdlerror.o

$(OBJDIR_RELEASE)/src/sdl.o: src/sdl.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/sdl.cpp -o $(OBJDIR_RELEASE)/src/sdl.o

$(OBJDIR_RELEASE)/src/renderer.o: src/renderer.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/renderer.c -o $(OBJDIR_RELEASE)/src/renderer.o

$(OBJDIR_RELEASE)/src/liquidsim.o: src/liquidsim.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/liquidsim.cpp -o $(OBJDIR_RELEASE)/src/liquidsim.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)
	rm -rf $(OBJDIR_RELEASE)/src

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

