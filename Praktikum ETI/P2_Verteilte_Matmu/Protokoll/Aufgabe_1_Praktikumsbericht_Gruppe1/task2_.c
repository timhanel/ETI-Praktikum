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
int rank, size;
double *finaloutput;
unsigned long rank_load_size = 0;
void printM(double *matrix){
    for(int j=0;j<SIZE;j++){
        for(int i=0;i<SIZE;i++){
            printf("%lld",(long long)matrix[j*SIZE+i]);
            printf(",");
        }
        printf("\n");
    }
}
void init(double * input1,double * input2){
    srand( time(NULL) );
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
                input1[i*SIZE+j]=(double)rand();
                input2[i*SIZE+j]=(double)rand();
        }
    }
}

void matmuljki(const double *input1, const double *input2, double *output) {
    int MPIInit=rank*(SIZE/size);
    int MPIStepBound=(rank+1)*(SIZE/size);
    for (int j = 0; j < SIZE; j++)  {             //jki
        for (int k = MPIInit; k < MPIStepBound; k++) {
            for (int i = 0; i < SIZE; i++){  
                    output[j * SIZE + i] += input1[j * SIZE + k] * input2[k * SIZE + i];
            }
        }
    }

}
//Base

int run(int S){
    struct timeval time;
    
    //build out file name
    SIZE = S;
    long long millis=0;

    finaloutput = malloc((SIZE * SIZE) * sizeof(double));
    for(int i=0;i<SIZE*SIZE;i++)finaloutput[i]=(double)0;



    double *input1=  malloc((SIZE * SIZE) * sizeof(double));
    double *input2=  malloc((SIZE * SIZE) * sizeof(double));
    double *output = malloc((SIZE * SIZE) * sizeof(double));
    for(int i=0;i<SIZE*SIZE;i++)output[i]=(double)0;

    MPI_Barrier(MPI_COMM_WORLD);
    if(rank==0){

        init(input1,input2);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(input1,SIZE*SIZE,MPI_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Bcast(input2,SIZE*SIZE,MPI_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank==0){
        gettimeofday(&time, NULL);
        millis = (time.tv_sec * (long long) 1000) + (time.tv_usec / 1000);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    matmuljki(input1,input2,output);
    MPI_Reduce(output,finaloutput,SIZE*SIZE,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD); 
    if(rank==0){
        gettimeofday(&time, NULL);
        long long t = ((time.tv_sec * (long long) 1000) + (time.tv_usec / 1000)) - millis;
        //long long durationMilliseconds=neumillis-millis;
        double res = 0;
        long long ops = 0;
        double a = SIZE * SIZE;
        a *= (2 * SIZE);
        a+=(SIZE * SIZE)*size; //MPI Reduce Steps
        ops = a;
        res = (double) a;
        res /= t;
        res /= (1000 * 1000);
        printf("%f",res);//GFLOPS

        printf(" %lld", ops);//Count of Operations
        printf(" %lld", t);//TimeInMilliseconds
        printf("\n");
        /*printf(" \n");
        printM(input1);
        printf(" \n");
        printM(input2);
        printf(" \n");
        printM(finaloutput);*/
    }
    /*if(rank==0){
        gettimeofday(&time, NULL);
        long long t = ((time.tv_sec * (long long) 1000) + (time.tv_usec / 1000)) - millis;
        //long long durationMilliseconds=neumillis-millis;
        double res = 0;
        long long ops = 0;
        double a = SIZE * SIZE;
        a *= (2 * SIZE);
        ops = a;
        res = (double) a;
        res /= t;
        res /= (1000 * 1000);
        printf("%f",res);//GFLOPS

        printf(" %lld", ops);//Count of Operations
        printf(" %lld", t);//TimeInMilliseconds
        printf("\n");
        /*printf(" \n");
        printM(input1);
        printf(" \n");
        printM(input2);
        printf(" \n");
        printM(finaloutput);
    }*/
    MPI_Barrier(MPI_COMM_WORLD);
    
    //matmulijk(input1,input2,output);

    free(input1);
    free(input2);
    free(output);
    free(finaloutput);

    return 0;
}
int main(int argc, char *argv[]) {
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double Sizes[3]={1024,2048,4096};
    for(int i=0;i<3;i++){
        if(rank==0){
            printf("\n");
            printf("Size ");
            printf("%d",(int)Sizes[i]);
            printf("\n");
        }
        for(int j=0;j<10;j++)run(Sizes[i]);
    }
    MPI_Finalize();
    
}
