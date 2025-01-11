TARGET=main

OBJECTS=main.o container.o

CC=clang
CFLAGS=-Wall

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

clean: 
	rm -f $(OBJECTS) $(TARGET)