

// Example Chapter 3 OpenMP Program
#include <stdio.h>
#include <math.h>
//#include <sys/time.h> 
//#include <sys/resource.h>  
#include <omp.h>
#include <windows.h>


const long int VERYBIG = 100000;
// ***********************************************************************
int main(void)
{
	int i;
	long int j, k, sum;
	double sumx, sumy, total, z;
	double starttime, elapsedtime , starthread,endthread;
	// ---------------------------------------------------------------------
	// Output a start message
	printf("OpenMP Parallel Timings for %ld iterations \n\n", VERYBIG);

	// repeat experiment several times
	for (i = 0; i < 6; i++)
	{
		// get starting time
		starttime = omp_get_wtime();
		// reset check sum and total
		sum = 0;
		total = 0.0;
		starthread = 0;
		// Work loop, do some work by looping VERYBIG times
	#pragma omp parallel     \
      num_threads (4)  private(sumx, sumy, k,starthread , endthread)   \
			reduction(+: sum, total)
		{
			starthread = omp_get_wtime();
			#pragma omp for schedule(dynamic,2000) nowait
			for (int j = 0; j < VERYBIG; j++)
			{
				// increment check sum
				sum += 1;

				// Calculate first arithmetic series
				sumx = 0.0;
				for (k = 0; k < j; k++)
					sumx = sumx + (double)k;

				// Calculate second arithmetic series
				sumy = 0.0;
				for (k = j; k > 0; k--)
					sumy = sumy + (double)k;

				if (sumx > 0.0)total = total + 1.0 / sqrt(sumx);
				if (sumy > 0.0)total = total + 1.0 / sqrt(sumy);
			}
			endthread = omp_get_wtime() - starthread;
			printf(" Run time thread %10d mSecs , with thread number= %d \n",
				(int)(endthread * 1000), omp_get_thread_num());

		}
		// get ending time and use it to determine elapsed time
		elapsedtime = omp_get_wtime() - starttime;

		// report elapsed time
		printf("Time Elapsed %10d mSecs Total=%lf Check Sum = %ld\n\n",
			(int)(elapsedtime * 1000), total, sum);
	}
	getchar();
	// return integer as required by function header
	return 0;
}
