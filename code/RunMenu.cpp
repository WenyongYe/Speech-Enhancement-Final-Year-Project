#include "RunMenu.h"

//constructor
RunMenu::RunMenu(){
    checkData=new CheckData;
    io=new IO;
}

//destructor
RunMenu::~RunMenu(){

}


//test IO
void RunMenu::testIO() {

    cout << endl << "----- IO test running -----" << endl;

    io->Input();
    io->Output();

}

//test IDFT
void RunMenu::testIDFT() {

    string in;
    int sam_period;

    cout << endl << "----- DFT and IDFT running -----" << endl;

    io->Input();

    sam_period=checkData->getSampPeriod(in);

    Analysis analysis(sam_period);

    analysis.frameLen(io->twavData);

    analysis.initArray();

    analysis.runAnalysis(io->twavData);

    analysis.runIDFT();

    analysis.halfAdd();

    //replace the original signal
    io->ReplaceWave(io->twavData, analysis.newWave, analysis.newWaveLen);

    io->Output();

}

//add noise to clean speech given SNR
void RunMenu::addNoise2Speech() {
    cout << endl << "----- Add noise to clean speech running -----" << endl;

    //io1 for clean speech, io2 for noise
    IO io1;
    IO io2;

    cout << endl << "Input the name of NOISY speech WAV file" << endl;
    io2.Input();
    cout << endl << "Input the name of CLEAN speech WAV file" << endl;
    io1.Input();

    Simulation s;
    s.inputSNR();

    s.addNoise(io1.twavData, io2.twavData);
    io1.Output();

}


//add white Gaussian noise
void RunMenu::addGaussianNoise2Speech() {

    cout << endl << "----- White Gaussian noise running -----" << endl;

    io->Input();

    Simulation s;
    s.inputSNR();

    s.addGaussianNoise(io->twavData);

    io->Output();
}

//spectral subtraction
void RunMenu::spectralSubtract() {

    float smoothing_factor;
    string in;
    int sam_period;

    cout << endl << "----- Spectral subtraction running -----" << endl;

    io->Input();

    //input sampling period
    sam_period=checkData->getSampPeriod(in);

    //input smoothing factor
    do {
        cout << endl << "Input smoothing factor for recursive noise estimation (between 0~1): ";
        cin >> smoothing_factor;

        checkData->checkInputValueType(smoothing_factor);

        //check validation of smoothing factor
        if (smoothing_factor <= 0 || smoothing_factor >= 1)
            cout << endl << "Incorrect value. Please input again";

    } while (smoothing_factor <= 0 || smoothing_factor >= 1);

    //spectral analysis
    Enhancement analysis(sam_period);

    analysis.frameLen(io->twavData);

    analysis.initArray();

    analysis.runAnalysis(io->twavData);

    analysis.recursiveNoiseEstimation(smoothing_factor);

    analysis.spectralSubtraction();

    analysis.runIDFT();

    analysis.halfAdd();

    //replace original signal
    io->ReplaceWave(io->twavData, analysis.newWave, analysis.newWaveLen);

    io->Output();

}

//wiener filter
void RunMenu::wienerFilter() {

    int select;
    float alpha, beta;
    bool flag;
    string in;
    int sam_period;

    cout << endl << "----- Wiener filter running -----" << endl;

    io->Input();

    //input sampling period
    sam_period=checkData->getSampPeriod(in);

    do {
        cout << endl << "Choose number to select operations" << endl;
        cout << "1. Wiener filter with default setting" << endl <<
             "2. Wiener filter with input parameters" << endl;
        cout << endl << "Input operation number: ";

        cin >> select;

        //input selections
        checkData->checkInputValueType(select);

        switch (select) {
            case 1:
                flag = true;
                break;
            case 2:
                do {
                    cout << endl << "Input smoothing factor alpha and beta (between 0~1) " << endl;
                    cout << "alpha (smoothing factor for estimated noise psd):";
                    cin >> alpha;

                    checkData->checkInputValueType(alpha);

                    cout << endl << "beta (smoothing factor fot priori SNR):";
                    cin >> beta;

                    checkData->checkInputValueType(beta);

                    if (alpha <= 0 || alpha >= 1 || beta <= 0 || beta >= 1)
                        cout << endl << "Incorrect value. Please input again" << endl;

                } while (alpha <= 0 || alpha >= 1 || beta <= 0 || beta >= 1);
                flag = false;
                break;
            default:
                cout << "Incorrect value. Please input again " << endl;
        }

    } while (select != 1 && select != 2);



    Enhancement analysis(sam_period);

    analysis.frameLen(io->twavData);

    analysis.initArray();

    analysis.runAnalysis(io->twavData);

    if (flag)
        analysis.runWiener();
    else
        analysis.runWiener(alpha, beta);

    analysis.runIDFT();

    analysis.halfAdd();

    io->ReplaceWave(io->twavData, analysis.newWave, analysis.newWaveLen);

    io->Output();
}

//evaluation
void RunMenu::evaluation() {

    cout << endl << "----- Segmental SNR evaluation running -----" << endl;

    IO io1;
    IO io2;

    cout << endl << "Input the name of original speech WAV file" << endl;
    io1.Input();
    cout << endl << "Input the name of enhanced speech WAV file" << endl;
    io2.Input();

    string in;
    int sam_period;

    //input samling period
    sam_period=checkData->getSampPeriod(in);


    Evaluation e;
    float result = e.segmentalSNR(io1.twavData, io2.twavData, sam_period);

    cout << endl << "Segmental SNR is : " << result << endl;
}

