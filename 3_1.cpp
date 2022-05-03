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
    double* send_arr, *avg_root;

    if (rank == 0)
    {
        send_arr = (double*) malloc(sizeof(double) * N);  // случайные числа
        avg_root = (double*) malloc(sizeof(double) * n);  // для сложения средних значений с потоков

        for (int i = 0; i < N; i++)
            send_arr[i] =  .0 + (double)(rand()) / ((double)(RAND_MAX/(100.0 - .0)));   // заполнение случайными значениями
    }

    double* recv_buf = new double[send_N];  // буфер для хранения (принятия)
    MPI_Scatter(send_arr, send_N, MPI_DOUBLE, recv_buf, send_N, MPI_DOUBLE, 0, MPI_COMM_WORLD); // отправка всем процессам по 100 чисел

    double avg = .0;
    for (int i = 0; i < send_N; i++)  avg += recv_buf[i];     avg /= send_N;    // среднее значение на потоке

    MPI_Gather(&avg, 1, MPI_DOUBLE, avg_root, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);      // отправка на root

    if (rank == 0)  // на нулевом процессе 
    {
        avg = .0;
        for (int i = 0; i < n; i++)     avg += avg_root[i];     avg /= n;   // некие расчеты

        cout << "Среднее значение: " << avg << endl;    // вывод на экран

        delete[] send_arr;
        delete[] avg_root;
    }

    delete[] recv_buf;  // особождение памяти

	MPI_Finalize();
    return 0;
}

/*
    "Задание 3.1 Вычислить среднее значение массива случайных чисел с помощью MPI_Scatter и MPI_Gather.

    Необходимо реализовать следующий алгоритм:
    1) Создайте массив и заполните его случайными числами в root процессе (процесс 0).
    2) Разошлите числа на все процессы, давая каждому процессу одинаковое количество чисел.
    3) Каждый процесс вычисляет среднее для своего подмножества чисел.
    4) Соберите все средние значения в root процесс. Root процесс затем вычисляет среднее значение 
    этих чисел для получения окончательного среднего значения для всего массива."

    В программе все так и реализовано
*/