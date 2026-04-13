#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 

int total = 51, totalTablas = 16, HandN = 7, kk;

typedef struct {
	char nombre[20];
	int costo;
	float multiplicador;
} Comodin;

void swap(int *a, int *b) {
	int c = 0;
	c = *a;
	*a = *b;
	*b = c;
}

void instrucciones(int a) {
	printf("+----------------------------------------------------------------+\n");
	printf("|                           COMO JUGAR                           |\n");
	printf("|  El objetivo del juego es obtener la mayor puntuacion posible  |\n");
	printf("|  mediante la coincidencia de cartas entre tu mano y una tabla  |\n");
	printf("|  ordenados de manera aleatoria.                                |\n");
	printf("|  En cada turno, puedrás utilizar una cantidad limitada de manos|\n");
	printf("|  para jugar, descartes para reemplazar cartas y comodines que  |\n");
	printf("|  podras adquirir en una tienda con base en los puntos 		 |\n");
	printf("|  obtenidos.                       							 |\n");
	printf("|  El juego continua mientras dispongas de recursos o cuando     |\n");
	printf("|  decidas terminar la partida.                                  |\n");
	printf("+----------------------------------------------------------------+\n");
}

/// MÓDULOS


/// JUGAR		(Edo)
	// MOD 1
	void barajear_tabla(int m[total]) {
		for (int i = total - 1; i > 0; i--) {
			int j = rand() % (i + 1);
			swap(&m[i], &m[j]);
		}
	}
	// MOD 2
	void barajear_mano(int m[total]) {
		for (int i = total - 1; i > 0; i--) {
			int j = rand() % (i + 1);
			swap(&m[i], &m[j]);
		}
	}
	// MOD 3
	void descartes(int c[total], int *dr) {
		int del, deleteCards;
		do {
			printf("Cuantos numeros quiere eliminar? ");
			scanf("%d", &deleteCards);
			if (deleteCards < 1 || deleteCards > HandN) {printf("\n (!) Opcion invalida\n\n");}

		} while (deleteCards < 1 || deleteCards > HandN);
							
		printf("\nQue cartas deseas eliminar? (1 - %d, negativo para salir): ", HandN);
							
		for (int i = 0; i<deleteCards; i++) {
			do {
				scanf("%d", &del);
				if (del <= 0 || del > HandN) {
					printf("\n (!) Opcion invalida\n\nQue cartas deseas eliminar? (1 - %d, negativo para salir): ", HandN);
				}
			} while (del <= 0 || del > HandN);
								
				for (int i = 0; i < HandN; i++) {
					if (c[i] == c[del - 1]) {
						int nuevo;
						bool repetido;
							do {
								nuevo = rand() % (total) + 1;
								repetido = false;
								for (int i = 0; i < HandN; i++) {
									if (c[i] == nuevo) {
										repetido = true;
										break;
									}
							}
							} while (repetido == true);
							c[i] = nuevo;
					}
				}
		}
		(*dr)--;
	}
	// MOD 4
	void jugada(int c[total], int t[totalTablas], int *mr) {
		if (*mr <= 0) {printf("\n (!) Ya no tienes manos disponibles\n\n"); return;}

		for (int i = total - 1; i > 0; i--) {								
			int k = rand() % (i + 1);
			swap(&c[i], &c[k]);
		}
		(*mr)--;
	}
	// MOD 5
	void puntuacion(int c[total], int t[totalTablas], int *sa, int *sc, float multi, int *mr) {
		if (*mr <= 0) {return;}
		
		int aciertosEnEstaMano = 0; 
		for (int i = 0; i < HandN; i++) {
			for (int j = 0; j < totalTablas; j++) {
				if (c[i] == t[j]) {
					aciertosEnEstaMano++;
				}
			}
		}
		// En esta parte se suma los aciertos multiplicados por el poder de la tienda
		*sa = aciertosEnEstaMano; 
		(*sc) = (*sc) + (aciertosEnEstaMano * multi);
	}
