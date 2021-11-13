#ifndef __MULTITHREADED_COMPUTING_H__
#define __MULTITHREADED_COMPUTING_H__

#include <vector>
#include <chrono>

template <class LibraryPolicy>
class multithreaded_computing final
{
public:
	multithreaded_computing(std::size_t threads_count, std::size_t n)
		:	threads_count_ {threads_count}, n_ {n}
	{}
	std::size_t threads_count() const noexcept { return threads_count_; }
	std::size_t n() const noexcept { return n_; }
	std::chrono::duration<double>
	compute(const std::vector<double> &, 
		const std::vector<double> &, std::vector<double> &) const;

private:
	std::size_t threads_count_;
	std::size_t n_;
};

template<class LibraryPolicy>
std::chrono::duration<double>
multithreaded_computing<LibraryPolicy>::compute(const std::vector<double> &a, 
	const std::vector<double> &b, std::vector<double> &c) const
{
	return LibraryPolicy::compute(a, b, c, n(), threads_count());
}

#endif // __MULTITHREADED_COMPUTING_H__
