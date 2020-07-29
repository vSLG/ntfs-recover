# SPDX-License-Identifier: LGPL-3.0-or-later

PHONY := __all
__all:

MAKEFLAGS := --no-print-directory

srctree := .
objtree := obj
bintree := bin

export srctree objtree bintree

include scripts/Makefile.include.sub

SHELL 	= sh
CC 		= gcc
CXX 	= g++
AR 		= ar
OBJCOPY = objcopy

INCLUDE_DIR   := -I$(srctree)/include
BUILD_CFLAGS  := -Wall -Wundef -O2 $(CFLAGS)
BUILD_CXXFLAGS := -g -Wall -O2 -std=c++17 $(CXXFLAGS)

export BUILD_CFLAGS BUILD_CXXFLAGS INCLUDE_DIR SHELL CC AR CXX AR OBJCOPY

link_libraries :=
build-dirs     := src
final-builtin  := $(objtree)/built-in.a
bin-name 	   := $(bintree)/main


PHONY += $(build-dirs) $(final-builtin)
$(build-dirs):
	@$(MAKE) $(build)=$@

$(final-builtin): $(build-dirs)
	$(AR) cDPrTSs $@ $(addsuffix /built-in.a,$(addprefix $(objtree)/,$^))

__all: build
	
PHONY += build run clean

build: $(bin-name)

$(bin-name): $(final-builtin)
	$(shell mkdir -p $(bintree))
	$(CXX) $(link_libraries) -o $@ $<

run: build
	@$(bin-name)

clean:
	-rm -rf $(objtree)
	-rm -rf $(bintree)

.PHONY: $(PHONY)