CC = g++
PROGNAME = Blacksmithing.out
LIBRARIES = main.o tableau.o SaveHistory.o

all: $(PROGNAME)

Blacksmithing.out: $(LIBRARIES)
	$(CC) -o $(PROGNAME) $(LIBRARIES)

main.o: main.cxx 
	$(CC) -c $<

tableau.o: tableau.cxx tableau.h
	$(CC) -c $<

SaveHistory.o: SaveHistory.cxx SaveHistory.h tableau.o
	$(CC) -c $<

.PHONY: clean

clean:
	rm -f *.o $(PROGNAME) *~
