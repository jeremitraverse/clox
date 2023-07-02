CC_FLAGS    := -Wall
SOURCES     := $(wildcard src/*.c)

default:
	gcc $(CC_FLAGS) -o clox $(SOURCES) 
