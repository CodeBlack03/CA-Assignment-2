#include <pthread.h>
#include <stdlib.h>

// Create other necessary functions here

typedef struct data
{
    int rowA;
    int colB;
    int N1;
    int *matA;
    int *matB;
    int *output;
} thr_data;

// // Fill in this function
void *help1(void *arr)
{
    thr_data *args = (thr_data *)arr;
    int *s = (int *)malloc(sizeof(int));
    *s = 0;
    // cerr << "N =" << args->N1 << " row = " << args->rowA << " col = " << args->colB << endl;
    for (int iter = 0; iter < (args->N1); iter++)
    {

        (*s) += (args->matA[(int)args->N1 * (int)args->rowA + (int)iter]) * (args->matB[(int)args->N1 * (int)iter + (int)args->colB]);
    }

    return (void *)s;
}

void multiThread(int N, int *matA, int *matB, int *output)
{
    assert(N >= 4 and N == (N & ~(N - 1)));
    for (int row = 0; row < N; row += 2)
    {
        for (int col = 0; col < N; col += 2)
        {
            pthread_t *thrs;
            thr_data *args;
            if ((thrs = (pthread_t *)malloc(sizeof(pthread_t) * (unsigned long)4)) == NULL ||
                (args = (thr_data *)malloc(sizeof(thr_data) * (unsigned long)6)) == NULL)
            {
                exit(EXIT_FAILURE);
            }
            for (int i = 0; i < 4; i++)
            {
                int r = row;
                int c = col;

                if (i == 1)
                {
                    r = r + 1;
                }
                else if (i == 2)
                {
                    c = c + 1;
                }
                else if (i == 3)
                {
                    r = r + 1;
                    c += 1;
                }
                args[i] = (thr_data){
                    .matA = matA,
                    .matB = matB,
                    .output = output,
                    .rowA = r,
                    .colB = c,
                    .N1 = N};
                pthread_create(&thrs[i], NULL, &help1, (void *)&args[i]);
            }
            int *sum1;
            int *sum2;
            int *sum3;
            int *sum4;

            if (pthread_join(thrs[0], (void **)&sum1) != 0)
                perror("failed to join1");
            if (pthread_join(thrs[1], (void **)&sum2) != 0)
                perror("failed to join2");
            if (pthread_join(thrs[2], (void **)&sum3) != 0)
                perror("failed to join3");
            if (pthread_join(thrs[3], (void **)&sum4) != 0)
                perror("failed to join4");

            // //      compute output indices

            int rowC = row >> 1;
            int colC = col >> 1;
            int indexC = rowC * (N >> 1) + colC;
            output[indexC] = (*sum1) + (*sum2) + (*sum3) + (*sum4);
        }
    }
}