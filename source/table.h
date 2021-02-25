#if !defined(__TABLE_H__)
#define __TABLE_H__

#include "drinking.h"
#include "philosopher.h"

class Philosopher;

class Table
{
    public:
    int session_size;
    mutex print_mtx;
    mutex request_mtx;
    vector<int> drink_session;

    public:
    Table (int session_size);
    void print_state(int id, string state);
    void total_thirsty_time(int id, int time);
    void print_thirsty_time(int id, int time);
    void print_holded_bottle(int id, int bottle_id);
    bool request_handler(int requester_id, int n_bottles, vector<Philosopher *> neighbors);
}; 

#endif // #if !defined(__TABLE_H__)