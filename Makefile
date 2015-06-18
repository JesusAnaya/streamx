CC:=clang++
CFLAGS:=-c -Wall -O2 -std=c++1y
LDFLAGS:=-lboost_system

OBJECTS:= src/main.o \
	src/server/handler_allocator.o
	src/server/server.o

TARGET:=server

all: $(OBJECTS) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -rf $(OBJECTS)
