# Project Name
TARGET = main

# Sources
CPP_SOURCES = main.cpp

# Library Locations
DAISYSP_DIR ?= ../../DaisySP
LIBDAISY_DIR ?= ../../libDaisy

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