/// DIFICULTAD		(Rooney)
	// MOD 6	
    void elegir_dificultad(int *manosR, int *descardR, int *scoreGoal) {
		int opcion;
		printf("\nSelecciona la dificultad:\n1. Facil\n2. Medio\n3. Dificil\nIngrese su opcion: ");
		scanf("%d", &opcion);

		switch (opcion) {
			case 1: //Más manos y descartes, menor puntaje necesario
				*scoreGoal = 1000;
				*manosR = 6;
				*descardR = 6;
				break;
			case 2: //Valores intermedios para manos, descartes y puntaje
				*scoreGoal = 2000;
				*manosR = 5;
				*descardR = 5;
				break;
			case 3: //Menos manos y descartes, mayor puntaje necesario
				*scoreGoal = 3000;
				*manosR = 4;
				*descardR = 4;
				break;
			default:
				printf("Opción invalida, se seleccionara dificultad Facil por defecto.\n");
				*scoreGoal = 1000;
				*manosR = 6;
				*descardR = 6;
				break;
		}

	}
/// TIENDA			(José)
	// MOD 7
	void mostrar_catalogo(Comodin opciones[3]) {
		Comodin lista[4] = {{"Picas x2", 200, 2.0}, {"Bono Suerte", 100, 1.5}, {"Eco-Duo", 150, 1.2}, {"Maxi-Mano", 500, 3.0}};
		printf("\n--- ARTICULOS DISPONIBLES ---\n");
		for(int i = 0; i < 3; i++) {
			opciones[i] = lista[rand() % 4];
			printf("%d. %s - Costo: %d pts\n", i + 1, opciones[i].nombre, opciones[i].costo);
		}
	}
	// MOD 8
	void procesar_compra(int seleccion, Comodin opciones[3], int *puntosJugador, float *multi) {
		int costoAjustado = opciones[seleccion-1].costo / 100;
		
		if (*puntosJugador >= costoAjustado) {
			*puntosJugador -= costoAjustado;
			
			// Aquí se vincula la compra:
			*multi = *multi + (opciones[seleccion-1].multiplicador - 1.0); 
			
			printf("Compra exitosa! Tu multiplicador ahora es: %.2fx\n", *multi);
		} else {
			printf("Puntos insuficientes.\n");
		}
	}
/// FINALIZAR	
	// MOD 9
	void resumen_partida(int puntajeFinal, int meta, float multiplicador, 
						int manosIniciales, int manosRestantes, 
						int descartesIniciales, int descartesRestantes, 
						int ganada) {
		printf("\n+--------------------------------------------------+\n");
		printf("|               RESUMEN DE PARTIDA                 |\n");
		printf("+--------------------------------------------------+\n");
		printf("| Puntuacion final:      %d puntos                 |\n", puntajeFinal);
		printf("| Meta requerida:        %d puntos                 |\n", meta);
		printf("| Multiplicador final:   %.2fx                      |\n", multiplicador);
		printf("| Manos utilizadas:      %d                        |\n", manosIniciales - manosRestantes);
		printf("| Descartes utilizados:  %d                        |\n", descartesIniciales - descartesRestantes);
		if (ganada) {
			printf("| Resultado:             ¡VICTORIA!               |\n");
		} else {
			printf("| Resultado:             DERROTA                  |\n");
		}
		printf("+--------------------------------------------------+\n");
		printf("Gracias por jugar. ¡Hasta la próxima!\n\n");
	}




