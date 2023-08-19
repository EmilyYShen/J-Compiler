# compile everything; this is the default rule that fires if a user
# just types "make" in the same directory as this Makefile

jc: jc.o token.o stack.o
	gcc -g -Wall -std=c11 -o jc jc.o token.o stack.o

jc.o: jc.c token.h
	gcc -g -Wall -c jc.c

token.o: token.c token.h
	gcc -g -Wall -std=c11 -c token.c

stack.o: stack.c stack.h
	gcc -g -Wall -std=c11 -c stack.c

clean:
	rm -f *.o *~ jc *.asm