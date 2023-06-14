ifeq '$(findstring ;,$(PATH))' ';'
    detected_OS := Windows
else
    detected_OS := $(shell uname 2>/dev/null || echo Unknown)
    detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
    detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
    detected_OS := $(patsubst MINGW%,MSYS,$(detected_OS))
endif

ifeq ($(detected_OS),MSYS)
    echo Detected Windows
endif

SRC_DIR = src
INCLUDE_SRC_DIR = include
BUILD_DIR = build/
DEBUG_RELEASE = debug
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(INCLUDE_SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp) static/argspp/src/args.cpp
INCLUDE_PATHS = -I static/json/include -I static/argspp/src -I static/glob/single_include/glob  -I static/zstr/src
COMPILER_FLAGS = -std=c++20 -Wall -O0 -Wno-deprecated -Wno-unknown-pragmas
LIBRARY_PATHS = 
LINKER_FLAGS = 
DEBUG_FLAG = -g
EXEC_NAME = wadarchive

ifeq ($(detected_OS),MSYS)    
    LINKER_FLAGS = -lmingw32 	
endif

# Build release binary
binary: clean
	echo Detected OS: ${detected_OS}
	echo Built $(DEBUG_RELEASE) to dir: $(BUILD_DIR)/$(DEBUG_RELEASE)
	$(CC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) -o $(BUILD_DIR)/$(DEBUG_RELEASE)/$(EXEC_NAME) $(SRC_FILES) $(LINKER_FLAGS) $(COMPILER_FLAGS) $(DEBUG_FLAG)

release: setrelease binary
	echo BUILT release

debugrelease: setdebugrelease binary
	echo BUILT release with debug flags

setdebugrelease:
	DEBUG_RELEASE = release	

setrelease:
	DEBUG_RELEASE = release
	DEBUG_FLAG = 
	
# Clean build folder and extraction folder
clean:	
	rm -rf $(BUILD_DIR)/$(DEBUG_RELEASE)/*
	rm -rf test/testoutput/extraction/*
	rm -rf test/testoutput/extraction/images
