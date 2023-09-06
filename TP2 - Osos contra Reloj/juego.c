#include <ctype.h>
#include <time.h>
#include "osos_contra_reloj.h"
#include "test_de_personalidad.h"
#include "utiles.h"

const int TIEMPO_OBJETIVO = 120;

/**
 * Lee la entrada del jugador, verifica que sea valida, y la almacena en
    un puntero. (W,A,S,D,L,V,E,T)
 */
void leer_jugada(char *jugada)
{
    printf("Movimiento: ");
    scanf(" %c", jugada);
    *jugada = (char)toupper(*jugada);
    while (!es_jugada_valida(*jugada))
    {
        printf("Jugada invalida, Ingrese otra: ");
        scanf(" %c", jugada);
        *jugada = (char)toupper(*jugada);
    }
    printf("\n");
}

/**
 * Espera a que el usuario inserte un caracter para seguir la ejecucion
    del programa.
 */
void esperar_entrada()
{
    char aux;
    scanf(" %c", &aux);
    system("clear");
}

/**
 * Imprime las reglas del juego en la terminal
 */
void imprimir_reglas()
{
    printf("El objetivo del juego es encontrar a Chloe en un bosque totalmente "
           "a oscuras, en menos de %i segundos.\n\n",
           TIEMPO_OBJETIVO);

    printf("En el bosque hay obstaculos que te retrasan, sumando segundos al "
           "tiempo total.\nTambien esta Nom Nom con sus sekoalaces, "
           "quienes organizaron una fiesta clandestina en el bosque.\n"
           "Los koalas aparecen cuando prendes la linterna, y al encontrarte te "
           "teletransportan\nhacia una posicion aleatoria de la "
           "columna inicial.\n\n"
           "Ademas, en el terreno se encuentran herramientas que iluminan "
           "algunos casilleros.\nLa VELA ilumina tus posiciones adyacentes, "
           "la LINTERNA ilumina hacia adelante hasta el fin del bosque,\n"
           "y la BENGALA ilumina un area aleatoria del terreno\n\n"
           "Las herramientas se pueden activar y desactivar a voluntad.\n"
           "excepto por las bengalas que se apagaran "
           "automaticamente al quedarse sin usos.\n"
           "Solo se puede tener una herramienta activa a la vez.\n");
}

/**
 * Imprime el mensaje final del juego en funcion del tiempo que tardo
 */
void imprimir_final(double tiempo_perdido, double tiempo_real)
{
    printf("Encontraste a Chloe en el bosque...\n");

    double tiempo_total = tiempo_perdido + tiempo_real;
    if (tiempo_total <= TIEMPO_OBJETIVO)
    {
        printf("Tardaste %.1f segundos. Gracias a tu gran esfuerzo, Chloe pudo "
               "llegar a su casa sana y salva donde se encuentra su Puff.\n"
               "Excelente trabajo.",
               tiempo_total);
    }
    else
    {
        printf("Tardaste %.1f segundos. Lamentablemente Chloe no pudo encontrar"
               " su camino a casa y ahora esta en riesgo.\n"
               "Pero esta bien, no te preocupes... No esperaba mucho de vos.",
               tiempo_total);
    }
}

int main()
{
    system("clear");
    srand((unsigned)time(NULL));

    char personaje;
    test_de_personalidad(&personaje);
    printf("\nIngrese cualquier caracter para continuar...\n");
    esperar_entrada();

    imprimir_reglas();
    printf("\nIngrese cualquier caracter para continuar...\n");
    esperar_entrada();

    juego_t juego;
    inicializar_juego(&juego, personaje);
    mostrar_juego(juego);
    printf("El reloj empezara despues de esto.\n"
           "Ingrese cualquier caracter para continuar...\n");
    esperar_entrada();

    iniciar_cronometro();
    while (estado_juego(juego) == 0)
    {
        mostrar_juego(juego);

        char jugada;
        leer_jugada(&jugada);
        system("clear");

        realizar_jugada(&juego, jugada);
    }

    imprimir_final(juego.personaje.tiempo_perdido, detener_cronometro());

    printf("\n");
    return 0;
}
