# Opciones de compilacion
CC = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lsqlite3 -lssl -lcrypto

# Makefile settings
TARGET = .\obj\ejecutable
EXT = .cpp
SRCDIR = src
OBJDIR = obj


SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)

# Variables para UNIX OS
RM = rm
DELOBJ = $(OBJ)
# Variable para Windows
DEL = del
EXE = .exe
WDELOBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)\\%.o)

# Compilacion y ejecucion
all: $(TARGET) run

$(TARGET): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)


$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $<

run:
	./$(TARGET)

################### Cleaning rules for Unix-based OS ###################
# Borra los archivos .o y .exe
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(TARGET)

#################### Cleaning rules for Windows OS #####################
# Borra los archivos .o y .exe
.PHONY: cleanw
cleanw:
	$(DEL) $(WDELOBJ)  $(TARGET)$(EXE)