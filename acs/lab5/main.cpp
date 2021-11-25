#include "../include/multithreaded_computing.h"
#include "../include/library_policies/posix_threads_policy.h"
#include "../include/library_policies/openmp_policy.h"
#include <cstdio>
#include <iostream>
#include <string>

using mc_pthreads = multithreaded_computing<posix_threads_policy>;
using mc_openmp = multithreaded_computing<openmp_policy>;

int main(int argc, char *argv[])
{
	char *end_line;
	const auto n {strtoull(argv[1], &end_line, 10)};
	const auto nthreads {strtoull(argv[2], &end_line, 10)};
	std::vector<double> a(n * n), b(n * n), c(n * n);
	mc_pthreads mc_pt {nthreads, n};
	mc_openmp mc_op {nthreads, n};
	auto gp {popen("gnuplot -p", "w")};
	if (!gp) {
		std::cerr << "Could not open gnuplot\n";
		return -1;
	}
	fprintf(gp, "set boxwidth 0.5\n");
	fprintf(gp, "set style fill solid\n");
	fprintf(gp, "plot '-' using 1:3:xtic(2) with boxes notitle\n");
	std::string command(
		std::string("0\tpthreads\t") + std::to_string(18.3 / mc_pt.compute(a, b, c).count()) + '\n' +
		std::string("1\topenmp\t") + std::to_string(18.3 / mc_op.compute(a, b, c).count()) + '\n'
	);
	fprintf(gp, command.c_str());
	pclose(gp);
	return 0;
}
