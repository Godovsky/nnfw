# A simple framework library for creating a simple neural network

## How to use

###  Examples of configuring a neural network.
A variant with 2 inputs, 1 hidden layer of 4 neurons and 3 outputs:

```C
N_CONFIG config[] = {2,4,3};
```

A variant with 4 inputs, with 3 hidden layers of 5, 6 and 4 neurons, respectively, with 1 output:

```C
N_CONFIG config[] = {4,5,6,4,1};
```

The configuration parameters are not limited to anything, except that it must describe the inputs and outputs, and there may not be hidden layers:

```C
N_CONFIG config[] = {10,1};
```

### Generating data for neural network training
The data is described in a one-dimensional array in such a way that all inputs and outputs are listed sequentially and alternately. The neural network reads these sections in blocks with inputs and outputs and makes a calculation, then proceeds to the next section:

```C
/*  ____|XOR|AND| OR|
    0 0 | 0 | 0 | 0 |
    0 1 | 1 | 0 | 1 |
    1 0 | 1 | 0 | 1 |
    1 1 | 0 | 1 | 1 |
    Two inputs and three outputs */
    float d[] = {
        0,0, 0,0,0, /* First section (2 inputs and 3 outputs) */
        0,1, 1,0,1, /* Second section */
        1,0, 1,0,1,
        1,1, 0,1,1
    };
```

The training data is stored in the Data structure with an indication of the number of columns and rows of the one-dimensional array d:

```C
int columns = config[0]+config[GET_ARRAY_SIZE(config)-1];
    int rows = GET_ARRAY_SIZE(d)/columns;
    DATA_TRAIN Data = {rows, columns, d};
```

### Training
Neural network training takes place in a loop in which the Train function is called. The number of iterations is selected experimentally, as are the last two arguments OFFSET and STEP of the Train function. The first argument is the address of the neural network, the second argument takes the address to the Data structure

```C
for (int i = 1; i <= NUM_OF_ITER; i++)
    Train (N, &Data, 0.01, 0.01);
```

### Displaying the entire neural network structure
The Printf(N) function outputs the entire structure of the neural network with all its current values of inputs, weights, neurons and outputs.

### Using an already trained neural network
In order for the neural network to recalculate new incoming data, it is necessary to write the necessary values at its inputs and call the Calculate function with passing the address of the neural network to it as an argument. After that, there will be new recalculated values at its outputs.

### Displaying the recalculation result on the screen
The PrintOutputs(N) function displays only the output values on the screen. You can also access the outputs by simply accessing them using the C syntax for structures (N->output.neurons[index].value).

### Deleting a neural network
The memory allocated for the neural network is freed by calling the FreeN(N) function

## Building

Enter the command:
```shell
make
```

After the build, the libneuronfw library and the example program will appear in the root directory of the project, in which the neural network is trained to calculate XOR, AND and OR. The program can be started by entering the command:
```shell
./example
```
or:
```shell
make run
```