/*!
* \file IO.h
* \brief IO Functions
*
* Input WAV file and output WAV file with given names, and check
 * the name with defined pattern.
* Including function for replacing samples of original input WAV.
*/
#ifndef CSC3002_IO_H
#define CSC3002_IO_H
#include "RIFF.h"
#include "WAVData.h"
#include "Analysis.h"
#include <iostream>
#include<string>
#include <regex>


/// \brief IO
///
/// IO functions, used in each operation
class IO {
public:
    IO();                              ///< constructor
    ~IO();                             ///< destructor
    TWAVData* twavData;                ///< WAV data

    char *output;                     ///< output file name
    char *input;                      ///< input file name


    /// Input function
    ///
    /// Input WAV file and load the data given file name.
    void Input();

    /// Output function
    ///
    /// Output WAV file and write the data given file name.
    void Output();

    /// Replace WAV file
    ///
    /// If applicable, replace all samples of original WAV file with new samples.
    /// @param twavData Original WAV file
    /// @param wave WAV file after replacement
    void ReplaceWave(TWAVData* &twavData, float*& wave,int newLength);
};


#endif //CSC3002_IO_H
