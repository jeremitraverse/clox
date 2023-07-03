#include <stdio.h>

#include "common.h"
#include "vm.h"
#include "chunk.h"
#include "debug.h"
#include "compiler.h"

VM vm;

static void resetStack() {
    vm.stackTop = vm.stack;
}

void initVM() {
    resetStack();
}

void freeVM() {

}

void pushToStack(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value popFromStack() {
    vm.stackTop--;
    return *vm.stackTop;
}

static InterpretResult run() {
    #define READ_BYTE() (*vm.ip++)
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
    #define BINARY_OP(op) \
        do { \ 
            double b = popFromStack(); \
            double a = popFromStack(); \
            pushToStack(a op b); \
        } while(false)

    for(;;) {
        #ifdef DEBUG_TRACE_EXECUTION
            printf("               ");
            for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
                printf("[");
                printValue(*slot);
                printf("] ");
            }
            printf("\n");
            disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
        #endif

        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                pushToStack(constant);
                printf("\n");
                break;
            }
            case OP_ADD:        BINARY_OP(+); break;
            case OP_SUBSTRACT:  BINARY_OP(-); break;
            case OP_MULTIPLY:   BINARY_OP(*); break;
            case OP_DIVIDE:     BINARY_OP(/); break;
            case OP_NEGATE:     pushToStack(-popFromStack()); break;
            case OP_RETURN: {
                printValue(popFromStack());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

    #undef READ_BYTE
    #undef READ_CONSTANT
    #undef BINARY_OP
}

InterpretResult interpret(const char* source) {
    compile(source);
    return INTERPRET_OK; 
}