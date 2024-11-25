#ifndef NNFW_H
#define NNFW_H

#include <n_types.h>

/** Creating a neural network using the specified parameters in the config array
 * 
 * @param   config  Array of neural network configuration.
 *                  The first element is always responsible
 *                  for the number of inputs, the last element
 *                  is always responsible for the number of outputs,
 *                  everything in between is responsible for
 *                  the number of hidden layers and the number of
 *                  neurons in them.
 *                  For example, {2, 4, 5, 3}:
 *                  the neural network will have 2 inputs,
 *                  there are three outputs,
 *                  the first hidden layer has 4 neurons,
 *                  the second hidden layer has 5 neurons
 * @param   size    The size of the configuration array
 * @return  
 */
N_NETWORK * CreateN (const N_CONFIG * const config, const N_CONFIG_SIZE size);


/** Freeing up memory allocated for the neural network
 * 
 * @param   N   A pointer to the neural network
 */
void FreeN (N_NETWORK * N);


/**  Displaying the output values of the neural network
 * 
 * @param   N   A pointer to the neural network
 */
void PrintOutputs (N_NETWORK * N);


/** Displaying all the values of the neural network
 * 
 * @param   N   A pointer to the neural network
 */
void PrintN (N_NETWORK * N);

#endif

