#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <tuple>
#include <algorithm>

using namespace std;
  
int main(int argc, char* argv[])
{
    if (argc != 2){
        printf("Syntax Error\n");
        printf("Correct Usage : [Program Name] [Argument 1]\n");
        printf("Argument 1 must be a file with n by n matrix entries\n");
        return EXIT_FAILURE;
    }

    string fileName;
    istringstream iss(argv[1]);
    vector<vector<int>> matrix;
    vector<int> one_line;
    int num_philosofers=0;

    // open file to initialize with input
    if (iss >> fileName){
        ifstream myFile(fileName);
        // grabbing input data
        if (myFile.is_open()){
            string line;
            while (std::getline(myFile, line)) {
                // assures that is empty
                one_line.clear();
                for (int i = 0; i<line.size(); i++)
                {
                   if (line[i] == '0'){
                        one_line.push_back(0);
                   }
                   else if (line[i] == '1'){
                        one_line.push_back(1);
                   }
                }
                matrix.push_back(one_line);
            }
            myFile.close();
        }
        else{
            printf("File Does Not Exist.");
            return EXIT_FAILURE;
        }
    }
    else { printf("Check File Name.\n");return EXIT_FAILURE;}

    // Printa a matriz
    for(vector<vector<int> >::iterator it = matrix.begin(); it != matrix.end(); ++it)
    {
        //it is now a pointer to a vector<int>
        for(vector<int>::iterator jt = it->begin(); jt != it->end(); ++jt)
        {
            // jt is now a pointer to an integer.
            cout << *jt;
        }
        cout << endl;
    }

    num_philosofers = matrix.size();

    cout << "numero de filosofos " << num_philosofers << endl;


    return EXIT_SUCCESS;
}