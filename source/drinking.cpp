#include "drinking.h"
#include "philosopher.cpp"
#include "table.cpp"

using namespace std;


/*
 * Main function
*/
int main (int argc, char* argv[])
{
    auto start = steady_clock::now();
    cout << endl << "DRINKING PHILOSOPHERS" << endl << endl;

    if (argc != 3){
        cout << "Syntax Error" << endl;
        cout << "Correct Usage : [Program Name] [Argument 1] [Argument 2]" << endl;
        cout << "Argument 1: file with N by N matrix entries" << endl;
        cout << "Argument 2: integer that defines the number of drinking sessions" << endl;
        cout << "Example: ./drinking 5phil 6" << endl;
        return EXIT_FAILURE;
    }

    string fileName;
    istringstream iss(argv[1]);
    int n_sessions = strtol(argv[2], nullptr, 0);
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
            cout << "File Does Not Exist." << endl << endl;
            return EXIT_FAILURE;
        }
    }
    else 
    {  
        cout << "File readinf error" << endl << endl;
        return EXIT_FAILURE;
    }

    num_philosofers = matrix.size();

    cout << "Number of philosophers " << num_philosofers << endl;
    cout << "Number of drinking sessions " << n_sessions << endl;


    Table table(num_philosofers);
    vector<thread *> threads(num_philosofers);
    vector<Philosopher *> philosopher;
    vector<Philosopher *> neighbors;

    // Create philosophers with its ID number
    for(int i=0; i < num_philosofers; i++) {
        philosopher.push_back(new Philosopher(i, table, n_sessions));
    }

    // For each filosopher sets a vector of neighbors philosophers
    for (int i = 0; i < num_philosofers; i++)
    {
        neighbors.clear();
        for (int j = 0; j < num_philosofers; j++)
        {
            if(matrix[j][i] == 1)
            {
                neighbors.push_back(philosopher[j]);
            }
        }
        philosopher[i]->set_neighbors(neighbors);
    }

    // Starts philosophers life cycle
    for(int i=0; i<num_philosofers; i++){
        threads[i] = philosopher[i]->spawn();
    }   

    // Join the threads and waint until they're finished
    for(int i=0; i < num_philosofers; i++) {
        threads[i]->join();
    }

    // Clear the resources
    philosopher.clear();
    threads.clear();

    for(int i=0; i<num_philosofers; i++){
        delete philosopher[i];
        delete threads[i];
    }

    // Return the simulation time
    auto end = steady_clock::now();
    auto simulation_time = duration_cast<milliseconds>(end - start).count();
    cout << endl << "SIMULATION LASTED " << simulation_time << " ms" << endl;
    cout << "END OF SIMULATION" << endl << endl;
    
    return EXIT_SUCCESS;
}