#Jonchnell.com
#cpre308 proj1

DEPS = shell.h

makefile: shell.o
	gcc -o shell shell.o
	
shell.o: shell.c
	gcc -c shell.c

clean:
	rm *.o
