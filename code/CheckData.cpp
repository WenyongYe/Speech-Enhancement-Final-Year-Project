
#include "CheckData.h"

//check data type if it is float
void CheckData::checkInputValueType(float& i) {
    while (cin.fail()) {
        cout << endl << "Incorrect value. Please input again " << endl;
        cout << "Input: ";
        cin.clear();
        cin.ignore(256, '\n');
        cin >> i;
    }
    cout << "Input value successfully..." << endl;
}

//check data type if it is int
void CheckData::checkInputValueType(int& i) {
    while (cin.fail()) {
        cout << endl << "Incorrect value. Please input again " << endl;
        cout << "Input: ";
        cin.clear();
        cin.ignore(256, '\n');
        cin >> i;
    }
    cout << "Input value successfully..." << endl;
}

//check input string if it is digit number
bool CheckData::isNumber(string s) {
    //if positive
    if (s[0] != '-') {
        for (int i = 0; i < s.length(); i++) {

            if (s[i] > '9' || s[i] < '0')
                return false;
        }
        return true;
    }
    //if negative
    else {

        for (int i = 0; i < s.length(); i++) {

            if (s[i] > '9' || s[i] < '0')
                return false;
        }
        return true;

    }
}

//get sampling period
int CheckData::getSampPeriod(string in) {
    int sam_period;
    bool flag;

    do {
        cout << endl << "Input sampling period (e.g. 20 ms) : ";
        cin >> in;

        //if digit
        if(isNumber(in)){
            flag=true;
            sam_period=stoi(in);
        }
        else
            flag=false;

        //check data type
        if(flag==true)
            checkInputValueType(sam_period);

        if (sam_period <= 0||flag== false)
            cout << endl << "Incorrect value. Please input again";

    } while (sam_period <= 0||flag== false);

    return sam_period;
}