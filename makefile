# Makefile
# P2-Advanced-Algorithms

CFLAGS= -g -ansi -pedantic -Wall
CC=gcc

project: project.c
		$(CC) $(CFLAGS) -o project project.c

test1:
	./project < Tests/input01

test2:
	./project < Tests/input02

test3:
	./project < Tests/input03

test4:
	./project < Tests/input04

test5:
	./project < Tests/input05

test6:
	./project < Tests/input06

val:
	valgrind -v --leak-check=full ./project < Tests/input01

vgdb:
	valgrind --vgdb=yes --vgdb-error=0 ./project < Tests/input01
