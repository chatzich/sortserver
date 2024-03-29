#include <iostream>
#include <gtest/gtest.h>
#include "cpuMergesort.h"

TEST(AddTest, CPU_Test){
	double rightsorted[10] =  {2,2,2,5,5,6,6,7,10,34};
	double unsorted[10] = {10,2,5,6,2,34,2,5,6,7};
	double testsorted[10];
	mergesort(unsorted, testsorted, 10);
	
	for(int index = 0; index < 10; index++) {
		ASSERT_EQ(rightsorted[index], testsorted[index]);
	}
}

int main(int argc, char *argv[]) {
	::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
