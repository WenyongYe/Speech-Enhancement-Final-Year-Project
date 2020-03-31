
/*!
* \file Simulation.h
* \brief Noisy Speech Simulation
*
* Header file for noisy speech simulation
* Include two ways for implementation
* 1. Add Gaussian white noise to clean speech
* 2. Add noisy file to clean speech file
*
*/

#ifndef CSC3002_SIMULATION_H
#define CSC3002_SIMULATION_H
#include <iostream>
#include "WAVData.h"
#include <math.h>
#include "IO.h"

class Simulation {

public:

    float SNR;           ///< SNR value
    float pNoise;        ///< power of noise
    float pSignal;       ///< power of signal
    float pSignal1;      ///< power of noise (Sample 1)
    float pSignal2;      ///< power of noise (Sample 2)
    float sum;           ///< sum value
    float noiseNewStd;   ///< new std of noise
    float noiseOldStd;   ///< old std of noise
    float delta;         ///< delta
    float sigma;         ///< sigma
    float *whiteNoise;   ///< Gaussian white noise
    float *noise;        ///< noisy recording
	float  PI = 3.1416;

    /// Input value of SNR
    ///
    void inputSNR();				


    /// Box-Muller Transform
    ///
    /// Box-Muller transform to perform Gaussian distribution.
    /// Convert uniform distribution to Gaussian distribution.
    /// Used for generating random number for Gaussian white noise.
    /// \param sigma standard deviation
    /// \return Gaussian distribution random number
    float box_muller(float sigma);

    /// Add Gaussian white noise to speech
    ///
    /// Generate Gaussian white noise and add it to given speech,
    /// according to specific SNR
    /// \param twavData WAV data
    void addGaussianNoise(TWAVData *twavData);

    /// Add noisy file to speech
    ///
    /// Given noisy speech and clean speech,
    /// add noise to clean speech according to specific SNR
    /// \param twavData1 WAV data 1 (Noisy speech)
    /// \param twavData2 WAV data 2 (Clean speech)
    void addNoise(TWAVData *twavData1,TWAVData *twavData2);
};


#endif //CSC3002_SIMULATION_H
