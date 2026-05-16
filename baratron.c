#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
 
// -----------------------------------------------
//   CODIGOS DE COLOR ANSI
// -----------------------------------------------
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"

#define VERDE       "\033[32m"
#define AMARILLO    "\033[33m"
#define CYAN        "\033[36m"
 
#define ROJO_B      "\033[91m"
#define VERDE_B     "\033[92m"
#define AMARILLO_B  "\033[93m"
#define AZUL_B      "\033[94m"
#define MAGENTA_B   "\033[95m"
#define CYAN_B      "\033[96m"
#define BLANCO_B    "\033[97m"
 
// -----------------------------------------------
//   VARIABLES GLOBALES / FUNCIONES EXTRA
// -----------------------------------------------
int total = 51, totalTablas = 16, HandN = 7, kk;
 
typedef struct {
    char nombre[20];
    int costo;
    float multiplicador;
} Comodin;
 
void swap(int *a, int *b) {
    int c = *a; *a = *b; *b = c;
}
 
void limpiar_pantalla() {
    printf("\033[2J\033[H");
}

const char* color_carta(int id) {
    if (id >= 26 && id <= 50) return ROJO_B;
    return BLANCO_B;
}
 
void imprimir_logo() {
    printf("\n");
    printf(AMARILLO_B BOLD);
    printf("  +---------------------------------------------------------+\n");
    printf("  |     _____ _____ _____ _____ _____ _____ _____ _____     |\n");
    printf("  |    | __  |  _  | __  |  _  |_   _| __  |     |   | |    |\n");
    printf("  |    | __ -|     |    -|     | | | |    -|  |  | | | |    |\n");
    printf("  |    |_____|__|__|__|__|__|__| |_| |__|__|_____|_|___|    |\n");
    printf("  +---------------------------------------------------------+\n");
    printf(RESET);
    printf(DIM CYAN "            [P] [T] Juego de cartas [C] [D]\n" RESET);
    printf("\n");
}

void imprimir_menu(int score, int scoreGoal) {
    int pct = (scoreGoal > 0) ? (score * 100 / scoreGoal) : 0;
    if (pct > 100) pct = 100;
    int barlen = pct / 5;   
    printf(AZUL_B BOLD "\n  +---------------------------------------+\n" RESET);
    printf(AZUL_B BOLD "  |" RESET BLANCO_B BOLD "           MENU PRINCIPAL              " RESET AZUL_B BOLD "|\n" RESET);
    printf(AZUL_B BOLD "  +---------------------------------------+\n" RESET);
    printf(AZUL_B BOLD "  |" RESET "  " CYAN        "0." RESET " Como jugar                        " AZUL_B BOLD "|\n" RESET);
    printf(AZUL_B BOLD "  |" RESET "  " VERDE_B     "1." RESET " Jugar                             " AZUL_B BOLD "|\n" RESET);
    printf(AZUL_B BOLD "  |" RESET "  " MAGENTA_B   "2." RESET " Dificultad                        " AZUL_B BOLD "|\n" RESET);
    printf(AZUL_B BOLD "  |" RESET "  " AMARILLO_B  "3." RESET " Tienda                            " AZUL_B BOLD "|\n" RESET);
    printf(AZUL_B BOLD "  |" RESET "  " ROJO_B      "4." RESET " Finalizar                         " AZUL_B BOLD "|\n" RESET);
    printf(AZUL_B BOLD "  +---------------------------------------+\n" RESET);
    printf(BLANCO_B "  Selecciona una opcion: " RESET);
}
 
