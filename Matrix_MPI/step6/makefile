# matrix: main.o matrix.o
# 	gcc -o matrix main.o matrix.o
# main.o: main.c matrix.h
# 	gcc -c main.c
# matrix.o: matrix.c matrix.h
# 	gcc -c matrix.c
# clean:
# 	rm matrix main.o matrix.o

objects= main.o matrix.o
matrix: $(objects)
	mpicc -g -Wall -fopenmp  -O3 -o matrix $(objects)
main.o: main.c matrix.h
	mpicc -O3 -c main.c
matrix.o: matrix.c matrix.h
	mpicc -fopenmp -O3 -c matrix.c
clean:
	rm matrix $(objects)

