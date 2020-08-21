#include <sys/mman.h>
#include <unistd.h>
#include<stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdbool.h>

//valores para cola circular
#define true 1
#define false 0
#define max 100

//Variable donde se aloja el numero de procesos.
int num_procesos;
//Variable donde se aloja el valor Q(segundos) para RoundRobin.
int Q = -1;
//estado para el waitpid()
int estado;
//variables para cola circular

int quantum;
int m;
int vector[max];

//vector para las prioridades
int vector_prioridad[max];

//estructura de cola circular
typedef struct{

	int inicio;
	int final;
	int vector[max];
} queue;

//condiciones de inicio para cola circular
void Iniciar(queue *Cola){

	Cola->inicio = -1;
	Cola->final = -1;
}

//consulta para saber si la Cola está vacía
int IsEmpty(queue *Cola){

	if(Cola->final == -1){
		return true;
	}
	return false;
}

//cant de elementos en la Cola
int Elementos(queue *Cola){

	if(IsEmpty(Cola)){
		return 0;
	} else {
		if(Cola->inicio <= Cola->final){
			return Cola->final - Cola->inicio +1; //cantidad de elementos totales
		} else {
			return max - Cola->inicio + Cola->final + 1;  //condicion que diferencia una cola circular de una cola
		}
	}
}

//consulta para saber si la Cola está llena
int IsFull(queue *Cola){
	if(Elementos(Cola) == max){
		return true;
	}
	return false;
}

//función para agregar en cola circular
void agregar(queue *Cola, int A){
	if(!IsFull(Cola)){
		if(!IsEmpty(Cola)){
			Cola->inicio = 0;
		}

		if(Cola->final == max - 1){
			Cola->final =0;
		} else {
			Cola->final++;
		}

	Cola->vector[Cola->final] = A;

		printf("Proceso(%d) agregado en la cola.(%i)\n",Cola->vector[Cola->final],Cola->final+1);
	} else {
		printf("COLA LLENA!\n");
	}
}

//función para agregar silenciosamente
void agregarsilencioso(queue *Cola, int A){
	if(!IsFull(Cola)){
		if(!IsEmpty(Cola)){
			Cola->inicio = 0;
		}

		if(Cola->final == max - 1){
			Cola->final =0;
		} else {
			Cola->final++;
		}
	Cola->vector[Cola->final] = A;
	} else {

	}
}

//función para eliminar en cola circular
void borrar(queue *Cola){

	if(!IsEmpty(Cola)){
		int tmp = Cola->vector[Cola->inicio];
		if(Elementos(Cola) == 1){
			Iniciar(Cola);
		} else {
			Cola->inicio++;
		}
		printf("[PID BORRADO(%d)]\n",tmp);
		fflush(stdin);
	} else {
		printf("Esta vacia\n");
	}

}

//función borrar de cola silenciosa
void borrarsilencioso(queue *Cola){

	if(!IsEmpty(Cola)){
		int tmp = Cola->vector[Cola->inicio];
		if(Elementos(Cola) == 1){
			Iniciar(Cola);
		} else {
			Cola->inicio++;
		}

	} else {

	}
}

//función mostrar usada para debuggear el código
void mostrar(queue *Cola){

		int tmp2 = Cola->final;
    int tmp3 = Cola->inicio;
		printf("Los elementos en la Cola circular son(%d) ->",tmp2-tmp3+1);
		for(int tmp1 = Cola->inicio; tmp1 <= tmp2; tmp1++){
			printf("(%d)",Cola->vector[tmp1]);
		}
		printf("\n");
}


