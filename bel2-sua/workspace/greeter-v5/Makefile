TARGET    = greeter
BUILDDIR  = bin
SRCDIR    = src

## OPTIONS --------------------------------------------------------#
#------------------------------------------------------------------#
#---------------------------- Compiler ----------------------------#
CC        = clang
CC_FLAGS := `pkg-config --cflags gtk+-3.0` -Wall
LD_FLAGS := `pkg-config --libs gtk+-3.0` -rdynamic

EXEC      = $(BUILDDIR)/$(TARGET)
SOURCES   = $(wildcard $(SRCDIR)/*.c) $(RES)
OBJECTS   = $(SOURCES:%.c=%.o)

#-------------------------- Gtk Resources -------------------------#
RESOURCE  = gtk_res
GTKRES    = glib-compile-resources
RESDIR    = res
RES       = $(RESDIR)/$(RESOURCE).c
RES_SRC   = $(RESDIR)/$(RESOURCE).gresource.xml
RES_DEP   = $(shell $(GTKRES) --sourcedir=. --generate-dependencies $(RES_SRC))

#------------------------------ Util ------------------------------#
FMT       = astyle -n --options=$(HOME)/.config/astyle/gnome.options

## TARGETS --------------------------------------------------------#
#------------------------------------------------------------------#
#---------------------------- Compiler ----------------------------#
all: build
build: $(BUILDDIR)/ $(EXEC)

debug: CC_FLAGS += -g2
debug: build

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) $(LD_FLAGS) -o $@

%.o: %.c
	$(CC) -c $(CC_FLAGS) $< -o $@

#-------------------------- Gtk Resources -------------------------#
$(RES): $(RES_SRC) $(RES_DEP)
	$(GTKRES) --sourcedir=. --target=$@ $< --generate-source

#------------------------------ Util ------------------------------#
clean:
	rm -f $(OBJECTS) $(RES)

distclean: clean
	rm -f $(EXEC)

format: $(SOURCES)
	$(FMT) $^

%/:
	mkdir -p $@

# Disable implicit suffix and built-in rules (for performance and profit)
.SUFFIXES:
MAKEFLAGS += --no-builtin-rules

.PHONY: format clean distclean