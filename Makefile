CC=mpicxx
CFLAGS=-c -Wall 
LDFLAGS=-limf -lm
SRC=main.cpp  integrate.cpp lgwt.cpp 
OBJ=$(SRC:.cpp=.o)
EXE=integrate

all: $(SRC) $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)
.cpp.o: 
	$(CC) $(CFLAGS) $< -o $@
touch: 
	touch *.cpp *.h
clean:
	rm *.o $(EXE) *~ *#

