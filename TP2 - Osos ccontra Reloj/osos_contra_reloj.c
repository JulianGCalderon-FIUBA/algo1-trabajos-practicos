#include <stdbool.h>
#include <string.h>
#include "utiles.h"

#include "osos_contra_reloj.h"

//COLORES
#define RESET_COLOR "\x1b[0m"
#define ROJO "\x1b[31m"
#define VERDE "\x1b[32m"
#define AZUL "\x1b[34m"
#define CYAN "\x1b[36m"
#define BLANCO "\x1b[37m"
#define AMARILLO "\x1b[33m"
#define MAX_COLOR 20

//TERRENO
#define MAX_FILAS_TERRENO 20
#define MAX_COLUMNAS_TERRENO 30

//CONTROLES
#define ARRIBA 'W'
#define IZQUIERDA 'A'
#define ABAJO 'S'
#define DERECHA 'D'
#define LINTERNA 'L'
#define VELA 'V'
#define BENGALA 'E'
#define TIEMPO_TOTAL 'T'

//ELEMENTOS
#define ARBOL 'A'
#define PIEDRA 'R'
#define KOALA 'K'
#define PILA 'B'

//PERSONAJES
#define POLAR 'I'
#define PARDO 'G'
#define PANDA 'P'
#define CHLOE 'C'
#define JUGADOR 'X'

//CANTIDADES
static const int CANTIDAD_ARBOLES = 350;
static const int CANTIDAD_PIEDRAS = 80;

static const int CANTIDAD_PILAS = 30;
static const int CANTIDAD_VELAS = 30;
static const int CANTIDAD_BENGALAS = 10;

//TIEMPO PERDIDO
static const double PERDIDA_ARBOL = 1;
static const double PERDIDA_ROCA = 2;
static const double PERDIDA_ARBOL_PARDO = 0.5;
static const double PERDIDA_ROCA_POLAR = 0;
static const int TIEMPO_LOCALIZACION_PANDA = 30;

//CONSUMIBLES
static const int DURACION_VELAS = 5;
static const int INICIAL_VELAS = 4;
static const int INICIAL_VELAS_POLAR = 6;

static const int INICIAL_LINTERNA = 10;
static const int INICIAL_LINTERNA_PARDO = 15;

static const int DURACION_BENGALAS = 3;
static const int DISTANCIA_BENGALAS = 3;
static const int INICIAL_BENGALAS = 0;
static const int INICIAL_BENGALAS_PANDA = 2;

static const int USOS_PILA = 5;

//UTILIDADES

/**
 * Inicializa los elementos del juego con valores predeterminados
 */
static void eliminar_basura(juego_t *juego)
{
    juego->cantidad_herramientas = 0;
    juego->cantidad_obstaculos = 0;

    juego->personaje.elemento_en_uso = -1;
    juego->personaje.cantidad_elementos = 0;
    juego->personaje.tiempo_perdido = 0;
    juego->personaje.ultimo_movimiento = DERECHA;
}

/**
 * PRE: El tiempo perdido por obstaculos
 * POST: El tiempo total del juego (tiempo_perdido + tiempo_actual)
 */
static double tiempo_total(double tiempo_perdido)
{
    return tiempo_perdido + tiempo_actual();
}

/**
 * POST:
 *  TRUE si la 2 coordenadas tienen los mismos valores.
 *  FALSE de lo contrario.
 */
static bool igual_posicion(coordenada_t pos1, coordenada_t pos2)
{
    return pos1.fil == pos2.fil &&
           pos1.col == pos2.col;
}

/**
 * POST: 
 *  TRUE si la coordenada no esta siendo ocupada por ningun elemento del juego
 *  FALSE de lo contrario
 */
static bool posicion_valida(juego_t juego, coordenada_t coordenada)
{
    bool valida = true;

    if (igual_posicion(juego.personaje.posicion, coordenada))
    {
        valida = false;
    }
    if (igual_posicion(juego.amiga_chloe, coordenada))
    {
        valida = false;
    }

    int i = 0;
    while (i < juego.cantidad_herramientas && valida)
    {
        if (igual_posicion(juego.herramientas[i].posicion, coordenada))
        {
            valida = false;
        }
        i++;
    }

    i = 0;
    while (i < juego.cantidad_obstaculos && valida)
    {
        if (igual_posicion(juego.obstaculos[i].posicion, coordenada))
        {
            valida = false;
        }
        i++;
    }

    return valida;
}

