.PHONY : clean run

CC = gcc
CFLAGS = -g -Wall
TARGET = driver

run: driver
	./driver

$(TARGET) : test-bst.o bst.o
	$(CC) -o $(TARGET) test-bst.o bst.o $(CFLAGS)

clean:
	rm -f *.o
	rm -f $(TARGET)
check: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)