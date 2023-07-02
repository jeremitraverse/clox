#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"

// Grow the capacity of the array by a factor of 2
#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

// Reallocate the array with a new size
#define GROW_ARRAY(type, pointer, oldCapacity, newCapacity) \
    (type*)reallocate(pointer, sizeof(type) * (oldCapacity), \
        sizeof(type) * (newCapacity))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * oldCount, 0)

void* reallocate(void* pointer, size_t oldCapacity, size_t newCapacity);

#endif