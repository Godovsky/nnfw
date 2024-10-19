.PHONY: all run nslib

CC = gcc
CFLAGS = -Wall -std=c99
INCLUDE = -Iinclude
PROJECTNAME = neuronfw

ifeq ($(OS),Windows_NT)
	EXAMPLE = example.exe
    NSLIB = lib$(PROJECTNAME).dll
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            
        endif
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        EXAMPLE = example
        NSLIB = lib$(PROJECTNAME).so
    endif
    ifeq ($(UNAME_S),Darwin)
        
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        
    endif
endif

SRC = ./src
CFILES += $(SRC)/nnfw.c
CFILES += $(SRC)/train.c

LIBRARIES += -lm -l$(PROJECTNAME)

all: nslib $(EXAMPLE)

$(EXAMPLE): example.c
	@echo "Building $(@F)"
	@$(CC) $(CFLAGS) $(INCLUDE) $< -o $@ -L./ -Wl,-R./ $(LIBRARIES)

nslib: $(NSLIB)

$(NSLIB): $(CFILES)
	@echo "Building $(@F)"
	@$(CC) $(CFLAGS) $(INCLUDE) -shared $(CFILES) -o $@ -lm

run: $(EXAMPLE)
	@./$(EXAMPLE)
