#include <iostream>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

int main()
{
    string equations[100];
    float coefficient[100][100], xIndex[100], RHS_Arr[100];
    int n;
    string command;

    //Insert the number of equations
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    for (int i = 0; i < n; ++i) {
        getline(cin, equations[i]);
    }

    // =========================
    // STAGE 2: Process each equation independently
    // =========================
    // Outer loop to loop each equation at a time


    for (int i = 0; i < n; ++i) {
        RHS_Arr[i] = 0;
        string RHS_Value = "";
        float LHS_const = 0;
        bool onRightSide = false;

        string cleaned = "";

        for (int ch = 0; ch < equations[i].length(); ch++) {
            if (equations[i][ch] != ' ') {
                cleaned += equations[i][ch];
            }
        }

        equations[i] = cleaned;
        string terms[100];
        int termsCounter = 0;
        string term = "";

        // =========================
        // Creating Matrix Rows     
        // =========================
        //Initialize the coefficient matrix to 0 so that we can add values to it later without garbage values

        for (int col = 0; col < 100; col++) {
            coefficient[i][col] = 0;
        }

        // =========================
        // Split equation into terms
        // =========================
        //Loop through each equation and check its components to separate terms

        for (int j = 0; j < equations[i].length(); j++) {
            //Add Everything that's not a character representing an operator forming a term
            if (equations[i][j] != '+' && equations[i][j] != '=' && equations[i][j] != '-') {
                term += equations[i][j];
            }
            else {
                //When an operator is found, store the found term so far and start a new one
                if (term != "") {
                    terms[termsCounter] = term;
                    ++termsCounter;
                    term = "";
                }
                //Add the operator to the new term
                term += equations[i][j];
            }
        }
        // To add the last term after the loop ends as well and clear the term variable
        if (term != "") {
            terms[termsCounter] = term;
            ++termsCounter;
            term = "";
        }

        // =========================
        // Creating Terms
        // =========================
        //Each iteration of this loop will go over a term to make it a standalone element that can be added to the terms array 
        //Say iteraion 1 --> "2x1" then iteration 2 --> "-3x2" and so on

        for (int termIndex = 0; termIndex < termsCounter; termIndex++) {
            //get the RHS term
            if (terms[termIndex][0] == '=') {
                onRightSide = true;
                terms[termIndex] = terms[termIndex].substr(1);
                if (terms[termIndex].empty()) continue;
            }
            bool hasX = false;
            //If the term contains an 'x' then break the inner loop and parse
            for (int m = 0; m < terms[termIndex].length(); m++) {
                if (terms[termIndex][m] == 'x') {
                    hasX = true;
                    break;
                }
            }
            //If the term does not contain an 'x' then skip the current iteration in the loop to the next term ( Catch the constant )
            if (!hasX) {
                float value = stof(terms[termIndex]);
                if (!onRightSide) {
                    LHS_const += value;
                } else {
                    RHS_Arr[i] += value;
                }
                continue;
            }

            //Reset the variables for each parsing process for a term so that previous values do not interfere
            float coeff = 0;
            int index = 0;
            int sign = 1;
            bool foundCoeff = false;
            int position = 0;



            // ================================
            // Finding Coefficients and Indexes
            // ================================
            //Check for the sign of the coefficient

            if (terms[termIndex][position] == '-') {
                sign = -1;
                position++;
            }
            else if (terms[termIndex][position] == '+') {
                position++;
            }


            while (position < terms[termIndex].length() && isdigit(terms[termIndex][position])) {
                // Build the coefficient number multiplied by 10 so that each digit is added in its correct place value 
                // say ( 12x1 --> first iteration coeff = 0*10 + (value of char '1' - '0' = 1, second iteration coeff = 1*10 + 2 = 12 )

                coeff = coeff * 10 + (terms[termIndex][position] - '0');
                foundCoeff = true;
                ++position;
            }
            //If no coefficient is found, it means it's 1 --> x1 means 1x1 or -x1 means -1x1
            if (!foundCoeff) {
                coeff = 1;
            }
            //Skip the 'x' character to get to the index number
            if (terms[termIndex][position] == 'x') {
                ++position;
            }

            while (position < terms[termIndex].length() && isdigit(terms[termIndex][position])) {
                index = index * 10 + (terms[termIndex][position] - '0');
                ++position;
            }

            //Apply the sign to the coefficient
            coeff *= sign;
            // the super hero --> It converts the xIndex to the correct column index in the matrix 
            // say x1 --> col 0, x2 --> col 1 and so on. No matter which order the terms are in the equation it will place them correctly in the matrix accordingly
            int col = index - 1;

            if (!onRightSide) {
                coefficient[i][col] += coeff; //Add the coefficient to the correct position in the matrix
            }
            else {
                coefficient[i][col] -= coeff;
            }
        }
        RHS_Arr[i] -= LHS_const;
    }
    // ================================
    // Running Commnads
    // ================================
    bool quit = true;
    while (quit) {
        getline(cin,command);
        string eqCheck = command.substr(0, 8);


        if (command == "num_vars"){
            cout << "Number of Values = " << n << "\n";
        }

        if (eqCheck == "equation") {
            int eNumber = 0;
            for (int i = command.length() - 1; i >= 0; --i) {
                if (isdigit(command[i])) {
                    eNumber = eNumber * 10 + (command[i] - '0');
                    if (eNumber > n) {
                        cout << "No Equations listed with that index, Last available equation = " << n << endl;
                    }
                    else {
                        break;
                    }
                }
            }
            cout << equations[eNumber] << "\n";
        }

        if (command == "quit") {
            quit = false;
        }
    }



    
    cout << "\nAugmented Matrix:\n\n";

    // Print header row
    cout << "      ";
    for (int j = 0; j < n; j++) {
        cout << "   x" << j + 1 << "   ";
    }
    cout << " |   RHS\n";

    // Print separator line
    cout << "--------------------------------------------------\n";

    // Print matrix rows
    for (int i = 0; i < n; i++) {
        cout << "Eq" << i + 1 << " | ";

        for (int j = 0; j < n; j++) {
            cout.width(7);
            cout << coefficient[i][j] << " ";
        }

        cout << " | ";
        cout.width(7);
        cout << RHS_Arr[i] << endl;
    }
    return 0;
}
  