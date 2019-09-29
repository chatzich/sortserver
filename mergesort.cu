#include <iostream>
#include <sys/time.h>

#define min(a, b) (a < b ? a : b)

__device__ unsigned int getIdx(dim3* threads, dim3* blocks) {
    int x;
    return threadIdx.x +
           threadIdx.y * (x  = threads->x) +
           threadIdx.z * (x *= threads->y) +
           blockIdx.x  * (x *= threads->z) +
           blockIdx.y  * (x *= blocks->z) +
           blockIdx.z  * (x *= blocks->y);
}

__device__ void gpu_bottomUpMerge(double* source, double* dest, long start, long middle, long end) {
    long i = start;
    long j = middle;
    for (long k = start; k < end; k++) {
        if (i < middle && (j >= end || source[i] < source[j])) {
            dest[k] = source[i];
            i++;
        } else {
            dest[k] = source[j];
            j++;
        }
    }
}

__global__ void gpu_mergesort(double* source, double* dest, long size, long width, long slices, dim3* threads, dim3* blocks) {
    unsigned int idx = getIdx(threads, blocks);
    long start = width*idx*slices, 
         middle, 
         end;

    for (long slice = 0; slice < slices; slice++) {
        if (start >= size)
            break;

        middle = min(start + (width >> 1), size);
        end = min(start + width, size);
        gpu_bottomUpMerge(source, dest, start, middle, end);
        start += width;
    }
}





int main(int argc, char** argv) {

      double* data;

    long size = 100000;
    data = (double *)malloc(size*sizeof(double));
    for(int i = size - 1; i >= 0; i--) {
	    data[i] = i;
    }

    std::cout << "sorting " << size << " numbers\n\n";

    mergesort_gpu(data, size);


    for (int i = 0; i < size; i++) {
        std::cout << data[i] << '\n';
    } 
}

void mergesort_gpu(double* data, long size, int xThreadPerBlock, int xBlocksPerGrid) {

    dim3 threadsPerBlock;
    dim3 blocksPerGrid;

    threadsPerBlock.x = xThreadPerBlock;
    threadsPerBlock.y = 1;
    threadsPerBlock.z = 1;

    blocksPerGrid.x = xBlocksPerGrid;
    blocksPerGrid.y = 1;
    blocksPerGrid.z = 1;



    double* D_data;
    double* D_swp;
    dim3* D_threads;
    dim3* D_blocks;
    
    cudaMalloc((void**) &D_data, size * sizeof(double));
    cudaMalloc((void**) &D_swp, size * sizeof(double));

    cudaMemcpy(D_data, data, size * sizeof(long), cudaMemcpyHostToDevice);
 
    cudaMalloc((void**) &D_threads, sizeof(dim3));
    cudaMalloc((void**) &D_blocks, sizeof(dim3));

    cudaMemcpy(D_threads, &threadsPerBlock, sizeof(dim3), cudaMemcpyHostToDevice);
    cudaMemcpy(D_blocks, &blocksPerGrid, sizeof(dim3), cudaMemcpyHostToDevice);


    double* A = D_data;
    double* B = D_swp;

    long nThreads = threadsPerBlock.x * threadsPerBlock.y * threadsPerBlock.z *
                    blocksPerGrid.x * blocksPerGrid.y * blocksPerGrid.z;

    for (int width = 2; width < (size << 1); width <<= 1) {
        long slices = size / ((nThreads) * width) + 1;

        gpu_mergesort<<<blocksPerGrid, threadsPerBlock>>>(A, B, size, width, slices, D_threads, D_blocks);


        A = A == D_data ? D_swp : D_data;
        B = B == D_data ? D_swp : D_data;
    }

    cudaMemcpy(data, A, size * sizeof(double), cudaMemcpyDeviceToHost);
    
    
    cudaFree(A);
    cudaFree(B);
}


