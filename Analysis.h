
/*!
* \file Analysis.h
* \brief Spectral Analysis
*
* Spectral analysis include functions: DFT, IDFT,
 * noise estimation, spectral subtraction and Wiener filter
*
*/
#ifndef CSC3002_Analysis_H
#define CSC3002_Analysis_H

#include <iostream>
#include "WAVData.h"
#include <math.h>


class Analysis {
public:

    Analysis(int i);         ///< constructor with sampling period
    ~Analysis();             ///< destructor

    float  PI=3.1416;        ///< pi
    int frmLen;              ///< frame length
    int frmShift;            ///< frame shift
    int nFFT;                ///< nearest power of 2 to frame length
    int nFFT2;               ///< half of nFFT
    int numFrms;             ///< number of frames

    int samp_rate;           ///< sampling rate
    int sam_period;          ///< sampling period

    float *w;                ///< speech wave
    float *x;                ///< frequency
    float *re;               ///< real part
    float *im;               ///< imaginary part
    float *phase;            ///< phase
    float *power;            ///< power of speech
    float *newWave;          /// new speech wave for output

    float *estimate_head;    /// noise estimation (head)
    float *estimate_tail;    /// noise estimation (tail)
    float *estimate_avg;     /// average noise estimation

    float **im_array;        /// imaginary part 2-D array
    float **re_array;        /// real part 2-D array
    float **pha_array;       /// phase 2-D array
    float **power_array;     /// power spectrum 2-D array
    float **estimate_recursion; /// recursive noise estimation 2-D array
    float **wave;            /// wave after IDFT 2-D array

	float* temp;

    /// Perform spectral analysis
    ///
    /// Include applying DFT and obtaining power spectrum
    /// \param twavData WAV data
    void runAnalysis(TWAVData* twavData);

    /// Phase of speech signal
    ///
    /// \param re real part
    /// \param im imaginary part
    /// \param phase of speech
    void Pha(float* re, float* im, float*& p);

    /// Set the value of nFFT
    ///
    /// Calculate nFFT given frame length
    /// \param n frame length
    /// \return nFFT
    int setnFFT(int n);

    /// Discrete Fourier transform
    ///
    /// Apply DFT on speech to obtain real and imaginary part
    /// \param x speech sample
    /// \param re real part
    /// \param im imaginary part
    void DFT(float*x, float*&re, float *&im);

    /// Calculate power spectrum
    ///
    /// \param re real part
    /// \param im imaginary part
    /// \param p power spectrum
    void pSpectrum(float* re, float* im, float*& p);

    /// Calculate total frame length
    ///
    /// \param twavData Wav data
    void frameLen(TWAVData* twavData);

    /// Array initialization
    ///
    /// \param twavData Wav data
    void initArray();

    /// Noise estimation
    ///
    /// \param twavData Wav data
    void noiseEstimation(int i,int j);

    /// Recursive noise estimation
    ///
    /// \param a smoothing factor
    void recursiveNoiseEstimation(float a);

    /// Inverse Discrete Fourier Transform
    ///
    /// \param a magnitude
    /// \param p power spectrum
    /// \param x return WAV
    void IDFT(float* a, float* p, float*& x);

    /// Perform IDFT on each frame
    ///
    void runIDFT();

    /// Spectral subtraction
    ///
    /// Subtract power spectrum of speech signal and estimated noise
    void spectrualSubtraction();

    /// Overlap half-add
    /// Half overlap addition to generate WAV signal
    void halfAdd();

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


#endif
