all: compile link

compile:
	g++ -Isrc/include -c coding/gravity.cpp

link:
	g++ gravity.o -o gravity -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system