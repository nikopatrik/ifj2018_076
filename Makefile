# Makefile for compiler project.
# Written by:
# 	xpatri00 Nikolas Patrik
# 	xjance00 Matej Jancek
# 	xhubin03 Robert Hubinak
# 	xhorna14 Peter Hornak
# Makefile version: 4.2.1
	
CC=gcc
CFLAGS= -std=c99 -Wall -Wextra -Werror -pedantic
SHELL=bash

.PHONY: all clean commit tests zip
