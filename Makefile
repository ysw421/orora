include Makefile.build

EXEC = orora
SOURCES = $(wildcard src/*.c src/*/*.c src/*/*/*.c )
OBJECTS = $(SOURCES:.c=.o)
DEPENDENCYS = $(SOURCES:.c=.d)
FLAGS = -g -I./src
#FLAGS = -g -Wall -lm -ldl -fPIC -rdynamic -fsanitize=address

# ./configure --prefix = /usr/local/orora

BUILD_SUBDIRS = os lexer parser visitor
CC = gcc

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c $< -o $@ $(FLAGS) -MMD -MF $(@:.o=.d)

window:
	-rm orora.exe
	x86_64-w64-mingw32-gcc -g -Wall -fstack-protector -o orora.exe $(OBJECTS)
#	i686-w64-mingw32-gcc -Wall -fstack-protector -o orora.exe $(OBJECTS)

clean:
	-rm -f $(EXEC)
	-rm -f $(OBJECTS)
	-rm -f $(DEPENDENCYS)
#	-rm orora.exe

lint:
	clang-tidy scr/*.c src/*/*.h src/*/*.c

debug:
	gdb $(EXEC)

