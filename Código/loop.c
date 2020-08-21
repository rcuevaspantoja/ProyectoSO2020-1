#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include<stdio.h>

int main(int argc, char** argv) {
	//printf("Bienvenidos  loop.c\n");
	int hasta;
	int i;
	srand(time(NULL));
	hasta = (rand() % 10) + 1;
	printf("Loop(%d)\n",hasta);
	fflush(stdin);
	for(i = 0 ; i <= hasta ; i++) {
		sleep(1);
	}
	printf("--------------------------Proceso(%d) terminado en %d segundos.\n",getpid(),i-1);
	fflush(stdin);
	return 0;
}
