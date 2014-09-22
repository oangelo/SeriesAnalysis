# Generic Makefile for compiling a simple executable.

CC := g++ 
SRCDIR := src
BUILDDIR := build
CFLAGS := -g -Wall -std=c++0x  -Weffc++ -Wextra -pedantic
LDFLAGS= -lstdc++ -lm -lpstatistics  -lboost_program_options 
TARGET := time_series 
SRCEXT := cpp
BINDIR := /usr/bin

SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DIRS = $(shell find $(SRCDIR) -type d)

HEADERS := $(shell find $(SRCDIR) -type f -name *.h)


.PHONY: bin 
bin:$(TARGET)
$(TARGET):$(OBJECTS)
	$(CC) -o $(TARGET) $? $(LDFLAGS)   

vpath %.$(SRCEXT) $(DIRS)
$(BUILDDIR)/%.o: %.$(SRCEXT) 
	@mkdir -p  $(shell dirname $@)
	$(CC) $(CFLAGS) $< -o $@  -c 

.PHONY: clean
clean:
	@echo " Cleaning..."; $(RM) -r $(BUILDDIR) $(TARGET)
