STD=-std=c++17
INCLUDE_PATH_ITBB=/opt/intel/oneapi/tbb/2021.4.0/include
LIB_PATH_ITBB=/opt/intel/oneapi/tbb/2021.4.0/lib/intel64/gcc4.8
SRC_PATH=../src/library_policies

all:
	g++ -Ofast -fconcepts-ts  $(STD) -o prog main.cpp $(SRC_PATH)/posix_threads_policy.cpp $(SRC_PATH)/intel_tbb_policy.cpp $(SRC_PATH)/openmp_policy.cpp -I $(INCLUDE_PATH_ITBB) -L $(LIB_PATH_ITBB) -ltbb -fopenmp -pthread

