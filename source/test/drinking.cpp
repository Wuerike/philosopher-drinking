#include "drinking.h"
//#include "philosopher.cpp"
//#include "table.cpp"

using namespace std;

/**
 * Class that implements a Table
 */
    Table::Table (int session_size)
    {
        this->session_size = session_size;
    }

    void Table::print_state(int id, string state){
        lock_guard <mutex> guard(print_mtx);
        cout << "Philosopher " << id << " is " << state << endl;
        // delay to slow down the message pop up at the terminal
        sleep_for(milliseconds(50)); 
    }

    void Table::total_thirsty_time(int id, int time){
        lock_guard <mutex> guard(print_mtx);
        cout << "Philosopher " << id << " TOTAL thirsty for " << time << " ms" << endl;
        // delay to slow down the message pop up at the terminal
        sleep_for(milliseconds(50));
    }

    void Table::print_thirsty_time(int id, int time){
        lock_guard <mutex> guard(print_mtx);
        cout << "Philosopher " << id << " was thirsty for " << time << " ms" << endl;
        // delay to slow down the message pop up at the terminal
        sleep_for(milliseconds(50));
    }

    void Table::print_holded_bottle(int id, int bottle_id){
        lock_guard <mutex> guard(print_mtx);
        cout << "Philosopher " << id << " has bottle from " << bottle_id << endl;
        // delay to slow down the message pop up at the terminal
        sleep_for(milliseconds(50));
    }


    bool Table::request_handler(int requester_id, int n_bottles, vector<Philosopher *> neighbors){
        // The mutex avoids threads to run this seccion mutually
        lock_guard <mutex> guard(request_mtx);

        // look if requester id is in drink session vector
        vector<int>::iterator position = find(drink_session.begin(), drink_session.end(), requester_id);
        // refuses this requester if it already drinked in this session
        if (position != drink_session.end()){ 
            return false;
        }

        int sended_bottles = 0;
        vector <int> available_bottles;

        // run through the philosopher neighbors
        for (int i = 0; i < neighbors.size(); i++)
        {
            // for each neighbor check if he is holding the bottle
            vector<int>::iterator position = find(neighbors[i]->holded_bottle.begin(), neighbors[i]->holded_bottle.end(), requester_id);
            // true if the neighbor is not holding the bottle
            if (position == neighbors[i]->holded_bottle.end())
            {   
                // add the neihbor id to the available bottles list
                available_bottles.push_back(neighbors[i]->philosopher_id);
            }
        }

        // check if there is enough bottles
        if (available_bottles.size() >= n_bottles)
        {
            // look for the neighbors add to available bottles list
            // and then make then send the bottle to the requester
            for (int i = 0; i < neighbors.size(); i++)
            { 
                for (int j = 0; j < available_bottles.size(); j++)
                {
                    if(neighbors[i]->philosopher_id == available_bottles[j])
                    {
                        neighbors[i]->send_bottle(requester_id);
                        sended_bottles++;
                    }
                }
            }
        }
        // If there isn't enough bottles try again later
        else 
        {
            return false;
        }

        // append the requester id in the drink session
        drink_session.push_back(requester_id);
        // reset if al the filosofers drinked in this session
        if (drink_session.size() == session_size){
            drink_session.clear();
        }
        return true;
    }


/**
 * Class that implements the existence of a Philosopher 
 */
    Philosopher::Philosopher (const int id, Table& table, int n_sessions)
    :philosopher_id(id),
    table(table),
    n_sessions(n_sessions)
    {
        this->num_bottle_max = 0;
        this->total_thirsty_time = 0;
        this->state = ("tranquil");
    }

    /*
     * Set which are the this philosopher's neighbors
     */
    void Philosopher::set_neighbors (vector<Philosopher *> neighbors)
    {
        this->neighbors = neighbors;
        this->num_bottle_max = neighbors.size();
    }

    bool Philosopher::send_request (vector<Philosopher *> neighbors, int n_bottles){
        return table.request_handler(this->philosopher_id, n_bottles, neighbors);
    }

    void Philosopher::send_bottle (int id){
        for (int i = 0; i < neighbors.size(); i++)
        {
            if (neighbors[i]->philosopher_id == id){
                neighbors[i]->holded_bottle.push_back(this->philosopher_id);
            }
        }
    }

    /*
     * Spawns the parallel execution of a Philosopher that shares the same
     * table with other Philosophers
     */
    thread* Philosopher::spawn ()
    {
        return new thread([=] { exist(); });
    }

    /**
     * The existence of a Philosopher
     */
    void Philosopher::exist ()
    {
        for (int i=0; i<n_sessions; i++) 
        {
            tranquil();
            thirsty();
            drinking();
        } 
        //send_request(neighbors, 0);   // call request handler just to verify the non executed orders
        table.total_thirsty_time(philosopher_id, total_thirsty_time);
    }

    /**
     * The randomized time that each Philosopher spends tranquil
     */
    void Philosopher::tranquil ()
    {
        state = ("tranquil");
        table.print_state(philosopher_id, state);
        srand(time(0));
        sleep_for(milliseconds(rand() % 2000));
    }

    void Philosopher::thirsty (){
        auto start = steady_clock::now();

        state = ("thirsty");
        table.print_state(philosopher_id, state);

        // Choose bottles from 2 to number of neighbors
        int num_bottle_needed = 2 + rand()%(this->num_bottle_max-1);
 
        // Send a request for his neighbors
        while(!send_request(neighbors, num_bottle_needed))
        {
            // Timeout between requests
            sleep_for(milliseconds(50));
        } 

        // Show the bottles he get
        for (int i=0; i<holded_bottle.size(); i++){
            table.print_holded_bottle(philosopher_id, holded_bottle[i]);
        }

        auto end = steady_clock::now();
        auto session_thirsty_time = duration_cast<milliseconds>(end - start).count();
        total_thirsty_time += session_thirsty_time;
        //table.print_thirsty_time(philosopher_id, session_thirsty_time);
    }

    void Philosopher::drinking (){
        state = ("drinking");
        table.print_state(philosopher_id, state);
        sleep_for(milliseconds(1000)); 
        holded_bottle.clear(); //após beber larga todas as garrafas
    }

/*
 * Main function
*//*
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


    for(int i=0; i<num_philosofers; i++){
        threads[i] = philosopher[i]->spawn();
    }   

    for(int i=0; i < num_philosofers; i++) {
        threads[i]->join();
    }

    philosopher.clear();
    threads.clear();

    for(int i=0; i<num_philosofers; i++){
        delete philosopher[i];
        delete threads[i];
    }

    auto end = steady_clock::now();
    auto simulation_time = duration_cast<milliseconds>(end - start).count();
    cout << endl << "SIMULATION LASTED " << simulation_time << " ms" << endl;
    cout << "END OF SIMULATION" << endl << endl;
    
    return EXIT_SUCCESS;
}*/