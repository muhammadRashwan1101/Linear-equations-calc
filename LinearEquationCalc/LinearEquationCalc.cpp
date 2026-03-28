#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <sstream>

using namespace std;

int main()
{
    string equations[100];
    string org_equations[100];
    float coefficient[100][100], RHS_Arr[100];
    int n;


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
                }
                else {
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
    string command;

    while (quit) {
        getline(cin, command);

        string eqCheck = command.substr(0, 8);
        if (command == "num_vars") {
            cout << "Number of Values = " << n << "\n";
        }


        // ================================
        // PRINT EQUATION OF N
        // ================================
        if (eqCheck == "equation") {
            int eNumber = stoi(command.substr(9)); // number after "equation "
            //Check if there's no equation with this index
            if (eNumber <= 0 || eNumber > n) {
                cout << "No Equations listed with that index, Last available equation = " << n << endl;
                continue;
            }
            int row = eNumber - 1;
            cout << "Equation " << eNumber << ": ";
            //Needed to recognize if it's the first term so we can ignore any operators at the beginning of the term
            bool firstTerm = true;
            for (int col = 0; col < n; col++) {
                float coeff = coefficient[row][col];
                if (coeff == 0)
                    continue;
                //If it's not the first term anymore and the coeff is positive We are double checking that the sign is a plus 
                if (!firstTerm && coeff > 0) {
                    cout << "+";
                }
                cout << coeff << "x" << col + 1;
                //Disable the firsTerm flag so the operators at the beginning of each term to be further used normally 
                firstTerm = false;
            }
            cout << " = " << RHS_Arr[row] << endl;
        }


        if (command.substr(0, 6) == "column") {
            int index = stoi(command.substr(8));
            for (int i = 0; i < n; i++) {
                if (index <= 0 || index > n) {
                    cout << "No Columns listed under that index, Last available column of variables = " << n << endl;
                    continue;
                }
                cout << coefficient[i][index - 1] << endl;
            }
        }

        // ================================
        // ADD 2 EQUATIONS
        // ================================
        if (command.substr(0, 3) == "add") {
            //Get the first equation number and the second
            string word;
            int eq1, eq2;
            stringstream ss(command);
            ss >> word;
            ss >> eq1;
            ss >> eq2;

            //Check if the equations number is valid
            if (eq1 <= 0 || eq2 <= 0 || eq1 > n || eq2 > n) {
                cout << "No equations available with this index, equations start from 1 to " << n << endl;
                continue;
            }

            //Adjust the equation numbers to be used in the coefficients array
            int row1 = eq1 - 1;
            int row2 = eq2 - 1;

            float RHSSum;
            bool firstTerm = true;
            for (int col = 0; col < n; col++) {
                // Add Coefficients and RHS values 
                float coeff1 = coefficient[row1][col];
                float coeff2 = coefficient[row2][col];
                float coeffSum = coeff1 + coeff2;

                float RHS1 = RHS_Arr[row1];
                float RHS2 = RHS_Arr[row2];
                RHSSum = RHS1 + RHS2;

                //Skip any term that will result a 0 Coefficient
                if (coeffSum == 0)
                    continue;
                if (!firstTerm && coeffSum > 0) {
                    cout << "+";
                }
                //print the output term
                cout << coeffSum << "x" << col + 1;
                firstTerm = false;
            }
            //print the RHS
            cout << " = " << RHSSum << endl;
        }

        // ================================
        // SUBTRACT 2 EQUATIONS
        // ================================
        if (command.substr(0, 8) == "subtract") {
            //Get the first equation number and the second
            string word;
            int eq1, eq2;
            stringstream sepCommand(command);
            sepCommand >> word;
            sepCommand >> eq1;
            sepCommand >> eq2;

            if (eq1 <= 0 || eq2 <= 0 || eq1 > n || eq2 > n) {
                cout << "No equations available with this index, equations start from 1 to " << n << endl;
                continue;
            }

            int row1 = eq1 - 1;
            int row2 = eq2 - 1;

            float RHSSum;
            bool firstTerm = true;
            for (int col = 0; col < n; col++) {
                float coeff1 = coefficient[row1][col];
                float coeff2 = coefficient[row2][col];
                float coeffSum = coeff1 - coeff2;

                float RHS1 = RHS_Arr[row1];
                float RHS2 = RHS_Arr[row2];
                RHSSum = RHS1 - RHS2;


                if (coeffSum == 0)
                    continue;
                if (!firstTerm && coeffSum > 0) {
                    cout << "+";
                }
                cout << coeffSum << "x" << col + 1;
                firstTerm = false;
            }
            cout << " = " << RHSSum << endl;
        }
        // ================================
        // SUBSTITUTE BETWEEN 2 EQUATIONS
        // ================================
        //Substitute X of n in equation No.1 with X of n in equation No.2
        if (command.substr(0, 10) == "substitute") {
            string word;
            string replacement;
            int repIndex = 0;
            int eq1;
            int eq2;

            stringstream sepCommand(command);
            sepCommand >> word;
            sepCommand >> replacement;
            sepCommand >> eq1;
            sepCommand >> eq2;

            if (eq1 <= 0 || eq2 <= 0 || eq1 > n || eq2 > n) {
                cout << "No equations available with this index, equations start from 1 to " << n << endl;
                continue;
            }

            for (int i = 0; i < replacement.length(); i++) {
                if (replacement[i] == 'x') continue;
                repIndex = repIndex * 10 + (replacement[i] - '0');
            }
            
            int row1 = eq1 - 1;
            int row2 = eq2 - 1;
            int col = repIndex - 1;
            //Replace X value from a function and substitute it in every other term with the same value of X
            //Say 2x1 + 3x2 = 6 --> x2 = (6 - 2x1)/3
            //Now in another equation 4x1 + 6x2 = 10 --> 4x1 + (6*(6-2x1))/3
                
                //if there's no X of n in the source equation
                if (coefficient[row2][col] == 0) {
                    cout << "x" << repIndex << " does not exist in equation " << eq2 << endl;
                    continue;
                }

                float tempCoeff[100];
                //Gaussian Elimination 
                // eq1: a1​x1​+a2​x2​+a3​x3​=b1​
                // eq2: c1​x1​+c2​x2​+c3​x3​=b2​
                // x2 from eq2 to use in 1 = b2-c1x1-c3x2/c2
                //substituting in eq1: a1x1+a2(b2-c1x1-c3x2/c2)+a3x3=b1
                //normalizing the eqaution: a1x1+(a2/c2)*b2-(a2/c2)*c1x1-(a2/c2)*c3x3+a3x3=b1
                //Grouping like terms: (a1-(a2/c2)c1)x1 | (a3-(a2/c2)c3)x3 | b1-(a2/c2)b2

                // coeffMultiplier = a2/c2
                float coeffMultiplier = coefficient[row1][col] / coefficient[row2][col];
                
                for (int CoeffInd = 0; CoeffInd < n; CoeffInd++) {
                  //tempCoeff           =         an                  -     (a2/c2)     *             cn
                    tempCoeff[CoeffInd] = coefficient[row1][CoeffInd] - coeffMultiplier * coefficient[row2][CoeffInd];
                }
                    //newRHS = b1            -     (a2/c2)           b2
                float newRHS = RHS_Arr[row1] - coeffMultiplier * RHS_Arr[row2];

                bool firstTerm = true;

                for (int cof = 0; cof < n; cof++) 
                {
                    //if the resulte of substitution is 0
                    if (tempCoeff[cof] == 0) continue;

                    if (!firstTerm && tempCoeff[cof] > 0) {
                        cout << "+";
                    }

                    cout << tempCoeff[cof] << "x" << cof + 1;
                    firstTerm = false;
                }
                cout << " = " << newRHS << endl;
        }
        // ================================
        // GET DETERMINENTS COEFFICIENT MATRIX
        // ================================
        if (command == "D") {
            cout << "      ";
            for (int j = 0; j < n; j++) {
                cout << "   x" << j + 1 << "   ";
            }
            cout << endl;
            cout << "----------------------------------\n";
           
            for (int i = 0; i < n; i++) {
                cout << "Eq" << i + 1 << " | ";

                for (int j = 0; j < n; j++) {
                    cout.width(6);
                    cout << coefficient[i][j] << " ";
                }
                cout << endl;
            }
        }

        // =================================================================
        // GET DETERMINENTS COEFFICIENT MATRIX BY REPLACTING VARIABLE COLUMN
        // =================================================================
        if (command.substr(0,1) == "D" && command != "D_value" && command != "D") {
            string D;
            string var;
            
            stringstream cramer(command);

            cramer >> D;
            cramer >> var;

            int varIndex = 0;

            for (int i = 0; i < var.length(); i++)
            {
                if (var[i] == 'x') continue;
                varIndex = varIndex * 10 + (var[i] - '0');
            }

            int targetCol = varIndex - 1;

            cout << "      ";
            for (int j = 0; j < n; j++) {
                cout << "   x" << j + 1 << "   ";
            }
            cout << endl;
            cout << "----------------------------------\n";
           
            for (int i = 0; i < n; i++) {
                cout << "Eq" << i + 1 << " | ";

                for (int j = 0; j < n; j++) {
                    //if we are at the target column, then print the RHS instead of the xj coefficients
                    if (j == targetCol) {
                        cout.width(6);
                        cout << RHS_Arr[i] << " ";
                    }
                    else {
                        cout.width(6);
                        cout << coefficient[i][j] << " ";
                    }
                }
                cout << endl;
            }
        }
        // ================================
        // Get the Determinant of the Matrix using Gaussian Elimination
        // ================================

        float det = 1;
        if (command == "D_value") {
            //Make a temp compy of the orignal matrix
            float tempMatrix[100][100];
            for (int row = 0; row < n; row++) {
                for (int col = 0; col < n; col++) {
                    tempMatrix[row][col] = coefficient[row][col];
                }
            }

            //use Gaussian Elimination to make the Matrix Diagonal
            //first loop to iterate n-1 ( The number of columns needed to produce a diagonal matrix )
            for (int targetCol = 0; targetCol < n-1; targetCol++) {

                //Loop on the rows starting from the second row each time
                for (int targetRow = targetCol + 1; targetRow < n; targetRow++) {

                    // tempMatrix[targetRow][targetCol] -> gets the target coefficient for the multiplier denominator 
                    // tempMatrix[targetCol][targetCol] -- > gets the one needed element ( Pivot element in Gauss Eliminations terminology) to eliminate all the values under the same column 
                    if (tempMatrix[targetCol][targetCol] == 0) {
                        cout << "Element used from Target column = Zero, Determinent can't be calculated";
                        break;
                    }
                    float multiplier = tempMatrix[targetRow][targetCol] / tempMatrix[targetCol][targetCol];


                    //after calculating the multiplier, eliminate the lower left triangle in the matrix to make an upper right triangle 
                    for (int col = targetCol; col < n; col++) {
                        tempMatrix[targetRow][col] -= multiplier * tempMatrix[targetCol][col];
                    }
                }
            }
            
            for (int i = 0; i < n; i++) {
                det *= tempMatrix[i][i];
            }
            cout << det << endl;
        }
        // ====================================================
        // Using Gaussian Elimination to Solve the System 
        // ====================================================
        if (command == "solve") {
            float tempMatrix[100][100];
            for (int row = 0; row < n; row++) {
                for (int col = 0; col < n; col++) {
                    tempMatrix[row][col] = coefficient[row][col];
                }
            }
            float tempRHS[100];
            for (int row = 0; row < n; row++) {
                for (int col = 0; col < n; col++) {
                    tempRHS[row] = RHS_Arr[row];
                }
            }
            //first loop to iterate n-1 ( The number of columns needed to produce a diagonal matrix )
            for (int targetCol = 0; targetCol < n - 1; targetCol++) {
                // tempMatrix[targetRow][targetCol] -> gets the target coefficient for the multiplier denominator 
                // tempMatrix[targetCol][targetCol] -- > gets the one needed element ( Pivot element in Gauss Eliminations terminology) to eliminate all the values under the same column 
                if (tempMatrix[targetCol][targetCol] == 0) {
                    cout << "Element used from Target column = Zero, Determinent can't be calculated";
                    break;
                }
                //Loop on the rows starting from the second row each time
                for (int targetRow = targetCol + 1; targetRow < n; targetRow++) {

                    float multiplier = tempMatrix[targetRow][targetCol] / tempMatrix[targetCol][targetCol];


                    //after calculating the multiplier, eliminate the lower left triangle in the matrix to make an upper right triangle 
                    for (int col = targetCol; col < n; col++) {
                        tempMatrix[targetRow][col] -= multiplier * tempMatrix[targetCol][col];
                    }
                }
            }
        }

        if (command == "quit") {
            quit = false;
        }
    }


    cout << "\nAugmented Matrix:\n\n";

    // Print header row representing X of n
    cout << "      ";
    for (int j = 0; j < n; j++) {
        cout << "   x" << j + 1 << "   ";
    }
    cout << " |   RHS\n";
    cout << "--------------------------------------------------\n";

    // Print matrix rows Eqch equation
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
  