TARGET = main

OBJECTS = main.o set2.o

CC = clang
CFLAGS = -Wall -arch arm64

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJECTS) $(TARGET)