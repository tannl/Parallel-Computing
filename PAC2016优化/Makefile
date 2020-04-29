
FC=ifort
LD= ifort
FFLAGS= -mcmodel=large

main: main.o Covariance1.o Covariance.o check.o
	${FC} ${FFLAGS} main.o Covariance1.o Covariance.o check.o -o main
main.o: main.f90
	${FC} ${FFLAGS} -c main.f90
Covariance1.o: Covariance1.f90 
	${FC} ${FFLAGS} -c Covariance1.f90
Covariance.o: Covariance.f90
	${FC} ${FFLAGS} -c Covariance.f90
check.o: check.f90
	${FC} ${FFLAGS} -c check.f90

clean:
	rm -rf *.o *.i main
