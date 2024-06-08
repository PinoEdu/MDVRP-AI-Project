Project:
	g++ -c functions.cpp -o functions.o
	g++ main.cpp functions.o -o program.out

run:
	make
	./program.out