/**
 * POST: Devuelva una coordenada aleatoria del terreno, 
 *  verificando que este vacia
 */
static coordenada_t posicion_aleatoria_vacia(juego_t juego)
{
    coordenada_t coordenada;
    coordenada.fil = rand() % MAX_FILAS_TERRENO;
    coordenada.col = rand() % MAX_COLUMNAS_TERRENO;
    while (!posicion_valida(juego, coordenada))
    {
        coordenada.fil = rand() % MAX_FILAS_TERRENO;
        coordenada.col = rand() % MAX_COLUMNAS_TERRENO;
    }

    return coordenada;
}

/**
 * PRE: 'tipo' debe ser valido (A,R,K)
 *      'tope' < MAX_ELEMENTOS 
 * 
 * Agrega un elemento del tipo indicado en una 
 *  posicion aleatoria vacia del terreno.
 */
static void agregar_elemento(elemento_del_mapa_t elementos[],
                             int *tope, char tipo, juego_t *juego)
{
    elementos[*tope].posicion = posicion_aleatoria_vacia(*juego);
    elementos[*tope].tipo = tipo;
    (*tope)++;
}

/**
 * PRE: 'indice' < 'tope'
 * 
 * Elimina el elemento del vector en el indice (fisicamente)
 */
static void eliminar_elemento(elemento_del_mapa_t elementos[],
                              int *tope, int indice)
{
    (*tope)--;
    elementos[indice] = elementos[*tope];
}

/**
 * Descuenta un movimiento de la herramienta actualmente en uso (si hay)
 */
static void descontar_herramienta(personaje_t *personaje)
{
    if (personaje->elemento_en_uso != -1)
    {
        if (personaje->mochila[personaje->elemento_en_uso]
                .movimientos_restantes == 0)
        {
            personaje->elemento_en_uso = -1;
        }
        else
        {
            personaje->mochila[personaje->elemento_en_uso]
                .movimientos_restantes--;
        }
    }
}

/**
 * PRE: 
 *  'tipo' debe ser valido (E,B,V)
 *  'inicial' debe ser positivo
 * 
 * Agrega la herramienta con el tipo y la cantidad de usos indicada, en el
 *  vector de herramientas del personaje.
 */
static void equipar_herramienta(personaje_t *personaje, char tipo, int inicial)
{
    personaje->mochila[personaje->cantidad_elementos].tipo = tipo;
    personaje->mochila[personaje->cantidad_elementos].movimientos_restantes =
        inicial;
    personaje->cantidad_elementos++;
}

/**
 * POST:
 *  TRUE si el elemento es visible por la bengala.
 *  FALSE de lo contrario.
 */
static bool es_visible_por_bengala(coordenada_t bengala, coordenada_t elemento)
{
    int distancia = abs(bengala.fil - elemento.fil) +
                    abs(bengala.col - elemento.col);

    return distancia <= DISTANCIA_BENGALAS;
}

/**
 * PRE: 'ultimo_movimiento' debe ser valido (W,A,S,D)
 * POST:
 *  TRUE si el elemento es visible por una linterna.
 *  FALSE de lo contrario.
 */
static bool es_visible_por_linterna(coordenada_t personaje,
                                    char ultimo_movimiento, coordenada_t elemento)
{
    bool visible = false;

    switch (ultimo_movimiento)
    {
    case DERECHA:
        if (elemento.fil == personaje.fil &&
            elemento.col > personaje.col)
        {
            visible = true;
        }
        break;
    case IZQUIERDA:
        if (elemento.fil == personaje.fil &&
            elemento.col < personaje.col)
        {
            visible = true;
        }
        break;
    case ABAJO:
        if (elemento.fil > personaje.fil &&
            elemento.col == personaje.col)
        {
            visible = true;
        }
        break;
    case ARRIBA:
        if (elemento.fil < personaje.fil &&
            elemento.col == personaje.col)
        {
            visible = true;
        }
        break;
    }
    return visible;
}

/**
 * POST:
 *  TRUE si el elemento es visible por una vela.
 *  FALSE de lo contrario.
 */
static bool es_visible_por_vela(coordenada_t personaje, coordenada_t elemento)
{
    return abs(personaje.fil - elemento.fil) <= 1 &&
           abs(personaje.col - elemento.col) <= 1;
}

