TARGET = ./a.out
FILES = main.o queue.o sorting.o file.o benchmark.o
.PHONY: all clean install unistall
all: $(TARGET)
run: 
	./$(TARGET)
main.o: main.c
	gcc -c -o main.o main.c
queue.o: queue.c
	gcc -c -o queue.o queue.c
sorting.o: sorting.c
	gcc -c -o sorting.o sorting.c
file.o: file.c
	gcc -c -o file.o file.c
benchmark.o: benchmark.c
	gcc -c -o benchmark.o benchmark.c
$(TARGET): main.o queue.o sorting.o benchmark.o file.o
	gcc -o $(TARGET) main.o queue.o sorting.o benchmark.o file.o