/*!
* \file Evaluation.h
* \brief Objective evaluation
*
* Segmental SNR used for objective quality measures
*
*/
#ifndef CSC3002_EVALUATION_H
#define CSC3002_EVALUATION_H
#include <iostream>
#include "WAVData.h"
#include "Analysis.h"
#include <math.h>


/// \brief Objective evaluation
///
/// Used in the RunMenu
class Evaluation {

public:
    /// Inverse Discrete Fourier Transform
    ///
    /// \param original original signal
    /// \param enhanced enhanced signal
    /// \param i sampling period
    /// \return segmental SNR result
    float segmentalSNR(TWAVData *original, TWAVData *enhanced,int i);
};


#endif //CSC3002_EVALUATION_H
