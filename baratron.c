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
	printf("|  El objetivo del juego es obtener la mayor puntuación posible  |\n");
	printf("|  mediante la coincidencia de cartas entre tu mano y una tabla  |\n");
	printf("|  ordenados de manera aleatoria.                                |\n");
	printf("|  En cada turno, puedrás utilizar una cantidad limitada de manos|\n");
	printf("|  para jugar, descartes para reemplazar cartas y comodines que  |\n");
	printf("|  podrás adquirir en una tienda con base en los puntos 		 |\n");
	printf("|  obtenidos.                       							 |\n");
	printf("|  El juego continúa mientras dispongas de recursos o cuando     |\n");
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
		for (int i = total - 1; i > 0; i--) {								
			int k = rand() % (i + 1);
			swap(&c[i], &c[k]);
		}
		(*mr)--;
	}
	// MOD 5
	void puntuacion(int c[total], int t[totalTablas], int *sa, int *sc, float multi) {
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
/// FINALIZAR		(Azul)
	// MOD 9








	



/// MAIN
int main() {
	srand(time(NULL));
	// static int ManosR, DescardR;		<-- idk
	int cartas[total], tabla[totalTablas], opc_1, opc_2, same, score = 0, ManosR, DescardR, c;
	Comodin opcionesTienda[3];
	float multiglobal = 1.0;
	
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
					
					switch (opc_2) {
						case 1: {
							same = 0;
							if (ManosR <= 0) {
								printf("\n (!) Ya no tienes manos disponibles\n\n");
							} else {
								jugada(cartas, tabla, &ManosR);
								puntuacion(cartas, tabla, &same, &score, multiglobal);
							}
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

				return 0;
			}
		}
		
	} while (1 > 0);
	
	return 0;
}

