#ifndef __MULTIPLICATION_H__
#define __MULTIPLICATION_H__

#include <stddef.h>
#include <stdint.h>

struct operations_number
{
	size_t additions, multiplications;
};

struct operations_number stupid_column_multiplication(const unsigned*, const unsigned*, unsigned*, size_t, size_t, size_t);
struct operations_number fast_multiplication(const unsigned*, const unsigned*, unsigned*, size_t, size_t);

#endif // __MULTIPLICATION_H__