/**
 * PRE: 'personaje' debe tener una herramienta en uso
 * POST:
 *  TRUE si la coordenada es visible por la herramienta actualmente equipada
 *  FALSE de lo contrario
 */
static bool elemento_es_visible(coordenada_t elemento,
                                personaje_t personaje,
                                coordenada_t bengala)
{
    bool visible = false;
    switch (personaje.mochila[personaje.elemento_en_uso].tipo)
    {
    case BENGALA:
        visible = es_visible_por_bengala(bengala, elemento);
        break;
    case VELA:
        visible = es_visible_por_vela(personaje.posicion, elemento);
        break;
    case LINTERNA:
        visible = es_visible_por_linterna(personaje.posicion,
                                          personaje.ultimo_movimiento, elemento);
        break;
    }

    return visible;
}

//CORE

/**
 * Posiciona los personajes (Jugador y Chloe) en el terreno 
    en distintas posiciones
 */
static void posicionar_personajes(coordenada_t *personaje, coordenada_t *chloe)
{
    personaje->fil = rand() % MAX_FILAS_TERRENO;
    personaje->col = 0;

    bool posicion_libre = false;
    while (!posicion_libre)
    {
        posicion_libre = true;
        chloe->fil = rand() % MAX_FILAS_TERRENO;
        chloe->col = rand() % MAX_COLUMNAS_TERRENO;

        if (igual_posicion(*personaje, *chloe))
        {
            posicion_libre = false;
        }
    }
}

/**
 * Posiciona los obstaculos del juego, verificando que no se repitan elementos
    en una misma posicion
 */
static void posicionar_obstaculos(juego_t *juego)
{
    for (int i = 0; i < CANTIDAD_ARBOLES; i++)
    {
        agregar_elemento(juego->obstaculos, &(juego->cantidad_obstaculos),
                         ARBOL, juego);
    }

    for (int i = 0; i < CANTIDAD_PIEDRAS; i++)
    {
        agregar_elemento(juego->obstaculos, &(juego->cantidad_obstaculos),
                         PIEDRA, juego);
    }

    agregar_elemento(juego->obstaculos, &(juego->cantidad_obstaculos),
                     KOALA, juego);
}

/**
 * Posiciona las herramientas del juego, verificando que no se repitan elementos
 *  en una misma posicion
 */
static void posicionar_herramientas(juego_t *juego)
{
    for (int i = 0; i < CANTIDAD_PILAS; i++)
    {
        agregar_elemento(juego->herramientas, &(juego->cantidad_herramientas),
                         PILA, juego);
    }

    for (int i = 0; i < CANTIDAD_VELAS; i++)
    {
        agregar_elemento(juego->herramientas, &(juego->cantidad_herramientas),
                         VELA, juego);
    }

    for (int i = 0; i < CANTIDAD_BENGALAS; i++)
    {
        agregar_elemento(juego->herramientas, &(juego->cantidad_herramientas),
                         BENGALA, juego);
    }
}

/**
 * PRE: 'personaje' debe tener una personalidad asignada
 * 
 * Equipa al personaje con las herramientas debidas,
 *  en funcion de su personalidad.
 */
static void equipar_personaje(personaje_t *personaje)
{
    int inicial_velas = INICIAL_VELAS;
    int inicial_bengalas = INICIAL_BENGALAS;
    int inicial_linterna = INICIAL_LINTERNA;
    if (personaje->tipo == POLAR)
    {
        inicial_velas = INICIAL_VELAS_POLAR;
    }
    else if (personaje->tipo == PANDA)
    {
        inicial_bengalas = INICIAL_BENGALAS_PANDA;
    }
    else if (personaje->tipo == PARDO)
    {
        inicial_linterna = INICIAL_LINTERNA_PARDO;
    }

    equipar_herramienta(personaje, LINTERNA, inicial_linterna);

    for (int i = 0; i < inicial_velas; i++)
    {
        equipar_herramienta(personaje, VELA, DURACION_VELAS);
    }
    for (int i = 0; i < inicial_bengalas; i++)
    {
        equipar_herramienta(personaje, BENGALA, DURACION_BENGALAS);
    }
}

/**
 * Define a todos los elementos del mapa, excepto el jugador, como 'no visible'
 */
static void oscurecer_terreno(juego_t *juego)
{
    for (int i = 0; i < juego->cantidad_obstaculos; i++)
    {
        juego->obstaculos[i].visible = false;
    }

    for (int i = 0; i < juego->cantidad_herramientas; i++)
    {
        juego->herramientas[i].visible = false;
    }

    juego->chloe_visible = false;
}

