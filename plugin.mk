# Add URack files to the build
SOURCES += $(wildcard $(URACK_DIR)/src/*.cpp)
FLAGS += -I$(URACK_DIR)/include -Wfatal-errors

# Add OSC lib src
SOURCES += $(wildcard $(URACK_DIR)/lib/oscpack/ip/*.cpp)
SOURCES += $(wildcard $(URACK_DIR)/lib/oscpack/osc/*.cpp)

# Add OSC lib platform specific src
include $(RACK_DIR)/arch.mk
ifeq ($(ARCH), win)
	SOURCES += $(wildcard $(URACK_DIR)/lib/oscpack/ip/win32/*.cpp)
	LDFLAGS += -lws2_32 -lwinmm
	LDFLAGS +=  -L$(RACK_DIR)/dep/lib
else
	SOURCES += $(wildcard $(URACK_DIR)/lib/oscpack/ip/posix/*.cpp)
endif
