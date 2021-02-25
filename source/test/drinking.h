#if !defined(__DRINKING_H__)
#define __DRINKING_H__

#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <string>
#include <algorithm>

using std::string;
using std::thread;
using std::cout;
using std::endl;
using std::vector;
using std::this_thread::sleep_for;
using std::rand;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::mutex;
using std::lock_guard;

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


#endif // #if !defined(__DRINKING_H__)