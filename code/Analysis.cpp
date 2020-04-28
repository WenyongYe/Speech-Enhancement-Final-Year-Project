#include "Analysis.h"
#include "WAVData.h"

//Constructor
Analysis::Analysis(int n) :sam_period(n) {
}

//Destructor
Analysis::~Analysis() {

    for(int i=0;i<numFrms;i++){
        delete[] re_array[i];
        delete[] im_array[i];
        delete[] pha_array[i];
        delete[] power_array[i];
        delete[] wave[i];
        delete[] estimate_recursion[i];
    }
    delete[] re_array;
    delete[] im_array;
    delete[] pha_array;
    delete[] power_array;
    delete[] wave;
    delete[] estimate_recursion;
}

//initialize array
void Analysis::initArray() {

    nFFT = setnFFT(frmLen);
    frmShift = frmLen / 2;
    nFFT2 = nFFT / 2;
    w = new float[numFrms];
    x = new float[nFFT];
    re = new float[nFFT2 + 1];
    im = new float[nFFT2 + 1];
    phase = new float[nFFT2 + 1];
    power = new float[nFFT2 + 1];

}

//set nFFT based on sampling period
int Analysis::setnFFT(int n) {
    int count = 0;
    while (n != 0)
    {
        n >>= 1;
        count += 1;
    }
    return 1 << count;
}

//perform spectral analysis
void Analysis::runAnalysis(TWAVData* twavData) {


    //number of frames
    int nFrms = (twavData->nSamples - frmLen) / frmShift + 1;
    numFrms = nFrms;

    //initialize second dimension of 2-d array
    im_array = new float* [nFrms];
    re_array = new float* [nFrms];
    pha_array = new float* [nFrms];
    power_array = new float* [nFrms];
    wave = new float* [nFrms];
    estimate_recursion=new float *[nFrms];

    for (int i = 0; i < nFrms; i++) {
        im_array[i] = new float[nFFT2 + 1];
        re_array[i] = new float[nFFT2 + 1];
        pha_array[i] = new float[nFFT2 + 1];
        power_array[i] = new float[nFFT2 + 1];
        wave[i] = new float[nFFT + 1];
        estimate_recursion[i]=new float[nFFT2+1];
    }


    for (int n = 0; n < nFrms; n++) {
        for (int i = 0; i < frmLen; i++)
            x[i] = twavData->Sample[n * frmShift + i];

        for (int i = frmLen; i < nFFT; i++)
            x[i] = 0.;

        //hamming window
        for (int n = 0; n < frmLen; n++)
            w[n] = 0.54 - 0.46 * cos(2 * PI * n / (frmLen - 1));
        //apply hamming window to frame
        for (int l = 0; l < frmLen; l++)
            x[l] *= w[l];

        //DFT
        DFT(x, re, im);
        //phase
        Pha(re, im, phase);
        //power spectrum
        pSpectrum(re, im, power);


        for (int i = 0; i <= nFFT2; i++) {
            im_array[n][i] = im[i];
            re_array[n][i] = re[i];
            pha_array[n][i] = phase[i];
            power_array[n][i] = power[i];
        }

    }
}
//DFT
void Analysis::DFT(float* x, float*& re, float*& im) {

    for (int i = 0; i <= nFFT2; i++) {
        re[i] = 0.;
        im[i] = 0.;
    }
    for (int k = 0; k <= nFFT2; k++) {
        for (int i = 0; i < nFFT; i++) {
            re[k] += x[i] * cos(2 * PI * k * i / nFFT);
            im[k] += x[i] * sin(2 * PI * k * i / nFFT);
        }
    }

}

//phase
void Analysis::Pha(float* re, float* im, float*& p)
{
    float ph;
    for (int k = 0; k <= nFFT2; k++) {
        if (re[k] == 0.)
            ph = (im[k] >= 0.F) ? PI / 2.F : -PI / 2.F;
        else {
            ph = atan(im[k] / re[k]);
            if (re[k] < 0. && im[k] < 0.)
                ph -= PI;
            else if (re[k] < 0. && im[k] >= 0.)
                ph += PI;
        }
        p[k] = ph;
    }
}

//power spectrum
void Analysis::pSpectrum(float* re, float* im, float*& p) {
    for (int i = 0; i <= nFFT2; i++) {
        p[i] = re[i] * re[i] + im[i] * im[i];
    }
}

//calculate frame length
void Analysis::frameLen(TWAVData* twavData) {
    samp_rate = twavData->hdr->Fmt_Chunk.Rate;
    frmLen = samp_rate * (sam_period / (double)1000);
}

// Inverse DFT given magnitude spectra a and phase p, return wav in x
void Analysis::IDFT(float* a, float* p, float*& x)
{
    float* re = new float[nFFT2 + 1];
    float* im = new float[nFFT2 + 1];

    for (int k = 0; k <= nFFT2; k++) {
        re[k] = sqrt(a[k]) * cos(p[k]);
        im[k] = sqrt(a[k]) * sin(p[k]);
    }

    for (int n = 0; n < nFFT; n++) {
        float a = (n % 2) ? -re[nFFT2] : re[nFFT2];
        x[n] = re[0] + a;

        for (int k = 1; k < nFFT2; k++)
            x[n] += re[k] * cos(2 * PI * k * n / nFFT) + im[k] * sin(2 * PI * k * n / nFFT);

        for (int k = nFFT2 + 1; k < nFFT; k++) {
            float a = re[nFFT - k] * cos(2 * PI * (k - nFFT2) * n / nFFT) - im[nFFT - k] * sin(2 * PI * (k - nFFT2) * n / nFFT);
            if (n % 2) a = -a;
            x[n] += a;
        }

        x[n] /= nFFT;
    }

    delete[] re;
    delete[] im;
}

//perform IDFT
void Analysis::runIDFT() {
    for (int i = 0; i < numFrms; i++)
        IDFT(power_array[i], pha_array[i], wave[i]);
}

//half overlap-add
void Analysis::halfAdd() {
    //frame overlap
    int frmOverlap = frmLen / 2;
    newWaveLen = numFrms * (frmLen - frmOverlap) + frmOverlap;
    newWave = new float[newWaveLen];

    //reconstruct signal
    for (int i = 0; i < numFrms; i++) {
        for (int j = 0; j <= frmOverlap; j++) {
            if (i + 1 != numFrms)
                newWave[i * frmOverlap + j] = wave[i][j];
            else {
                newWave[i * frmOverlap + j] = wave[i][j];
                newWave[i * frmOverlap + frmOverlap + j] = 0;
            }
        }
    }

    //half overlap-add
    float* temp = new float[frmOverlap + 1];
    for (int i = 0; i < numFrms; i++) {
        for (int j = 1; j <= frmOverlap; j++) {
            temp[j] = wave[i][frmOverlap + j];
            newWave[(i + 1) * frmOverlap + j] += temp[j];

        }
    }
    delete[] temp;
}

