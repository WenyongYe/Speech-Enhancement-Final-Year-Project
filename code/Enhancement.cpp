
#include "Enhancement.h"



//noise estimation
void Enhancement::noiseEstimation(int z, int x) {

    if (x <= 0 || z <= 0)
    {
        cout << "Error" << endl;
        return;
    }
    else {
        estimate_head = new float[nFFT2];
        estimate_tail = new float[nFFT2];
        estimate_avg = new float[nFFT2];

        float avg_head, avg_tail;
        float sum_head = 0, sum_tail = 0;

        for (int i = 0; i <= nFFT2; i++) {
            for (int j = 0; j < z; j++) {
                sum_head += power_array[j][i];
            }
            avg_head = sum_head / z;
            sum_head = 0;
            estimate_head[i] = avg_head;
        }


        for (int i = 0; i <= nFFT2; i++) {
            for (int j = numFrms - 1 - x; j < numFrms - 1; j++) {
                sum_tail += power_array[j][i + 1];
            }
            avg_tail = sum_tail / x;
            sum_tail = 0;
            estimate_tail[i] = avg_tail;
        }

        //calculate average noise estimation of head and tail
        for (int i = 0; i <= nFFT2; i++)
            estimate_avg[i] = (estimate_head[i] + estimate_tail[i]) / 2;

    }
}



//recursive noise estimation
void Enhancement::recursiveNoiseEstimation(float a) {

    cout << endl << "Recursive noise estimation running..." << endl;
    if (a < 0 || a > 1) {
        cout << "Error" << endl;
        return;
    }
    else {
        float **temp= new float* [numFrms];
        //recursive noise estimation
        for (int i = 0; i < numFrms; i++) {
            temp[i] = new float[nFFT2+1];

            for (int j = 0; j <= nFFT2; j++) {
                //if is is the first frame
                if (i == 0)
                    temp[i][j] = power_array[i][j];
                else
                    temp[i][j] = a * temp[i - 1][j] + (1 - a) * power_array[i][j];
            }
        }
        //copy to array
        for(int i=0;i<numFrms;i++) {
            for (int j = 0; j <= nFFT2; j++)
                estimate_recursion[i][j] = temp[i][j];
        }

        //destructor
        for(int i=0;i<numFrms;i++)
            delete[] temp[i];
        delete[] temp;
    }
}

//spectral subtraction
void Enhancement::spectralSubtraction() {

    cout << endl << "Spectral subtraction running..." << endl;
    float temp;
    for (int n = 0; n < numFrms; n++) {
        for (int i = 0; i <= nFFT2; i++) {
            temp = power_array[n][i] - estimate_recursion[n][i];
            //non-negative value
            if (temp > 0)
                power_array[n][i] = temp;
            else
                power_array[n][i] = 0;
        }
    }
}

//Wiener fillter
void Enhancement::wienerFilter(float alpha, float beta) {
    cout << endl << "Wiener filter running..." << endl;
    // Minimum a priori SNR (-11db)
    float snr_min = 0.079432823F;
    // Noise psd init window size
    int nLen = 10;

    float** wiener_array = new float* [numFrms];
    for (int i = 0; i < numFrms; i++)
        wiener_array[i] = new float[nFFT2 + 1];

    // For each frequency index k
    for (int k = 0; k <= nFFT2; k++) {
        // Signal spectral amplitude estimate
        float a = 0.;

        // Noise psd estimate & initialization
        float n = 0.;
        for (int t = 0; t < nLen; t++)
            n += power_array[t][k] * power_array[t][k];
        n /= nLen;

        // For each frame t
        for (int t = 0; t < numFrms; t++) {

            // Input periodgram
            float o = power_array[t][k] * power_array[t][k];

            // Smoothed noise psd estimate
            n = alpha * n + (1.F - alpha) * power_array[t][k] * power_array[t][k];

            // Calculate filter weight h
            float h = 1.;
            if (n > 0.) {
                // a priori SNR
                float f = o / n - 1.F;

                if (f < 0.)
                    f = 0.;
                //smoothing the SNR
                float snr = beta * (a * a / n) + (1.F - beta) * f;

                if (snr < snr_min)
                    snr = snr_min;
                // wf
                h = snr / (1.F + snr);
            }
            // Filtering
            a = h * power_array[t][k];

            wiener_array[t][k] = a;
        }//t
    }//k
    for (int i = 0; i < numFrms; i++)
        for (int j = 0; j <= nFFT2; j++)
            power_array[i][j] = wiener_array[i][j];

    //destructor
    for (int i = 0; i < numFrms; i++)
        delete[]wiener_array[i];
    delete[]wiener_array;

}

//perform Wiener filter
void Enhancement::runWiener() {
    float alpha = 0.95, beta = 0.98;
    wienerFilter(alpha, beta);
}

//perform Wiener filter
void Enhancement::runWiener(float alpha, float beta) {
    wienerFilter(alpha, beta);
}