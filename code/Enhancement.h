/*!
* \file Enhancement.h
* \brief Speech Enhancement
*
* Speech enhancement such as spectral subtraction and the Wiener
 * filter are included and implemented in the class, which is derived
 * from the class Analysis
*
*/
#ifndef INC_3002_ENHANCEMENT_H
#define INC_3002_ENHANCEMENT_H

#include "Analysis.h"


/// \brief Speech enhancement
///
/// Derived class from Analysis
class Enhancement: public Analysis{
public:


    Enhancement(int i) : Analysis(i) {

    }         ///< derived class constructor

    ~Enhancement(){}                            ///< derived class destructor


    /// Noise estimation
    ///
    /// \param z  the number of frames used in head
    /// \param x  the number of frames used in tail
    void noiseEstimation(int z, int x);

    /// Recursive noise estimation
    ///
    /// Used together with the spectralSubtraction()
    /// \param a  the smoothing factor used for recursive noise estimation
    void recursiveNoiseEstimation(float a);

    /// Spectral subtraction
    ///
    /// Subtract power spectrum of speech signal and estimated noise
    void spectralSubtraction();


    /// Wiener filter
    ///
    /// \param alpha smoothing factor alpha
    /// \param beta  smoothing factor beta
    void wienerFilter(float alpha,float beta);

    /// Perform Wiener filter with default parameters
    ///
    void runWiener();

    /// Perform Wiener filter with input parameters
    ///
    /// \param alpha smoothing factor alpha
    /// \param beta  smoothing factor beta
    void runWiener(float alpha,float beta);
};

#endif //INC_3002_ENHANCEMENT_H
