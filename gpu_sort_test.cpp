#include <iostream>
#include <gtest/gtest.h>
#include "gpuMergesort.h"
#define ARRAY_SIZE 100
TEST(AddTest, GPU_Test){

	double rightsorted[ARRAY_SIZE];
	for(int i = 0; i < ARRAY_SIZE; i++) {
		rightsorted[i] = i;
	}
	double unsorted[ARRAY_SIZE];
	for(int i = ARRAY_SIZE - 1; i >= 0; i--) {
		unsorted[i] = ARRAY_SIZE - 1 - i;
	}
	for(int index = 0; index < ARRAY_SIZE; index++) {
		printf("%f\n",unsorted[index]);
	}
	printf("===================================\n");
	double testsorted[ARRAY_SIZE];
	mergesort_gpu(unsorted, ARRAY_SIZE);
	
	for(int index = 0; index < ARRAY_SIZE; index++) {
		printf("%f\n",unsorted[index]);
	}
	
	for(int index = 0; index < ARRAY_SIZE; index++) {
		ASSERT_EQ(rightsorted[index], unsorted[index]);
	}
}

int main(int argc, char *argv[]) {
	::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