/// MAIN
int main() {

	srand(time(NULL));
	// static int ManosR, DescardR;		<-- idk
	int cartas[total], tabla[totalTablas], opc_1, opc_2, same, score = 0, ManosR, DescardR, c,scoreGoal=1000;
	Comodin opcionesTienda[3];
	float multiglobal = 1.0;
	int partidaGanada = 0; //variable que recuerda si se ganó o se perdió 
	int manosIniciales = 0, descartesIniciales = 0;
	
	// TODAS LAS CARTAS
	char POKER[53][4] = {
		"2P", "3P", "4P", "5P", "6P", "7P", "8P", "9P", "10P", "JP", "QP", "KP", "AP", // PICAS
		"2T", "3T", "4T", "5T", "6T", "7T", "8T", "9T", "10T", "JT", "QT", "KT", "AT", // TRÉBOLES
		"2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "10C", "JC", "QC", "KC", "AC", // CORAZONES
		"2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "10D", "JD", "QD", "KD", "AD", // DIAMANTES
		"NULL"
	};
	
	// No dejar tabla y cartas en 0s
	for (int i = 0; i < total; i++) {tabla[i] = i + 1; cartas[i] = i + 1; }

	do {
		
		printf("\n	d8888b.  .d8b.  d8888b.  .d8b.  d888888b d8888b.  .d88b.  d8b   db ");
		printf("\n	88  `8D d8' `8b 88  `8D d8' `8b `~~88~~' 88  `8D .8P  Y8. 888o  88 ");
		printf("\n	88oooY' 88ooo88 88oobY' 88ooo88    88    88oobY' 88    88 88V8o 88 ");
		printf("\n	88~~~b. 88~~~88 88`8b   88~~~88    88    88`8b   88    88 88 V8o88 ");
		printf("\n	88   8D 88   88 88 `88. 88   88    88    88 `88. `8b  d8' 88  V888 ");
		printf("\n	Y8888P' YP   YP 88   YD YP   YP    YP    88   YD  `Y88P'  VP   V8P \n\n");                                                                
		printf("Que desea hacer?\n0. Como jugar\n1. Jugar\n2. Dificultad\n3. Tienda\n4. Finalizar\n Seleccione una opcion: ");
		scanf("%d", &opc_1);
		
		switch (opc_1) {
			case 0: {
				instrucciones(1);
				break;
			}
			/// JUGAR		(Edo)
			case 1: {
				
				// Dejar todo listo para una siguiente ronda
				barajear_tabla(tabla);
				barajear_mano(cartas);
				ManosR = 4;
				DescardR = 4;
				c = 1;
				score = 0; //reiniciar puntuación 
				multiglobal = 1.0; //reiniciar multiplicador
				manosIniciales = ManosR;
				descartesIniciales = DescardR;
				partidaGanada = 0;

				// RONDA
				do {
					// INTERFAZ
					printf("+----------------------------------------------------------------+\n|       TABLA:\n|     ");
					for (int i = 0; i<16; i++) {
						if (i % 4 == 0) {printf("\n|     ");}
						printf("%s ", POKER[tabla[i]]);
					}
					printf("\n|\n|  MANO ACTUAL: \n|\n|  ");
					for (int i = 0; i < HandN; i++) {printf("(%d) %s  ", i+1, POKER[cartas[i]]);}
					printf("\n|\n|  MANOS: %d DESCARTES: %d PUNTUACION: %d\n+----------------------------------------------------------------+\n", ManosR, DescardR, score*100);
					printf("\nQue quieres hacer?\n\n1. Jugar mano\n2. Eliminar carta\n3. Terminar\n\nIngrese la opcion: ");
					scanf("%d", &opc_2);
					
					if(score>=scoreGoal){
						printf("\n\n¡Felicidades! Has alcanzado la meta de puntuación, veamos hasta dónde eres capaz de llegar!\n\n");
						 partidaGanada = 1;
					}
					if(ManosR <= 0){
						printf("\n\n¡Oh no! Has agotado todas tus manos. La partida ha terminado.\n\n");
						// aqui se podria llamar a la fn de finalizar partida, pero por ahora solo se sale del ciclo
						partidaGanada = 0; 
						break;
					}
					

					switch (opc_2) {
						case 1: {
							same = 0;
							jugada(cartas, tabla, &ManosR);
							puntuacion(cartas, tabla, &same, &score, multiglobal, &ManosR);

							break;
						}
						case 2: {
							if (DescardR <= 0) {
								printf("\n (!) Ya no tienes descartes disponibles\n\n");
							} else {
								descartes(cartas, &DescardR);
							}							
							break;
						}
						case 3: {
							c = 0;
						}
					}
				} while (c > 0);
				break;
			}
			/// DIFICULTAD		(Rooney)
			case 2: {
				elegir_dificultad(&ManosR, &DescardR, &scoreGoal);
				break;
			}
			/// TIENDA			(José)
			case 3: {
				mostrar_catalogo(opcionesTienda); 
				int seleccion;
				printf("\nSeleccione su compra (1-3, o 0 para salir): ");
				scanf("%d", &seleccion);
				
				if (seleccion > 0 && seleccion <= 3) {
					procesar_compra(seleccion, opcionesTienda, &score, &multiglobal);
				}
				break;
			}	
			/// TERMINAR		(Azul)	
			case 4: {
				resumen_partida(score, scoreGoal, multiglobal, 
                    manosIniciales, ManosR, 
                    descartesIniciales, DescardR, 
                    partidaGanada);
				break;
			}
		}
		
	} while (opc_1 != 4);
	
	return 0;
}

