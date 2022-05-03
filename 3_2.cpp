#include <iostream>
#include "mpi.h"
#include <cstdlib>
using namespace std;

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

	int rank, n, i;

	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int mN = n;                             // размер стороны матрицы
    srand(rank + time(NULL));               // различная генерация на каждом потоке

    cout << "На процессе " << rank << ": "; // вывод до MPI_Alltoall

    unsigned int matrix_elem[mN];
    for (int i = 0; i < mN; i++) 
    {
        matrix_elem[i] = rand() % 1001;     // случайное целое число от 0 до 1000
        if (i + 1 == mN)
            cout << matrix_elem[i] << endl;
        else
            cout << matrix_elem[i] << ", ";
    }

    MPI_Barrier(MPI_COMM_WORLD);    // синхронизация вывода

    unsigned int recv_buf0[mN];
    MPI_Alltoall(&matrix_elem, 1, MPI_INT, recv_buf0, 1, MPI_INT, MPI_COMM_WORLD);      // отправка всем
    cout << "После MPI_Alltoall. На процессе " << rank << ": ";
    for (int i = 0; i < mN; i++) 
    {
        if (i + 1 == mN)
            cout << recv_buf0[i] << endl;
        else
            cout << recv_buf0[i] << ", ";
    }

	MPI_Finalize();
    return 0;
}

/*
    "Реализовать программу для рассылки частей квадратной матрицы с использованием команды MPI_Alltoall."
*/