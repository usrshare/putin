putin: main.o
	gcc main.o -g -o putin

main.o: main.c
	gcc -c main.c -g

clean:
	rm -rf *.o putin
