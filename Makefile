include Makefile.build

EXEC = orora
OBJECTS = $(SOURCES:.c=.o)
DEPENDENCYS = $(SOURCES:.c=.d)

FLAGS = -g -I./src
#FLAGS = -g -Wall -lm -ldl -fPIC -rdynamic -fsanitize=address

# ./configure --prefix = /usr/local/orora

MKBUILD_SUBDIRS = lib
BUILD_FOLDER = build
BUILD_FOLDER_PATH = $(join $(BUILD_FOLDER),/)
BUILD_LIB_PATH = $(join $(BUILD_FOLDER_PATH),lib/)

CC = gcc

all: $(MKBUILD_SUBDIRS) $(EXEC)

$(MKBUILD_SUBDIRS):
	mkdir -p $(BUILD_FOLDER)
	mkdir -p $(join $(BUILD_FOLDER_PATH),$@)

$(EXEC): $(OBJECTS)
	$(CC) -o $(join $(BUILD_FOLDER_PATH),$(notdir $@)) $(wildcard $(BUILD_LIB_PATH)*.o)

%.o: %.c
	$(CC) -c $< -o $(join $(BUILD_LIB_PATH),$(notdir $@)) $(FLAGS) -MMD -MF $(join $(BUILD_LIB_PATH),$(notdir $(@:.o=.d)))

window:
	-rm orora.exe
	x86_64-w64-mingw32-gcc -g -Wall -fstack-protector -o orora.exe $(OBJECTS)
#	i686-w64-mingw32-gcc -Wall -fstack-protector -o orora.exe $(OBJECTS)

clean:
	-rm -rf $(BUILD_FOLDER)
#	-rm orora.exe

lint:
	clang-tidy scr/*.c src/*/*.h src/*/*.c

debug:
	gdb $(EXEC)

