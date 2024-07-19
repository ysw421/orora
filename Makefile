include Makefile.build

EXEC = orora
OBJECTS = $(SOURCES:.c=.o)
DEPENDENCYS = $(SOURCES:.c=.d)
FLAGS = -g -Wall -lm -ldl -fPIC -rdynamic -I./src
MKBUILD_SUBDIRS = lib
BUILD_FOLDER = build
BUILD_FOLDER_PATH = $(join $(BUILD_FOLDER),/)
BUILD_LIB_PATH = $(join $(BUILD_FOLDER_PATH),lib/)
CC = gcc

all: check_dependencies $(MKBUILD_SUBDIRS) $(EXEC)

check_dependencies:
	@echo "Checking dependencies..."
	@if ! pkg-config --exists readline; then \
		echo "Readline not found. Please run './install.sh' to install dependencies."; \
		exit 1; \
	fi

READLINE_AVAILABLE := $(shell pkg-config --exists readline && echo yes || echo no)
ifeq ($(READLINE_AVAILABLE),yes)
    CFLAGS += -DUSE_READLINE $(shell pkg-config --cflags readline)
    LDFLAGS += $(shell pkg-config --libs readline)
else
    $(warning Readline not found. Building without readline support.)
endif

$(MKBUILD_SUBDIRS):
	mkdir -p $(BUILD_FOLDER)
	mkdir -p $(join $(BUILD_FOLDER_PATH),$@)

$(EXEC): $(OBJECTS)
	$(CC) -o $(join $(BUILD_FOLDER_PATH),$(notdir $@)) $(wildcard $(BUILD_LIB_PATH)*.o) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $(join $(BUILD_LIB_PATH),$(notdir $@)) $(FLAGS) $(CFLAGS) -MMD -MF $(join $(BUILD_LIB_PATH),$(notdir $(@:.o=.d)))

window:
	-rm orora.exe
	x86_64-w64-mingw32-gcc -g -Wall -fstack-protector -o orora.exe $(OBJECTS)

clean:
	-rm -rf $(BUILD_FOLDER)

lint:
	clang-tidy scr/*.c src/*/*.h src/*/*.c

debug:
	gdb $(EXEC)

