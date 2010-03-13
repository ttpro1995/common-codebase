# Project: Project1
# Makefile created by Dev-C++ 4.9.9.2

CPP  = g++.exe
CC   = gcc.exe
WINDRES = windres.exe
RES  = 
OBJ  = ImageSig.o $(RES)
LINKOBJ  = ImageSig.o $(RES)
LIBS =  -L"D:/compiler/Dev-Cpp/lib"  
INCS =  -I"D:/compiler/Dev-Cpp/include" 
CXXINCS =  -I"D:/compiler/Dev-Cpp/lib/gcc/mingw32/3.4.2/include"  -I"D:/compiler/Dev-Cpp/include/c++/3.4.2/backward"  -I"D:/compiler/Dev-Cpp/include/c++/3.4.2/mingw32"  -I"D:/compiler/Dev-Cpp/include/c++/3.4.2"  -I"D:/compiler/Dev-Cpp/include" 
BIN  = ImageSig.exe
CXXFLAGS = $(CXXINCS)  
CFLAGS = $(INCS)  
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before ImageSig.exe all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o "ImageSig.exe" $(LIBS)

ImageSig.o: ImageSig.c
	$(CC) -c ImageSig.c -o ImageSig.o $(CFLAGS)

all-after :
	mv $(BIN) ../bin/
