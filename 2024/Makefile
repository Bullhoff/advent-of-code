CXX=g++
DAY=01
_DAY:=$(shell printf '%02d' $(DAY))

all: build run

build:
	$(CXX) -o build/day$(_DAY) day$(_DAY).cpp

run:
	@build/day$(_DAY)

