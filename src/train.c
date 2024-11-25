#include <stdio.h>
#include <string.h>
#include <math.h>

#include <nnfw.h>
#include <train.h>

NEURON_TYPE ActivationFunction (NEURON_TYPE n, const int funcType)
{
    switch (funcType)
    {
        case FUNC_SIGMOID:
            return 1.0 / (1.0 + expf (-n));
    }

    return 0;
}

void Calculate (N_NETWORK * N)
{
    for (int out = 0; out < N->output.numOfNeurons; out++)
    {
        N->output.neurons[out].value = 0.0;
        if (0 != N->numOfLayers)
        {
            for (int lay = 0; lay < N->numOfLayers; lay++)
            {
                for (int neu = 0; neu < N->layers[lay].numOfNeurons; neu++)
                {
                    N->layers[lay].neurons[neu].value = 0.0;
                    NEURON_TYPE tmp = 0;
                    for (int wei = 0; wei < N->layers[lay].neurons[neu].numOfWeights; wei++)
                        tmp += (lay == 0 ? N->inputs[wei] : N->layers[lay-1].neurons[wei].value) * N->layers[lay].neurons[neu].weights[wei];

                    N->layers[lay].neurons[neu].value = ActivationFunction (tmp + N->layers[lay].bias, FUNC_SIGMOID);
                }
            }

            for (int wei = 0; wei < N->output.neurons[out].numOfWeights; wei++)
                N->output.neurons[out].value += N->layers[N->numOfLayers-1].neurons[wei].value * N->output.neurons[out].weights[wei];

            N->output.neurons[out].value += N->output.bias;
        }
        else
        {
            for (int wei = 0; wei < N->output.neurons[out].numOfWeights; wei++)
                N->output.neurons[out].value += N->inputs[wei] * N->output.neurons[out].weights[wei];

            N->output.neurons[out].value += N->output.bias;
        }
    }
}

DIFFERENCE Difference (N_NETWORK * N, DATA_TRAIN * Data, int outIndex)
{
    float result = 0;
    for (int i = 0; i < Data->row; i++)
    {
        memcpy (N->inputs, &Data->data[i*Data->col], sizeof(INPUT_TYPE)*((size_t)(Data->col - N->output.numOfNeurons)));
        Calculate (N);

        DIFFERENCE dif = N->output.neurons[outIndex].value - Data->data[i*Data->col + Data->col - N->output.numOfNeurons + outIndex];
        result += dif*dif;
    }

    return result/Data->row;
}

void Train (N_NETWORK * N, DATA_TRAIN * Data, OFFSET offset, STEP step)
{
    if (NULL != N)
    {
        for (int out = 0; out < N->output.numOfNeurons; out++)
        {
            DIFFERENCE curCost = Difference (N, Data, out);
            DIFFERENCE newCost = 0.0;

            for (int wei = 0; wei < N->output.neurons[out].numOfWeights; wei++)
            {
                N->output.neurons[out].weights[wei] += offset;
                newCost = Difference (N, Data, out);
                N->output.neurons[out].weights[wei] -= offset;
                N->output.neurons[out].weights[wei] -= step*((newCost - curCost)/offset);
            }
            N->output.bias += offset;
            newCost = Difference (N, Data, out);
            N->output.bias -= offset;
            N->output.bias -= step*((newCost - curCost)/offset);
            
            if (NULL != N->layers)
            {
                for (int lay = 0; lay < N->numOfLayers; lay++)
                {
                    for (int neu = 0; neu < N->layers[lay].numOfNeurons; neu++)
                    {
                        for (int wei = 0; wei < N->layers[lay].neurons[neu].numOfWeights; wei++)
                        {
                            N->layers[lay].neurons[neu].weights[wei] += offset;
                            newCost = Difference (N, Data, out);
                            N->layers[lay].neurons[neu].weights[wei] -= offset;
                            N->layers[lay].neurons[neu].weights[wei] -= step*((newCost - curCost)/offset);
                        }
                    }
                    N->layers[lay].bias += offset;
                    newCost = Difference (N, Data, out);
                    N->layers[lay].bias -= offset;
                    N->layers[lay].bias -= step*((newCost - curCost)/offset);
                }
            }
        }
    }
}

