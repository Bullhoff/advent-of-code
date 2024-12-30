CXX=g++
DAY?=01
_DAY:=$(shell printf '%02d' $(DAY))
MAKE += --no-print-directory

ifeq ($(OS),Windows_NT)
    MKDIR = if not exist build mkdir build
else
    MKDIR = mkdir -p build
endif

.PHONY: default all build run

default: build run

all:
	@for number in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25; do \
		if [ -f day$$number.cpp ]; then \
			$(MAKE) _DAY=$$number build run; \
		fi; \
	done

build:
	@$(MKDIR)
	@$(CXX) -o build/day$(_DAY) day$(_DAY).cpp

run:
	@build/day$(_DAY)