/**
 * PRE: 'tope' <= 'MAX_ELEMENTOS'
 *      'personaje' debe tener una herramienta equipada
 * 
 * Calcula la visibilidad de todos los elementos partir de sus
 *  posiciones y la herramienta actualmente equipada.
 */
static void calcular_visibilidad_de_elementos(elemento_del_mapa_t elementos[],
                                              int tope,
                                              personaje_t personaje,
                                              coordenada_t bengala)
{
    for (int i = 0; i < tope; i++)
    {
        elementos[i].visible = elemento_es_visible(elementos[i].posicion,
                                                   personaje, bengala);
    }
}

/**
 * Analiza cada elemento del juego y define si es visible o no, 
 *  segun las condiciones del juego (herramienta equipada)
 *      -Y tiempo_perdido en caso de ser panda-
 */
static void calcular_visibilidad(juego_t *juego)
{
    if (juego->personaje.elemento_en_uso == -1)
    {
        oscurecer_terreno(juego);
    }
    else
    {
        coordenada_t bengala = {.fil = rand() % MAX_FILAS_TERRENO,
                                .col = rand() % MAX_COLUMNAS_TERRENO};

        calcular_visibilidad_de_elementos(juego->herramientas,
                                          juego->cantidad_herramientas,
                                          juego->personaje, bengala);
        calcular_visibilidad_de_elementos(juego->obstaculos,
                                          juego->cantidad_obstaculos,
                                          juego->personaje, bengala);

        juego->chloe_visible = elemento_es_visible(juego->amiga_chloe,
                                                   juego->personaje,
                                                   bengala);
    }

    if (juego->personaje.tipo == PANDA &&
        juego->personaje.tiempo_perdido >= TIEMPO_LOCALIZACION_PANDA)
    {
        juego->chloe_visible = true;
    }
}

/**
 * PRE: 'movimiento' debe ser valido (W,A,S,D)
 * 
 * Analiza la jugada, y mueve al personaje en la direccion deseada,
 *  de ser posible
 */
static void controlar_movimiento(coordenada_t *posicion, char movimiento)
{
    switch (movimiento)
    {
    case ARRIBA:
        if (posicion->fil != 0)
            posicion->fil--;
        break;

    case ABAJO:
        if (posicion->fil != MAX_FILAS_TERRENO - 1)
            posicion->fil++;
        break;

    case IZQUIERDA:
        if (posicion->col != 0)
            posicion->col--;
        break;

    case DERECHA:
        if (posicion->col != MAX_COLUMNAS_TERRENO - 1)
            posicion->col++;
        break;
    }
}

/**
 * PRE: 'elemento' debe estar inicializo
 * 
 * Interactura con el elemento ingresado
 */
static void interactuar_con_elemento(personaje_t *personaje,
                                     elemento_del_mapa_t elemento)
{
    switch (elemento.tipo)
    {
    case VELA:
        equipar_herramienta(personaje, VELA, DURACION_VELAS);
        break;
    case BENGALA:
        equipar_herramienta(personaje, BENGALA, DURACION_BENGALAS);
        break;
    case PILA:
        personaje->mochila[0].movimientos_restantes += USOS_PILA;
        break;
    case ARBOL:
        if (personaje->tipo == PARDO)
        {
            personaje->tiempo_perdido += PERDIDA_ARBOL_PARDO;
        }
        else
        {
            personaje->tiempo_perdido += PERDIDA_ARBOL;
        }
        break;
    case PIEDRA:
        if (personaje->tipo == POLAR)
        {
            personaje->tiempo_perdido += PERDIDA_ROCA_POLAR;
        }
        else
        {
            personaje->tiempo_perdido += PERDIDA_ROCA;
        }
        break;
    case KOALA:
        personaje->posicion.fil = rand() % MAX_FILAS_TERRENO;
        personaje->posicion.col = 0;
        break;
    }
}

/**
 * Busca la posicion en la que se encuentra el jugador, e interactua con
 *  el elemento que se encuentra en esa posicion
 */
