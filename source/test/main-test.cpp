#include "gtest/gtest.h"
#include "philosopher-test.cpp"
#include "table-test.cpp"

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}