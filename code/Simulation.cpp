#include "Simulation.h"
#include <random>


//input SNR
void Simulation::inputSNR() {

    cout << endl << "Input value of SNR (e.g. 5 dB): ";
    cin >> SNR;

    //check the input value data type
    while(cin.fail()||typeid(SNR)!=typeid(float)) {
        cout<< endl<<"Incorrect value, input again..." <<endl;
        cout << endl << "Input value of SNR (e.g. 5 dB): ";
        cin.clear();
        cin.ignore(256,'\n');
        cin>>SNR;
    }
}

//add noisy recording to clean speech
void Simulation::addNoise(TWAVData *cleanSpeech, TWAVData *noiseSpeech) {

    cout<<endl<<"Add noise to speech running..."<<endl;
    int nSample;
    // power of signal of clean speech
    sum=0;
    for(int i=0;i<cleanSpeech->nSamples;i++){
        sum+=pow(cleanSpeech->Sample[i],2);
    }
    pSignalClean=sum/cleanSpeech->nSamples;

    //power of signal of noise
    sum=0;
    for(int i=0;i<noiseSpeech->nSamples;i++){
        sum+=pow(noiseSpeech->Sample[i],2);
    }
    pSignalNoise=sum/noiseSpeech->nSamples;

    //power of noise estimated
    pNoise=pSignalClean/(pow(10,SNR/10));

    noise=new float[noiseSpeech->nSamples];

    //generate noise according to SNR
    for(int i=0;i<noiseSpeech->nSamples;i++)
    {
        noise[i]=noiseSpeech->Sample[i]*sqrt(pNoise/pSignalNoise);

    }
    //compare the length of noise and clean speech
    if(cleanSpeech->nSamples>noiseSpeech->nSamples)
        nSample=noiseSpeech->nSamples;
    else
        nSample=cleanSpeech->nSamples;

    cleanSpeech->nSamples=nSample;
    //add noise given SNR to clean speech
    for(int i=0;i<cleanSpeech->nSamples;i++){
        cleanSpeech->Sample[i]+=noise[i];
    }
}

//add white Gaussian noise to speech
void Simulation::addGaussianNoise(TWAVData *twavData) {

    cout<<endl<<"Add white Gaussian noise running..."<<endl;

    whiteNoise=new float[twavData->nSamples];
    sum=0;
    for(int i=0;i<twavData->nSamples;i++){
        sum+=pow(twavData->Sample[i],2);
    }
    //power of signal
    pSignal=sum/twavData->nSamples;
    //power of noise
    pNoise=pSignal/(pow(10,SNR/10));

    //noise standard deviation
    noiseOldStd=sqrt(pSignal);
    noiseNewStd=sqrt(pNoise);

    //random noise interval
    delta=sqrt(12*pNoise);

    sigma=(delta/2)*100;

    for(int i=0;i<twavData->nSamples;i++){
        //white Gaussian noise
        float noise=box_muller(sigma)/100;
        whiteNoise[i]=noise*(noiseNewStd/noiseOldStd);
        twavData->Sample[i]+=whiteNoise[i];
    }

}


//Box-Muller transform
//Use and modify code from website.
//Reference: https://stackoverflow.com/questions/5817490/implementing-box-mueller-random-number-generator-in-c-sharp
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