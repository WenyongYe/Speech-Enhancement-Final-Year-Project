#include "Simulation.h"
#include <random>



void Simulation::inputSNR() {
    cout<<endl<<"Input value of SNR (e.g. 5 dB): ";
    cin>>SNR;
}


void Simulation::addNoise(TWAVData *twavData1, TWAVData *twavData2) {
    sum=0;
    for(int i=0;i<twavData1->nSamples;i++){
        sum+=pow(twavData1->Sample[i],2);
    }
    pSignal1=sum/twavData1->nSamples;

    sum=0;
    for(int i=0;i<twavData2->nSamples;i++){
        sum+=pow(twavData2->Sample[i],2);
    }
    pSignal2=sum/twavData2->nSamples;

    pNoise=pSignal1/(pow(10,SNR/10));

    noise=new float[twavData2->nSamples];

    for(int i=0;i<twavData2->nSamples;i++)
    {
        noise[i]=twavData2->Sample[i]*sqrt(pNoise/pSignal2);

    }

    int nSample;
    if(twavData1->nSamples>twavData2->nSamples)
        nSample=twavData2->nSamples;
    else
        nSample=twavData1->nSamples;

    twavData1->nSamples=nSample;
    for(int i=0;i<twavData1->nSamples;i++){
        twavData1->Sample[i]+=noise[i];
    }

}


void Simulation::addGaussianNoise(TWAVData *twavData) {

    whiteNoise=new float[twavData->nSamples];
    sum=0;
    for(int i=0;i<twavData->nSamples;i++){
        sum+=pow(twavData->Sample[i],2);
    }
    pSignal=sum/twavData->nSamples;

    pNoise=pSignal/(pow(10,SNR/10));


    noiseOldStd=sqrt(pSignal);
    noiseNewStd=sqrt(pNoise);


    delta=sqrt(12*pNoise);

    sigma=(delta/2)*100;

    for(int i=0;i<twavData->nSamples;i++)
    {
        float noise=box_muller(sigma)/100;
        whiteNoise[i]=noise*(noiseNewStd/noiseOldStd);
        twavData->Sample[i]+=whiteNoise[i];
    }

}

///
/// \details Use and modify code from website.
/// Reference: https://stackoverflow.com/questions/5817490/implementing-box-mueller-random-number-generator-in-c-sharp
///
float Simulation::box_muller(float sigma) {
    static double x1, x2;
    static int phase = 0;
    float z;

    if(phase == 0)
    {
        x1 = rand() / (RAND_MAX + 1.0);
        x2 = rand() / (RAND_MAX + 1.0);
        z = sqrt(-2.0 * log(x1))* sin(2.0 * PI * x2);
    }
    else
    {
        z = sqrt(-2.0 * log(x1)) * cos(2.0 * PI * x2);
    }

    phase = 1 - phase;
    return z*sigma;
}