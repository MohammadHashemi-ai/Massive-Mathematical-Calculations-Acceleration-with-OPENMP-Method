
#include "stdio.h"
#include "omp.h"
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

#define		NUM_OF_THREADS		2

static long num_steps = 10000000;
//static long num_steps = 100000;
double step;

void main()
{
int i;
double x, pi, sum = 0;

step = 1.0 / ((double)num_steps);

high_resolution_clock::time_point start_serial = high_resolution_clock::now();

{
for (i = 0; i < num_steps; i++) {
x = (i + 0.5) * step;
sum = sum + (4.0 / (1.0 + x * x));
}
}
pi = step * sum;


high_resolution_clock::time_point end_serial = high_resolution_clock::now();
duration<double> time_span_serial = duration_cast<duration<double>>(end_serial - start_serial);
cout << "Execution time of Serial code: " << time_span_serial.count() << " seconds" << endl;

printf("	%lf \n", pi);

sum = 0;

high_resolution_clock::time_point start_omp = high_resolution_clock::now();

#pragma omp parallel num_threads(NUM_OF_THREADS) shared(num_steps,step) private(i,x) reduction(+:sum)
{
#pragma omp for
for (i = 0; i < num_steps; i++) {
x = (i + 0.5) * step;
sum = sum + (4.0 / (1.0 + x * x));
}
}
pi = step * sum;

high_resolution_clock::time_point end_omp = high_resolution_clock::now();
duration<double> time_span_omp = duration_cast<duration<double>>(end_omp - start_omp);
cout << "Execution time of OpenMP code: " << time_span_omp.count() << " seconds" << endl;
printf("	%lf\n", pi);
printf("Speedup from Serial to OpenMP = %4.2f\n", (float)(time_span_serial.count()) / (float)(time_span_omp.count()));

getchar();

}
