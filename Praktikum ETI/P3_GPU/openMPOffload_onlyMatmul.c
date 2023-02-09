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
#include <math.h>
#ifndef SIZE
#define SIZE 3840
#endif 
#define NUMTEAMS 80
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
void printM(double * matrix){
    for (int j = 0; j < SIZE; j++)  {             //jki
        for (int i = 0; i < SIZE; i++){
            printf("%f ",round(matrix[j*SIZE+i]));
        }
        printf("\n");
    }
}
void matmuljki(const double *input1, const double *input2, double *output){
    for (int j = 0; j < SIZE; j++)  {             //jki
        for (int k = 0; k < SIZE; k++) {
            for (int i = 0; i < SIZE; i++){  
                    output[j * SIZE + i] += input1[j * SIZE + k] * input2[k * SIZE + i];
            }
        }
    }

}
int verify(double * matrix,double *test){
    for (int j = 0; j < SIZE; j++)  {             //jki
        for (int i = 0; i < SIZE; i++){
            if(matrix[j*SIZE+i]!=test[j*SIZE+i])return 0;
        }
    }
    return 1;
}
void getteam(){
        int team = omp_get_team_num();
        printf("Team: %d \n,",team);
}
void getthread(){
        int thread = omp_get_thread_num();
        printf("Thread: %d \n,",thread);
}
int main() {

    double *a, *b, *c;
    a = (double*)malloc(SIZE*SIZE*sizeof(double));
    b = (double*)malloc(SIZE*SIZE*sizeof(double));
    c = (double*)malloc(SIZE*SIZE*sizeof(double));
    /*test = (double*)malloc(SIZE*SIZE*sizeof(double));
    initInput(a,b);
    initOutput(test);

    matmuljki(a,b,test);
    */
    //initOutput(c);
    //printf("\n");
    #pragma omp target data map(to:a[:SIZE*SIZE]) map(to: b[:SIZE*SIZE]) map(from: c[:SIZE*SIZE])
    {
                    #pragma omp target teams num_teams(NUMTEAMS)
                    {
                    int team=omp_get_team_num();
                    int init=team*SIZE/NUMTEAMS;
                    int stop=init+SIZE/NUMTEAMS;
                    for (int j = init; j < stop; j++)  {  

                                #pragma omp parallel for
                                for (int i = 0; i < SIZE; i++) {
                                    //printf("Team: %d , Thread: %d ,Index: %d \n,",omp_get_team_num(),omp_get_thread_num(),i);
                                    //double tmp = 0;
                                    for (int k = 0; k < SIZE; k++){  
                                        c[j*SIZE+i]+= a[j * SIZE + k] * b[k * SIZE + i];
                                    }
                                    //c[j*SIZE+i]=tmp;
                                }
                        }
                    }
    }
    /*if(verify(c,test)){printf("Verification success");}
    else{printf("Verification Failed check thread and team Sizes");};
    */
    free(a);
    free(b);
    free(c);
    //free(test);

}
