# neste template de Makefile so muda a lista
# dos SOURCES e o nome do EXECUTABLE.

CC=g++
CFLAGS=-c -Wall -Weffc++ -Wextra -pedantic    
LDFLAGS= -lgsl -lgslcblas -lm -lpthread
SOURCES= src/utils.cpp src/chaos.cpp src/main.cpp src/recurrence_plot.cpp src/time_series.cpp src/statistics.cpp
 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=time_series


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC)  $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

