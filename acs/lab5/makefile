STD=-std=c++17
SRC_PATH=../src/library_policies

all:
	g++ -fconcepts-ts  $(STD) -o prog main.cpp $(SRC_PATH)/posix_threads_policy.cpp $(SRC_PATH)/openmp_policy.cpp -fopenmp -pthread

