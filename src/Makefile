# Makefile for compiler project.
# Written by:
#   xpatri00 Nikolas Patrik
#   xjance00 Matej Jancek
#   xhubin03 Robert Hubinak
#   xhorna14 Peter Hornak
# Makefile version: 4.2.1

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g
SHELL = /bin/bash



.PHONY: all clean

object_file = main.o garbagecollector.o generate.o instrlist.o parseexp.o parser.o queue.o scanner.o stack.o symtable.o

ifj18: $(object_file)
    $(CC) $^ -o $@

main.o: main.c parser.h symtable.h scanner.h generate.h
    $(CC) $(CFLAGS) -c $< -o $@

garbagecollector.o: garbagecollector.c garbagecollector.h
    $(CC) $(CFLAGS) -c $< -o $@

scanner.o: scanner.c scanner.h queue.h garbagecollector.h
    $(CC) $(CFLAGS) -c $< -o $@

parseexp.o: parseexp.c parseexp.h scanner.h garbagecollector.h
    $(CC) $(CFLAGS) -c $< -o $@

symtable.o: symtable.c symtable.h
    $(CC) $(CFLAGS) -c $< -o $@

queue.o: queue.c queue.h scanner.h
    $(CC) $(CFLAGS) -c $< -o $@

stack.o: stack.c stack.h
    $(CC) $(CFLAGS) -c $< -o $@

parser.o: parser.c parser.h scanner.h symtable.h parseexp.h garbagecollector.h instrlist.h generate.h
    $(CC) $(CFLAGS) -c $< -o $@

generate.o: generate.c generate.h instrlist.h symtable.h garbagecollector.h
    $(CC) $(CFLAGS) -c $< -o $@

instrlist.o: instrlist.c instrlist.h
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f *.o ifj18
