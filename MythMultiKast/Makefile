CXX=g++
CC=gcc
CPPFLAGS=-I. -I/usr/include/SDL2 -fpermissive -Wdeprecated-declarations -w -O3
CPPSOURCES=$(wildcard *.cpp)
CPPOBJECTS=$(CPPSOURCES:.cpp=.o)
CSOURCES=$(wildcard *.c)
COBJECTS=$(CSOURCES:.c=.o)
TARGET=mythmultikast

.PHONY: all clean

all: .dcpp .dc $(CPPSOURCES) $(CSOURCES) $(TARGET)

.dcpp: $(CPPSOURCES)
	$(CXX) $(CPPFLAGS) -MM $(CPPSOURCES) >.dcpp
.dc: $(CSOURCES)
	$(CC) $(CFLAGS) -MM $(CSOURCES) >.dc
-include .dcpp
-include .dc
$(TARGET): $(CPPOBJECTS) $(COBJECTS)
	$(CXX) $(CPPOBJECTS) $(COBJECTS) -o $@ -L/usr/lib -lSDL2 -lSDL2_net -L/usr/local/lib/ 

clean:
	rm $(CPPOBJECTS) $(COBJECTS) .dcpp .dc
install:
	cp $(TARGET) /usr/local/bin
