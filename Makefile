default:
	gcc -Wall -o clox main.c .\chunk.c .\debug.c .\value.c .\vm.c && .\clox
