#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//COLORES
#define ROJO "\x1b[31m"
#define NORMAL "\x1b[0m"

//OPCIONES
const char ANIME = 'A';
const char MUSICA_POP = 'M';
const char LIMPIEZA = 'L';

const char BAMBU = 'B';
const char PESCADO = 'P';
const char FOCAS = 'F';

const int MIN_VALOR_PISO = 1;
const int MAX_VALOR_PISO = 18;

const int MIN_VALOR_GRITO = 1;
const int MAX_VALOR_GRITO = 18;

//PUNTAJES
const int PUNTAJE_LIMPIEZA = 1;
const int PUNTAJE_ANIME = 2;
const int PUNTAJE_MUSICA_POP = 3;

const int PUNTAJE_FOCAS = 3;
const int PUNTAJE_BAMBU = 6;
const int PUNTAJE_PESCADO = 9;

//PERSONALIDADES
const char POLAR = 'I';
const int MIN_PUNTAJE_POLAR = 5;
const int MAX_PUNTAJE_POLAR = 24;

const char PANDA = 'P';
const int MIN_PUNTAJE_PANDA = 25;
const int MAX_PUNTAJE_PANDA = 43;

const char PARDO = 'G';
const int MIN_PUNTAJE_PARDO = 44;
const int MAX_PUNTAJE_PARDO = 63;

//EASTER EGG
const int EASTER_EGG = 9000;

/**
 * Post: TRUE si el canal esta dentro de (A, M, L)
 */
bool es_canal_valido(char canal)
{
    bool validez = (canal == ANIME) ||
                   (canal == MUSICA_POP) ||
                   (canal == LIMPIEZA);
    return validez;
}

/**
 * Post: TRUE si el alimento esta dentro de (B, P, F)
 */
bool es_alimento_valido(char alimento)
{
    bool validez = (alimento == BAMBU) ||
                   (alimento == PESCADO) ||
                   (alimento == FOCAS);
    return validez;
}

/**
 * Post: TRUE si el piso preferido esta dentro del rango [1-18]
 */
bool es_piso_valido(int piso)
{
    bool validez = (piso >= MIN_VALOR_PISO) && (piso <= MAX_VALOR_PISO);
    return validez;
}

/**
 * Post: TRUE si el nivel de grito esta dentro del rango [1-18]
 */
bool es_grito_valido(int grito)
{
    bool validez = (grito >= MIN_VALOR_GRITO) && (grito <= MAX_VALOR_GRITO);
    return validez;
}

/**
 * Pide el canal al usuario, verificando que sea valido (A, M, L)
 * y almacena el valor en una direccion de memoria
 */
void leer_canal(char *ptr_canal)
{
    char canal;

    printf("Tus viejos dejaron la television prendida y quedo " ROJO
           "C5N" NORMAL ". "
           "¿A que categoria lo cambias?"
           "\n-Anime -> %c"
           "\n-Musica Pop -> %c"
           "\n-Limpieza -> %c",
           ANIME, MUSICA_POP, LIMPIEZA);

    printf("\nEntrada: ");
    scanf(" %c", &canal);
    while (!es_canal_valido(canal))
    {
        printf("\nInvalido! Recorda que las opciones son:"
               "\n-Anime -> %c"
               "\n-Musica Pop -> %c"
               "\n-Limpieza -> %c",
               ANIME, MUSICA_POP, LIMPIEZA);

        printf("\nEntrada: ");
        scanf(" %c", &canal);
    }

    *ptr_canal = canal;
}

/**
 * Pide el alimento al usuario, verificando que sea valido (B, P, F)
 * y almacena el valor en una direccion de memoria
 */
void leer_alimento(char *ptr_alimento)
{
    char alimento;

    printf("Felicidades! Conseguiste un boleto para escapar de " ROJO
           "latinoamerica " NORMAL
           "¿Que alimento llevas para el avion?"
           "\n-Bambu -> %c"
           "\n-Pescado -> %c"
           "\n-Focas -> %c",
           BAMBU, PESCADO, FOCAS);

    printf("\nEntrada: ");
    scanf(" %c", &alimento);
    while (!es_alimento_valido(alimento))
    {
        printf("\nInvalido! Recorda que las opciones son:"
               "\n-Bambu -> %c"
               "\n-Pescado -> %c"
               "\n-Focas -> %c",
               BAMBU, PESCADO, FOCAS);

        printf("\nEntrada: ");
        scanf(" %c", &alimento);
    }

    *ptr_alimento = alimento;
}

/**
 * Pide el piso preferido al usuario, verificando que sea valido [1-18]
 * y almacena el valor en una direccion de memoria
 */
void leer_piso(int *ptr_piso)
{
    int piso;

    printf("Luego de dos semanas de intenso trabajo como " ROJO
           "diputado nacional" NORMAL ", "
           "te podes comprar una torre de %i pisos con tus dos hermanos. "
           "¿En que piso te gustaria vivir? [%i-%i]",
           MAX_VALOR_PISO, MIN_VALOR_PISO, MAX_VALOR_PISO);

    printf("\nEntrada: ");
    scanf("%i", &piso);
    while (!es_piso_valido(piso))
    {
        printf("\nInvalido! Recorda que el rango es [%i-%i]",
               MIN_VALOR_PISO, MAX_VALOR_PISO);

        printf("\nEntrada: ");
        scanf(" %i", &piso);
    }

    *ptr_piso = piso;
}

/**
 * Pide el nivel de grito al usuario, verificando que sea valido [1-18]
 * y almacena el valor en una direccion de memoria
 */
