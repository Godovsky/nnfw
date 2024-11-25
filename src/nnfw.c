#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nnfw.h>

void PrintOutputs (N_NETWORK * N)
{
    printf ("\n----------------------------------------------------------------------------------------------------\n");
    printf ("Outputs:\n");
    for (int neu = 0; neu < N->output.numOfNeurons; neu++)
        printf ("  N->output.neurons[%d]value = %.2f\n", neu, N->output.neurons[neu].value);
    printf ("----------------------------------------------------------------------------------------------------\n\n");
}

void PrintN (N_NETWORK * N)
{
    printf ("\n----------------------------------------------------------------------------------------------------\n");
    if (NULL != N)
    {
        printf ("Inputs: %u\n", N->numOfInputs);
        for (int i = 0; i < N->numOfInputs; i++)
        {
            printf ("  N->inputs[%d] = %f\n", i, N->inputs[i]);
        }
        printf ("\n");

        if (NULL != N->layers)
        {
            for (int lay = 0; lay < N->numOfLayers; lay++)
            {
                printf ("Layer %u:\n", lay+1);
                for (int neu = 0; neu < N->layers[lay].numOfNeurons; neu++)
                {
                    printf ("  N->layers[%d].neyrons[%d].value = %f\n", lay, neu, N->layers[lay].neurons[neu].value);

                    for (int wei = 0; wei < N->layers[lay].neurons[neu].numOfWeights; wei++)
                        printf ("    N->layers[%d].neyrons[%d].weights[0][%d] = %f\n", lay, neu, wei, N->layers[lay].neurons[neu].weights[wei]);
                }
                printf ("  Bias: %f\n\n", N->layers[lay].bias);
            }
        }
        else printf ("Layers: Structure N_NETWORK does'nt have layers\n\n");

        printf ("Outputs:\n");
        for (int neu = 0; neu < N->output.numOfNeurons; neu++)
        {
            printf ("  N->output.neurons[%d]value = %f\n", neu, N->output.neurons[neu].value);
            for (int wei = 0; wei < N->output.neurons[neu].numOfWeights; wei++)
            {
                printf ("    N->output.neurons->weights[%d] = %f\n", wei, N->output.neurons[neu].weights[wei]);
            }
        }
        printf ("  Bias: %f\n", N->output.bias);
    }
    else printf ("Structure N_NETWORK does'nt exist\n");

    printf ("----------------------------------------------------------------------------------------------------\n\n");
}

