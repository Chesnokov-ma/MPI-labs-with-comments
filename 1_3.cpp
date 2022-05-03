#include "mpi.h"
#include <stdio.h>

int main(int argc,char *argv[])
{
    int myrank, numprocs;
    MPI_Status status;
    int message;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if (myrank % 2 == 0)    // если процесс четный
    {
        message = myrank;   // то в качестве отправляемого сообщения он использует свой номер
        MPI_Send(&message,1,MPI_INT,myrank+1,0,MPI_COMM_WORLD); // и отправляет его на свой (номер + 1) процесс, то есть на следующий
    }
    else    // если процесс не четный
    {
		MPI_Recv(&message, 1, MPI_INT, myrank - 1,MPI_ANY_TAG, MPI_COMM_WORLD, &status);    // он принимает сообщение с (номер - 1), то есть предыдущего				
        printf("received: %d for %d\n", message, myrank);       // и выводит его на экран
    }

    MPI_Finalize();
    return 0;
}

/*
    Каждый четный процесс посылает свой номер следующему процессу

    Надо отметить, что если в прошлой программе посылался массив символов (или строка),
    то здесь отсылается одно число типа int
*/