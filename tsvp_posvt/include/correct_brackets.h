#ifndef __CORRECT_BRACKETS_H__
#define __CORRECT_BRACKETS_H__

#include <vector>

class correct_brackets
{
public:
	correct_brackets(std::size_t, const std::vector<std::size_t> &);
private:
	std::size_t lhs_;
	correct_brackets rhs_;
};

#endif // __CORRECT_BRACKETS_H__
