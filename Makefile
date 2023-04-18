all: compile link

compile:
	g++ -Isrc/include -c coding/main.cpp

link:
	g++ *.o -o main -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system