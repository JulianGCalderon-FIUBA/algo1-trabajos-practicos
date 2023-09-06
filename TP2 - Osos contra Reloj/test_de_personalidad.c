#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

//COLORES
#define ROJO "\x1b[31m"
#define NORMAL "\x1b[0m"

//OPCIONES
static const char ANIME = 'A';
static const char MUSICA_POP = 'M';
static const char LIMPIEZA = 'L';

static const char BAMBU = 'B';
static const char PESCADO = 'P';
static const char FOCAS = 'F';

static const int MIN_VALOR_PISO = 1;
static const int MAX_VALOR_PISO = 18;

static const int MIN_VALOR_GRITO = 1;
static const int MAX_VALOR_GRITO = 18;

//PUNTAJES
static const int PUNTAJE_LIMPIEZA = 1;
static const int PUNTAJE_ANIME = 2;
static const int PUNTAJE_MUSICA_POP = 3;

static const int PUNTAJE_FOCAS = 3;
static const int PUNTAJE_BAMBU = 6;
static const int PUNTAJE_PESCADO = 9;

//PERSONALIDADES
static const char POLAR = 'I';
static const int MIN_PUNTAJE_POLAR = 5;
static const int MAX_PUNTAJE_POLAR = 24;

static const char PANDA = 'P';
static const int MIN_PUNTAJE_PANDA = 25;
static const int MAX_PUNTAJE_PANDA = 43;

static const char PARDO = 'G';
static const int MIN_PUNTAJE_PARDO = 44;
static const int MAX_PUNTAJE_PARDO = 63;

//EASTER EGG
static const int EASTER_EGG = 9000;

/**
 * Post: TRUE si el canal esta dentro de (A, M, L)
 */
static bool es_canal_valido(char canal)
{
    return (canal == ANIME) ||
           (canal == MUSICA_POP) ||
           (canal == LIMPIEZA);
}

/**
 * Post: TRUE si el alimento esta dentro de (B, P, F)
 */
static bool es_alimento_valido(char alimento)
{
    return (alimento == BAMBU) ||
           (alimento == PESCADO) ||
           (alimento == FOCAS);
}

/**
 * Post: TRUE si el piso preferido esta dentro del rango [1-18]
 */
static bool es_piso_valido(int piso)
{
    return (piso >= MIN_VALOR_PISO) && (piso <= MAX_VALOR_PISO);
}

/**
 * Post: TRUE si el nivel de grito esta dentro del rango [1-18]
 */
static bool es_grito_valido(int grito)
{
    return (grito >= MIN_VALOR_GRITO) && (grito <= MAX_VALOR_GRITO);
}

/**
 * Pide el canal al usuario, verificando que sea valido (A, M, L)
 * y almacena el valor en una direccion de memoria
 */
static void leer_canal(char *canal)
{
    printf("Tus viejos dejaron la television prendida y quedo " ROJO
           "C5N" NORMAL ". "
           "¿A que categoria lo cambias?"
           "\n-Anime -> %c"
           "\n-Musica Pop -> %c"
           "\n-Limpieza -> %c",
           ANIME, MUSICA_POP, LIMPIEZA);

    printf("\nEntrada: ");
    scanf(" %c", canal);
    *canal = (char)toupper(*canal);
    while (!es_canal_valido(*canal))
    {
        printf("\nInvalido! Recorda que las opciones son:"
               "\n-Anime -> %c"
               "\n-Musica Pop -> %c"
               "\n-Limpieza -> %c",
               ANIME, MUSICA_POP, LIMPIEZA);

        printf("\nEntrada: ");
        scanf(" %c", canal);
        *canal = (char)toupper(*canal);
    }
}

/**
 * Pide el alimento al usuario, verificando que sea valido (B, P, F)
 * y almacena el valor en una direccion de memoria
 */
