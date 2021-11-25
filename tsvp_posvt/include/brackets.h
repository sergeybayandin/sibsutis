#ifndef __BRACKETS_H__
#define __BRACKETS_H__

#include <vector>
#include <ostream>

class __brackets_observer;

std::ostream &operator<<(std::ostream &, const __brackets_observer &);

class __brackets_observer
{
	using value_type = std::pair<std::size_t, std::size_t>;
  using storage_type = std::vector<value_type>;
  using complexity_type = std::size_t;

public:
  __brackets_observer() = delete;
  __brackets_observer(complexity_type complexity, storage_type &&storage)
    : complexity_ {complexity}, storage_(std::move(storage))
  {}
  complexity_type complexity() const noexcept { return complexity_; }
	friend std::ostream &operator<<(std::ostream &, const __brackets_observer &);

private:
  const complexity_type complexity_;
  const storage_type storage_;
	
	void __show_brackets_recursive(std::string &, auto, auto) const noexcept;
	auto __find_optimal(auto, auto, std::size_t, std::size_t) const noexcept;
};

__brackets_observer
brackets_problem(const std::vector<std::size_t> &);

#endif // __BRACKETS_H__
