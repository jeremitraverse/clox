#include <stdlib.h>
#include <stdio.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
    chunk->capacity = 0;
    chunk->count = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }

    void* result = realloc(pointer, newSize);

    // result is null when there's not enough free memory to allocate 
    if (result == NULL) exit(1);

    return result;
}

void writeChunk(Chunk* chunk, uint8_t byte, int lineNumber) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, 
            oldCapacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, 
            oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = lineNumber;
    chunk->count++;
}

// Add a constant to the constant list of a chunk
// Returns the index of the newly added constant
int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    // - 1 since writeValueArray increments the count 
    return chunk->constants.count - 1;
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}