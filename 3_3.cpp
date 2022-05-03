#include <iostream>
#include "mpi.h"
#include <cstdlib>
using namespace std;

int main(int argc, char* argv[])
{
    srand(time(NULL));

    MPI_Init(&argc, &argv);
	int rank, n, i;
    
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N = n * 100;
    int send_N = 100;    // числа на процессы

    // создание и заполнение массива случайными числами
    double* send_arr;
    double sum_root =.0;


    if (rank == 0)
    {
        send_arr = (double*) malloc(sizeof(double) * N);  // случайные числа

        for (int i = 0; i < N; i++)
            send_arr[i] =  .0 + (double)(rand()) / ((double)(RAND_MAX/(100.0 - .0)));   // заполнение случайными зн-ми
    }

    double* recv_buf = new double[send_N];  // буфер для хранения (принятия)
    MPI_Scatter(send_arr, send_N, MPI_DOUBLE, recv_buf, send_N, MPI_DOUBLE, 0, MPI_COMM_WORLD); // отправка всем процессам по 100 чисел

    double sum = .0;
    for (int i = 0; i < send_N; i++)  sum += recv_buf[i];
    cout << "Локальная сумма на потоке " << rank << ": " << sum << endl;

    // MPI_Barrier(MPI_COMM_WORLD);    // синхронизация вывода
    MPI_Reduce(&sum, &sum_root, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);       // сумма и отправка на root

    if (rank == 0)
    {
        cout << "Сумма: " << sum_root << endl;
        delete[] send_arr;
    }

    delete[] recv_buf;

	MPI_Finalize();
    return 0;
}

/*
    'Реализовать программу для рассылки данных от root процесса всем процессам с помощью MPI_Scatter. Затем в каждом процессе просуммировать полученные данные и
    с помощью команды MPI_Reduce отправить назад в root процесс и посчитать общую сумму, вывести ее на экран.'
*/