void imprimir_interfaz_juego(int cartas[], int tabla[], int ManosR, int DescardR, int score, int scoreGoal, char POKER[][4]) {
    limpiar_pantalla();
    printf("\n");
 
    // -- TABLA --
    printf(VERDE_B BOLD                      "+------------------------------------------------------------+\n" RESET);
    printf(VERDE_B BOLD "|" RESET AMARILLO_B BOLD "                       TABLA DE JUEGO                       " RESET VERDE_B BOLD "|\n" RESET);
    printf(VERDE_B BOLD "+------------------------------------------------------------+\n" RESET);
 
    for (int fila = 0; fila < 4; fila++) {
        printf(VERDE_B BOLD "|            " RESET "   ");
        for (int col = 0; col < 4; col++) {
            int idx = tabla[fila * 4 + col] - 1;
            const char *cc = color_carta(idx);
            printf("%s" BOLD " [%-3s]" RESET "  ", cc, POKER[idx]);
        }
        printf("             " VERDE_B BOLD "|\n" RESET);
    }
 
    // -- STATS --
    printf(VERDE_B BOLD "+------------------------------------------------------------+\n" RESET);
 
    // Barra de manos
    printf(VERDE_B BOLD "|" RESET "  " CYAN_B BOLD "MANOS    [" RESET);
    for (int i = 0; i < 4; i++) printf(i < ManosR ? CYAN_B "#" RESET : DIM "-" RESET);
    printf(CYAN_B "]    " RESET);
 
    // Barra de descartes
    printf(CYAN_B BOLD "DESCARTES [" RESET);
    for (int i = 0; i < 4; i++) printf(i < DescardR ? CYAN_B "#" RESET : DIM "-" RESET);
    printf(CYAN_B "]  " RESET "                     " VERDE_B BOLD "|\n" RESET);

    // Puntuacion
    printf(VERDE_B BOLD "|" RESET "  " CYAN_B "Puntuacion: " RESET AMARILLO_B BOLD "%6d" RESET " / " BLANCO_B "%d pts" RESET "                             " VERDE_B BOLD "|\n" RESET, score * 100, scoreGoal);
 
    // -- MANO --
    printf(VERDE_B BOLD "+------------------------------------------------------------+\n" RESET);
    printf(VERDE_B BOLD "|" RESET BLANCO_B BOLD "                          TU MANO                           " RESET VERDE_B BOLD "|\n" RESET);
    printf(VERDE_B BOLD "|" RESET "   ");
    for (int i = 0; i < HandN; i++) {
        int idx = cartas[i] - 1;
        const char *cc = color_carta(idx);
        printf("%s" BOLD "(%d)%-3s" RESET "  ", cc, i + 1, POKER[idx]);
    }
    printf(" " VERDE_B BOLD "|\n" RESET);
 
    // -- ACCIONES --
    printf(VERDE_B BOLD "+------------------------------------------------------------+\n" RESET);
    printf(VERDE_B BOLD "|  1." RESET " Jugar mano   " AMARILLO_B "2." RESET " Descartar carta   " ROJO_B    "3." RESET " Salir al menu     " VERDE_B BOLD "|\n" RESET);
    printf(VERDE_B BOLD "+------------------------------------------------------------+\n" RESET);
    printf(BLANCO_B "\n  Elige tu accion: " RESET);
}

void instrucciones(int a) {
    limpiar_pantalla();
    printf("\n");
    printf(CYAN_B BOLD "+----------------------------------------------------------------+\n" RESET);
    printf(CYAN_B BOLD "|" RESET AMARILLO_B BOLD "                          COMO JUGAR                            " RESET CYAN_B BOLD "|\n" RESET);
    printf(CYAN_B BOLD "+----------------------------------------------------------------+\n" RESET);
    printf(CYAN_B BOLD "|" RESET "  El objetivo del juego es obtener la mayor puntuacion          " CYAN_B BOLD "|\n" RESET);
    printf(CYAN_B BOLD "|" RESET "  posible mediante la coincidencia de cartas entre tu mano      " CYAN_B BOLD "|\n" RESET);
    printf(CYAN_B BOLD "|" RESET "  y la tabla ordenada de manera aleatoria.                      " CYAN_B BOLD "|\n" RESET);
    printf(CYAN_B BOLD "|" RESET "                                                                " CYAN_B BOLD "|\n" RESET);
    printf(CYAN_B BOLD "|" RESET VERDE "  En cada turno podras:                                         " RESET CYAN_B BOLD "|\n" RESET);
    printf(CYAN_B BOLD "|" RESET AMARILLO "   * Jugar una mano   " RESET "-> baraja tus cartas y suma aciertos      " CYAN_B BOLD "|\n" RESET);
    printf(CYAN_B BOLD "|" RESET AMARILLO "   * Descartar cartas " RESET "-> reemplaza cartas de tu mano            " CYAN_B BOLD "|\n" RESET);
    printf(CYAN_B BOLD "|" RESET AMARILLO "   * Ir a la Tienda   " RESET "-> compra comodines con tus puntos        " CYAN_B BOLD "|\n" RESET);
    printf(CYAN_B BOLD "|" RESET "                                                                " CYAN_B BOLD "|\n" RESET);
    printf(CYAN_B BOLD "|" RESET "  El juego continua mientras tengas manos disponibles o         " CYAN_B BOLD "|\n" RESET);
    printf(CYAN_B BOLD "|" RESET "  hasta que decidas finalizar la partida.                       " CYAN_B BOLD "|\n" RESET);
    printf(CYAN_B BOLD "+----------------------------------------------------------------+\n" RESET);
    printf(DIM "\n  Presiona Enter para continuar..." RESET);
    getchar(); getchar();
}
 