static void interactuar_con_terreno(juego_t *juego)
{
    coordenada_t posicion = juego->personaje.posicion;

    int i = 0;
    bool hay_conincidencia = false;
    while (i < juego->cantidad_herramientas &&
           !hay_conincidencia)
    {
        if (igual_posicion(juego->herramientas[i].posicion, posicion))
        {
            hay_conincidencia = true;

            interactuar_con_elemento(&(juego->personaje), juego->herramientas[i]);
            eliminar_elemento(juego->herramientas,
                              &(juego->cantidad_herramientas), i);
        }
        i++;
    }

    i = 0;
    while (i < juego->cantidad_obstaculos &&
           !hay_conincidencia)
    {
        if (igual_posicion(juego->obstaculos[i].posicion, posicion))
        {
            hay_conincidencia = true;

            interactuar_con_elemento(&(juego->personaje), juego->obstaculos[i]);
        }
        i++;
    }
}

/**
 * PRE: 'herramienta' debe ser valida (V,E,L)
 * 
 * Verifica que la herramienta deseada tenga usos disponibles,
 *  y la activa (Desactivando la anterior si hay)
 *      -En caso de activar linterna, agrega KOALA-
 */
static void activar_herramienta(juego_t *juego, char herramienta)
{
    personaje_t *personaje = &(juego->personaje);

    bool encontrado = false;
    int i = 0;
    while (i < personaje->cantidad_elementos && !encontrado)
    {
        if (personaje->mochila[i].tipo == herramienta &&
            personaje->mochila[i].movimientos_restantes > 0)
        {
            encontrado = true;

            personaje->elemento_en_uso = i;

            if (herramienta == LINTERNA)
            {
                agregar_elemento(juego->obstaculos, &(juego->cantidad_obstaculos),
                                 KOALA, juego);
            }
        }
        i++;
    }
}

/**
 * PRE: 'herramienta' debe ser valida (V,E,L)
 * 
 * Decide si tiene que activar la herramienta ingresada, o desactivarla
 *      -BENGALA no se puede desactivar-
 */
static void controlar_herramienta(juego_t *juego, char herramienta)
{
    elemento_mochila_t elemento_actual = juego->personaje.mochila
                                             [juego->personaje.elemento_en_uso];

    if (juego->personaje.elemento_en_uso == -1)
    {
        activar_herramienta(juego, herramienta);
    }
    else
    {
        if (elemento_actual.tipo != BENGALA)
        {
            if (elemento_actual.tipo != herramienta)
            {
                activar_herramienta(juego, herramienta);
            }
            else
            {
                juego->personaje.elemento_en_uso = -1;
            }
        }
    }
}

// TERRENO

/**
 * Inicializa la matriz 'terreno' con el char ' ' en todas sus posiciones
 */
static void inicializar_terreno(char terreno[MAX_FILAS_TERRENO]
                                            [MAX_COLUMNAS_TERRENO])
{
    for (int i = 0; i < MAX_FILAS_TERRENO; i++)
    {
        for (int j = 0; j < MAX_COLUMNAS_TERRENO; j++)
        {
            terreno[i][j] = ' ';
        }
    }
}

/**
 * PRE: 'tope' <= MAX_ELEMENTOS
 * 
 * Agrega los elementos visibles del vector 'elementos' en la matriz 'terreno'
 */
static void cargar_al_terreno(char terreno[MAX_FILAS_TERRENO]
                                          [MAX_COLUMNAS_TERRENO],
                              elemento_del_mapa_t elementos[],
                              int tope)
{
    for (int i = 0; i < tope; i++)
    {
        if (elementos[i].visible)
        {
            int fil = elementos[i].posicion.fil;
            int col = elementos[i].posicion.col;

            terreno[fil][col] = elementos[i].tipo;
        }
    }
}

/**
 * Imprime un caracter de A-Z, 0-9, para cada columna, como referencia.
 */
static void imprimir_indice_columnas()
{
    for (int i = 0; i < MAX_COLUMNAS_TERRENO; i++)
    {
        if (i < 26)
        {
            printf("%c ", i + 65);
        }
        else
        {
            printf("%c ", i + 22);
        }
    }
    printf("\n");
}

/**
 * Imprime la matriz 'terreno' con los indices y los colores correspondientes,
 *  segun el tipo de elemento.
 */
