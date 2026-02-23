#include <iostream>
#include <string>

using namespace std;

int main()
{
    string equations[100], syllabus[100];
    
    float RHS = 0;
    float coefficient[100][100], xIndex[100], RHS_Arr[100];
    float coeff = 0, index = 0, sign = 1;
    bool foundCoeff = false;
    int n;

    //Insert the number of equations
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> equations[i];
    }
        
        // =========================
        // STAGE 2: Process each equation independently
        // =========================
        // Outer loop to loop each equation at a time


    for (int i = 0; i < n; ++i) {
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
            } else {
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
        // Get the right hand side
        // =========================

        for (int k = termsCounter - 1; k >= 0; --k) {
            if (terms[k][0] == '=') {
                string RHS_Value = "";
                for (int c = 1; c < terms[k].length(); c++) {
                    RHS_Value += terms[k][c];
                }
                RHS_Arr[i] = stof(RHS_Value);
                break;
            }
        }


        // =========================
        // Creating Terms
        // =========================
		//Each iteration of this loop will go over a term to make it a standalone element that can be added to the terms array 
        //Say iteraion 1 --> "2x1" then iteration 2 --> "-3x2" and so on

        for (int termIndex = 0; termIndex < termsCounter; termIndex++) {
			//Skip the RHS term
            if (terms[termIndex][0] == '=') {
                continue;
            }
            bool hasX = false;
			//If the term contains an 'x' then break the inner loop and parse
            for (int m = 0; m < terms[termIndex].length(); m++) {
                if (terms[termIndex][m] == 'x') {
                    hasX = true;
                    break;
                }
            }
			//If the term does not contain an 'x' then skip the current iteration in the loop to the next term ( A constaant in the LHS )
            if (!hasX) {
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
            coefficient[i][col] += coeff; //Add the coefficient to the correct position in the matrix
        }
    }
    cout << "\nAugmented Matrix:\n";
	for (int varIndex = 1; varIndex <= n; varIndex++) {
        cout << "x" << varIndex << " ";
    }
    cout << endl;
    for (int i = 0; i < n; i++) { // create rows
        cout << "coeff" << i+1 << " ";
		for (int j = 0; j < n; j++) { // create columns 
            cout << coefficient[i][j] << " ";
        }
        cout << "| " << RHS_Arr[i] << endl;
    }
    return 0;
}
    /*
        Next Step--> Split Coeefficients and indexes--> -2x1, 3x2, x3 
        if term[0] == '-' --> sign = -1
        if term[1] == Number --> coefficient = number / if not? coefficient = 1
        if term[2] == X --> Do nothing
        if term[3] == Number --> Index = Number 
    */