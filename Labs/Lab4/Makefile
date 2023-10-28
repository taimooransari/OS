CC = gcc
SRC = main.c my_stack.c my_linkedlist.c
OUT = prog.o

build: $(SRC)
	$(CC) $(SRC) -o $(OUT)

rebuild: clean build

run: build
	./$(OUT)
clean: 
	rm -f $(OUT)