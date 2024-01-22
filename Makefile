exec = orora.out
sources = $(wildcard src/*.c src/*/*.c src/*/*/*.c )
objects = $(sources:.c=.o)
flags = -g -Wall -lm -ldl -fPIC -rdynamic

# ./configure --prefix = /usr/local/orora


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
	-rm src/*/*/*.o
	-rm src/*/*.o
	-rm src/*.o
#	-rm rm *.a

lint:
	clang-tidy scr/*.c src/*/*.h src/*/*.c

debug:
	gdb $(exec)
