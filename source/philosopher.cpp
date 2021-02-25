//#include "drinking.h"
#include "philosopher.h"

using namespace std;

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