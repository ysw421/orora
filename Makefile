exec = orora.out
sources = $(wildcard src/*.c src/*/*.c src/*/*/*.c )
objects = $(sources:.c=.o)
flags = -g -Wall -lm -ldl -fPIC -rdynamic #-fsanitize=address
#flags = -g -Wall -lm -ldl -fPIC -rdynamic -fsanitize=address

# ./configure --prefix = /usr/local/orora


$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c $.h
	gcc -c $(flags) $< -o $@

%.o: %.c $*/$.h
	gcc -c $(flags) $< -o $@

%.o: %.c $*/$*/$.h
	gcc -c $(flags) $< -o $@

window:
	-rm orora.exe
	x86_64-w64-mingw32-gcc -g -Wall -fstack-protector -o orora.exe $(objects)
#	i686-w64-mingw32-gcc -Wall -fstack-protector -o orora.exe $(objects)

clean:
	-rm *.out
	-rm orora.exe
#	-rm *.o
	-rm src/*/*/*.o
	-rm src/*/*.o
	-rm src/*.o
#	-rm rm *.a

lint:
	clang-tidy scr/*.c src/*/*.h src/*/*.c

debug:
	gdb $(exec)
