#include "IO.h"


IO::IO(){
    twavData=new TWAVData;
}

IO::~IO() {
    if(twavData)
    delete twavData;
}

void IO::Input(){
    cout<<"Input function running..."<<endl;
    cout<<"Type the name of input WAV file: ";

    string input;
    cin>>input;
    char* s=&input[0];

    bool success= false;
    do {
        success= twavData->load(s);
        if (success)
            cout << endl << "Successfully loading WAV file..." << endl;
        else {
            cout << endl << "Fail to load WAV file..." << endl;
            cout << "Try again..." << endl;
            cout<<"Type the name of input WAV file: ";
            cin>>input;
            char* s=&input[0];
            success = twavData->load(s);
        }
    }while(!success);
}

void IO::Output(){
    cout<<"Output function running..."<<endl;
    cout<<"Type the name of output WAV file: ";
    string output;
    cin>>output;
    char* s=&output[0];

    twavData->write(s);
    cout<<endl<<"Successfully output WAV file"<<endl;
}

void IO::ReplaceWave(TWAVData *twavData,float *&wave) {

    for(int i=0;i< twavData->nSamples;i++)
        twavData->Sample[i]=(short)wave[i];
}