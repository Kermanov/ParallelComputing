#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include "mpi.h"
using namespace std;


//placing matrixes into heap, in order to allocate a larger amount than the stack offers
const int k = 1500, n = 1500, p = 1500;
int matrix1[n][k] = { 0 }, matrix2[k][p] = { 0 }, matrix3[n][p] = { 0 }; //define matrixes


int main(int argc, char* argv[])
{
	double dif, start, end; //variables to calculate the time difference between the parallelization

						//place to define constants and variables
	int i, j, z, x; //define variables

	x = 10;	//setting the maximum value of matrix A values


			/*** Spawn a parallel region ***/
	MPI_Init(&argc, &argv);


	int thread_size;
	MPI_Comm_size(MPI_COMM_WORLD, &thread_size);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Status status;

	for (int i = 0; i < thread_size; i++)
	{
		if (rank == i)
		{
			srand(time(NULL) - i * i);
		}			
	}
		
	//printin matrix sizes
	if (rank == 0)
	{
		printf("Matrix1 size: %dx%d\n", n, k);
		printf("Matrix2 size: %dx%d\n", k, p);
		printf("Matrix3 size: %dx%d\n\n", n, p);
	}

	//starting the timer
	start = MPI_Wtime();


	//actions with matrix1
	for (i = rank; i < n; i += thread_size)
	{
		for (j = 0; j < k; j++) 
		{
			//generating random number to matrix1 from 1 to x
			matrix1[i][j] = 1 + rand() % x;
		}
	}

	//sending parallelized data to rank 0
	if (rank != 0)
	{
		for (i = rank; i < n; i += thread_size)
		{
			MPI_Send(&matrix1[i][0], k, MPI_INT, 0, i, MPI_COMM_WORLD);
		}
	}

	//rank 0 receiving data
	if (rank == 0)
	{
		for (i = 1; i < thread_size; i++)
		{
			for (j = i; j < n; j += thread_size)
			{
				MPI_Recv(&matrix1[j][0], k, MPI_INT, i, j, MPI_COMM_WORLD, &status);
			}
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);

	//***********************************************************************************
	// actions with matrix2
	if (rank == 0)
	{
		for (i = 0; i < k; i++)
		{
			for (j = 0; j < p; j++)
			{
				//generating randomly 1 or -1 to matrix2
				int temp = rand() % 2;
				if (temp == 0)
					matrix2[i][j] = 1;
				else
					matrix2[i][j] = -1;
			}
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);

	//sending parallelized data from 0 to every other thread
	if (rank == 0)
	{
		for (i = 1; i < thread_size; i++)
		{
			for (j = 0; j < k; j++)
			{
				MPI_Send(&matrix2[j][0], p, MPI_INT, i, j, MPI_COMM_WORLD);
			}
		}
	}


	//rank 0 receiving data
	if (rank != 0)
	{
		for (i = 0; i < k; i++)
		{
			MPI_Recv(&matrix2[i][0], p, MPI_INT, 0, i, MPI_COMM_WORLD, &status);
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	//***********************************************************************************
	//matrix multiplication
	for (i = rank; i < n; i += thread_size)
	{
		for (j = 0; j < p; j++)
		{
			int sum = 0;
			//calculating the sum of multiplication products
			for (z = 0; z < k; z++) {
				sum = sum + matrix1[i][z] * matrix2[z][j];
			}
			matrix3[i][j] = sum; //assigning value to matrix position
		}
	}


	//sending parallelized data to rank 0
	if (rank != 0)
	{
		for (i = rank; i < n; i += thread_size)
		{
			MPI_Send(&matrix3[i][0], p, MPI_INT, 0, i, MPI_COMM_WORLD);
		}
	}

	//rank 0 receiving data
	if (rank == 0)
	{
		for (i = 1; i < thread_size; i++)
		{
			for (j = i; j < p; j += thread_size)
			{
				MPI_Recv(&matrix3[j][0], p, MPI_INT, i, j, MPI_COMM_WORLD, &status);
			}
		}
	}



	end = MPI_Wtime(); //end the timer
	dif = end - start; //store the difference

					   //print results

	//if (rank == 0)
	//{
	//	//print matrix1
	//	printf("\n\nMatrix1:\n");
	//	for (int i = 0; i < n; i++)
	//	{
	//		for (int j = 0; j < k; j++)
	//		{
	//			printf("%d", matrix1[i][j]);
	//			printf(" ");
	//		}
	//		printf(" \n");
	//	}

	//	//print matrix2
	//	printf("\n\nMatrix2:\n");
	//	for (int i = 0; i < k; i++)
	//	{
	//		for (int j = 0; j < p; j++)
	//		{
	//			printf("%d", matrix2[i][j]);
	//			printf(" ");
	//		}
	//		printf(" \n");
	//	}

	//	//print matrix3
	//	printf("\n\nMatrix3:\n");
	//	for (int i = 0; i < n; i++)
	//	{
	//		for (int j = 0; j < p; j++)
	//		{
	//			printf("%d", matrix3[i][j]);
	//			printf(" ");
	//		}
	//		printf(" \n");
	//	}
	//}

	if (rank == 0)
	{
		printf("Parallelization took %f sec. time.\n", dif);
		printf("******************************************************\n");
	}

	MPI_Finalize();
	/*****end of parallel region*****/

	return 0;
}