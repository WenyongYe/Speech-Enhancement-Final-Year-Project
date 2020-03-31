#include "RunMenu.h"

void RunMenu::testIO(){

    cout<<endl<<"----- IO test running -----"<<endl;

    IO io;
    io.Input();
    io.Output();
}

void RunMenu::testIDFT(){

    cout<<endl<<"----- DFT and IDFT running -----"<<endl;

    IO io1;
    io1.Input();

    int sam_period;
    cout<<"Input sampling period (e.g. 20 ms) : ";
    cin>>sam_period;

    Analysis analysis(sam_period);

    analysis.frameLen(io1.twavData);

    analysis.initArray();

    analysis.runAnalysis(io1.twavData);

    analysis.runIDFT();

    analysis.halfAdd();

    io1.ReplaceWave(io1.twavData,analysis.newWave);
    io1.Output();


}

void RunMenu::addNoise2Speech(){
    cout<<endl<<"----- Add noise to clean speech running -----"<<endl;

    IO io1;
    IO io2;

    cout<<endl<<"Input the name of NOISY speech WAV file"<<endl;
    io2.Input();
    cout<<endl<<"Input the name of CLEAN speech WAV file"<<endl;
    io1.Input();

    Simulation s;
    s.inputSNR();

    s.addNoise(io1.twavData,io2.twavData);
    io1.Output();

}

void RunMenu::addGaussianNoise2Speech(){

    cout<<endl<<"----- Gaussian white running -----"<<endl;

    IO io1;

    io1.Input();

    Simulation s;
    s.inputSNR();

    s.addGaussianNoise(io1.twavData);

    io1.Output();
}

void RunMenu::spectralSubtract() {

    cout<<endl<<"----- Spectral subtraction running -----"<<endl;

    IO io1;
    io1.Input();

    int sam_period;
    float smoothing_factor;
    cout << "Input sampling period (e.g. 20 ms) : ";
    cin >> sam_period;

    do {
        cout << endl << "Input smoothing factor for recursive noise estimation (between 0~1): ";
        cin >> smoothing_factor;
        if (smoothing_factor <= 0 || smoothing_factor >= 1)
            cout << endl << "Incorrect value. Please input again";

    } while (smoothing_factor <= 0 || smoothing_factor >= 1);


    Analysis analysis(sam_period);

    analysis.frameLen(io1.twavData);

    analysis.initArray();

    analysis.runAnalysis(io1.twavData);

    analysis.recursiveNoiseEstimation(smoothing_factor);

    analysis.spectrualSubtraction();

    analysis.runIDFT();

    analysis.halfAdd();

    io1.ReplaceWave(io1.twavData,analysis.newWave);

    io1.Output();

}

void RunMenu::wiener() {

    cout << endl << "----- Wiener filter running -----" << endl;

    IO io1;
    io1.Input();

    int sam_period;
    int select;
    float alpha, beta;
    bool flag;

    cout << "Input sampling period (e.g. 20 ms) : ";
    cin >> sam_period;

    do{
        cout << endl << "Choose number to select operations" << endl;
        cout << "1. Wiener filter with default setting" << endl <<
         "2. Wiener filter with input parameters" << endl;
        cout << endl << "Input operation number: ";

        cin >> select;
        switch (select){
            case 1:
                flag=true;
                break;
            case 2:
                do {
                    cout << endl << "Input smoothing factor alpha and beta (between 0~1) "<<endl;
                    cout<<"alpha :";
                    cin >> alpha;
                    cout<<endl<<"beta :";
                    cin>>beta;
                    if (alpha <= 0 || alpha >= 1||beta <= 0 || beta >= 1)
                        cout << endl << "Incorrect value. Please input again"<<endl;

                } while (alpha <= 0 || alpha >= 1||beta <= 0 || beta >= 1);
                flag=false;
                break;
            default:
                cout<<"Incorrect value. Please input again "<<endl;
        }

    }while(select!=1&&select!=2);



    Analysis analysis(sam_period);

    analysis.frameLen(io1.twavData);

    analysis.initArray();

    analysis.runAnalysis(io1.twavData);

    if(flag)
        analysis.runWiener();
    else
        analysis.runWiener(alpha,beta);

    analysis.runIDFT();

    analysis.halfAdd();

    io1.ReplaceWave(io1.twavData,analysis.newWave);

    io1.Output();
}

void RunMenu::evaluation() {

    cout<<endl<<"----- Segmental SNR evaluation running -----"<<endl;

    IO io1;
    IO io2;

    cout<<endl<<"Input the name of original speech WAV file"<<endl;
    io1.Input();
    cout<<endl<<"Input the name of enhanced speech WAV file"<<endl;
    io2.Input();

    int sam_period;
    cout << "Input sampling period (e.g. 20 ms) : ";
    cin >> sam_period;


    Evaluation e;
    float result=e.segmentalSNR(io1.twavData,io2.twavData,sam_period);

    cout<<endl<<"Segmental SNR is : "<<result<<endl;
}