void leer_grito(int *ptr_grito)
{
    int grito;

    printf("¡Aaaah, Una " ROJO "ecuacion diferencial" NORMAL "! "
           "¿Que tan fuerte gritas? [%i-%i]",
           MIN_VALOR_GRITO, MAX_VALOR_GRITO);

    printf("\nEntrada: ");
    scanf("%i", &grito);

    while (!es_grito_valido(grito))
    {
        if (grito > EASTER_EGG)
        {
            printf("\n>>>>>>>>>>>>>>>"
                   "\n" ROJO "It's over %i! " NORMAL
                   "Recorda que el rango es [%i-%i]"
                   "\n>>>>>>>>>>>>>>>",
                   EASTER_EGG, MIN_VALOR_GRITO, MAX_VALOR_GRITO);
        }
        else
        {
            printf("\nInvalido! Recorda que el rango es [%i-%i]",
                   MIN_VALOR_GRITO, MAX_VALOR_GRITO);
        }
        printf("\nEntrada: ");
        scanf(" %i", &grito);
    }

    *ptr_grito = grito;
}

/**
 * Pre: Canal valido (A, M, L)
 * Post: Puntaje correspondiente al canal introducido (1, 2, 3)
 */
int calcular_puntaje_canal(char canal)
{
    int puntaje;
    if (canal == LIMPIEZA)
    {
        puntaje = PUNTAJE_LIMPIEZA;
    }
    else if (canal == ANIME)
    {
        puntaje = PUNTAJE_ANIME;
    }
    else if (canal == MUSICA_POP)
    {
        puntaje = PUNTAJE_MUSICA_POP;
    }
    return puntaje;
}

/**
 * Pre: Alimento valido (B, P, F)
 * Post: Puntaje correspondiente al alimento introducido (3, 6, 9)
 */
int calcular_puntaje_alimento(char alimento)
{
    int puntaje;
    if (alimento == FOCAS)
    {
        puntaje = PUNTAJE_FOCAS;
    }
    else if (alimento == BAMBU)
    {
        puntaje = PUNTAJE_BAMBU;
    }
    else if (alimento == PESCADO)
    {
        puntaje = PUNTAJE_PESCADO;
    }
    return puntaje;
}

/**
 * Pre: 
 *      - Canal valido (A, M, L)
 *      - Alimento valido (B, P, F)
 *      - Piso preferido valido [1-18]
 *      - Nivel de grito valido [1-18]
 * 
 * Post: Puntaje total dentro del rango [5-63]
 */
int calcular_puntaje_total(char canal, char alimento, int piso, int grito)
{
    int puntaje_canal = calcular_puntaje_canal(canal);
    int puntaje_alimento = calcular_puntaje_alimento(alimento);
    int puntaje_piso = piso;
    int puntaje_grito = grito;

    int puntaje_total = (puntaje_canal * puntaje_alimento) +
                        puntaje_piso + puntaje_grito;

    return puntaje_total;
}

/**
 * Pre: Puntaje total valido [5-63]
 * Post: Personalidad correspondiente al puntaje (I, P, G)
 */
char determinar_personalidad(int puntaje)
{
    char personalidad;
    if ((puntaje >= MIN_PUNTAJE_POLAR) && (puntaje <= MAX_PUNTAJE_POLAR))
    {
        personalidad = POLAR;
    }
    else if ((puntaje >= MIN_PUNTAJE_PANDA) && (puntaje <= MAX_PUNTAJE_PANDA))
    {
        personalidad = PANDA;
    }
    else if ((puntaje >= MIN_PUNTAJE_PARDO) && (puntaje <= MAX_PUNTAJE_PARDO))
    {
        personalidad = PARDO;
    }
    return personalidad;
}

/**
 * Imprime un texto personalizado dependiendo de la personalidad introducida
 * 
 * Pre: Personalidad valida (I, P, G)
 */
void imprimir_personalidad(char personalidad)
{
    if (personalidad == POLAR)
    {
        printf("Sos la " ROJO "insesiblidad " NORMAL "en persona,"
               "haces todo de manera fria y distante. "
               "Tus amigos se alejan de vos por tu " ROJO
               "agresividad " NORMAL
               "y conocimiento en las artes marciales. ");

        printf("\n- Polar (%c) -", POLAR);
    }
    else if (personalidad == PANDA)
    {
        printf("Sos el " ROJO "pinaculo " NORMAL "de la generacion de cristal, "
               "tenes una " ROJO "adiccion " NORMAL "a las redes sociales. "
               "Vivis en busca de un amor aunque "
               "tu madurez mental no te la permita. ");

        printf("\n- Panda (%c) -", PANDA);
    }
    else if (personalidad == PARDO)
    {
        printf("Tus objetivos en la vida tienen menos " ROJO
               "profundidad " NORMAL
               "que una ecuacion polinomica de 1er grado. "
               "Vivis tratando de hacer reir a la gente "
               "para ocultar tu " ROJO "vacio interno. " NORMAL);

        printf("\n- Pardo (%c) -", PARDO);
    }
}

int main()
{
    char canal;
    char alimento;
    int piso;
    int grito;

    system("clear");
    leer_canal(&canal);

    system("clear");
    leer_alimento(&alimento);

    system("clear");
    leer_piso(&piso);

    system("clear");
    leer_grito(&grito);

    int puntaje_total = calcular_puntaje_total(canal, alimento, piso, grito);
    char personalidad = determinar_personalidad(puntaje_total);

    system("clear");
    imprimir_personalidad(personalidad);

    printf("\n");
    return 0;
}

/** CORRECION
 * .Bools quedarian mas claros devolviendo directamente la condicion
 * .No es correcto hacer referencia al tipo de dato en una variale 
 *  (ej. ptr_canal)
 * .Algunas funciones quedaron un poco extensas
 */
