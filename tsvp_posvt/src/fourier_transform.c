#define _USE_MATH_DEFINES

#include "../../include/tsvp_posvt/fourier_transform.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

size_t cdft(const carray_t fs, carray_t as, size_t num)
{
	size_t n = 0;
  const complex_t _2pii_n = -2.0 * M_PI / num * I;
  for (size_t i = 0; i < num; ++i) {
    as[i] = 0;
    for (size_t j = 0; j < num; ++j, ++n)
      as[i] += cexp(_2pii_n * i * j) * fs[j];
    as[i] /= num;
  }
	return n;
}

size_t cidft(const carray_t as, carray_t fs, size_t num)
{
  size_t n = 0;
  const complex_t _2pii_n = 2.0 * M_PI / num * I;
  for (size_t i = 0; i < num; ++i) {
    fs[i] = 0;
    for (size_t j = 0; j < num; ++j, ++n)
      fs[i] += cexp(_2pii_n * i * j) * as[j];
  }
  return n;
}

size_t csfft(const carray_t fs, carray_t as, size_t num)
{
  size_t n = 0, idx, p1, p2;
  carray_t _as = malloc(num * sizeof *_as);
  if (_as == NULL)
  	return n;
	for (p1 = sqrt(num); num % p1 != 0; --p1); p2 = num / p1;
  const complex_t _2pii_p1 = -2.0 / p1 * M_PI * I;
  for (size_t i = 0; i < p1; ++i) {
    for (size_t j = 0; j < p2; ++j) {
      idx = i + p1 * j;
      _as[idx] = 0;
      for (size_t k = 0; k < p1; ++k, ++n)
        _as[idx] += cexp(_2pii_p1 * k * i) * fs[j + p2 * k];
      _as[idx] /= p1;
    }
  }
  const complex_t _2pii_num = -2.0 / num * M_PI * I;
  for (size_t i = 0; i < p1; ++i) {
    for (size_t j = 0; j < p2; ++j) {
      idx = i + p1 * j;
      as[idx] = 0;
      for (size_t k = 0; k < p2; ++k, ++n)
        as[idx] += cexp(_2pii_num * k * idx) * _as[i + p1 * k];
      as[idx] /= p2;
    }
  }
  free(_as);
  return n;
}

size_t cisfft(const carray_t as, carray_t fs, size_t num)
{
  size_t n = 0, idx, p1, p2;
  carray_t _fs = malloc(num * sizeof *_fs);
  if (_fs == NULL)
    return n;
	for (p1 = sqrt(num); num % p1 != 0; --p1); p2 = num / p1;
  const complex_t _2pii_p1 = 2.0 / p1 * M_PI * I;
  for (size_t i = 0; i < p1; ++i) {
    for (size_t j = 0; j < p2; ++j) {
      idx = i + p1 * j;
      _fs[idx] = 0;
      for (size_t k = 0; k < p1; ++k, ++n)
        _fs[idx] += cexp(_2pii_p1 * k * i) * as[j + p2 * k];
    }
  }
  const complex_t _2pii_num = 2.0 / num * M_PI * I;
  for (size_t i = 0; i < p1; ++i) {
    for (size_t j = 0; j < p2; ++j) {
      idx = i + p1 * j;
      fs[idx] = 0;
      for (size_t k = 0; k < p2; ++k, ++n)
        fs[idx] += cexp(_2pii_num * k * idx) * _fs[i + p1 * k];
    }
  }
  free(_fs);
  return n;
}

size_t cfft(const carray_t fs, carray_t as, size_t num)
{
  size_t n = 0;
  const size_t mid = num / 2;
  carray_t ws = malloc(mid * sizeof *ws);
  if (ws == NULL)
    return n;
  const complex_t w = cexp(-2.0 / num * I * M_PI);
  ws[0] = 1;
  for (size_t i = 1; i < mid; ++i)
    ws[i] = ws[i - 1] * w;
  cfft_impl(fs, as, ws, 1, num, &n);
 	for (size_t i = 0; i < num; ++i)
  	as[i] /= num;
  free(ws);
  return n;
}

size_t cifft(const carray_t as, carray_t fs, size_t num)
{
  size_t n = 0;
  const size_t mid = num / 2;
  carray_t ws = malloc(mid * sizeof *ws);
  if (ws == NULL)
    return n;
  const complex_t w = cexp(2.0 / num * I * M_PI);
  ws[0] = 1;
  for (size_t i = 1; i < mid; ++i)
    ws[i] = ws[i - 1] * w;
  cfft_impl(as, fs, ws, 1, num, &n);
  free(ws);
  return n;
}

void cfft_impl(const carray_t fs, carray_t as, const carray_t ws, size_t h, size_t num, 
	size_t* n)
{
  if (num < 2)
    return;
  const size_t mid = num / 2;
  carray_t a0, a1;
  if ((a0 = malloc(mid * sizeof *a0)) == NULL)
    return;
	if ((a1 = malloc(mid * sizeof *a1)) == NULL) {
		free(a0);
		return;
	}
  for (size_t i = 0; i < mid; ++i, ++*n) {
    a0[i] = fs[2 * i];
    a1[i] = fs[2 * i + 1];
  }
  cfft_impl(a0, a0, ws, h * 2, mid, n);
  cfft_impl(a1, a1, ws, h * 2, mid, n);
  for (size_t i = 0, j = 0; i < mid; ++i, ++*n, j += h) {
    as[i] = a0[i] + ws[j] * a1[i];
    as[mid + i] = a0[i] - ws[j] * a1[i];
  }
  free(a0); free(a1);
}
