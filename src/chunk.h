#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

// Each instruction has a one-byte operation code (OpCode for short)
typedef enum {
    OP_CONSTANT,
    OP_RETURN
} OpCode;

/*
    Dynamic Array impl
    If we have to add an element into the Chunk, but the capacity = count
    1. Allocate a new array with more capacity (usually double)
    2. Copy the existing elements form the old array to the new one
    3. Store the new capacity
    4. Delete the old array
    5. Update *code* to point to the new array
    6. Store the new element into the new array
    7. Update the count
*/
typedef struct {
    int count; // Number of active element in the array
    int capacity; // Maximum amount of element that can be stored in the array
    uint8_t* code; // Pointer to the array that contains the bytecode
    int* lines; // Pointer to an array where we store the line numbers 
    ValueArray constants; // Array that contains all of the chunk's constants
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int lineNumber);
int addConstant(Chunk* chunk, Value value);
void freeChunk(Chunk* chunk);

#endif