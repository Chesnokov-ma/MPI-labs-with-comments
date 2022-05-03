#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h>
#include <math.h>   // библиотека для математических функций
int main(int argc,char **argv) 
{    
    int myrank, numprocs;
    MPI_Status status; 
    int mytag,kk;
    int count_k = 5;
    int j[count_k],*mas; 

    // Для демонстрации я немного изменил программу (это из старого кода)
  
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    printf(" Hello from process: %d  Numprocs is %d\n",myrank,numprocs);
    mytag=150;

    if(myrank == 0) 
    {
        for (int i = 0; i < count_k; i++)   j[i] = 200 + 100 * i;    // в буфере 5 элементов
        MPI_Send(&j,count_k,MPI_INT,1,mytag,MPI_COMM_WORLD);         // они отправляются на 1 процесс
    } 
    if(myrank == 1)
    {
        MPI_Probe(0, mytag, MPI_COMM_WORLD, &status);   // 1 процесс ждет и "осматривает" полученное сообщение
        MPI_Get_count(&status, MPI_INT, &count_k);      // смотрит, сколько пришло сообщений (в данном случае 5)

        mas=(int*)malloc(count_k*sizeof(int));      // память динамически выделяется под 5 эл-ов
        printf("getting %d\n",count_k);

        MPI_Recv(mas, count_k, MPI_INT, 0, mytag, MPI_COMM_WORLD, &status);     // выделенная память используется для принятия сообщения
        printf("mas= ");

        for(int i=0; i<count_k; i++)
            printf("%d ", mas[i]); 
        printf("\n"); 
    } 
    MPI_Finalize(); 
} 

/*
    Таким образом, с помощью MPI_Probe можно "поймать" сообщение и узнать информаию о нем.
    Это позволит избежать ошибкти с нехваткой памяти или, наоборот, тех ситуаций, когда память простаивает
*/