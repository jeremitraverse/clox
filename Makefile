CC_FLAGS    := -Wall
SOURCES     := $(wildcard src/*.c)
OUTPUT      := -o clox 

default:
	gcc $(CC_FLAGS) $(OUTPUT) $(SOURCES) 

bnr:
	make && ./clox