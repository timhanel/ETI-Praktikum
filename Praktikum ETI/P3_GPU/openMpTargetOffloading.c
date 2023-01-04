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
#define SIZE 4096
void initOutput(double * input){
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
                input[i*SIZE+j]=0;
        }
    }
}
void initInput(double * input1,double * input2){
    srand( time(NULL) );
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
                input1[i*SIZE+j]=(double)rand();
                input2[i*SIZE+j]=(double)rand();
        }
    }
}
void matmuljki(const double *input1, const double *input2, double *output){
    #pragma omp target teams distribute parallel for
    for (int j = 0; j < SIZE; j++)  {             //jki
        for (int k = 0; k < SIZE; k++) {
            for (int i = 0; i < SIZE; i++){  
                    output[j * SIZE + i] += input1[j * SIZE + k] * input2[k * SIZE + i];
            }
        }
    }

}
int main(int argc, char *argv[]) {

    double *a, *b, *c;
    a = (double*)malloc(SIZE*SIZE*sizeof(double));
    b = (double*)malloc(SIZE*SIZE*sizeof(double));
    c = (double*)malloc(SIZE*SIZE*sizeof(double));
    initInput(a,b);
    initOutput(c);


    #pragma omp target data map(tofrom:a[:SIZE]) map(tofrom: b[:SIZE]) map(tofrom: c[:SIZE])
        #pragma omp target teams num_teams(3)
        {
                    matmuljki(a,b,c);
        }

    for (int j = 0; j < SIZE; j++)  {             //jki
                for (int i = 0; i < SIZE; i++){
                    printf("%f",c[j*SIZE+i]);
                }
                printf("\n");
    }

    free(a);
    free(b);
    free(c);

}