
/*!
* \file RunMenu.h
* \brief Menu to choose and execute different operations.
*
* Functions: testIO, testIDFT, addNoise2Speech, addGaussianNoise2Speech,
* spectralSubtract, wiener
*/
#ifndef CSC3002_RUNMENU_H
#define CSC3002_RUNMENU_H

#include <iostream>
#include "IO.h"
#include "Simulation.h"
#include "Evaluation.h"

class RunMenu {
public:

    /// Test IO function
    ///
    /// Input speech WAV file, output the same WAV file
    void testIO();

    /// Test IDFT
    ///
    /// Input speech WAV file to obtain power spectrum and phase,
    /// apply IDFT to restore WAV file
    void testIDFT();

    /// Add noise to speech
    ///
    /// Add noise from WAV file to another WAV file given SNR
    void addNoise2Speech();

    /// Test IDFT
    ///
    /// Input speech WAV file to obtain power spectrum and phase,
    /// apply IDFT to restore WAV file
    void addGaussianNoise2Speech();

    /// Spectral subtraction
    ///
    /// Apply recursive noise estimation and perform spectral subtraction
    /// for speech enhancement
    void spectralSubtract();

    /// Spectral subtraction
    ///
    /// Apply recursive noise estimation and perform spectral subtraction
    /// for speech enhancement
    void wiener();

    void evaluation();
};


#endif //CSC3002_RUNMENU_H
