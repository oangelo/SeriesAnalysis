# neste template de Makefile so muda a lista
# dos SOURCES e o nome do EXECUTABLE.

CC=g++
CFLAGS=-c -Wall 
LDFLAGS=-lUnitTest++  -lgsl -lgslcblas -lm -lpthread
SOURCES=   src/numerical_integration.cpp  src/teste_time_series.cpp src/utils.cpp src/chaos.cpp src/main.cpp            src/time_series.cpp   src/models.cpp          src/statistics.cpp             src/lib_exceptions.cpp
 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=time_series


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC)  $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