static void leer_alimento(char *alimento)
{

    printf("Felicidades! Conseguiste un boleto para escapar de " ROJO
           "latinoamerica " NORMAL
           "¿Que alimento llevas para el avion?"
           "\n-Bambu -> %c"
           "\n-Pescado -> %c"
           "\n-Focas -> %c",
           BAMBU, PESCADO, FOCAS);

    printf("\nEntrada: ");
    scanf(" %c", alimento);
    *alimento = (char)toupper(*alimento);
    while (!es_alimento_valido(*alimento))
    {
        printf("\nInvalido! Recorda que las opciones son:"
               "\n-Bambu -> %c"
               "\n-Pescado -> %c"
               "\n-Focas -> %c",
               BAMBU, PESCADO, FOCAS);

        printf("\nEntrada: ");
        scanf(" %c", alimento);
        *alimento = (char)toupper(*alimento);
    }
}

/**
 * Pide el piso preferido al usuario, verificando que sea valido [1-18]
 * y almacena el valor en una direccion de memoria
 */
static void leer_piso(int *piso)
{
    printf("Luego de dos semanas de intenso trabajo como " ROJO
           "diputado nacional" NORMAL ", "
           "te podes comprar una torre de %i pisos con tus dos hermanos. "
           "¿En que piso te gustaria vivir? [%i-%i]",
           MAX_VALOR_PISO, MIN_VALOR_PISO, MAX_VALOR_PISO);

    printf("\nEntrada: ");
    scanf("%i", piso);

    *piso = (char)toupper(*piso);
    while (!es_piso_valido(*piso))
    {
        printf("\nInvalido! Recorda que el rango es [%i-%i]",
               MIN_VALOR_PISO, MAX_VALOR_PISO);

        printf("\nEntrada: ");
        scanf(" %i", piso);
        *piso = (char)toupper(*piso);
    }
}

/**
 * Pide el nivel de grito al usuario, verificando que sea valido [1-18]
 * y almacena el valor en una direccion de memoria
 */
static void leer_grito(int *grito)
{
    printf("¡Aaaah, Una " ROJO "ecuacion diferencial" NORMAL "! "
           "¿Que tan fuerte gritas? [%i-%i]",
           MIN_VALOR_GRITO, MAX_VALOR_GRITO);

    printf("\nEntrada: ");
    scanf("%i", grito);
    *grito = (char)toupper(*grito);
    while (!es_grito_valido(*grito))
    {
        if (*grito > EASTER_EGG)
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
        scanf(" %i", grito);
        *grito = (char)toupper(*grito);
    }
}

/**
 * Pre: Canal valido (A, M, L)
 * Post: Puntaje correspondiente al canal introducido (1, 2, 3)
 */
static int calcular_puntaje_canal(char canal)
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
static int calcular_puntaje_alimento(char alimento)
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
static int calcular_puntaje_total(char canal, char alimento, int piso, int grito)
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
static char determinar_personalidad(int puntaje)
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
static void imprimir_personalidad(char personalidad)
{
    if (personalidad == POLAR)
    {
        printf("Sos la " ROJO "insensiblidad " NORMAL "en persona,"
               "haces todo de manera fria y distante. "
               "Tus amigos se alejan de vos por tu " ROJO
               "agresividad " NORMAL
               "y conocimiento en las artes marciales. \n");

        printf("\n- Polar (%c) -\n", POLAR);
    }
    else if (personalidad == PANDA)
    {
        printf("Sos el " ROJO "pinaculo " NORMAL "de la generacion de cristal, "
               "tenes una " ROJO "adiccion " NORMAL "a las redes sociales. "
               "Vivis en busca de un amor aunque "
               "tu madurez mental no te la permita. ");

        printf("\n- Panda (%c) -\n", PANDA);
    }
    else if (personalidad == PARDO)
    {
        printf("Tus objetivos en la vida tienen menos " ROJO
               "profundidad " NORMAL
               "que una ecuacion polinomica de 1er grado. "
               "Vivis tratando de hacer reir a la gente "
               "para ocultar tu " ROJO "vacio interno. " NORMAL);

        printf("\n- Pardo (%c) -\n", PARDO);
    }
}

void test_de_personalidad(char *personalidad_detectada)
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

    *personalidad_detectada = personalidad;
}
