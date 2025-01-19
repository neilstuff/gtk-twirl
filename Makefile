CC = gcc
PKGCONFIG = pkg-config

ifeq ($(OS),Windows_NT)
	DELETE = del
else
	DELETE = rm
endif


ifdef DEPLOY 
	WINDOWS=-mwindows
else
	WINDOWS=
endif

COMPILE_RESOURCES = glib-compile-resources
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk4)
LIBS = $(shell $(PKGCONFIG)  --libs gtk4)

PROD = -mwindows

SRC = geometry.c drawer.c event.c node.c connector.c net.c controller.c main.c
BUILT_SRC = resource.c

OBJS = $(BUILT_SRC:.c=.o) $(SRC:.c=.o)

all: twirl

resource.c: twirl.gresource.xml twirl.ui
	$(COMPILE_RESOURCES) twirl.gresource.xml --target=$@ --sourcedir=. --generate-source

%.o: %.c
	$(CC) -c -o $(@F) $(CFLAGS) $<

twirl: $(OBJS)
	$(CC) -o $(@F) $(WINDOWS) $(OBJS) $(LIBS)

clean:
	$(DELETE) $(OBJS)
	$(DELETE) resource.c
	$(DELETE) twirl.exe