void borrar_especifico(queue *Cola, int posicionmayor, int num_pararestar){

	if(num_pararestar == 1){
		int tmp = Cola->vector[posicionmayor];
		if(Elementos(Cola) == 1){
			printf("[PID BORRADO(%d)]\n",tmp);
			exit(0);
		}
	}

	if(!IsEmpty(Cola)){
		int tmp = Cola->vector[posicionmayor];

		if(Elementos(Cola) == 0){

			exit(0);
		}

		//si borro el del inicio
		if(Cola->vector[posicionmayor] == Cola->vector[Cola->inicio]){
			Cola->inicio++;
		}

		else {
			int tmp2 = Cola->final;
			int tmp3 = Cola->inicio;
			int quitavariables[num_pararestar];

			//guarda todos los pids en un arreglo menos el que está en Cola->vector[tmp1]
			for(int tmp1 = Cola->inicio; tmp1 <= tmp2; tmp1++){

				if(Cola->vector[tmp1] != Cola->vector[posicionmayor]){

					quitavariables[tmp1] = Cola->vector[tmp1];

				} else {
					quitavariables[tmp1] = -1;
				}
			}

			//borra toda la cola
			for(int tmp4 = 0 ; tmp4 <= tmp2 ; tmp4++){
				borrarsilencioso(Cola);
			}

			for(int tmp5 = 0 ; tmp5 <= num_pararestar ; tmp5++){

				if(quitavariables[tmp5] != -1){
					agregarsilencioso(Cola, quitavariables[tmp5]);

				}
			}
		}

		printf("[PID BORRADO(%d)]\n",tmp);
		fflush(stdin);
	} else {
		printf("Esta vacia\n");
	}
}

//Bandera utilizada en RR
volatile sig_atomic_t print_flag = false;
void handle_alarm( int sig ) {
		//desactiva proceso hijo
		printf("Turno en CPU completado.(◔ ʖ̯ ◔)\n");
		print_flag = true;

}

//FUNCION FIFO
int fifo(int num_procesos,queue *Cola){
	printf("\nSe procederá a usar el algoritmo FIFO con %d procesos.\n", num_procesos);

	int *status;
	int tmp3 = Cola->inicio;
	int tmp2 = Cola->final;
	fflush(stdin);
	sleep(3);

	do{

		//activa proceso hijo
		kill(Cola->vector[tmp3],SIGCONT);
		printf("Se ha activado el proceso(%d)\n",Cola->vector[tmp3]);

		wait(status);
		printf("Se ha dormido el proceso(%d)\n",Cola->vector[tmp3]);

		borrar(Cola);

		//itera dentro de Cola circular
		printf("\n");
		printf("\n");
		if(tmp3 < tmp2){
			tmp3 ++;
		} else {
			tmp3 = Cola->inicio;
		}
	}while(!IsEmpty(Cola));
	exit(0);

}

//FUNCION RR
int round_robin(int num_procesos, int Q,queue *Cola){

	printf("\nSe procederá a usar el algoritmo Round Robin con %d procesos y con Q = %d.\n", num_procesos, Q);
	signal(SIGALRM,handle_alarm);
	int *status;
  //variable que se pasara a la alarma
  int tiempo_alarma = Q;

	int tmp3 = Cola->inicio;
	int tmp2 = Cola->final;
	fflush(stdin);
	sleep(3);

	do{

		//activa proceso hijo
		kill(Cola->vector[tmp3],SIGCONT);
		printf("Se ha activado el proceso(%d)\n",Cola->vector[tmp3]);

		alarm(tiempo_alarma);
		sleep(tiempo_alarma);

		//cuando suene la alarma detiene el proceso loop
		if(print_flag){

			print_flag = false;
			kill(Cola->vector[tmp3],SIGSTOP);
			printf("Se ha dormido el proceso(%d)\n",Cola->vector[tmp3]);
			if( waitpid(Cola->vector[tmp3], status, WNOHANG) == -1){

				borrar(Cola);
			}
		}
		//itera dentro de Cola circular
		printf("\n");
		printf("\n");
		if(tmp3 < tmp2){
			tmp3 ++;
		} else {
			tmp3 = Cola->inicio;
		}
	}while(!IsEmpty(Cola));

}

