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

MSYSINC = C:/msys64/mingw64/include

COMPILE_RESOURCES = glib-compile-resources
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk4)
LIBS = $(shell $(PKGCONFIG)  --libs gtk4)

PROD = -mwindows
XMLINC = -I$(MSYSINC)/libxml2
XMLLIB = -llibxml2
SRCDIR = src
OBJDIR = obj

SRC = handler.c \
container.c \
writer.c \
reader.c \
geometry.c \
drawer.c \
editor.c \
artifact.c \
event.c \
vertex.c \
node.c  \
arc.c  \
connector.c \
mover.c  \
selector.c \
net.c \
controller.c \
tracker.c \
main.c \
resource.c

INC = $(CFLAGS) $(XMLINC)
BUILT_SRC = resource.c
ALL_SRC = $(SRC) $(BUILD_SRC)

OBJS =$(BUILT_SRC:.c=.o) $(SRC:.c=.o)

OBJECTS = $(patsubst %.c,$(OBJDIR)/%.o,$(ALL_SRC))

all: twirl

resource.c: twirl.gresource.xml twirl.ui
	$(COMPILE_RESOURCES) twirl.gresource.xml --target=$(SRCDIR)/$@ --sourcedir=. --generate-source

obj/%.o:$(SRCDIR)/%.c resource.c
	$(CC) -c -o $(OBJDIR)/$(@F) $(INC) $<

twirl: $(OBJECTS)
	$(CC) -o $(@F) $(WINDOWS) $(OBJECTS) $(LIBS) $(XMLLIB)

clean:
	$(DELETE) $(OBJDIR)\*.o
	$(DELETE) $(SRCDIR)\resource.c
	$(DELETE) twirl.exe