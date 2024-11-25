#ifndef TRAIN_H
#define TRAIN_H

#include <n_types.h>

#define FUNC_SIGMOID 0

/** Calculation of the input parameters of the neural network and
 * saving all the results in neurons in each hidden and output layers.
 * The values of the weights do not change
 * 
 * @param   N   Pointer to neural network
 */
void Calculate (N_NETWORK * N);


/** Determining the difference between the value received from the
 * neural network and the expected value
 * 
 * @param   N           A pointer to the neural network
 * @param   Data        A pointer to the training data
 * @param   outIndex    The index of the neuron in the output layer
 * 
 * @return  The value of the difference
 */
DIFFERENCE Difference (N_NETWORK * N, DATA_TRAIN * Data, int outIndex);


/** Neural network training based on the received data
 * 
 * @param   N       A pointer to the neural network
 * @param   Data    A pointer to the training data (passed through)
 * @param   offset  How much to offset the parameters of the neural network to determine the difference
 * @param   step    The step of gradient descent along the learning curve
 */
void Train (N_NETWORK * N, DATA_TRAIN * Data, OFFSET offset, STEP step);

#endif