//FUNCION PRIORIDADES
int prioridades(int num_procesos, queue *Cola){

	printf("\nSe procederá a usar el algoritmo Prioridades con %d procesos y prioridades de ", num_procesos);

	for(int V=0 ; V< num_procesos; V++){
		printf("[%d]",vector_prioridad[V]);
	}

	int *status;
	int tmp3 = Cola->inicio;
	int tmp2 = Cola->final;
	fflush(stdin);
	int num_pararestar = num_procesos;
	sleep(3);

	do{
		printf("\nVector de prioridades: ");
		for(int V=0 ; V< num_procesos; V++){
			printf("[%d]",vector_prioridad[V]);
		}
		//mostrar(Cola);
		printf("\nLa prioridad mayor actual es de: ");
		int vectormayor = 0;
		int posicionmayor = 0;
		//busca la posicion de la prioridad más alta
		for (int dv = 0; dv < num_procesos; dv++) {
			if (vectormayor < vector_prioridad[dv]){
				vectormayor = vector_prioridad[dv];
				posicionmayor = dv;
			}
		}

		//activa proceso hijo
		kill(Cola->vector[posicionmayor],SIGCONT);
		printf("Se ha activado el proceso(%d)\n",Cola->vector[posicionmayor]);

		wait(status);
		printf("Se ha dormido el proceso(%d)\n",Cola->vector[posicionmayor]);

		num_pararestar--;
		borrar_especifico(Cola, posicionmayor,num_pararestar);
		sleep(1);
		//itera dentro de Cola circular
		printf("\n");
		printf("\n");
		if(tmp3 < tmp2){
			tmp3 ++;
		} else {
			tmp3 = Cola->inicio;
		}
		vector_prioridad[posicionmayor] = -1;
	}while(num_pararestar > 0);

	exit(0);
}



// -argc "contador de argumentos"
// -argv "vector de argumentos"
int main(int argc, char *argv[]){

//Pasa a int el argumento que será el número de procesos a ejecutar.
	char *a = argv[2];
	num_procesos = atoi(a);

//Comprueba el número de procesos sea mayor a 1
	if (num_procesos < 0){
		printf("No puede ejecutarse el programa con el numero de procesos ingresados.\n");
		exit(-1);
	}

	//variables
	char* text[] = {"2", "1", NULL};
	int XX = num_procesos;

	//inicio de Cola circular
	queue Cola;
	Iniciar(&Cola);

	pid_t pid;

	// Ciclo para crear N hijos de un mismo padre
  for(int i=0;i<num_procesos;i++){

			pid = fork();
			int pid_id_hijo = getpid();

	    if(pid == 0){//-----------------------------------------------------------------------------HIJO

				//proceso hijo que solo va a tener lo que quiero que ejecute (loop)
	      while(1){
					execvp("./loop", text);
	        usleep(1);

	      }
	    } else{//--------------------------------------------------------------------------------PADRE
					//para que el proceso hijo se pause
					kill(pid, SIGSTOP);
					agregar(&Cola, pid);
					sleep(1);
					printf("Padre ha pausado el Proceso(%d)\n",pid);
					sleep(1);
	    }
}

	//FIFO
	if(strcmp(argv[1],"-F") == 0){

		fifo(num_procesos, &Cola);

	}

	//RR
	if(strcmp(argv[1],"-R") == 0){

		if(strcmp(argv[3], "-Q") == 0){
			char *b = argv[4];
			Q = atoi(b);
			//TERMINA EL PROGRAMA SI EL VALOR DE Q ES INFERIOR A 1.
			if (Q < 1){
				printf("No puede ejecutarse el programa con el valor de Q ingresado.\n");
				exit(-1);
			}
			else{
				round_robin(num_procesos, Q,&Cola);
			}
		}

	}

	//Prioridades no Expulsivo
	if(strcmp(argv[1],"-P") == 0){
		//guardo las prioridades en vector_prioridad[max]
		if(strcmp(argv[3], "-p") == 0){
			int W = 4;
			int R2D2;

			//ciclo que pasa a integer los caracteres pasados por consola de orden de prioridad
			for(int K3=0;K3<num_procesos;K3++){

				char *KK = argv[W];
				R2D2 = atoi(KK);
				vector_prioridad[K3] = R2D2;
				W++;

			}
		}
		prioridades(num_procesos, &Cola);
	}

	exit(0);
}
