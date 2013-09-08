FLAGS = -g -Wall -O
CC = gcc
INCLUDE=
LIB=


all: bmt2xml

bmt2xml: obj/bmt2xml.o obj/bmxdata.o obj/bmxconstants.o
	$(CC) $(FLAGS) -o $@ $^

obj/bmt2xml.o: src/bmt2xml.c src/bmxconstants.h src/bmxdata.h
	$(CC) $(FLAGS) -o $@ -c $< $(INCLUDE)

obj/bmxdata.o: src/bmxdata.c src/bmxconstants.h src/bmxdata.h
	$(CC) $(FLAGS) -o $@ -c $< $(INCLUDE)

obj/bmxconstants.o: src/bmxconstants.c src/bmxconstants.h
	$(CC) $(FLAGS) -o $@ -c $< $(INCLUDE)

clean:
	rm obj/*.o bmt2xml
