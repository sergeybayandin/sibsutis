#include "../../include/library_policies/posix_threads_policy.h"
#include <pthread.h>
#include <stdexcept>

void *posix_threads_policy::__worker(void *args)
{
	auto worker_args {static_cast<__worker_args *>(args)};
	auto [b_first, a_first, c_first, n, ndiv] {*worker_args};
  auto a_last {std::next(a_first, ndiv * n)};
  decltype(a_first) row_first, row_last;
  decltype(b_first) col_first, col_last {std::next(b_first, n)}, col_it;
  for (; a_first < a_last; a_first = row_last) {
    row_last = std::next(a_first, n);
    for (col_first = b_first; col_first < col_last; ++col_first) {
      row_first = a_first;
      col_it = col_first;
      for (*c_first = {}; row_first < row_last; ++row_first, std::advance(col_it, n))
        *c_first += *row_first * *col_it;
      ++c_first;
    }
  }
	return nullptr;
}

std::chrono::duration<double>
posix_threads_policy::compute(const std::vector<double> &a, const std::vector<double> &b,
	std::vector<double> &c, std::size_t n, std::size_t threads_count)
{
  auto                   		 c_first {std::begin(c)};
  auto                   		 a_first {std::cbegin(a)};
  const auto             		 ndiv {n / threads_count};
  const auto             		 offset {ndiv * n};
  std::vector<pthread_t> 		 threads(threads_count);
  std::vector<__worker_args> args(threads_count, __worker_args {n, ndiv, std::cbegin(b)});
  auto                   		 args_first {std::begin(args)};
  auto                   		 threads_first {std::begin(threads)};
  auto                   		 threads_last {std::cend(threads)};
	auto									 		 start {std::chrono::steady_clock::now()};
  while (threads_first < threads_last) {
    args_first->a_first = a_first;
    args_first->c_first = c_first;
    if (pthread_create(&(*threads_first), nullptr, __worker, &(*args_first)))
      throw std::runtime_error {"pthread_create() failed"};
    ++args_first;
    ++threads_first;
    std::advance(a_first, offset);
    std::advance(c_first, offset);
  }
  for (auto tid : threads)
    pthread_join(tid, nullptr);
	auto finish {std::chrono::steady_clock::now()};
	return finish - start;
}
