#include "Evaluation.h"
#include "RIFF.h"


//segmental SNR
float Evaluation::segmentalSNR(TWAVData *original, TWAVData *enhanced,int sam_period) {

    //frame length and num of frame
    int frameLength=enhanced->hdr->Fmt_Chunk.Rate/(1000/sam_period);
    int numFrm=enhanced->nSamples/frameLength;


    float res=0;

    for(int i=0;i<numFrm;i++){
        float numerator=0;
        float denominator=0;

        for(int j=i*frameLength;j<frameLength*(i+1);j++){
            numerator+=original->Sample[j]*original->Sample[j];
            denominator+=pow((original->Sample[j]-enhanced->Sample[j]),2);
        }

        res+=log(numerator/denominator);

    }

    return 10*res/numFrm;
}