PROJECT_NAME := odroid-go-examples
PROJECT_ARDUINO_LIBRARY_PATH := $(CURDIR)/libraries

include $(IDF_PATH)/make/project.mk

CFLAGS += -I$(PROJECT_PATH)/components/ODROID_GO/src

$(info CFLAGS: ${CFLAGS})
$(info CXXFLAGS: ${CXXFLAGS})