// -----------------------------------------------
//   JUGAR  (EDO)
// -----------------------------------------------

//  MOD 1
void barajear_tabla(int m[]) {
    for (int i = total - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(&m[i], &m[j]);
    }
}

//  MOD 2
void barajear_mano(int m[]) {
    for (int i = total - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(&m[i], &m[j]);
    }
}

//  MOD 3
void descartes(int c[], int *dr) {
    int del, deleteCards;
 
    printf("\n" AMARILLO_B "+-----------------------------------+\n" RESET);
    printf(     AMARILLO_B "|         MODO DESCARTE             |\n" RESET);
    printf(     AMARILLO_B "+-----------------------------------+\n" RESET);
 
    do {
        printf(BLANCO_B "  Cuantas cartas quieres descartar? " RESET AMARILLO_B "(1-%d): " RESET, HandN);
        scanf("%d", &deleteCards);
        if (deleteCards < 1 || deleteCards > HandN)
            printf(ROJO_B "  (!) Opcion invalida. Intenta de nuevo.\n" RESET);
    } while (deleteCards < 1 || deleteCards > HandN);
 
    printf(BLANCO_B "\n  Que cartas deseas eliminar?" RESET DIM " (posicion 1-%d):\n" RESET, HandN);
 
    for (int i = 0; i < deleteCards; i++) {
        printf(AMARILLO "  Carta #%d -> " RESET, i + 1);
        do {
            scanf("%d", &del);
            if (del <= 0 || del > HandN)
                printf(ROJO_B "  (!) Invalida, intenta de nuevo: " RESET);
        } while (del <= 0 || del > HandN);
 
        for (int j = 0; j < HandN; j++) {
            if (c[j] == c[del - 1]) {
                int nuevo;
                bool repetido;
                do {
                    nuevo = rand() % total + 1;
                    repetido = false;
                    for (int k = 0; k < HandN; k++) {
                        if (c[k] == nuevo) { repetido = true; break; }
                    }
                } while (repetido);
                c[j] = nuevo;
            }
        }
    }
    (*dr)--;
    printf(VERDE_B "\n  >> Descarte realizado.\n" RESET);
}

//  MOD 4
void jugada(int c[], int t[], int *mr) {
    if (*mr <= 0) {
        printf(ROJO_B "\n  (!) Ya no tienes manos disponibles\n" RESET);
        return;
    }
    for (int i = total - 1; i > 0; i--) {
        int k = rand() % (i + 1);
        swap(&c[i], &c[k]);
    }
    (*mr)--;
}

