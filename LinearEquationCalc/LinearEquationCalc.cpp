#include <iostream>
#include <string>

using namespace std;

int main()
{
    string equations[100], syllabus[100], terms[100];
    string term = "";
    int RHS = 0;
    int coefficient[100], xIndex[100];
    int termsCounter = 0;
    int n;

    //Insert the number of equations
    cout << "How many equations do you want to add? ";
    cin >> n;


    for (int i = 0; i < n; ++i) {
        cout << "Enter the equation: ";
        cin >> equations[i];
    }
    //Outer loop to loop each equation at a time
    for (int i = 0; i < n; ++i) {
        //Loop through each equation and check its components
        for (int j = 0; j < equations[i].length(); j++) {
            if(equations[i][j] != '+' && equations[i][j] != '=' && equations[i][j] != '-') {
                    term += equations[i][j];
            }
            else {
                if (term != "") {
                    terms[termsCounter] = term;
                    ++termsCounter;
                    term = "";
                }
                term += equations[i][j];
            }
        }
        // To add the last term as well and empty the term variable
        if (term != "") {
            terms[termsCounter] = term;
            ++termsCounter;
            term = "";
        }
    }

    //Get the right hand side
    for (int k = 0; k < termsCounter; k++) {
        if (terms[k][0] == '=') {
            string RHS_Value = "";
            for (int i = 1; i < terms[k].length(); ++i) {
                RHS_Value += terms[k][i];
            }
            RHS = stoi(RHS_Value) ;

            cout << "RHS = " <<  RHS << endl;
            }
        }
    //Check if the element has x 
    for (int k = 0; k < termsCounter; k++) {
            for (int i = 1; i < terms[k].length(); ++i) {
                if (terms[k][i] == 'x') {

                }
            }
    return 0;
}
