#!/bin/bash
# Emmett Lam
# CSE374 HW5
# Makefile
#
# Compiles all the files for the t9 program

t9: t9.o TrieNode.o
	gcc -Wall -std=c99 -g -o t9 t9.o TrieNode.o

t9.o: TrieNode.h t9.c
	gcc -Wall -std=c99 -g -c t9.c

TrieNode.o: TrieNode.h TrieNode.c
	gcc -Wall -std=c99 -g -c TrieNode.c
