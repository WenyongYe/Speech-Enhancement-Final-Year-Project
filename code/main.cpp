#include <iostream>
#include <fstream>
#include "RIFF.h"
#include "WAVData.h"
#include "IO.h"
#include "Analysis.h"
#include "Simulation.h"
#include "RunMenu.h"
#include <string>

using namespace std;


//menu for the program
void menu() {

	cout << endl << "CSC3002 Program Running ..." << endl << endl;


	string num;

	RunMenu menu;

	do {
		cout << endl;
		cout << "------------------------------------" << endl;
		cout << "--------------- Menu ---------------" << endl;
		cout << "------------------------------------" << endl;

		cout << "Choose number to select operations" << endl << endl;
		cout << "1. Test IO function" << endl << "2. Test DFT and IDFT" <<
			endl << "3. Noisy Speech Simulation (Add noise to clean speech)" <<
			endl << "4. Noisy Speech Simulation (Add white Gaussian noise)" <<
			endl << "5. Speech Enhancement (Spectral subtraction)" <<
			endl << "6. Speech Enhancement (Wiener filter)" <<
			endl << "7. Evaluation (Segmental SNR evaluation)" <<
			endl << "8. Exit" << endl;

		cout << endl << "Input operation number: ";
		cin >> num;

		if (num == "1") {
			menu.testIO();
		}
		else if (num == "2") {
			
			menu.testIDFT();
		}
		else if (num == "3") {
			
			menu.addNoise2Speech();
		}
		else if (num == "4") {
			
			menu.addGaussianNoise2Speech();
		}
		else if (num == "5") {
			
			menu.spectralSubtract();
		}
		else if (num == "6") {
			menu.wienerFilter();
		}
		else if (num == "7") {
			menu.evaluation();
		}
		else if (num == "8") {
			cout << endl << "Exit program...";
			exit(0);
		}
		else
			cout << "Incorrect value. Please input again " << endl;

	} while (num != "8");
}


int main() {

	menu();
}

