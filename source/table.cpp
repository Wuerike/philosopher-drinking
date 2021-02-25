//#include "drinking.h"
#include "table.h"

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