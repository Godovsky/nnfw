#ifndef N_TYPES_H
#define N_TYPES_H

typedef float           OFFSET;
typedef float           STEP;
typedef float           DIFFERENCE;

typedef int             N_CONFIG;
typedef int             N_CONFIG_SIZE;

typedef int             NUM_OF_INPUTS;
typedef int             NUM_OF_LAYERS;
typedef int             NUM_OF_WEIGHTS;
typedef int             NUM_OF_NEURONS;

typedef float           INPUT_TYPE;
typedef float           NEURON_TYPE;
typedef float           WEIGHT_TYPE;
typedef float           BIAS_TYPE;

typedef INPUT_TYPE *    ARRAY_OF_INPUTS;
typedef WEIGHT_TYPE *   ARRAY_OF_WEIHGTS;

typedef struct
{
    NUM_OF_WEIGHTS      numOfWeights;
    NEURON_TYPE         value;
    ARRAY_OF_WEIHGTS    weights;
} N_NEURON;

typedef N_NEURON *      ARRAY_OF_NEURONS;

typedef struct
{
    NUM_OF_NEURONS      numOfNeurons;
    ARRAY_OF_NEURONS    neurons;
    BIAS_TYPE           bias;
} N_LAYER, OUTPUT_LAYER;

typedef N_LAYER *       ARRAY_OF_LAYERS;

typedef struct
{
    NUM_OF_LAYERS       numOfLayers;
    NUM_OF_INPUTS       numOfInputs;
    ARRAY_OF_INPUTS     inputs;
    ARRAY_OF_LAYERS     layers;
    OUTPUT_LAYER        output;
} N_NETWORK;

typedef int             ROW, COL;
typedef float           FLOAT_DATA;

typedef struct
{
    ROW                 row;
    COL                 col;
    FLOAT_DATA *        data;
} DATA_TRAIN;

#endif

