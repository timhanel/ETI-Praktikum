#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>
#include <cuda_runtime.h>
#ifndef SIZE
#define SIZE 3840
#endif
#define V100CORES 2560
#define SMs 80
#define WARPSIZE 32
#define M 32
#define N 1

void initOutput(double *input) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            input[i * SIZE + j] = 0;
        }
    }
}

void initInput(double *input1, double *input2) {
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            input1[i * SIZE + j] = (double) rand();
            input2[i * SIZE + j] = (double) rand();
        }
    }
}

void printM(double *matrix) {
    printf("\n");
    for (int j = 0; j < SIZE; j++) {             //jki
        for (int i = 0; i < SIZE; i++) {
            printf("%f ", round(matrix[j * SIZE + i]));
        }
        printf("\n");
    }
}

void matmuljki(const double *input1, const double *input2, double *output) {
    for (int j = 0; j < SIZE; j++) {             //jki
        for (int k = 0; k < SIZE; k++) {
            for (int i = 0; i < SIZE; i++) {
                output[j * SIZE + i] += input1[j * SIZE + k] * input2[k * SIZE + i];
            }
        }
    }

}

int verify(double *matrix, double *test) {
    for (int j = 0; j < SIZE; j++) {             //jki
        for (int i = 0; i < SIZE; i++) {
            if (abs(matrix[j * SIZE + i] / test[j * SIZE + i])<double(1-(double)1/100000000))return 0;
        }
    }
    return 1;
}

__global__ void gpu_test(){
    if(!threadIdx.x)printf("GPU is on!\n");
}

__global__ void cuda_matmulkji(double *a, double *b, double *c){
    int i= threadIdx.x;
    int iStep= blockDim.x;/*
    int jinit= blockIdx.x;
    int jStep= gridDim.x;*/
    //printf("i: %d, i Step: %d\n",i,iStep);
    //printf("j: %d, j Step: %d\n",jinit,jStep);
    for(;i<SIZE;i+=iStep){
        double tmp=0;
        for(int k=0; k < SIZE; k ++){
            tmp += a[blockIdx.x * SIZE + k] * b[k * SIZE + i];
        }
        c[blockIdx.x*SIZE + i]=tmp;
    }

            //printf("i: %d , j: %d , nr: %f \n",i,j,c[j*SIZE + i]);
}
int main(int argc, char *argv[]) {

    //gpu_test<<<1,1>>>();

    double *a, *b, *c;
    cudaMallocManaged(&a,SIZE*SIZE*sizeof(double));
    cudaMallocManaged(&b,SIZE*SIZE*sizeof(double));
    cudaMallocManaged(&c,SIZE*SIZE*sizeof(double));

    //test = (double *) malloc(SIZE * SIZE * sizeof(double));
    initInput(a, b);
    //initOutput(test);

    //matmuljki(a, b, test);

    initOutput(c);
    printf("\n");

    //a warp/thread block has 32 threads and an NVIDA V100 has 5120 cores
    //this means we have 160 thread blocks so "micro processors" with multiple cores each
    //those 80 thread blocks match memory alignment with SIZE/160 for example 1024/160 = 6
    //the process will take exactly 6 gpu context switches/iterations to finish

    //use all 80 available thread blocks and the full warp size of 32
    int deviceId;
    cudaGetDevice(&deviceId);
    cudaMemPrefetchAsync(a, SIZE*SIZE*sizeof(double),deviceId);
    cudaMemPrefetchAsync(b, SIZE*SIZE*sizeof(double),deviceId);
    cudaMemPrefetchAsync(c, SIZE*SIZE*sizeof(double),deviceId);
    cuda_matmulkji<<<SIZE,32>>>(a,b,c);
    cudaDeviceSynchronize();

    cudaMemPrefetchAsync(c, SIZE*SIZE*sizeof(double),cudaCpuDeviceId);

    cudaDeviceSynchronize();

    /*if (verify(c, test)) {
        printf("Verification success");
    }
    else { printf("Verification Failed check thread and team Sizes"); };*/
    //printM(c);
    //printM(test);
    cudaFree(a);
    cudaFree(b);
    cudaFree(c);
   // free(test);

    return 0;
}
