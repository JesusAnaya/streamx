CC:=clang++
CFLAGS:=-c -Wall -O2 -std=c++1y
LDFLAGS:=-lboost_system -v

OBJECTS:= \
	src/main.o \
	src/server.o

TARGET:=server

all: $(OBJECTS) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -rf $(OBJECTS)
