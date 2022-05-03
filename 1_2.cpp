#include "mpi.h"
#include <stdio.h>
int main(int argc,char *argv[])
{
    int myid, numprocs;
    char message[20];   // пустое текстовое сообшение (под него выделили память, но пока не заполнили)
    int myrank;

    MPI_Status status;  // статус, который следит за выполнением программы; пока нужен, только потому, что используется в ситаксисе MPI_Recv
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if (myrank == 0)    // на процессе 0
    {
        strcpy(message, "Hi, Second Processor!");               // заполнение message (strcpy - копирует в него текст)
        MPI_Send(&message,20,MPI_CHAR,1,0,MPI_COMM_WORLD);		// отправка 20 (первых) символов из message типа MPI_CHAR (знаковый) на 1 процесс		  	
    }
    else    // на всех остальных процессах 
    {
		MPI_Recv(&message, 20, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);		// 1-й процесс получает сообщение с 0 процесса	
        printf("received: %s\n", message);      // и выводит на экран
    }

    MPI_Finalize();
    return 0;
}

/*
    В этой программе мы отправляем с 0 процесса на 1 сообщение и выводим его на экран

    Запускать лучше на 2 процессах

    MPI_ANY_TAG - принимать с любым тегом (тег - идентификатор, чтобы находить нужное сообщение при MPI_Recv)
    MPI_COMM_WORLD - пространство имен MPI, пока не используется
*/