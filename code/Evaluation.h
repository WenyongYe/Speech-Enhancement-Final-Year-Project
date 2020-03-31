#ifndef CSC3002_EVALUATION_H
#define CSC3002_EVALUATION_H
#include <iostream>
#include "WAVData.h"
#include "Analysis.h"
#include <math.h>

class Evaluation {

public:
    float segmentalSNR(TWAVData *twavData1, TWAVData *twavData2,int i);
};


#endif //CSC3002_EVALUATION_H
