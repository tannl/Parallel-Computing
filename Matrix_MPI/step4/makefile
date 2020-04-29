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
	mpicc -g -Wall  -o matrix $(objects)
main.o: main.c matrix.h
	mpicc -c main.c
matrix.o: matrix.c matrix.h
	mpicc -c matrix.c
clean:
	rm matrix $(objects)

