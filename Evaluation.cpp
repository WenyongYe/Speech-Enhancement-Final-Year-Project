#include "Evaluation.h"
#include "RIFF.h"


//segmental SNR
float Evaluation::segmentalSNR(TWAVData *twavData1, TWAVData *twavData2,int sam_period) {

    int frameLength=twavData2->hdr->Fmt_Chunk.Rate/(1000/sam_period);
    int numFrm=twavData2->nSamples/frameLength;


    float res=0;

    for(int i=0;i<numFrm;i++){
        float numerator=0;
        float denominator=0;

        for(int j=i*frameLength;j<frameLength*(i+1);j++){
            numerator+=twavData1->Sample[j]*twavData1->Sample[j];
            denominator+=pow((twavData1->Sample[j]-twavData2->Sample[j]),2);
        }

        res+=log(numerator/denominator);

    }

    return 10*res/numFrm;
}