static void imprimir_terreno(char terreno[MAX_FILAS_TERRENO]
                                         [MAX_COLUMNAS_TERRENO])
{
    imprimir_indice_columnas();
    for (int i = 0; i < MAX_FILAS_TERRENO; i++)
    {
        for (int j = 0; j < MAX_COLUMNAS_TERRENO; j++)
        {
            char letra = terreno[i][j];
            char color[MAX_COLOR] = RESET_COLOR;

            switch (letra)
            {
            case JUGADOR:
            case CHLOE:
                strcpy(color, AZUL);
                break;
            case ARBOL:
                strcpy(color, ROJO);
                break;
            case PIEDRA:
                strcpy(color, BLANCO);
                break;
            case KOALA:
                strcpy(color, CYAN);
                break;
            case PILA:
            case BENGALA:
            case VELA:
                strcpy(color, VERDE);
                break;
            }
            printf("%s%c " RESET_COLOR, color, letra);
        }
        printf("%c\n", i + 65);
    }
}

// VISUALES

/**
 * Imprime el terreno, mostrando solo los elementos visibles.
 */
static void visualizar_bosque(juego_t juego)
{
    char terreno[MAX_FILAS_TERRENO][MAX_COLUMNAS_TERRENO];
    inicializar_terreno(terreno);

    cargar_al_terreno(terreno, juego.obstaculos,
                      juego.cantidad_obstaculos);
    cargar_al_terreno(terreno, juego.herramientas,
                      juego.cantidad_herramientas);

    if (juego.chloe_visible)
    {
        coordenada_t chloe = juego.amiga_chloe;
        terreno[chloe.fil][chloe.col] = CHLOE;
    }

    coordenada_t jugador = juego.personaje.posicion;
    terreno[jugador.fil][jugador.col] = JUGADOR;

    imprimir_terreno(terreno);
}

/**
 * PRE: 'tipo debe ser valido (I,P,G)
 * Imprime las caracteristicas del personaje, dependiendo del tipo.
 */
static void imprimir_personaje(char tipo)
{
    switch (tipo)
    {
    case PANDA:
        printf(AZUL "Personaje: " RESET_COLOR "PANDA\n");
        printf(ROJO "Movimiento Caracterisico: " RESET_COLOR
                    "Tiene un excelente manejo de la \ntecnologia, "
                    "cuando se concentra es capaz de leer "
                    "los impulsos\nelectricos del aire. "
                    "Es capaz de LOCALIZAR A CHLOE en TODO\nMOMENTO, "
                    "incluso bajo condiciones climaticas extremas\n");
        break;
    case POLAR:
        printf(AZUL "Personaje: " RESET_COLOR "POLAR\n");
        printf(ROJO "Movimiento Caracterisico: " RESET_COLOR
                    "Despues de horas y horas de\n"
                    "enfrentamientos karatequisticos, "
                    "POLAR aprendio a SALTAR\nPIEDRAS. "
                    "Esta es una tecnica ancestral que pocos dominan.\n");
        break;
    case PARDO:
        printf(AZUL "Personaje:  " RESET_COLOR "PARDO\n");
        printf(ROJO "Movimiento Caracterisico: " RESET_COLOR
                    "Debido a su experiencia en "
                    "el area\nforestal, PARDO aprendio a "
                    "EVADIR ARBOLES."
                    " No solo eso, sino\nque tambien "
                    "lo hace con mucho estilo. (Galactico Facherdovich)\n");
        break;
    }
}

/**
 * Imprime los controles del juego (W,A,S,D,T,V,E,L)
 */
static void imprimir_controles()
{
    printf(AMARILLO "%c%c%c%c" RESET_COLOR ": MOVIMIENTO, " AMARILLO
                    "%c" RESET_COLOR ": TIEMPO RESTANTE\n",
           ARRIBA, IZQUIERDA, ABAJO, DERECHA, TIEMPO_TOTAL);
    printf(AMARILLO "%c" RESET_COLOR ": BENGALA, " AMARILLO "%c:" RESET_COLOR
                    " VELA, " AMARILLO "%c" RESET_COLOR ": LINTERNA\n",
           BENGALA, VELA, LINTERNA);
}

/**
 * (VELAS Y BENGALAS)
 * Cuenta cuantas unidades completas hay de cada herramienta, 
 *  y cuantos usos tiene la herramienta usada disponible (si hay).
 * 
 * Almacena la informacion en los punteros ingresados
 */
