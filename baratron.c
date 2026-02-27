#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 

void change(int *a, int *b) {
	int c = 0;
	c = *a;
	*a = *b;
	*b = c;
}

int main() {
	srand(time(NULL));
	
	int total = 51, totalTablas = 16;
	int cartas[total], tabla[totalTablas], opc_1, opc_2, HandN = 7, same, score = 0, a = 1, c = 1;
	static int ManosR = 4, DescardR = 4;
	
	// TODAS LAS CARTAS
	char POKER[53][4] = {
		"2P", "3P", "4P", "5P", "6P", "7P", "8P", "9P", "10P", "JP", "QP", "KP", "AP", // PICAS
		"2T", "3T", "4T", "5T", "6T", "7T", "8T", "9T", "10T", "JT", "QT", "KT", "AT", // TRÉBOLES
		"2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "10C", "JC", "QC", "KC", "AC", // CORAZONES
		"2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "10D", "JD", "QD", "KD", "AD", // DIAMANTES
		"NULL"
	};
	
	// No dejar tabla y cartas en 0s
	for (int i = 0; i < total; i++) {
		tabla[i] = i + 1;
		cartas[i] = i + 1;
	}

	do {
		printf("-- Baratón --\n¿Qué desea hacer?\n1. Jugar\n2. Dificultad\n3. Tienda\n0. Finalizar\n Seleccione una opción: ");
		scanf("%d", &opc_1);
		
		switch (opc_1) {
			case 0: {
				a = 0;
				break;
			}
			case 1: {
				// Barajear tabla y cartas
				srand(time(0));
				for (int i = total - 1; i > 0; i--) {
					int j = rand() % (i + 1);
					change(&tabla[i], &tabla[j]);
					
					int k = rand() % (i + 1);
					change(&cartas[i], &cartas[k]);
				}
				
				do {
					printf("+----------------------------------------------------------------+\n|       TABLA:\n|     ");
					for (int i = 0; i<16; i++) {
						if (i % 4 == 0) {printf("\n|     ");}
						printf("%s ", POKER[tabla[i]]);
					}
					printf("\n|\n|  MANO ACTUAL: \n|\n|  ");
					for (int i = 0; i < HandN; i++) {printf("(%d) %s  ", i+1, POKER[cartas[i]]);}
					printf("\n|\n|  MANOS: %d DESCARTES: %d PUNTUACION: %d\n+----------------------------------------------------------------+\n", ManosR, DescardR, score*100);
					printf("\n¿Que quieres hacer?\n\n1. Jugar mano\n2. Eliminar carta\n3. Terminar\n\nIngrese la opcion: ");
					scanf("%d", &opc_2);
					
					same = 0;
				
					switch (opc_2) {
						case 1: {
							if (ManosR <= 0) {
								printf("\n (!) Ya no tienes manos disponibles\n\n");
								break;
							}
							
							for (int i = 0; i < HandN; i++) {
								for (int j = 0; j < totalTablas; j++) {
									if (cartas[i] == tabla[j]) {
										same++;
									}
								}
							}
							for (int i = total - 1; i > 0; i--) {								
								int k = rand() % (i + 1);
								change(&cartas[i], &cartas[k]);
							}
							
							ManosR--;
							score = score + same;
							break;
						}
						case 2: {
							if (DescardR <= 0) {
								printf("\n (!) Ya no tienes descartes disponibles\n");
								break;
							}
							int del, deleteCards;
							do {
								printf("Cuantos numeros quiere eliminar? ");
								scanf("%d", &deleteCards);
								if (deleteCards < 1 || deleteCards > HandN) {
									printf("\n (!) Opcion invalida\n\n");
								}
							} while (deleteCards < 1 || deleteCards > HandN);
							
							printf("\n¿Que cartas deseas eliminar? (1 - %d, negativo para salir): ", HandN);
							
							for (int i = 0; i<deleteCards; i++) {
								do {
									scanf("%d", &del);
									if (del <= 0 || del > HandN) {
										printf("\n (!) Opcion invalida\n\nQue cartas deseas eliminar? (1 - %d, negativo para salir): ", HandN);
									}
								} while (del <= 0 || del > HandN);
								
								for (int i = 0; i < HandN; i++) {
									if (cartas[i] == cartas[del - 1]) {
										int nuevo;
										bool repetido;
										do {
											nuevo = rand() % (total) + 1;
											repetido = false;
											for (int i = 0; i < HandN; i++) {
												if (cartas[i] == nuevo) {
													repetido = true;
													break;
												}
											}
										} while (repetido == true);
										cartas[i] = nuevo;
									}
								}
							}
							DescardR--;
							break;
						}
						case 3: {
							c = 0;
						}
					}
				} while (c > 0);
				break;
			}
			case 2: {
				
				break;
			}
			case 3: {
					
				break;
			}	
		}
		
	} while (a > 0);
	
	
	
	
	
	return 0;
}

