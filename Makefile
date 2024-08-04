include Makefile.build

EXEC = orora
OBJECTS = $(SOURCES:.c=.o)
DEPENDENCYS = $(SOURCES:.c=.d)
CFLAGS = -g -Wall -ldl -fPIC -rdynamic -I./src
LDFLAGS = -lm
MKBUILD_SUBDIRS = lib
BUILD_FOLDER = build
BUILD_FOLDER_PATH = $(join $(BUILD_FOLDER),/)
BUILD_LIB_PATH = $(join $(BUILD_FOLDER_PATH),lib/)
CC = gcc

# Installation paths
PREFIX = /usr
BINDIR = $(PREFIX)/bin
DATADIR = $(PREFIX)/share/orora/
SRCDIR = $(DATADIR)src/
LIBDIR = $(PREFIX)/lib/orora/

all: check_dependencies $(MKBUILD_SUBDIRS) $(EXEC) copy_lib_files

check_dependencies:
	@echo "Checking dependencies..."
	@if ! pkg-config --exists readline; then \
		echo "Readline not found. Please run './install.sh' to install dependencies."; \
		exit 1; \
	fi

copy_lib_files:
	@mkdir -p $(LIBDIR)
	@cp -r src/lib/* $(LIBDIR)

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
	mkdir -p $(SRCDIR)

$(EXEC): $(OBJECTS)
	$(CC) -o $(join $(BUILD_FOLDER_PATH),$(notdir $@)) $(wildcard $(BUILD_LIB_PATH)*.o) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $(join $(BUILD_LIB_PATH),$(notdir $@)) $(CFLAGS) -MMD -MF $(join $(BUILD_LIB_PATH),$(notdir $(@:.o=.d)))

install: all
	@echo "Installing Orora..."
	@sudo mkdir -p $(DATADIR)
	@sudo mv -f $(BUILD_FOLDER_PATH) $(DATADIR)
	@sudo mv $(DATADIR)$(BUILD_LIB_PATH) $(DATADIR)src/
	@sudo mv $(DATADIR)src/lib/ $(DATADIR)lib/
	@sudo mkdir -p $(BINDIR)
	@sudo mv -f $(DATADIR)$(BUILD_FOLDER_PATH)$(EXEC) $(BINDIR)/$(EXEC)
	@sudo rm -rf $(DATADIR)$(BUILD_FOLDER_PATH)
	@echo "Orora installed successfully."

uninstall:
	@echo "Uninstalling Orora..."
	@sudo rm -f $(BINDIR)/$(EXEC)
	@sudo rm -rf $(DATADIR)
	@sudo rm -f $(SRCDIR)
	@echo "Orora uninstalled successfully."

window:
	-rm orora.exe
	x86_64-w64-mingw32-gcc -g -Wall -fstack-protector -o orora.exe $(OBJECTS)

clean:
	-rm -rf $(BUILD_FOLDER)

lint:
	clang-tidy scr/*.c src/*/*.h src/*/*.c

debug:
	gdb $(EXEC)

.PHONY: all check_dependencies install uninstall clean lint debug

