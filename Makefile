# neste template de Makefile so muda a lista
# dos SOURCES e o nome do EXECUTABLE.

CC=g++
CFLAGS=-c -Wall -Weffc++ -Wextra -pedantic -std=c++0x   
LDFLAGS= -lgsl -lgslcblas -lm -lpthread
SOURCES= src/utils.cpp src/chaos.cpp src/main.cpp src/rp.cpp src/rqa.cpp src/time_series.cpp src/statistics.cpp
 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=time_series


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC)  $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

