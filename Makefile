ifeq ($(DEBUG), 1)
	CFLAGS = -DDEBUG
endif

all: htest itest
	rm *.o

itest: interactive_test.c jenkins.o hashmap.o
	gcc interactive_test.c jenkins.o hashmap.o -o interactive_test

htest: test.c jenkins.o hashmap.o
	gcc test.c jenkins.o hashmap.o -o test

hashmap.o: hashmap.h hashmap.c jenkins.o
	gcc $(CFLAGS) -c hashmap.c -o hashmap.o

jenkins.o: jenkins.h jenkins.c
	gcc -c jenkins.c -o jenkins.o

clean:
	rm -f *.o
