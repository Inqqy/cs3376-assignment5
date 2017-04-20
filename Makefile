#Makefile for assignment5

PROG = assg5
CC = g++
CPPFLAGS = -Wall -g

all: TwoChildren ThreeChildren

TwoChildren:	TwoPipesTwoChildren.cpp
		$(CC) $(FLAGS) -o $@ TwoPipesTwoChildren.cpp

ThreeChildren:	TwoPipesThreeChildren.cpp
		$(CC) $(FLAGS) -o $@ TwoPipesThreeChildren.cpp

clean:
		rm TwoChildren ThreeChildren
