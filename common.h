#ifndef COMMON_H
#define COMMON_H


#include <assert.h>  // IWYU pragma: export


#define unwrap(c) assert(!(c) && "discarded result indicate error")


#endif
