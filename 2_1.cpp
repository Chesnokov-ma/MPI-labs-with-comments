#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>     // библиотке для вывода
int main(int argc,char *argv[]) 
{
	MPI_Init(&argc, &argv);
    int *buffer; int myrank;    // буфер для сообшения и номер процесса
    MPI_Status status;
    int buffsize = 1;   // размер буфера - 1 число
    int TAG = 0;        // тег (идентификатор) равен нулю
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if (myrank == 0)    // на нулевом процессе
    {
        buffer = (int *) malloc(buffsize + MPI_BSEND_OVERHEAD);     // выделяем под буфер память (+ MPI_BSEND_OVERHEAD - небольшой добавок сверху, чтобы исбежать ошибок с памятью)
        MPI_Buffer_attach(buffer, buffsize);                        // подключаем его к программе
        buffer = (int *) 10;                                        // в буфер заносится число

        MPI_Bsend(&buffer,buffsize,MPI_INT,1,0,MPI_COMM_WORLD);     // буфер ртправляется на первый процесс
    }
    else 
    { 
        MPI_Recv(&buffer, buffsize, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);  // первый проццесс получает сообщение
        printf("received: %i\n", buffer);                                       // и выводит на экран
    }

    MPI_Buffer_detach(&buffer, &buffsize);  
    MPI_Finalize();
    return 0; 
}  


/*
    Капитан пишет: "Выделение буфера и его размер контролируются программистом, который должен заранее создать буфер достаточного размера. 
    Буферизованная передача завершается сразу, поскольку сообщение немедленно копируется в буфер для последующей передачи."

    Насколько я помню, MPI_Send и MPI_Recv блокируют дальнейший код до тех пор, пока не будет принято сообщение
    MPI_Bsend работает с буфером, отправка которого не блокирует дальнейший код... что-то такое 

    Здесь тоже лучше запускать на 2 процессах
*/