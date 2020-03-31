#include "Analysis.h"
#include "WAVData.h"

//Constructor
Analysis::Analysis(int n):sam_period(n){
}

//Destructor
Analysis::~Analysis(){}

//initialize array
void Analysis::initArray(){

    nFFT=setnFFT(frmLen);
    frmShift= frmLen/2;
    nFFT2=nFFT/2;
    w=new float[numFrms];
    x= new float[nFFT];
    re = new float[nFFT2 + 1];
    im = new float[nFFT2 + 1];
    phase=new float[nFFT2 + 1];
    power= new float[nFFT2 + 1];
}

//calculate nFFT
int Analysis::setnFFT(int n) {
    int count=0;
    while( n != 0)
    {
        n >>= 1;
        count += 1;
    }
    return 1<<count;
}

//perform spectral analysis
void Analysis::runAnalysis(TWAVData* twavData) {

    //number of frames
    int nFrms=(twavData->nSamples-frmLen)/frmShift+1;
    numFrms=nFrms;

    //initialize second dimension of 2-d array
    im_array=new float*[nFrms];
    re_array=new float*[nFrms];
    pha_array=new float*[nFrms];
    power_array=new float*[nFrms];
    wave=new float*[nFrms];

    for(int i=0;i<nFrms;i++){
        im_array[i]=new float[nFFT2+1];
        re_array[i]=new float[nFFT2+1];
        pha_array[i]=new float [nFFT2+1];
        power_array[i]=new float[nFFT2+1];
        wave[i]=new float[nFFT+1];
    }


    for(int n=0;n<nFrms;n++){
        //retrieve samples from file
        for(int i=0;i<frmLen;i++)
            x[i]=twavData->Sample[n*frmShift+i];

        for(int i=frmLen;i<nFFT;i++)
            x[i]=0.;

        //hamming window
        for(int n=0;n<frmLen;n++)
            w[n]=0.54-0.46*cos(2*PI*n/(frmLen-1));
        //apply hamming window to frame
        for(int l=0;l<frmLen;l++)
            x[l]*=w[l];

        //DFT on each frame
        DFT(x,re,im);

        //calculate phase
        Pha(re,im,phase);

        //calculate power spectrum
        pSpectrum(re,im,power);


        for(int i=0;i<=nFFT2;i++) {
            im_array[n][i] = im[i];
            re_array[n][i] = re[i];
            pha_array[n][i]=phase[i];
            power_array[n][i]=power[i];
        }

    }
}

//DFT
void Analysis::DFT(float *x, float *&re, float *&im) {

    for(int i=0;i<=nFFT2;i++) {
        re[i] = 0.;
        im[i] = 0.;
    }
    for(int k=0;k<=nFFT2;k++){
        for(int i=0;i<nFFT;i++){
            re[k]+=x[i]*cos(2*PI*k*i/nFFT);
            im[k]+=x[i]*sin(2*PI*k*i/nFFT);
        }
    }

}

//phase
void Analysis::Pha(float* re, float* im, float*& p)
{
    float ph;
    for(int k = 0; k <= nFFT2; k++) {
        if(re[k] == 0.)
            ph = (im[k] >= 0.F) ? PI / 2.F : -PI / 2.F;
        else {
            ph = atan(im[k] / re[k]);
            if(re[k] < 0. && im[k] < 0.)
                ph -= PI;
            else if(re[k] < 0. && im[k] >= 0.)
                ph += PI;
        }
        p[k] = ph;
    }
}

//power spectrum
void Analysis::pSpectrum(float* re, float* im, float*& p) {
    for(int i=0;i<=nFFT2;i++){
        p[i]=re[i]*re[i]+im[i]*im[i];
    }
}

//calculate frame length
void Analysis::frameLen(TWAVData *twavData) {
    samp_rate=twavData->hdr->Fmt_Chunk.Rate;
    frmLen=samp_rate*(sam_period/(double)1000);
}

//noise estimation
void Analysis::noiseEstimation(int z,int x){

    if(x<=0||z<=0)
    {   cout<<"Error"<<endl;
        return;
    }else {
        estimate_head = new float[nFFT2];
        estimate_tail = new float[nFFT2];
        estimate_avg =new float[nFFT2];

        float avg_head, avg_tail;
        float sum_head = 0, sum_tail = 0;

        for (int i = 0; i <=nFFT2; i++) {
            for (int j = 0; j < z; j++) {
                sum_head += power_array[j][i];
            }
            avg_head = sum_head / z;
            sum_head=0;
            estimate_head[i] = avg_head;
        }


            for (int i = 0; i <=nFFT2; i++) {
                for (int j = numFrms - 1 - x; j < numFrms - 1; j++) {
                sum_tail += power_array[j][i+1];
            }
            avg_tail = sum_tail / x;
            sum_tail=0;
            estimate_tail[i] = avg_tail;
        }

            //calculate average noise estimation of head and tail
            for(int i=0;i<=nFFT2;i++)
                estimate_avg[i]=(estimate_head[i]+estimate_tail[i])/2;

    }
}

