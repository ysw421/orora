exec = orora.out
sources = $(wildcard src/*.c src/*/*.c src/*/*/*.c )
objects = $(sources:.c=.o)
flags = -g -Wall -lm -ldl -fPIC -rdynamic


$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c $.h
	gcc -c $(flags) $< -o $@

%.o: %.c $*/$.h
	gcc -c $(flags) $< -o $@

%.o: %.c $*/$*/$.h
	gcc -c $(flags) $< -o $@

clean:
	-rm *.out
#	-rm *.o
	-rm src/*/*.o
#	-rm rm *.a
	-rm src/*.o

lint:
	clang-tidy scr/*.c src/*/*.h src/*/*.c

debug:
	gdb $(exec)
