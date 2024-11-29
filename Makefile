ifeq ($(OS), Windows_NT)
    OSFLAG = WINDOWS
    INCLUDE = -IC:\sqlite -IC:\Program-Files\OpenSSL-Win64\include
    LIBDIRS = -LC:\sqlite -LC:\Program-Files\OpenSSL-Win64
    LIBS = -lsqlite3 -lssl -lcrypto
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S), Darwin)  # macOS
        OSFLAG = MAC
        INCLUDE = -I/opt/homebrew/opt/sqlite/include -I/opt/homebrew/opt/openssl@3/include
        LIBDIRS = -L/opt/homebrew/opt/sqlite/lib -L/opt/homebrew/opt/openssl@3/lib
        LIBS = -lsqlite3 -lssl -lcrypto
    else
        OSFLAG = LINUX
        INCLUDE = -I/usr/include
        LIBDIRS = -L/usr/lib
        LIBS = -lsqlite3 -lssl -lcrypto
    endif
endif

TARGET = completo
SRCS = src/main.cpp src/CrearCuenta.cpp src/Verificacion.cpp src/AtencionCliente.cpp src/InformacionPrestamos.cpp src/UU.cpp src/baseDatos.cpp
HEADERS = src/CrearCuenta.hpp src/Verificacion.hpp src/AtencionCliente.hpp src/InformacionPrestamos.hpp src/baseDatos.hpp
OBJS = $(SRCS:.cpp=.o)
CC = g++
CFLAGS = -Wall -std=c++11 $(INCLUDE)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBDIRS) $(LIBS)

src/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
ifeq ($(OSFLAG), LINUX)
	./$(TARGET)
else ifeq ($(OSFLAG), MAC)
	./$(TARGET)
else
	$(TARGET).exe
endif

clean:
ifeq ($(OSFLAG), LINUX)
	rm -f $(OBJS) $(TARGET)
else ifeq ($(OSFLAG), MAC)
	rm -f $(OBJS) $(TARGET)
else
	del /F /Q $(subst /,\,$(OBJS)) $(subst /,\,$(TARGET).exe)
endif


