#include <stdio.h>
#include <time.h>

extern void mergesort_gpu(double* data, long size, int xThreadPerBlock = 32, int xBlocksPerGrid = 8);
