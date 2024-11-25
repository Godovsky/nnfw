#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nnfw.h>
#include <train.h>

#define GET_ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])

#define NUM_OF_ITER 100000

int main (int argc, char * argv[])
{
    /* Configuring. Two inputs, one hidden layer with 4 neurons, three outputs */
    N_CONFIG config[] = {2,4,3};

    N_NETWORK * N = CreateN (config, GET_ARRAY_SIZE(config)); 
    if (NULL == N)
    {
        printf ("Error of neural network creating\n");
        return 1;
    }

    /* PrintN (N); */

    /*  ____|XOR|AND| OR|
        0 0 | 0 | 0 | 0 |
        0 1 | 1 | 0 | 1 |
        1 0 | 1 | 0 | 1 |
        1 1 | 0 | 1 | 1 |
        Two inputs and three outputs */
    FLOAT_DATA d[] = {
        0,0, 0,0,0,
        0,1, 1,0,1,
        1,0, 1,0,1,
        1,1, 0,1,1
    };

    /* columns = <num of inputs> + <num of outputs> */
    COL columns = config[0]+config[GET_ARRAY_SIZE(config)-1];
    ROW rows = GET_ARRAY_SIZE(d)/columns;
    DATA_TRAIN Data = {rows, columns, d};

    for (int i = 1; i <= NUM_OF_ITER; i++)
    {
        if (i % (NUM_OF_ITER/100) == 0) printf ("\rLerning: %d%%", i / (NUM_OF_ITER/100));

        Train (N, &Data, 0.01, 0.01);
    }
    printf ("\n");
    
    /* PrintN (N); */

    int quit = 0;
    while (1)
    {
        printf ("Your inputs (less than 0 to quit)\n");
        for (int i = 0; i < N->numOfInputs; i++)
        {
            printf ("Input %d: ", i+1);
            scanf ("%f", &N->inputs[i]);
            if (N->inputs[i] < 0)
            {
                FreeN (N);
                printf ("Done\n");
                quit = 1;
                break;
            } 
        }
        if (quit) break;
        Calculate (N);

        PrintOutputs (N);

        printf ("____|XOR|AND| OR|\n");

        printf ("%d %d | %s | %s | %s |\n\n",
                (int)N->inputs[0],
                (int)N->inputs[1],
                (N->output.neurons[0].value < 0.5 ? "0" : "1"),
                (N->output.neurons[1].value < 0.5 ? "0" : "1"),
                (N->output.neurons[2].value < 0.5 ? "0" : "1"));
    }

    return 0;
}

