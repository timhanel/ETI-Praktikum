#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <mpi.h>

typedef struct unsigend unsigend;
int SIZE = 0;
int rank = 0, size = 0;
unsigned long rank_load_size = 0;

void init(double *input1) {
    double* tmp=malloc(rank_load_size * sizeof(double));
    srand(time(NULL));
    for (int i = 0; i < rank_load_size; i++) {
        tmp[i] = (double) rand();
    }

    MPI_Gather(tmp,rank_load_size,MPI_DOUBLE,input1,rank_load_size,MPI_DOUBLE,0,MPI_COMM_WORLD);
    free(tmp);

    MPI_Bcast(input1,SIZE*SIZE,MPI_DOUBLE,0,MPI_COMM_WORLD);
}

void matmulikj(const double *input1, const double *input2, double *output) {
    for (int i = rank*(SIZE/size); i < (rank+1)*(SIZE/size); i++) { //ikj
        for (int k = 0; k < SIZE; k++) {
            for (int j = 0; j < SIZE; j++) {
                output[j * SIZE + i] += input1[j * SIZE + k] * input2[k * SIZE + i];
            }
        }
    }
    MPI_Allreduce(output,output,SIZE*SIZE,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
}

void matmulijkLoopUnrolling(const double *input1, const double *input2, double *output) {
    for (int i = rank*(SIZE/size); i < (rank+1)*(SIZE/size); i++) //ijk
        for (int j = 0; j + 3 < SIZE; j++) {
            for (int k = 0; k + 15 < SIZE; k += 16) {
                output[j * SIZE + i] += input1[j * SIZE + k] * input2[k * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 1] * input2[(k + 1) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 2] * input2[(k + 2) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 3] * input2[(k + 3) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 4] * input2[(k + 4) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 5] * input2[(k + 5) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 6] * input2[(k + 6) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 7] * input2[(k + 7) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 8] * input2[(k + 8) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 9] * input2[(k + 9) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 10] * input2[(k + 10) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 11] * input2[(k + 11) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 12] * input2[(k + 12) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 13] * input2[(k + 13) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 14] * input2[(k + 14) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 15] * input2[(k + 15) * SIZE + i];
            }
            j++;
            for (int k = 0; k + 15 < SIZE; k += 16) {
                output[j * SIZE + i] += input1[j * SIZE + k] * input2[k * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 1] * input2[(k + 1) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 2] * input2[(k + 2) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 3] * input2[(k + 3) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 4] * input2[(k + 4) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 5] * input2[(k + 5) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 6] * input2[(k + 6) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 7] * input2[(k + 7) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 8] * input2[(k + 8) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 9] * input2[(k + 9) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 10] * input2[(k + 10) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 11] * input2[(k + 11) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 12] * input2[(k + 12) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 13] * input2[(k + 13) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 14] * input2[(k + 14) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 15] * input2[(k + 15) * SIZE + i];
            }
            j++;
            for (int k = 0; k + 15 < SIZE; k += 16) {
                output[j * SIZE + i] += input1[j * SIZE + k] * input2[k * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 1] * input2[(k + 1) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 2] * input2[(k + 2) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 3] * input2[(k + 3) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 4] * input2[(k + 4) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 5] * input2[(k + 5) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 6] * input2[(k + 6) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 7] * input2[(k + 7) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 8] * input2[(k + 8) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 9] * input2[(k + 9) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 10] * input2[(k + 10) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 11] * input2[(k + 11) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 12] * input2[(k + 12) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 13] * input2[(k + 13) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 14] * input2[(k + 14) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 15] * input2[(k + 15) * SIZE + i];
            }
            j++;
            for (int k = 0; k + 15 < SIZE; k += 16) {
                output[j + 1 * SIZE + i] += input1[j * SIZE + k] * input2[k * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 1] * input2[(k + 1) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 2] * input2[(k + 2) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 3] * input2[(k + 3) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 4] * input2[(k + 4) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 5] * input2[(k + 5) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 6] * input2[(k + 6) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 7] * input2[(k + 7) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 8] * input2[(k + 8) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 9] * input2[(k + 9) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 10] * input2[(k + 10) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 11] * input2[(k + 11) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 12] * input2[(k + 12) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 13] * input2[(k + 13) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 14] * input2[(k + 14) * SIZE + i];
                output[j * SIZE + i] += input1[j * SIZE + k + 15] * input2[(k + 15) * SIZE + i];
            }
        }
    MPI_Allreduce(output,output,SIZE*SIZE,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
}

void matmulijkTiling(const double *input1, const double *input2, double *output) {
    for (int i = rank*(SIZE/size); i < (rank+1)*(SIZE/size); i+=8)
        for (int j = 0; j < SIZE; j += 8)
            for (int k = 0; k < SIZE; k += 8)
                for (int i2 = i; i2 < i + 8; i2 += 1)
                    for (int j2 = j; j2 < j + 8; j2 += 1)
                        for (int k2 = k; k2 < k + 8; k2 += 1)
                            output[j * SIZE + i] += input1[j * SIZE + k] * input2[k * SIZE + i];
    MPI_Allreduce(output,output,SIZE*SIZE,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
}

//Baseline
void matmulijk(const double *input1, const double *input2, double *output) {
    for (int i = rank*(SIZE/size); i < (rank+1)*(SIZE/size); i++) { //kij
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < SIZE; k++) {
                output[j * SIZE + i] += input1[j * SIZE + k] * input2[k * SIZE + i];
            }
        }
    }
    MPI_Allreduce(output,output,SIZE*SIZE,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
}

void matmulSchleifenvertauschjki(const double *input1, const double *input2, double *output) {
    for (int j = rank*(SIZE/size); j < (rank+1)*(SIZE/size); j++) {                //jki
        for (int k = 0; k < SIZE; k++) {
            for (int i = 0; i < SIZE; i++) {
                output[j * SIZE + i] += input1[j * SIZE + k] * input2[k * SIZE + i];
            }
        }
    }
    MPI_Allreduce(output,output,SIZE*SIZE,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
}

void matmulijkWTemporary(const double *input1, const double *input2, double *output) {
    for (int i = rank*(SIZE/size); i < (rank+1)*(SIZE/size); i++) { //kij
        for (int j = 0; j < SIZE; j++) {
            double d = 0;
            for (int k = 0; k < SIZE; k++) {
                d += input1[j * SIZE + k] * input2[k * SIZE + i];
            }
            output[j * SIZE + i] = d;
        }
    }
    MPI_Allreduce(output,output,SIZE*SIZE,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
}

void matmuljik(const double *input1, const double *input2, double *output) {
    for (int j = rank*(SIZE/size); j < (rank+1)*(SIZE/size); j++) {        //jik
        for (int i = 0; i < SIZE; i++) {
            for (int k = 0; k < SIZE; k++) {
                output[j * SIZE + i] += input1[j * SIZE + k] * input2[k * SIZE + i];
            }
        }
    }
    MPI_Allreduce(output,output,SIZE*SIZE,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
}

void matmulkji(const double *input1, const double *input2, double *output) {
    for (int k = rank*(SIZE/size); k < (rank+1)*(SIZE/size); k++) {
        for (int j = 0; j < SIZE; j++) {                //kji
            for (int i = 0; i < SIZE; i++) {
                output[j * SIZE + i] += input1[j * SIZE + k] * input2[k * SIZE + i];
            }
        }
    }
    MPI_Allreduce(output,output,SIZE*SIZE,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_rank(MPI_COMM_WORLD, &size);

    if (argc != 4) {
        if(rank == 0){
            printf("args1: MatrixSize, args2: LoopSize");
            printf("%d", argc);
        }
        MPI_Finalize();
        return 1;
    };

    //build out file name
    char *l = argv[1];
    char *d = argv[2];
    char *f = argv[3];
    unsigned long loopsize = atoll(d);
    SIZE = atoi(l);
    unsigned long funcnum = atoll(f);

    double *input1 = malloc((SIZE * SIZE) * sizeof(double));
    double *input2 = malloc((SIZE * SIZE) * sizeof(double));
    double *output = malloc((SIZE * SIZE) * sizeof(double));

    unsigned long rest_el=SIZE%size;
    if(rest_el){
        if(rank==0){
            printf("Number of ranks did not evenly divide problem size! Aborting!");
            printf(" \n");
        }
        MPI_Finalize();
        return 1;
    }
    rank_load_size=SIZE*SIZE/size;//all ranks share load

    init(input1);
    init(input2);

    for(unsigned long i=0;i<SIZE*SIZE;i++){
        output[i]=(double)0;//random bytes interpreted as double can cause segmentation fault by randomly showing infinity
    }

    if(rank==0){
        double res = 0;
        long long ops = 0;

        struct timeval time;
        gettimeofday(&time, NULL);

        MPI_Barrier(MPI_COMM_WORLD);
        long long millis = (time.tv_sec * (long long) 1000) + (time.tv_usec / 1000);
        for (unsigned long a = 0; a < loopsize; a++) {
            switch (funcnum) {
                case 0: matmulijk(input1, input2, output); break;
                case 1: matmulSchleifenvertauschjki(input1,input2,output); break;
                case 2: matmulijkTiling(input1,input2,output); break;
                default: matmulijkWTemporary(input1,input2,output); break;
            }
        }

        gettimeofday(&time, NULL);
        long long t = ((time.tv_sec * (long long) 1000) + (time.tv_usec / 1000)) - millis;
        //long long durationMilliseconds=neumillis-millis;

        double a = SIZE * SIZE;
        a *= (2 * SIZE);
        a *= loopsize;
        ops = a;
        res = (double) a;
        res /= t;
        res /= (1000 * 1000);
        printf(" %f", res);//GFLOPS
        printf(" %lld", ops);//Count of Operations
        printf(" %lld", t);//TimeInMilliseconds
        printf(" \n");
    }
    else{
        MPI_Barrier(MPI_COMM_WORLD);
        for (unsigned long a = 0; a < loopsize; a++) {
            switch (funcnum) {
                case 0: matmulijk(input1, input2, output); break;
                case 1: matmulSchleifenvertauschjki(input1,input2,output); break;
                case 2: matmulijkTiling(input1,input2,output); break;
                default: matmulijkWTemporary(input1,input2,output); break;
            }
        }
    }

    free(input1);
    free(input2);
    free(output);
    MPI_Finalize();
    return 0;
}