//  MOD 5
void puntuacion(int c[total], int t[totalTablas], int *sa, int *sc, float multi, int mr) {
	if (mr <= 0) {return;}
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
 
// -----------------------------------------------
//   DIFICULTAD     (ROONEY)
// -----------------------------------------------

// MOD 6
void elegir_dificultad(int *manosR, int *descardR, int *scoreGoal) {
    limpiar_pantalla();
    int opcion;
 
    printf("\n");
    printf(MAGENTA_B BOLD "+---------------------------------------------------+\n" RESET);
    printf(MAGENTA_B BOLD "|" RESET AMARILLO_B BOLD "             SELECCIONA LA DIFICULTAD              " RESET MAGENTA_B BOLD "|\n" RESET);
    printf(MAGENTA_B BOLD "+---------------------------------------------------+\n" RESET);
    printf(MAGENTA_B BOLD "|" RESET VERDE_B    "  1. FACIL  " RESET "-> 6 manos | 6 descartes | Meta: 1000  " MAGENTA_B BOLD "|\n" RESET);
    printf(MAGENTA_B BOLD "|" RESET AMARILLO_B "  2. MEDIO  " RESET "-> 5 manos | 5 descartes | Meta: 2000  " MAGENTA_B BOLD "|\n" RESET);
    printf(MAGENTA_B BOLD "|" RESET ROJO_B     "  3. DIFICIL" RESET "-> 4 manos | 4 descartes | Meta: 3000  " MAGENTA_B BOLD "|\n" RESET);
    printf(MAGENTA_B BOLD "+---------------------------------------------------+\n" RESET);
    printf(BLANCO_B "\n  Ingresa tu opcion: " RESET);
    scanf("%d", &opcion);
 
    switch (opcion) {
        case 1: *scoreGoal = 1000; *manosR = 6; *descardR = 6;
            printf(VERDE_B "\n  >> Dificultad FACIL seleccionada.\n" RESET); break;
        case 2: *scoreGoal = 2000; *manosR = 5; *descardR = 5;
            printf(AMARILLO_B "\n  >> Dificultad MEDIO seleccionada.\n" RESET); break;
        case 3: *scoreGoal = 3000; *manosR = 4; *descardR = 4;
            printf(ROJO_B "\n  >> Dificultad DIFICIL seleccionada.\n" RESET); break;
        default:
            printf(ROJO_B "\n  (!) Opcion invalida -> se aplicara FACIL por defecto.\n" RESET);
            *scoreGoal = 1000; *manosR = 6; *descardR = 6;
    }
    printf(DIM "\n  Presiona Enter para continuar..." RESET);
    getchar(); getchar();
}
 
// -----------------------------------------------
//   TIENDA     (JOSE)
// -----------------------------------------------

// MOD 7
void mostrar_catalogo(Comodin opciones[3]) {
    Comodin lista[4] = {
        {"Picas x2",    200, 2.0},
        {"Bono Suerte", 100, 1.5},
        {"Eco-Duo",     150, 1.2},
        {"Maxi-Mano",   500, 3.0}
    };
    printf("\n" AMARILLO_B BOLD "+---------------------------------------------------+\n" RESET);
    printf(     AMARILLO_B BOLD "|                TIENDA DE COMODINES                |\n" RESET);
    printf(     AMARILLO_B BOLD "+---------------------------------------------------+\n" RESET);
    for (int i = 0; i < 3; i++) {
        opciones[i] = lista[rand() % 4];
        printf(AMARILLO_B BOLD "|" RESET CYAN_B "  %d. %-16s" RESET " Costo: " VERDE_B "%3d pts" RESET "  Multi: " MAGENTA_B "x%.1f  " RESET AMARILLO_B BOLD "|\n" RESET,
               i + 1, opciones[i].nombre, opciones[i].costo, opciones[i].multiplicador);
    }
    printf(     AMARILLO_B BOLD "+---------------------------------------------------+\n" RESET);
}

// MOD 8
void procesar_compra(int seleccion, Comodin opciones[3], int *puntosJugador, float *multi) {
    int costoAjustado = opciones[seleccion - 1].costo / 100;
    if (*puntosJugador >= costoAjustado) {
        *puntosJugador -= costoAjustado;
        *multi += opciones[seleccion - 1].multiplicador - 1.0f;
        printf(VERDE_B "\n  >> Compra exitosa! Multiplicador actual: " BOLD "%.2fx\n" RESET, *multi);
    } else {
        printf(ROJO_B "\n  (!) Puntos insuficientes para esta compra.\n" RESET);
    }
}
 
// -----------------------------------------------
//   FINALIZAR      (AZUL)
// -----------------------------------------------

// MOD 9
void resumen_partida(int puntajeFinal, int meta, float multiplicador,
                     int manosIniciales, int manosRestantes,
                     int descartesIniciales, int descartesRestantes,
                     int ganada) {
    limpiar_pantalla();
    printf("\n\n");
 
    const char *cr   = ganada ? VERDE_B : ROJO_B;
    const char *res  = ganada ? ">> VICTORIA! <<" : "   DERROTA...  ";
 
    printf("%s" BOLD "+------------------------------------------------+\n" RESET, cr);
    printf("%s" BOLD "|" RESET AMARILLO_B BOLD "            RESUMEN FINAL DE PARTIDA            " RESET "%s" BOLD "|\n" RESET, cr, cr);
    printf("%s" BOLD "+------------------------------------------------+\n" RESET, cr);
    printf("%s" BOLD "|" RESET "  Puntuacion final  : " AMARILLO_B BOLD "%8d pts" RESET "              %s" BOLD      "|\n" RESET, cr, puntajeFinal * 100, cr);
    printf("%s" BOLD "|" RESET "  Meta requerida    : " BLANCO_B        "%8d pts" RESET "              %s" BOLD      "|\n" RESET, cr, meta, cr);
    printf("%s" BOLD "|" RESET "  Multiplicador     : " MAGENTA_B       "     x%.2f" RESET "                %s" BOLD "|\n" RESET, cr, multiplicador, cr);
    printf("%s" BOLD "|" RESET "  Manos utilizadas  : " CYAN_B          "%8d" RESET "                  %s" BOLD      "|\n" RESET, cr, manosIniciales - manosRestantes, cr);
    printf("%s" BOLD "|" RESET "  Descartes usados  : " CYAN_B          "%8d" RESET "                  %s" BOLD      "|\n" RESET, cr, descartesIniciales - descartesRestantes, cr);
    printf("%s" BOLD "+------------------------------------------------+\n" RESET, cr);
    printf("%s" BOLD "|" RESET "  Resultado :  %s" BOLD "%-33s" RESET "%s" BOLD "|\n" RESET, cr, cr, res, cr);
    printf("%s" BOLD "+------------------------------------------------+\n" RESET, cr);
    printf("\n" DIM "  Gracias por jugar Baratron! Hasta la proxima.\n\n" RESET);
}












// -----------------------------------------------
//   MAIN
// -----------------------------------------------
int main() {
    srand(time(NULL));
 
    int cartas[51], tabla[16];
    int opc_1, opc_2, same, score = 0;
    int ManosR = 4, DescardR = 4, c, scoreGoal = 1000;
    Comodin opcionesTienda[3];
    float multiglobal = 1.0;
    int partidaGanada = 0;
    int manosIniciales = 0, descartesIniciales = 0;
 
    char POKER[53][4] = {
        "2P","3P","4P","5P","6P","7P","8P","9P","10P","JP","QP","KP","AP",
        "2T","3T","4T","5T","6T","7T","8T","9T","10T","JT","QT","KT","AT",
        "2C","3C","4C","5C","6C","7C","8C","9C","10C","JC","QC","KC","AC",
        "2D","3D","4D","5D","6D","7D","8D","9D","10D","JD","QD","KD","AD",
        "NULL"
    };
 
    for (int i = 0; i < total; i++) { tabla[i] = i + 1; cartas[i] = i + 1; }
 
    do {
        limpiar_pantalla();
        imprimir_logo();
        imprimir_menu(score, scoreGoal);
        scanf("%d", &opc_1);
 
        switch (opc_1) {
 
            case 0:
                instrucciones(1);
                break;
 
            case 1: {
                barajear_tabla(tabla);
                barajear_mano(cartas);
                ManosR = 4; DescardR = 4; c = 1;
                score = 0; multiglobal = 1.0;
                manosIniciales = ManosR;
                descartesIniciales = DescardR;
                partidaGanada = 0;
 
                do {
                    imprimir_interfaz_juego(cartas, tabla, ManosR, DescardR, score, scoreGoal, POKER);
                    scanf("%d", &opc_2);
 
                    if (score >= scoreGoal) {
                        printf(VERDE_B BOLD "\n  >> Felicidades! Alcanzaste la meta. Sigue adelante!\n" RESET);
                        partidaGanada = 1;
                    }
                    if (ManosR <= 0) {
                        printf(ROJO_B BOLD "\n  (!) Sin manos. La partida ha terminado.\n" RESET);
                        partidaGanada = 0;
                        printf(DIM "  Presiona Enter..." RESET);
                        getchar(); getchar();
                        break;
                    }
 
                    switch (opc_2) {
                        case 1:
                            same = 0;
                            puntuacion(cartas, tabla, &same, &score, multiglobal, ManosR);
                            jugada(cartas, tabla, &ManosR);
                            printf(CYAN_B "\n  >> Aciertos esta mano: " BOLD "%d" RESET CYAN_B " -> +" BOLD "%.0f pts\n" RESET, same, same * multiglobal * 100);
                            printf(DIM "  Presiona Enter..." RESET);
                            getchar(); getchar();
                            break;
                        case 2:
                            if (DescardR <= 0)
                                printf(ROJO_B "\n  (!) Sin descartes disponibles\n" RESET);
                            else
                                descartes(cartas, &DescardR);
                            printf(DIM "  Presiona Enter..." RESET);
                            getchar(); getchar();
                            break;
                        case 3:
                            c = 0;
                            break;
                    }
                } while (c > 0);
                break;
            }
 
            case 2:
                elegir_dificultad(&ManosR, &DescardR, &scoreGoal);
                break;
 
            case 3: {
                limpiar_pantalla();
                printf("\n");
                mostrar_catalogo(opcionesTienda);
                int seleccion;
                printf(BLANCO_B "\n  Selecciona (1-3, o 0 para salir): " RESET);
                scanf("%d", &seleccion);
                if (seleccion >= 1 && seleccion <= 3)
                    procesar_compra(seleccion, opcionesTienda, &score, &multiglobal);
                printf(DIM "\n  Presiona Enter..." RESET);
                getchar(); getchar();
                break;
            }
 
            case 4:
                resumen_partida(score, scoreGoal, multiglobal,
                                manosIniciales, ManosR,
                                descartesIniciales, DescardR,
                                partidaGanada);
                break;
        }
 
    } while (opc_1 != 4);
 
    return 0;
}