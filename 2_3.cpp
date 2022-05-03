#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h> 
#include <math.h> 
  
  
int main(int argc, char *argv[])
{
    int myrank, numprocs;     
    int buffer;

    MPI_Status status; 
    MPI_Request request; 
  
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    int tag=1500, source=0, purpose=1, count=1; 
    request=MPI_REQUEST_NULL;   // для нас не важна

    if(myrank == source)    // на нулевом процессе 
    {
        buffer=6780; 
        MPI_Isend(&buffer,count,MPI_DOUBLE,purpose,tag,MPI_COMM_WORLD, &request);   // отправка числа неблокируемой оправкой

        // код дальше бы выполнялся сразу
    } 
    if(myrank == purpose)   // на 1 процессе
    { 
        MPI_Irecv(&buffer, count, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &request);   // неблокируемое принятие 

        // код дальше бы выполнялся сразу

    }   

    MPI_Wait(&request,&status);     // синхронизация процессов (код не выполняется дальше, пока все операции выше не завершатся)

    if(myrank == source)
    { 
        printf("processor %d  sent %d\n",myrank,buffer);    // просто выаод на экран
    } 
    if(myrank == purpose)
    { 
        printf("processor %d  receive %d\n",myrank,buffer);
    } 

    MPI_Finalize(); 
} 
 
/*
    Вызовы процедур неблокирующих операций обмена, очевидно, не блокируют выполнение последующего кода.
    Но чтобы избежать ошибок, нужно самостоятельно синхронизировать процессы
*/