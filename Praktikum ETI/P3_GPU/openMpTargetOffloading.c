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
#define SIZE 3
void initOutput(double * input3){
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
                input1[i*SIZE+j]=-1
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

    int n=500;
    double *a, *b, *c;
    a = (double*)malloc(n*sizeof(double));
    b = (double*)malloc(n*sizeof(double));
    c = (double*)malloc(n*sizeof(double));
    initInput(a,b);
    initOutput(c);


    #pragma omp target data map(fromto:a[:SIZE]) map(fromto: b[:SIZE]) map(fromto: c[:SIZE]){
    #pragma omp target teams num_teams(3)
    {
                matmuljki(a,b,c);
    }
    }
    for (int j = 0; j < SIZE; j++)  {             //jki
                for (int i = 0; i < SIZE; i++){  
                    printf("%f",c[j*SIZE+i]);
                }
                printf("\n");
    }

    }