N_NETWORK * CreateN (const N_CONFIG * const config, const N_CONFIG_SIZE size)
{
    if (size < 2)
    {
        printf ("The size of network configuration is less than 2\n");
        return NULL;
    }

    for (int i = 0; i < size; i++)
    {
        if (config[i] < 1)
        {
            printf ("One of the config parameter is less than 1\n");
            return NULL;
        }
    }
    
    N_NETWORK * N = malloc (sizeof(N_NETWORK));
    if (NULL == N)
    {
        printf ("Unsuccessful memory allocatin for N_NETWORK\n");
        return NULL;
    }
    N->numOfInputs = config[0];

    int inputs_size = config[0] * sizeof(INPUT_TYPE);
    N->inputs = malloc (inputs_size);
    if (NULL == N->inputs)
    {
        printf ("Unsuccessful memory allocating for inputs\n");
        free (N);
        return NULL;
    }

    for (int i = 0; i < N->numOfInputs; i++)
        N->inputs[i] = 0;

    N->numOfLayers = 0;
    int layers_size = (size-2) * sizeof(N_LAYER);
    if (layers_size > 0)
    {
        N->numOfLayers = size-2;

        N->layers = malloc (layers_size);
        if (NULL == N->layers)
        {
            printf ("Unsuccessful memory allocating for layers\n");
            free (N->inputs);
            free (N);
            return NULL;
        }

        for (int i = 0; i < N->numOfLayers; i++)
        {
            N->layers[i].bias = 0.0;
            N->layers[i].numOfNeurons = config[i+1];

            int neurons_size = config[i+1] * sizeof(N_NEURON);
            N->layers[i].neurons = malloc (neurons_size);
            if (NULL == N->layers[i].neurons)
            {
                printf ("Unsuccessful memory allocatin for neurons in layer %d\n", i);
                free (N->inputs);

                for (int j = 0; j < i; j++)
                    free (N->layers[j].neurons);

                free (N->layers);
                free (N);
                return NULL;
            }

            int weights_size = config[i] * sizeof(WEIGHT_TYPE);
            for (int j = 0; j < N->layers[i].numOfNeurons; j++)
            {
                N->layers[i].neurons[j].value = 0.0;
                N->layers[i].neurons[j].numOfWeights = config[i];

                N->layers[i].neurons[j].weights = malloc (weights_size);
                if (NULL == N->layers[i].neurons[j].weights)
                {
                    printf ("Unsuccessful memory allocating for weights in neyron %d in layer %d\n", j, i);
                    free (N->inputs);

                    for (int k = 0; k < i; k++)
                        for (int l = 0; l < j; l++)
                            free (N->layers[k].neurons[l].weights);

                    for (int k = 0; k < i; k++)
                        free (N->layers[k].neurons);

                    free (N->layers);
                    free (N);
                    return NULL;
                }

                for (int k = 0; k < N->layers[i].neurons[j].numOfWeights; k++)
                    N->layers[i].neurons[j].weights[k] = 0.5;
            }
        }
    }
    else
    {
        printf ("NOTE: N_NETWORK does'nt have any layers\n");
        N->layers = NULL;
    }

    int output_layer_size = config[size-1] * sizeof(OUTPUT_LAYER);
    N->output.numOfNeurons = config[size-1];

    N->output.neurons = malloc (output_layer_size);
    if (NULL == N->output.neurons)
    {
        printf ("Unsuccessful memory allocatin for output neurons\n");
        free (N->inputs);

        for (int i = 0; i < N->numOfLayers; i++)
            for (int j = 0; j < N->layers[i].numOfNeurons; j++)
                free (N->layers[i].neurons[j].weights);

        for (int i = 0; i < N->numOfLayers; i++)
            free (N->layers[i].neurons);

        free (N->layers);
        free (N);
        return NULL;
    }

    for (int i = 0; i < N->output.numOfNeurons; i++)
    {
        N->output.neurons[i].value = 0.0;
        N->output.neurons[i].numOfWeights = config[size-2];

        int output_weights_size = config[size-2] * sizeof(WEIGHT_TYPE);
        N->output.neurons[i].weights = malloc (output_weights_size * sizeof(WEIGHT_TYPE));
        if (NULL == N->output.neurons[i].weights)
        {
            printf ("Unsuccessful memory allocating for weights in output neyron\n");
            free (N->inputs);

            for (int k = 0; k < N->numOfLayers; k++)
                for (int l = 0; l < N->layers[k].numOfNeurons; l++)
                    free (N->layers[k].neurons[l].weights);

            for (int k = 0; k < N->numOfLayers; k++)
                free (N->layers[k].neurons);

            for (int k = 0; k < i; k++)
                free (N->output.neurons[k].weights);

            free (N->output.neurons);

            free (N->layers);
            free (N);
            return NULL;
        }
        for (int k = 0; k < N->output.neurons[i].numOfWeights; k++)
            N->output.neurons[i].weights[k] = 0.5;

        N->output.bias = 0.0;
    }

    return N;
}

void FreeN (N_NETWORK * N)
{
    if (NULL != N->inputs) free (N->inputs);

    if (NULL != N->layers)
    {
        for (int i = 0; i < N->numOfLayers; i++)
        {
            for (int j = 0; j < N->layers[i].numOfNeurons; j++)
                if (NULL != N->layers[i].neurons[j].weights)
                    free (N->layers[i].neurons[j].weights);

            if (NULL != N->layers[i].neurons)
                free (N->layers[i].neurons);
        }
    }

    if (NULL != N->layers) free (N->layers);

    for (int i = 0; i < N->output.numOfNeurons; i++)
        if (NULL != N->output.neurons[i].weights)
            free (N->output.neurons[i].weights);

    if (NULL != N->output.neurons)
        free (N->output.neurons);
    
    if (NULL != N) free (N);
}

