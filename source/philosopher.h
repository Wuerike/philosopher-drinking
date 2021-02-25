#if !defined(__PHILOSOPHER_H__)
#define __PHILOSOPHER_H__

#include "drinking.h"
#include "table.h"

class Philosopher
{
    public:
    string state;
    Table& table;
    int philosopher_id;
    int num_bottle_max;
    int n_sessions;
    int total_thirsty_time;
    vector<int> holded_bottle;
    vector<Philosopher *> neighbors;
    
    Philosopher (const int id, Table& table, int n_sessions);
    void set_neighbors (vector<Philosopher *> neighbors);
    bool send_request (vector<Philosopher *> neighbors, int n_bottles);
    void send_bottle (int id);
    thread* spawn ();
    void exist ();
    void tranquil ();
    void thirsty ();
    void drinking ();
};

#endif // #if !defined(__PHILOSOPHER_H__)