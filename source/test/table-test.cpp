#include "drinkingtest.h"

/*
*	The fixture for testing class Table
*/
class TableTest : public ::testing::Test {
protected:
	Table* table;
	Philosopher* p0;
	Philosopher* p1;
	Philosopher* p2;
	int n_sessions = 6;
	int num_philosofers = 3;
	vector<Philosopher *> neighbors;

	TableTest() {
		table = new Table(num_philosofers);
		p0 = new Philosopher(0, *table, n_sessions);
		p1 = new Philosopher(1, *table, n_sessions);
		p2 = new Philosopher(2, *table, n_sessions);

		// Add p1 and p2 as p0 neighbors
		neighbors.push_back(p1);
		neighbors.push_back(p2);
		p0->set_neighbors(neighbors);
		// Add p0 and p2 as p1 neighbors
		neighbors.clear();
		neighbors.push_back(p0);
		neighbors.push_back(p2);
		p1->set_neighbors(neighbors);
		// Add p0 and p1 as p2 neighbors
		neighbors.clear();
		neighbors.push_back(p0);
		neighbors.push_back(p1);
		p2->set_neighbors(neighbors);

		// Clear the bottles
		p0->holded_bottle.clear();
		p1->holded_bottle.clear();
		p2->holded_bottle.clear();
	}

  	~TableTest() override {
  		delete table;
  		delete p0;
  		delete p1;
  		delete p2;
	}
};

/*
*	Auxiliary function to test the fair concurrency in multithreading
*/
void ThirstyDrinkAux(Philosopher* philosopher){
	philosopher->thirsty();
	philosopher->drinking();
}

/*
*	Tests the request_handler method in a single thread
*/
TEST_F(TableTest, RequestHandlerSingleThreadTest) {
	// Assert the philosopher isnt holding bottles  
	ASSERT_EQ(p0->holded_bottle.size(), 0);

	// Starts single threading
	thread t0 (ThirstyDrinkAux, p0);
	t0.join();

	// Assert only p0 drinkend so far
	ASSERT_EQ(table->drink_session.size(), 1);
	ASSERT_EQ(table->drink_session[0], p0->philosopher_id);

	// After get the bottles, they're released in drinking method
	ASSERT_EQ(p0->holded_bottle.size(), 0);
}


/*
*	Tests the request_handler method in multithread
*/
TEST_F(TableTest, RequestHandlerMultiThreadTest) {
	// Assert that philosophers arent holding bottles  
	ASSERT_EQ(p0->holded_bottle.size(), 0);
	ASSERT_EQ(p1->holded_bottle.size(), 0);

	// Starts the multi threading with p0 and p1
	thread t0 (ThirstyDrinkAux, p0);
	thread t1 (ThirstyDrinkAux, p1);
	t0.join(); t1.join(); 

	// Assert only p0 and p1 drinkend so far
	ASSERT_EQ(table->drink_session.size(), 2);
	ASSERT_EQ(table->drink_session[0], p0->philosopher_id);
	ASSERT_EQ(table->drink_session[1], p1->philosopher_id);
	// After get the bottles, they're released in drinking method
	ASSERT_EQ(p0->holded_bottle.size(), 0);
	ASSERT_EQ(p1->holded_bottle.size(), 0);

	// Starts the multi threading with all philosophers
	thread t10 (ThirstyDrinkAux, p0);
	thread t11 (ThirstyDrinkAux, p1);
	thread t12 (ThirstyDrinkAux, p2);
	t10.join(); t11.join(); t12.join();
	// p0 and p1 have drinked before, so the first to drink now is p2
	// when p2 drinks a drinking session in complete and drink_session is cleared
	// this way drink_session now should record p0 and p1 id's like before
	ASSERT_EQ(table->drink_session.size(), 2);

	// Assert the 2 itens in drink session are p0 and p1 no matter the order
	vector<int>::iterator position;
	position = find(table->drink_session.begin(), table->drink_session.end(), p0->philosopher_id);
	ASSERT_NE(position, table->drink_session.end());
	position = find(table->drink_session.begin(), table->drink_session.end(), p1->philosopher_id);
	ASSERT_NE(position, table->drink_session.end());
  
}