#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <omp.h>
#include <time.h>

int n = 5;
int **matrix;


void print();
int** instantiateRandomMatrix(int n);
void *swapTrans(void *info);


int main()
{
    printf("Enter the size of the matrix: ");
    scanf("%d",&n);
    printf("Size of Matrix: %d\n",n);
    matrix = instantiateRandomMatrix(n);
    // print();    
    pthread_t threads[n];
    clock_t first_time,last_time;
    float diff;
    // clock_t last_time;
    printf("This is the basic algorithm\n");
        first_time = clock();
        for(int i = 0; i < n; i++)
        {
            for(int j = i; j < n; j++)
            {
                int swap = matrix[i][j];
                matrix[i][j] = matrix[j][i];
                matrix[j][i] = swap;
            }
        }
        last_time = clock();
        diff = (float)(last_time-first_time)/CLOCKS_PER_SEC;
        printf("Times in seconds: %f\n",diff);
    #ifdef _OPENMP
        matrix = instantiateRandomMatrix(n);
        printf("\nopenMp Naive-Threaded Algorithm\n");
        first_time = clock();
        #pragma omp parallel for
        for(int i = 0; i < n; i++)
        {
            for(int j = i; j < n; j++)
            {
                int swap = matrix[i][j];
                matrix[i][j] = matrix[j][i];
                matrix[j][i] = swap;
            }
        }
        last_time = clock();
        diff = (float)(last_time-first_time)/CLOCKS_PER_SEC;
        printf("Times in seconds: %f\n",diff);

        matrix = instantiateRandomMatrix(n);
        printf("\nopenMp Diagonal Algorithm\n");
        first_time = clock();
        mp_set_num_threads(n);
        
        #pragma omp parallel 
        
        for (int i = omp_get_thread_num(); i < n; i++){
            int j = omp_get_thread_num();
            int swap = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = swap;
        }
//         #pragma omp parallel for
//         for(int i = 0; i < n; i++)
//         {
//             for(int j = i; j < n; j++)
//             {
//                 // printf("This is the thread number: %d\n",omp_get_thread_num());
//                 int swap = matrix[i][j];
//                 matrix[i][j] = matrix[j][i];
//                 matrix[j][i] = swap;
//             }
            
//         }
        last_time = clock();
        diff = (float)(last_time-first_time)/CLOCKS_PER_SEC;
        printf("Times in seconds: %f\n",diff);

    #else
        printf("\nPthread Algorithm\n");
        first_time = clock();
        for (int i = 0; i < n; i++)
        {
            long index = i;
            pthread_create(&threads[i],NULL,*swapTrans,(void*)index);
        }
        for (int i = 0; i < n; i++)
        {
            pthread_join(threads[i],NULL);
        }
        last_time = clock();
        diff = (double)(last_time-first_time)/CLOCKS_PER_SEC;
        printf("Times in seconds %f\n",diff);
        
    #endif
        printf("Done ...\n");
    // print(); 
    
    return 0;
}

/*
Elements of the matrix A[N0][N1] should
be 4-byte integers, generated as random integers between 0 and N0.
*/
void *swapTrans(void *info)
{   
    long j = (long) info;
    for (int i = (int)j; i < n; i++){

        int swap = matrix[i][j];
        matrix[i][j] = matrix[j][i];
        matrix[j][i] = swap;
    }
    
}

int ** instantiateRandomMatrix(int n){
    int  **m = (int **)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        m[i] = (int *)malloc(n * sizeof(int));
    
    int num = 1;
    srand(time(0));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            m[i][j]=rand();
        }
    }
    return m;
}


void print(){
    for (int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
