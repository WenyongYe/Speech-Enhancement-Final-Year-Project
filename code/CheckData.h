
/*!
* \file CheckData.h
* \brief Check input data
*
* Input data is checked, including file names and
 * the parameters for performing the functions
*
*/
#ifndef INC_3002_CHECKDATA_H
#define INC_3002_CHECKDATA_H

#include <iostream>
using namespace std;

/// \brief Check input value
///
/// Used in the RunMenu
class CheckData{
public:
    /// Check input value
    ///
    /// Check the input value if it is correct, if not, input again
    ///\param i Check float number as input value

    void checkInputValueType(float &i);

    /// Check input value
    ///
    /// Check the input value if it is correct, if not, input again
    ///\param i Check int number as input value
    void checkInputValueType(int &i);

    /// Check if the input string is number
    ///
    /// Check if the components in the string are all digit numbers
    ///\param s string to be checked
    bool isNumber(string s);

    /// Input sampling period
    ///
    /// Get sampling period and check if it is valid
    ///\param in string to be checked
    int getSampPeriod(string in);
};

#endif //INC_3002_CHECKDATA_H