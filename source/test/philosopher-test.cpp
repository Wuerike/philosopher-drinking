#include "drinkingtest.h"

/*
*	The fixture for testing class Philosopher
*/
class PhilosopherTest : public ::testing::Test {
protected:
	Table* table;
	Philosopher* p0;
	Philosopher* p1;
	Philosopher* p2;
	int n_sessions = 6;
	int num_philosofers = 3;
	vector<Philosopher *> neighbors;

	PhilosopherTest() {
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

  	~PhilosopherTest() override {
  		delete table;
  		delete p0;
  		delete p1;
  		delete p2;
	}
};

/*
*	Tests the Philosopher constructor for 4 instances
*/
TEST_F(PhilosopherTest, ConstructorTest) {
	// Assert the philosopher 0 atributtes when instantiated
	ASSERT_EQ(p0->philosopher_id, 0);
	ASSERT_EQ(p0->table.session_size, 3);
	ASSERT_EQ(p0->n_sessions, 6);
	ASSERT_EQ(p0->num_bottle_max, 2);
	ASSERT_EQ(p0->total_thirsty_time, 0);
	ASSERT_STREQ(p0->state.c_str(), "tranquil");

	// Assert the philosopher 1 atributtes when instantiated
	ASSERT_EQ(p1->philosopher_id, 1);
	ASSERT_EQ(p1->table.session_size, 3);
	ASSERT_EQ(p1->n_sessions, 6);
	ASSERT_EQ(p1->num_bottle_max, 2);
	ASSERT_EQ(p1->total_thirsty_time, 0);
	ASSERT_STREQ(p1->state.c_str(), "tranquil");

	// Assert the philosopher 2 atributtes when instantiated
	ASSERT_EQ(p2->philosopher_id, 2);
	ASSERT_EQ(p2->table.session_size, 3);
	ASSERT_EQ(p2->n_sessions, 6);
	ASSERT_EQ(p2->num_bottle_max, 2);
	ASSERT_EQ(p2->total_thirsty_time, 0);
	ASSERT_STREQ(p2->state.c_str(), "tranquil");

	// Build the 4th instance and test it passing a pointer of Table without instantiate it
	Philosopher* p3;
	Table* table3;
	p3 = new Philosopher(3, *table3, 6);
	ASSERT_EQ(p3->philosopher_id, 3);
	// Should be 0 because table3 is a pointer to the class not to an object of a class
	ASSERT_EQ(p3->table.session_size, 0);
	ASSERT_EQ(p3->n_sessions, 6);
	ASSERT_EQ(p3->num_bottle_max, 0);
	ASSERT_EQ(p3->total_thirsty_time, 0);
	ASSERT_STREQ(p3->state.c_str(), "tranquil");
}

/*
*	Tests the set_neighbors method
*/
TEST_F(PhilosopherTest, SetNeighborsTest) {
	// create a empty vector of neighbors
	vector<Philosopher *> neighbors;
	// Sets zero neighbors to p0
	p0->set_neighbors(neighbors);
	// Check if p0 has a bottle for each neighbor
	ASSERT_EQ(p0->num_bottle_max, 0);

	// Add p1 and p2 as p0 neighbors
	neighbors.push_back(p1);
	neighbors.push_back(p2);
	p0->set_neighbors(neighbors);
	// Check if p0 has a bottle for each neighbor
	ASSERT_EQ(p0->num_bottle_max, neighbors.size());
	// Check if p0 has access to its neighbors
	ASSERT_EQ(p0->neighbors[0]->philosopher_id, p1->philosopher_id);
	ASSERT_EQ(p0->neighbors[1]->philosopher_id, p2->philosopher_id);
}

/*
*	Tests the send_request method
*/
TEST_F(PhilosopherTest, SendRequestTest) {
	int num_bottle_needed = 2;
	bool request;

	// Test p0 requesting bottles of both neighbors when anyone is holding a bottle
	// Clear the drinking session
	p0->table.drink_session.clear();
	request = p0->send_request(p0->neighbors, num_bottle_needed);
	ASSERT_TRUE(request);

	// Test p0 requesting bottles of both neighbors when it already holds bottle from p1
	p0->holded_bottle.clear();
	p0->holded_bottle.push_back(p1->philosopher_id);
	// Clear the drinking session
	p0->table.drink_session.clear();
	request = p0->send_request(p0->neighbors, num_bottle_needed);
	ASSERT_TRUE(request);

	// Test p0 requesting bottles of both neighbors when it already holds both bottles
	p0->holded_bottle.clear();
	p0->holded_bottle.push_back(p1->philosopher_id);
	p0->holded_bottle.push_back(p2->philosopher_id);
	// Clear the drinking session
	p0->table.drink_session.clear();
	request = p0->send_request(p0->neighbors, num_bottle_needed);
	ASSERT_TRUE(request);

	// Test p0 requesting bottles of both neighbors when p1 holds the bottle shared with p0
	p0->holded_bottle.clear();
	p1->holded_bottle.push_back(p0->philosopher_id);
	// Clear the drinking session
	p0->table.drink_session.clear();
	request = p0->send_request(p0->neighbors, num_bottle_needed);
	ASSERT_FALSE(request);

	// Test p0 requesting bottles of both neighbors when p1 and p2 hold the bottle shared with p0
	p0->holded_bottle.clear();
	p1->holded_bottle.clear();
	p2->holded_bottle.clear();
	p1->holded_bottle.push_back(p0->philosopher_id);
	p2->holded_bottle.push_back(p0->philosopher_id);
	// Clear the drinking session
	p0->table.drink_session.clear();
	request = p0->send_request(p0->neighbors, num_bottle_needed);
	ASSERT_FALSE(request);
}

/*
*	Tests the send_bottle method
*/
TEST_F(PhilosopherTest, SendBottleTest) {
	
	// Assert all philosophers have no bottle
	ASSERT_EQ(p0->holded_bottle.size(), 0);
	ASSERT_EQ(p1->holded_bottle.size(), 0);
	ASSERT_EQ(p2->holded_bottle.size(), 0);

	// p1 shares the bottle with p0
	p1->send_bottle(p0->philosopher_id);
	// p2 shares the bottle with p1
	p2->send_bottle(p1->philosopher_id);
	// p0 shares the bottle with p2
	p0->send_bottle(p2->philosopher_id);
	// Assert each philosopher has one bottle
	ASSERT_EQ(p0->holded_bottle.size(), 1);
	ASSERT_EQ(p1->holded_bottle.size(), 1);
	ASSERT_EQ(p2->holded_bottle.size(), 1);
	// Assert if the bottle each phiplosopher is the right shared bottle
	ASSERT_EQ(p0->holded_bottle[0], p1->philosopher_id);
	ASSERT_EQ(p1->holded_bottle[0], p2->philosopher_id);
	ASSERT_EQ(p2->holded_bottle[0], p0->philosopher_id);

	// Clear the bottles p2 has
	p2->holded_bottle.clear();
	// p2 shares the bottle with p0
	p2->send_bottle(p0->philosopher_id);
	// Assert p0 has now 2 bottles
	ASSERT_EQ(p0->holded_bottle.size(), 2);
	// Assert p0 has its neighbors bottles in the right order
	ASSERT_EQ(p0->holded_bottle[0], p1->philosopher_id);
	ASSERT_EQ(p0->holded_bottle[1], p2->philosopher_id);
}

/*
*	Tests the tranquil method
*/
TEST_F(PhilosopherTest, TranquilTest) {
	// Change philosophers state for any state
	p0->state = ("any state");
	p1->state = ("any state");
	p2->state = ("any state");

	// Set philosophers to tranquil, this call will print data on console
	p0->tranquil();
	p1->tranquil();
	p2->tranquil();

	// Assert the tranquil state
	ASSERT_STREQ(p0->state.c_str(), "tranquil");
	ASSERT_STREQ(p1->state.c_str(), "tranquil");
	ASSERT_STREQ(p2->state.c_str(), "tranquil");
}

/*
*	Tests the thirsty method
*/
TEST_F(PhilosopherTest, ThirstyTest) {
	// Change philosophers state to any state
	p0->state = ("any state");
	ASSERT_EQ(p0->holded_bottle.size(), 0);

	p0->thirsty();
	
	ASSERT_STREQ(p0->state.c_str(), "thirsty");
	ASSERT_EQ(p0->holded_bottle[0], p1->philosopher_id);
	ASSERT_EQ(p0->holded_bottle[1], p2->philosopher_id);
}

/*
*	Tests the drinking method
*/
TEST_F(PhilosopherTest, DrinkingTest) {
	// p1 and p2 share its bottles with p0
	p1->send_bottle(p0->philosopher_id);
	p2->send_bottle(p0->philosopher_id);
	// Assures the philosopher has 2 bottles
	ASSERT_EQ(p0->holded_bottle.size(), 2);

	// Change p0 state to any state and drink
	p0->state = ("any state");
	p0->drinking();

	// Assert the changed state
	ASSERT_STREQ(p0->state.c_str(), "drinking");
	// Assures the philosopher reliase the bottles after drinking
	ASSERT_EQ(p0->holded_bottle.size(), 0);
}