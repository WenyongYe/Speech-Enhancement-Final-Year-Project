#include "IO.h"

//constructor
IO::IO() {
    twavData = new TWAVData;
}

//destructor
IO::~IO() {
    if (twavData)
        delete twavData;
}

//input function
void IO::Input() {
    cout << endl << "Input function running..." << endl;
    cout << "Type the name of input WAV file: ";

    input = new char[1024];
    cin >> input;


    //if not successfully loading
    bool success = false;
    do {
        success = twavData->load(input);
        if (success)
            cout << endl << "Successfully loading WAV file..." << endl;
        else {
            cout << endl << "Fail to load WAV file..." << endl;
            cout << "Try again..." << endl;
            cout << "Type the name of input WAV file: ";
            cin >> input;

            success = twavData->load(input);
        }
    } while (!success);
    delete[] input;
}

//output function
void IO::Output() {
    cout << endl << "Output function running..." << endl;
    cout << "Type the name of output WAV file: ";
    output = new char[1024];
    cin >> output;

    twavData->write(output);
    cout << endl << "Successfully output WAV file" << endl;
    delete[] output;
}

//replace the original wave signal
void IO::ReplaceWave(TWAVData*& twavData, float*& wave, int newLength) {

    for (int i = 0; i < twavData->nSamples; i++)
        if (i < newLength)
            twavData->Sample[i] = (short)wave[i];
        else
            twavData->Sample[i] = 0;

}
