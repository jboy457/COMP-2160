TARGET=main3
OBJECTS=main3.o REO.o

CC=clang++
CFLAGS=-Wall

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)


clean:
	rm -f $(OBJECTS) $(TARGET)