static void contar_herramientas(int *cant_bengalas, int *bengala_actual,
                                int *cant_velas, int *vela_actual,
                                personaje_t jugador)
{
    for (int i = 1; i < jugador.cantidad_elementos; i++)
    {
        switch (jugador.mochila[i].tipo)
        {
        case BENGALA:
            if (jugador.mochila[i].movimientos_restantes == DURACION_BENGALAS)
            {
                (*cant_bengalas)++;
            }
            else if (jugador.mochila[i].movimientos_restantes > 0)
            {
                *bengala_actual = jugador.mochila[i].movimientos_restantes;
            }
            break;

        case VELA:
            if (jugador.mochila[i].movimientos_restantes == DURACION_VELAS)
            {
                (*cant_velas)++;
            }
            else if (jugador.mochila[i].movimientos_restantes > 0)
            {
                *vela_actual = jugador.mochila[i].movimientos_restantes;
            }
            break;
        }
    }
}

/**
 * Imprime informacion sobre las herramientas disponibles del jugador,
    separando en las herramientas completas y las parcialmente usadas. 
 */
static void imprimir_herramientas(personaje_t jugador)
{
    int cant_bengalas = 0;
    int bengala_actual = 0;
    int cant_velas = 0;
    int vela_actual = 0;
    int usos_linterna = jugador.mochila[0].movimientos_restantes;

    contar_herramientas(&cant_bengalas, &bengala_actual,
                        &cant_velas, &vela_actual,
                        jugador);

    if (jugador.elemento_en_uso != -1)
    {
        printf(AZUL "En uso: " RESET_COLOR "%c\n",
               jugador.mochila[jugador.elemento_en_uso].tipo);
    }
    else
    {
        printf(AZUL "En uso: " RESET_COLOR "NADA\n");
    }

    printf(VERDE "Linterna: " RESET_COLOR "%i\n", usos_linterna);

    if (vela_actual == 0)
    {
        printf(VERDE "Velas: " RESET_COLOR "%ix\n", cant_velas);
    }
    else
    {
        printf(VERDE "Velas: " RESET_COLOR "%i + %ix\n",
               vela_actual, cant_velas);
    }

    if (bengala_actual == 0)
    {
        printf(VERDE "Bengalas: " RESET_COLOR "%ix\n", cant_bengalas);
    }
    else
    {
        printf(VERDE "Bengalas: " RESET_COLOR "%i + %ix\n",
               bengala_actual, cant_bengalas);
    }
}

// PRINCIPALES

void inicializar_juego(juego_t *juego, char tipo_personaje)
{
    juego->personaje.tipo = tipo_personaje;
    eliminar_basura(juego);

    posicionar_personajes(&(juego->personaje.posicion), &(juego->amiga_chloe));

    posicionar_obstaculos(juego);
    posicionar_herramientas(juego);

    equipar_personaje(&(juego->personaje));

    oscurecer_terreno(juego);
};

int estado_juego(juego_t juego)
{
    int estado = 0;
    if (juego.personaje.posicion.fil == juego.amiga_chloe.fil &&
        juego.personaje.posicion.col == juego.amiga_chloe.col)
    {
        estado = -1;
    }
    return estado;
};

void realizar_jugada(juego_t *juego, char jugada)
{

    switch (jugada)
    {
    case ARRIBA:
    case ABAJO:
    case IZQUIERDA:
    case DERECHA:
        juego->personaje.ultimo_movimiento = jugada;
        controlar_movimiento(&(juego->personaje.posicion), jugada);
        descontar_herramienta(&(juego->personaje));
        interactuar_con_terreno(juego);
        break;

    case LINTERNA:
    case BENGALA:
    case VELA:
        controlar_herramienta(juego, jugada);
        descontar_herramienta(&(juego->personaje));
        break;

    case TIEMPO_TOTAL:;
        printf("TIEMPO TOTAL: %.1f\n", tiempo_total(
                                           juego->personaje.tiempo_perdido));
        break;
    }

    calcular_visibilidad(juego);
}

void mostrar_juego(juego_t juego)
{
    visualizar_bosque(juego);
    printf("-----------------------------------------------------------\n");
    imprimir_personaje(juego.personaje.tipo);
    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    imprimir_herramientas(juego.personaje);
    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    imprimir_controles();
    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
};

bool es_jugada_valida(char jugada)
{
    return jugada == ARRIBA ||
           jugada == ABAJO ||
           jugada == IZQUIERDA ||
           jugada == DERECHA ||
           jugada == LINTERNA ||
           jugada == VELA ||
           jugada == BENGALA ||
           jugada == TIEMPO_TOTAL;
}
