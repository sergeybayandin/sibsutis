#ifndef __POSIX_THREADS_POLICY_H__
#define __POSIX_THREADS_POLICY_H__

#include <vector>
#include <chrono>

struct posix_threads_policy final
{
	static std::chrono::duration<double>
	compute(const std::vector<double> &, const std::vector<double> &, 
		std::vector<double> &, std::size_t, std::size_t);	
private:
	struct __worker_args {
		using iterator = std::vector<double>::iterator;
		using const_iterator = std::vector<double>::const_iterator;
		const_iterator b_first;
		const_iterator a_first;
		iterator 			 c_first;
		std::size_t		 n;
		std::size_t		 ndiv;
		__worker_args(std::size_t n, std::size_t ndiv,
			auto b_first) : n {n}, ndiv {ndiv}, b_first {b_first}
		{}
	};
	static void *__worker(void *);
};

#endif // __POSIX_THREADS_POLICY_H__
