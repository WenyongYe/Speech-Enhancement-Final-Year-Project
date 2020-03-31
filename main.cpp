#include <iostream>
#include <fstream>
#include "RIFF.h"
#include "WAVData.h"
#include "IO.h"
#include "Analysis.h"
#include "Simulation.h"
#include "RunMenu.h"


using namespace std;


//menu for the program
void menu(){

    cout<<endl<<"CSC3002 Program Running ..."<<endl<<endl;


    RunMenu menu;

    int num;


    do{
        cout<<endl;
        cout<<"------------------------------------"<<endl;
        cout<<"--------------- Menu ---------------"<<endl;
        cout<<"------------------------------------"<<endl;

        cout<<"Choose number to select operations"<<endl<<endl;
        cout<<"1. Test IO function"<<endl<<"2. Test DFT and IDFT"
        <<endl<<"3. Noisy Speech Simulation (Add noise to clean speech)"<<
        endl<<"4. Noisy Speech Simulation (Add Gaussian white noise)"<<endl<<
        "5. Speech Enhancement (Spectral subtraction)"<<endl<<"6. Speech Enhancement (Wiener filter)"<<endl<<
        "7. Evaluation (Segmental SNR evaluation)"<<endl<<"8. Exit"<<endl;

        cout<<endl<<"Input operation number: ";
        cin>>num;
        switch (num){
            case 1:
                menu.testIO();
                break;
            case 2:
                menu.testIDFT();
                break;
            case 3:
                menu.addNoise2Speech();
                break;
            case 4:
                menu.addGaussianNoise2Speech();
                break;
            case 5:
                menu.spectralSubtract();
                break;
            case 6:
                menu.wiener();
                break;
            case 7:
                menu.evaluation();
                break;
            case 8:
                cout<<endl<<"Exit program...";
                exit(0);
                
            default:
            cout<<"Incorrect value. Please input again "<<endl;

        }

    }while(num!=8);


}


int main(){
    menu();
}

