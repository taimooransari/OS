position: 
	gcc -Wall -fPIC -c insert.c
	gcc -Wall -fPIC -c print.c
	gcc -Wall -fPIC -c remove.c
shared: position
	gcc -shared -o linkedlist.so insert.o print.o remove.o
copy: shared
	sudo cp linkedlist.so /usr/lib
	sudo ldconfig
compile: copy
	gcc -Wall -c task_6_1.c
	gcc -Wall -o a.out task_6_1.o linkedlist.so
run: compile
	./a.out
clean: 
	rm -rf *.o a.out
