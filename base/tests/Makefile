CC=g++
CFLAGS=-c -Wall -std=c++11 -I/usr/local/Cellar/leveldb/1.18/include/
LIBS=-L/usr/local/Cellar/leveldb/1.18/lib/ -lleveldb
SOURCES=tests_record.cpp \
    tests_storage.cpp \
    ../Storage.cpp \
	../Record.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=tests

# /usr/local/Cellar/leveldb/1.18
# /usr/local/Cellar/leveldb/1.18/include/

all: $(EXECUTABLE) $(OBJECTS)
	./$(EXECUTABLE)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

clean:
	rm -rf *.o tests ../*.o test.db