//recursive noise estimation
void Analysis::recursiveNoiseEstimation(float a) {

    cout<<"Recursive noise estimation running..."<<endl;
    if (a < 0 || a > 1) {
        cout << "Error" << endl;
        return;
    } else {
        estimate_recursion = new float *[numFrms];

        for (int i = 0; i < numFrms; i++) {
            estimate_recursion[i] = new float[nFFT2];
            for (int j = 0; j <= nFFT2; j++) {
                if (i == 0)
                    estimate_recursion[i][j] = power_array[i][j];
                else
                    estimate_recursion[i][j] = a * estimate_recursion[i - 1][j] + (1 - a) * power_array[i][j];
            }

        }
    }

}


// Inverse DFT given magnitude spectra a and phase p, return wav in x
void Analysis::IDFT(float* a, float* p, float*& x)
{
    float* re = new float[nFFT2 + 1];
    float* im = new float[nFFT2 + 1];

      for(int k = 0; k <= nFFT2; k++) {
        re[k] = sqrt(a[k]) * cos(p[k]);
        im[k] = sqrt(a[k]) * sin(p[k]);
    }

    for(int n = 0; n < nFFT; n++) {
        float a = (n % 2)? -re[nFFT2] : re[nFFT2];
        x[n] = re[0] + a;

        for(int k = 1; k < nFFT2; k++)
            x[n] += re[k] * cos(2*PI*k*n/nFFT) + im[k] * sin(2*PI*k*n/nFFT);

        for(int k = nFFT2 + 1; k < nFFT; k++) {
            float a = re[nFFT - k] * cos(2*PI*(k-nFFT2)*n/nFFT) - im[nFFT - k] * sin(2*PI*(k-nFFT2)*n/nFFT);
            if(n % 2) a = -a;
            x[n] += a;
        }

        x[n] /= nFFT;
    }

    delete [] re;
    delete [] im;
}

//perform IDFT on each frame
void Analysis::runIDFT() {
    for(int i=0;i<numFrms;i++)
       IDFT(power_array[i],pha_array[i],wave[i]);
}

//half overlap addition on samples
void Analysis::halfAdd() {

    int frmOverlap=frmLen/2;
    newWave = new float[numFrms *(frmLen-frmOverlap)+frmLen];


    for(int i=0;i<numFrms;i++){
        for(int j=0;j<=frmOverlap;j++){
            newWave[i*frmOverlap+j]=wave[i][j];
        }
    }

    temp=new float[frmOverlap+1];

    for(int i=0;i<numFrms;i++){
        for(int j=0;j<=frmOverlap;j++){
			//temp = new float[161];
            temp[j]=wave[i][frmOverlap+j];
            newWave[(i+1)*frmOverlap+j]+=temp[j];
        }
    }

   // delete []temp;

}

//spectral subtraction
void Analysis::spectrualSubtraction(){

    cout<<endl<<"Spectral subtraction running..."<<endl;
    float temp;
    for(int n=0;n<numFrms;n++) {
        for (int i = 0; i <= nFFT2; i++) {
            temp=power_array[n][i]-estimate_recursion[n][i];
            if(temp>0)
                power_array[n][i]=temp;
            else
                power_array[n][i]=0;
        }
    }
}

//Wiener Filter
void Analysis::wienerFilter(float alpha,float beta) {

    cout<<endl<<"Wiener filter running..."<<endl;
    // Minimum a priori SNR (-11db)
    float snr_min = 0.079432823F;
    // Noise psd init window size
    int   nLen = 10;

    float **wiener_array=new float*[numFrms];
    for(int i=0;i<numFrms;i++)
        wiener_array[i]=new float [nFFT2+1];

    // For each frequency index k
    for(int k = 0; k <= nFFT2; k++) {
        // Signal spectral amplitude estimate
        float a = 0.;

        // Noise psd estimate & initialization
        float n = 0.;
        for(int t = 0; t < nLen; t++)
            n += power_array[t][k]*power_array[t][k];
        n /= nLen;

        // For each frame t
        for(int t = 0; t < numFrms; t++) {

            // Input periodgram
            float o = power_array[t][k]*power_array[t][k];

            // Smoothed noise psd estimate
            n = alpha * n + (1.F - alpha) * power_array[t][k] * power_array[t][k];

            // Calculate filter weight h
            float h = 1.;
            if(n > 0.) {
                // a priori SNR
                float f = o / n - 1.F;

                if(f < 0.)
                    f = 0.;
                float snr = beta * (a * a / n) + (1.F - beta) * f;

                if(snr < snr_min)
                    snr = snr_min;

                // wf
                h = snr / (1.F + snr);
            }

            // Filtering
            a = h * power_array[t][k];

            wiener_array[t][k] = a;
        }//t
    }//k


    for(int i=0;i<numFrms;i++)
        for(int j=0;j<=nFFT2;j++)
            power_array[i][j]=wiener_array[i][j];

    //destructor
    for(int i=0;i<numFrms;i++)
        delete []wiener_array[i];
    delete []wiener_array;
}

//perform Wiener filter
void Analysis::runWiener() {
    float alpha=0.95,beta=0.98;
    wienerFilter(alpha,beta);
}

//perform Wiener filter
void Analysis::runWiener(float alpha,float beta) {
    wienerFilter(alpha,beta);
}