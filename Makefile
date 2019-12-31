# If RACK_DIR is not defined when calling the Makefile, default to two directories above
RACK_DIR ?= ../..

# FLAGS will be passed to both the C and C++ compiler
FLAGS +=
CFLAGS +=
CXXFLAGS +=

# Add .cpp files to the build
SOURCES += $(wildcard src/*.cpp)

# OSC lib
SOURCES += $(wildcard lib/oscpack/ip/*.cpp)
SOURCES += $(wildcard lib/oscpack/osc/*.cpp)

include $(RACK_DIR)/arch.mk

ifeq ($(ARCH), win)
	SOURCES += $(wildcard lib/oscpack/ip/win32/*.cpp) 
	LDFLAGS += -lws2_32 -lwinmm
	LDFLAGS +=  -L$(RACK_DIR)/dep/lib #-lglew32 -lglfw3dll
	#LDFLAGS += -lrtmidi
else
	SOURCES += $(wildcard lib/oscpack/ip/posix/*.cpp) 
endif

# Add files to the ZIP package when running `make dist`
# The compiled plugin and "plugin.json" are automatically added.
DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)

# Include the Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk
