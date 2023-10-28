task1: 
	gcc reverse_unnamed_pipe.c -o task1.o
run1: 
	./task1.o
task2: 
	gcc fifo_write1.c -o write1.o 
	gcc fifo_reader1.c -o read1.o
	gcc fifo_reader2.c -o read2.o
	gcc fifo_reader3.c -o read3.o
write1:
	./write1.o
read1: 
	./read1.o
read2: 
	./read2.o
read3: 
	./read3.o
task3: 
	gcc p1.c -o p1.o
	gcc p2.c -o p2.o
p1: 
	./p1.o
p2: 
	./p2.o
full: 
	gcc full_p1.c -o f1.o
	gcc full_p2.c -o f2.o
f1: 
	./f1.o
f2: 
	./f2.o
clean: 
	rm -rf *.o