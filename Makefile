SRC_DIR = src
INCLUDE_SRC_DIR = include
BUILD_DIR = build
DEBUG_RELEASE = debug
CC = g++
LIB_SRC_FILES = $(wildcard $(INCLUDE_SRC_DIR)/*.cpp) 
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(INCLUDE_SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp) static/argspp/src/args.cpp
LIB_INCLUDE_PATHS = -I static/json/include
INCLUDE_PATHS = -I static/json/include -I static/argspp/src
COMPILER_FLAGS = -std=c++20 -Wall -O3 -Wsign-compare -Wuninitialized -Wmissing-field-initializers -Wextra -Werror -Winit-self
LIBRARY_PATHS = 
LINKER_FLAGS = 
DEBUG_FLAG = -g
EXEC_NAME = wadarchive

ifeq ($(detected_OS),MSYS)    
    LINKER_FLAGS = -lmingw32 	
endif

# Build release binary
binary: clean
	@echo Detected OS: ${detected_OS}
	@echo Building $(DEBUG_RELEASE) to dir: $(BUILD_DIR)/$(DEBUG_RELEASE)...
	$(CC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) -o $(BUILD_DIR)/$(DEBUG_RELEASE)/$(EXEC_NAME) $(SRC_FILES) $(LINKER_FLAGS) $(COMPILER_FLAGS) $(DEBUG_FLAG)

release: setrelease binary
	@echo BUILT release

debugrelease: setdebugrelease binary
	@echo BUILT release with debug flags

setdebugrelease:
	$(eval DEBUG_RELEASE := release)

setrelease:
	$(eval DEBUG_RELEASE := release)
	$(eval DEBUG_FLAG := )	
	
# Clean build folder and extraction folder
clean:	
	rm -rf $(BUILD_DIR)/$(DEBUG_RELEASE)/*
	rm -rf test/testoutput/extraction/*
	rm -rf test/testoutput/extraction/images
