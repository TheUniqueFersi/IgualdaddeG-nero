#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <time.h>
#include "../include/resources.c"
#define DEPURACION 1
#define GRAVEDAD 10
#define movimiento_SALTO_valor 150 //200: 130, Orig: 150
#define movimiento_CAMINAR_valor 5 //*2
#define LIMITE_MAIN_COLISION_LADO_IZQ_COORDENADAS 0 //para que el personaje no se salga de la pantalla
#define LIMITE_MAIN_COLISION_LADO_DER_COORDENADAS 1200 //para que el personaje no se salga de la pantalla
// -- PROTOTIPOS
int inicializar_allegro();
void main_camino_igualdad();

/* ----> ALLEGRO {TIPO DE DATOS} <---- */
// Displays
ALLEGRO_DISPLAY *disp;

// Eventos
ALLEGRO_EVENT_QUEUE *cola_eventos;
ALLEGRO_EVENT evento;

//SONIDO
ALLEGRO_SAMPLE_ID id_SAMPLE;
ALLEGRO_SAMPLE *SAMPLE;

//BITMAPS:
ALLEGRO_BITMAP *BITMAP;

//TIMERS
ALLEGRO_TIMER *timer_0_01, *timer_0_5;

//COLORES
ALLEGRO_FONT *fuente1;
/* ---- termina; ---- */


//Clovery
struct HITBOX {
    int MAIN_PERSONAJE_abajo_0_X;
    float MAIN_PERSONAJE_abajo_0_Y;

    int PERSONAJE_abajo_1_X;
    float PERSONAJE_abajo_1_Y;

    int PERSONAJE_2_X;
    float PERSONAJE_2_Y;

    int PERSONAJE_3_X;
    float PERSONAJE_3_Y;

    int CLOVERY_DAMAGE_X;
    float CLOVERY_DAMAGE_Y;
    int DAMAGE_RADIO;

    int CLOVERY_ATAQUE_X;
    float CLOVERY_ATAQUE_Y;
    int ATAQUE_RADIO;
};

struct HITBOX HITBOX_PANTALLA;
struct HITBOX HITBOX_LIMBO;
int altura_PERSONAJE = 100, largo_PERSONAJE = 50;
int localidad_CENTRO_PERSONAJE_coordenadas_x, localidad_CENTRO_PERSONAJE_coordenadas_y;

int momento = 0, nivel=1;

//HABILIDADES (Variables verificables pasando desde el documento binario)
int HABILIDAD_doble_salto = 2;

int SPRITES_posicion_personaje_COORDENADAS_X = 0;
float SPRITES_posicion_personaje_COORDENADAS_Y = -100;

//Localidades DE COLISIONES
//int posicion_COORDENADAS_anterior, posicion_COORDENADAS_actual;
int localidad_COORDENADAS, localidad_COORDENADAS_1, localidad_COORDENADAS_2_y, localidad_COORDENADAS_3_y, localidad_coordenadas_y;
int localizador_PERSONAJE_COORDENADAS[120][12][5], x, y, z;
int localizador_COINS_COORDENADAS[120][6][3];
int localizador_COVI_SIMPLES_COORDENADAS[120][6][3];

//LIMITES // COLISIONES
float LIMITE_piso_COORDENADAS = 500;//LIMITE Y (SE CAMBIA A CADA RATO)
int LIMITE_COLISION_LADO_IZQ_COORDENADAS = -1, LIMITE_COLISION_LADO_DER_COORDENADAS = -1, LIMITE_COLISION_ARRIBA_COORDENADAS = 250, LIMITE_COLISION_ESQUINA_ARRIBA_IZQ_COORDENADAS = -1, LIMITE_COLISION_ESQUINA_ARRIBA_DER_COORDENADAS = -1;


//CAMARA:
int posicion_CAMARA_COORDENADAS = 0,// Coordenadas para que se mueva la CAMARA
switch_mover_CAMARA_izquierda = 1,//0: si se puede mover la camara || 1: no se puede mover la CAMARA
switch_mover_CAMARA_derecha = 1;
int COINS_CAMARA = 0;

//LIMITES MAPA
int LIMITE_MAPA_IZQUIERDA = 0, LIMITE_MAPA_DERECHA = 12000;//Cambiar primera cuando se complete la oleada

//GRAVEDAD RELATED
int ESTADO_SALTO = 0;//Esta estara en records y pasara entre funciones para indicar si esta activada
int inc_time_0_5sec = 0;
int inc_time_1_sec = 0, TIME_FLOATING = 0, TIME_FALLING = 0;
float MULTIPLICADOR_impulso_arriba = 2, //POSICION Y INCIAL DEL PERSONAJE, no afecta la gravedad en este punto todavia***
POSICION_final_salto = 0;

//KEY_HOLDS
int KEY_HOLD_flecha_derecha = 0, KEY_HOLD_flecha_izquierda = 0, KEY_HOLD_flecha_abajo = 0, KEY_HOLD_X_ATACANDO_automatico = 0;
int KEY_PRESS_looking_side = 2;

/*Transiciones*/
void DIBUJO_transicion_oscurecimiento_en_negro()
{
    if(inc_time_1_sec % 5 == 0/* && inc_transparencia < 255*/)
        al_draw_filled_rectangle(0, 0, 1200, 600, al_map_rgba(0, 0, 0, 30));
}

void COLISIONES_MAIN()
{
    LIMITE_piso_COORDENADAS = 500;
    LIMITE_COLISION_ARRIBA_COORDENADAS = -100;
    LIMITE_COLISION_LADO_IZQ_COORDENADAS = -1;
    LIMITE_COLISION_LADO_DER_COORDENADAS = -1;
}
void verificador_COLISIONES()
{
    if(localizador_PERSONAJE_COORDENADAS[localidad_COORDENADAS][0][0] == 1)
    {
        if(localizador_PERSONAJE_COORDENADAS[localidad_COORDENADAS][localidad_coordenadas_y][4] != 0)
        {
            if(localizador_PERSONAJE_COORDENADAS[localidad_COORDENADAS][localidad_coordenadas_y][0] != 0)//PISO
                LIMITE_piso_COORDENADAS = localizador_PERSONAJE_COORDENADAS[localidad_COORDENADAS][localidad_coordenadas_y][0];
            else
                LIMITE_piso_COORDENADAS = 500;
            if(localizador_PERSONAJE_COORDENADAS[localidad_COORDENADAS][localidad_coordenadas_y][1] != 0)//IZQUIERDA
                LIMITE_COLISION_LADO_IZQ_COORDENADAS = localizador_PERSONAJE_COORDENADAS[localidad_COORDENADAS][localidad_coordenadas_y][1];
            else
                LIMITE_COLISION_LADO_IZQ_COORDENADAS = -1;
            if(localizador_PERSONAJE_COORDENADAS[localidad_COORDENADAS][localidad_coordenadas_y][2] != 0)//DERECHA
                LIMITE_COLISION_LADO_DER_COORDENADAS = localizador_PERSONAJE_COORDENADAS[localidad_COORDENADAS][localidad_coordenadas_y][2];
            else
                LIMITE_COLISION_LADO_DER_COORDENADAS = -1;
            if(localizador_PERSONAJE_COORDENADAS[localidad_COORDENADAS][localidad_coordenadas_y][3] != 0)//ARRIBA
                LIMITE_COLISION_ARRIBA_COORDENADAS = localizador_PERSONAJE_COORDENADAS[localidad_COORDENADAS][localidad_coordenadas_y][3];
            else
                LIMITE_COLISION_ARRIBA_COORDENADAS = -100;
        }
        else
            COLISIONES_MAIN();

        if(localizador_PERSONAJE_COORDENADAS[localidad_COORDENADAS][localidad_COORDENADAS_2_y][1] != 0)
            LIMITE_COLISION_ESQUINA_ARRIBA_IZQ_COORDENADAS = localizador_PERSONAJE_COORDENADAS[localidad_COORDENADAS][localidad_COORDENADAS_2_y][1];
        else
            LIMITE_COLISION_ESQUINA_ARRIBA_IZQ_COORDENADAS = -1;
        if(localizador_PERSONAJE_COORDENADAS[localidad_COORDENADAS][localidad_COORDENADAS_3_y][2] != 0)
            LIMITE_COLISION_ESQUINA_ARRIBA_DER_COORDENADAS = localizador_PERSONAJE_COORDENADAS[localidad_COORDENADAS][localidad_COORDENADAS_3_y][2];
        else
            LIMITE_COLISION_ESQUINA_ARRIBA_DER_COORDENADAS = -1;
    }
    else
        COLISIONES_MAIN();

    if (localidad_COORDENADAS == localidad_COORDENADAS_1 && HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_Y != LIMITE_piso_COORDENADAS && ESTADO_SALTO == 1)
        ESTADO_SALTO = 0;

}
void inicializador_matriz_COLISIONES()
{
    int verif = 0;
    for (x=0; x<120; x++)
    {
        for(y=0; y<12; y++)
        {
            for(z=0; z<5; z++)
            {
                localizador_PERSONAJE_COORDENADAS[x][y][z] = 0;
                verif++;
            }
        }
    }

    if (nivel == 1) //plataformas Américan posibles colisiones en 2000 y en 8800
    {
        localizador_PERSONAJE_COORDENADAS[4][0][0] = 1;
        localizador_PERSONAJE_COORDENADAS[4][8][4] = 1;
        localizador_PERSONAJE_COORDENADAS[4][8][0] = 400; //[X][Y][0] = PISO
    }
}

void DIBUJO_barras_sanidad(int coord_x, int coord_y, int inicio_barra_inf)
{
    //Barras de Sanidad //int 25, 10
    int infeccion_y = coord_y+50;

    /*
    if(var_PANTALLA_barra_salud >=10)
        al_draw_filled_rounded_rectangle(coord_x, coord_y, (coord_x+350)-elemento_PANTALLA_barra_salud, coord_y+20, 10, 10, al_map_rgb(0, 255, 136));
    if(var_PANTALLA_barra_infeccion >= 10)
        al_draw_filled_rounded_rectangle(coord_x, infeccion_y, inicio_barra_inf+elemento_PANTALLA_barra_infeccion, infeccion_y+20, 10, 10, al_map_rgb(255, 45, 0));

        */
}
void DIBUJO_SPRITES_personaje() {
    int FLIP_horizontal;
    if (KEY_PRESS_looking_side == 1)
        FLIP_horizontal = ALLEGRO_FLIP_HORIZONTAL;
    else
        FLIP_horizontal = 0;
    al_draw_filled_rectangle(SPRITES_posicion_personaje_COORDENADAS_X - 15,
                             SPRITES_posicion_personaje_COORDENADAS_Y - 10, SPRITES_posicion_personaje_COORDENADAS_X + 100, SPRITES_posicion_personaje_COORDENADAS_Y + 200, al_map_rgba(255,255,255,100));
    printf("%i\n", SPRITES_posicion_personaje_COORDENADAS_X);
    printf("%f\n", SPRITES_posicion_personaje_COORDENADAS_Y);
    /*
    if (ESTADO_SALTO == 1 && KEY_HOLD_flecha_izquierda == 0 && KEY_HOLD_flecha_derecha == 0 &&
        KEY_HOLD_flecha_abajo == 0 && !(HITBOX_LIMBO.PERSONAJE_2_Y < LIMITE_COLISION_ARRIBA_COORDENADAS || HITBOX_LIMBO.PERSONAJE_3_Y < LIMITE_COLISION_ARRIBA_COORDENADAS) && KEY_HOLD_X_ATACANDO_automatico == 0) {

        if (inc_time_SPRITES <= 9) {
            al_draw_scaled_bitmap(SPRITES_CLOVERY_sin_mov[0], 0, 0, 750, 1200,
                                  SPRITES_posicion_personaje_COORDENADAS_X - 15,
                                  SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
        } else if (inc_time_SPRITES <= 18)
            al_draw_scaled_bitmap(SPRITES_CLOVERY_sin_mov[1], 0, 0, 750, 1200,
                                  SPRITES_posicion_personaje_COORDENADAS_X - 15,
                                  SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
        else if (inc_time_SPRITES <= 27)
            al_draw_scaled_bitmap(SPRITES_CLOVERY_sin_mov[2], 0, 0, 750, 1200,
                                  SPRITES_posicion_personaje_COORDENADAS_X - 15,
                                  SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
        else if (inc_time_SPRITES <= 36)
            al_draw_scaled_bitmap(SPRITES_CLOVERY_sin_mov[3], 0, 0, 750, 1200,
                                  SPRITES_posicion_personaje_COORDENADAS_X - 15,
                                  SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
        else if (inc_time_SPRITES <= 45)
            al_draw_scaled_bitmap(SPRITES_CLOVERY_sin_mov[4], 0, 0, 750, 1200,
                                  SPRITES_posicion_personaje_COORDENADAS_X - 15,
                                  SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
        else if (inc_time_SPRITES <= 54)
            al_draw_scaled_bitmap(SPRITES_CLOVERY_sin_mov[5], 0, 0, 750, 1200,
                                  SPRITES_posicion_personaje_COORDENADAS_X - 15,
                                  SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
    } else if (KEY_HOLD_flecha_abajo == 1 && ESTADO_SALTO == 1 && KEY_HOLD_X_ATACANDO_automatico == 0) {

        al_draw_scaled_bitmap(SPRITE_CLOVERY_agachada, 0, 0, 750, 1200,
                              SPRITES_posicion_personaje_COORDENADAS_X - 15,
                              SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
    } else if (ESTADO_SALTO == 4 && KEY_HOLD_X_ATACANDO_automatico == 0)
        al_draw_scaled_bitmap(SPRITE_CLOVY_elevacion, 0, 0, 750, 1200, SPRITES_posicion_personaje_COORDENADAS_X - 15,
                              SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
    else if (ESTADO_SALTO == 0 && KEY_HOLD_X_ATACANDO_automatico == 0)
        al_draw_scaled_bitmap(SPRITE_CLOVY_caida, 0, 0, 750, 1200, SPRITES_posicion_personaje_COORDENADAS_X - 15,
                              SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
    else if ((KEY_HOLD_flecha_izquierda == 1 || KEY_HOLD_flecha_derecha == 1) && ESTADO_SALTO == 1 && !(HITBOX_LIMBO.PERSONAJE_2_Y < LIMITE_COLISION_ARRIBA_COORDENADAS || HITBOX_LIMBO.PERSONAJE_3_Y < LIMITE_COLISION_ARRIBA_COORDENADAS) && KEY_HOLD_X_ATACANDO_automatico == 0) {
        if (inc_time_SPRITES_corriendo <= 11)
            al_draw_scaled_bitmap(SPRITES_CLOVY_correr[0], 0, 0, 750, 1200,
                                  SPRITES_posicion_personaje_COORDENADAS_X - 15,
                                  SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
        else if (inc_time_SPRITES_corriendo <= 22)
            al_draw_scaled_bitmap(SPRITES_CLOVY_correr[1], 0, 0, 750, 1200,
                                  SPRITES_posicion_personaje_COORDENADAS_X - 15,
                                  SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
        else if (inc_time_SPRITES_corriendo <= 33)
            al_draw_scaled_bitmap(SPRITES_CLOVY_correr[2], 0, 0, 750, 1200,
                                  SPRITES_posicion_personaje_COORDENADAS_X - 15,
                                  SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
        else if (inc_time_SPRITES_corriendo <= 44)
            al_draw_scaled_bitmap(SPRITES_CLOVY_correr[3], 0, 0, 750, 1200,
                                  SPRITES_posicion_personaje_COORDENADAS_X - 15,
                                  SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
    }
    if((HITBOX_LIMBO.PERSONAJE_2_Y < LIMITE_COLISION_ARRIBA_COORDENADAS || HITBOX_LIMBO.PERSONAJE_3_Y < LIMITE_COLISION_ARRIBA_COORDENADAS  && (KEY_HOLD_flecha_derecha == 0 && KEY_HOLD_flecha_izquierda == 0)) && KEY_HOLD_X_ATACANDO_automatico == 0)
    {
        al_draw_scaled_bitmap(SPRITE_CLOVERY_agachada, 0, 0, 750, 1200,
                              SPRITES_posicion_personaje_COORDENADAS_X - 15,
                              SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
    }
    if(HITBOX_PANTALLA.DAMAGE_RADIO == 25 && (KEY_HOLD_flecha_izquierda == 1 || KEY_HOLD_flecha_derecha == 1) && KEY_HOLD_X_ATACANDO_automatico == 0)
    {
        if (inc_time_SPRITES_corriendo <= 11)
            al_draw_scaled_bitmap(SPRITES_CLOVERY_agachada[0], 0, 0, 750, 1200,
                                  SPRITES_posicion_personaje_COORDENADAS_X - 15,
                                  SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
        else if (inc_time_SPRITES_corriendo <= 22)
            al_draw_scaled_bitmap(SPRITES_CLOVERY_agachada[1], 0, 0, 750, 1200,
                                  SPRITES_posicion_personaje_COORDENADAS_X - 15,
                                  SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
        else if (inc_time_SPRITES_corriendo <= 33)
            al_draw_scaled_bitmap(SPRITES_CLOVERY_agachada[2], 0, 0, 750, 1200,
                                  SPRITES_posicion_personaje_COORDENADAS_X - 15,
                                  SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
        else if (inc_time_SPRITES_corriendo <= 44)
            al_draw_scaled_bitmap(SPRITES_CLOVERY_agachada[3], 0, 0, 750, 1200,
                                  SPRITES_posicion_personaje_COORDENADAS_X - 15,
                                  SPRITES_posicion_personaje_COORDENADAS_Y - 10, 75, 120, FLIP_horizontal);
    }*/
/*
    if(KEY_HOLD_X_ATACANDO_automatico == 1)
    {
        if(inc_time_SPRITES_ataque <= 4)
            al_draw_scaled_bitmap(SPRITES_CLOVY_ataque[0], 160, 760, 375, 475, SPRITES_posicion_personaje_COORDENADAS_X-10, SPRITES_posicion_personaje_COORDENADAS_Y-3, 90, 130, FLIP_horizontal);

        if(inc_time_SPRITES_ataque <= 8 && inc_time_SPRITES_ataque > 4)
            al_draw_scaled_bitmap(SPRITES_CLOVY_ataque[1], 160, 760, 375, 475, SPRITES_posicion_personaje_COORDENADAS_X-10, SPRITES_posicion_personaje_COORDENADAS_Y-3, 90, 130, FLIP_horizontal);

        if(inc_time_SPRITES_ataque <= 12 && inc_time_SPRITES_ataque > 8)
            al_draw_scaled_bitmap(SPRITES_CLOVY_ataque[2], 200, 630, 320, 550, SPRITES_posicion_personaje_COORDENADAS_X-15, SPRITES_posicion_personaje_COORDENADAS_Y-18, 90, 130, FLIP_horizontal);

        if(inc_time_SPRITES_ataque <= 16 && inc_time_SPRITES_ataque > 12)
            al_draw_scaled_bitmap(SPRITES_CLOVY_ataque[3], 200, 630, 320, 550, SPRITES_posicion_personaje_COORDENADAS_X-15, SPRITES_posicion_personaje_COORDENADAS_Y-18, 90, 130, FLIP_horizontal);

        if(inc_time_SPRITES_ataque <= 20 && inc_time_SPRITES_ataque > 16)
            al_draw_scaled_bitmap(SPRITES_CLOVY_ataque[4], 200, 630, 320, 550, SPRITES_posicion_personaje_COORDENADAS_X-15, SPRITES_posicion_personaje_COORDENADAS_Y-18, 90, 130, FLIP_horizontal);

        if(inc_time_SPRITES_ataque <= 25 && inc_time_SPRITES_ataque > 20)
            al_draw_scaled_bitmap(SPRITES_CLOVY_ataque[5], 0, 600, 800, 600, SPRITES_posicion_personaje_COORDENADAS_X-70, SPRITES_posicion_personaje_COORDENADAS_Y-60, 225, 190, FLIP_horizontal);

        if(inc_time_SPRITES_ataque <= 30 && inc_time_SPRITES_ataque > 25)
            al_draw_scaled_bitmap(SPRITES_CLOVY_ataque[6], 0, 600, 800, 600, SPRITES_posicion_personaje_COORDENADAS_X-70, SPRITES_posicion_personaje_COORDENADAS_Y-60, 225, 190, FLIP_horizontal);

        if(inc_time_SPRITES_ataque <= 35 && inc_time_SPRITES_ataque > 30)
            al_draw_scaled_bitmap(SPRITES_CLOVY_ataque[7], 0, 600, 800, 600, SPRITES_posicion_personaje_COORDENADAS_X-70, SPRITES_posicion_personaje_COORDENADAS_Y-60, 225, 190, FLIP_horizontal);

    }
    */
}

ALLEGRO_COLOR color_fondo;
void DIBUJO_fondo()//en num_pantalla = 5
{
    color_fondo = al_map_rgb(0 ,75, 158);
    al_clear_to_color(color_fondo);

    //INSERTAR IMAGEN BITMAP DEL MAPA
    /*
    if (num_mundos == 1)
        al_draw_scaled_bitmap(cielo_america, posicion_CAMARA_COORDENADAS, 0, 1200, 600, 0, 0, 1200, 600, 0);
    if (num_mundos == 2)
        al_draw_scaled_bitmap(cielo_europa, posicion_CAMARA_COORDENADAS, 0, 1200, 600, 0, 0, 1200, 600, 0);
    */

    //PISO
    //al_draw_scaled_bitmap(PISO, posicion_CAMARA_COORDENADAS, 0, 12000, 600, 0, 0, 12000, 600, 0);

    // Plano 4.2.1 = Interfaz de elementos POINTS.
    ALLEGRO_COLOR COLOR_barritas;
    COLOR_barritas = al_map_rgb(255, 4, 78);

    //Recuadro que oscurece los datos en la pantalla //arriba
    al_draw_filled_rectangle(0, 0, 1200, 100, al_map_rgba(0, 0, 0, 100));
    al_draw_filled_rectangle(400, 0, 700, 100, al_map_rgba(0, 0, 0, 100));// POINTS vidas
    al_draw_filled_rectangle(900, 0, 1200, 100, al_map_rgba(0, 0, 0, 100));// POINTS vidas
    //al_draw_filled_rectangle(0, 100, 100, 250, al_map_rgba(0, 0, 0, 100));

    al_draw_line(400, 0, 400, 100, COLOR_barritas, 5);
    al_draw_line(500, 0, 500, 100, COLOR_barritas, 5);
    al_draw_line(700, 0, 700, 100, COLOR_barritas, 5);
    al_draw_line(400, 100, 700, 100, COLOR_barritas, 5);
    //al_draw_line(750, 0, 750, 100, COLOR_barritas, 5);
    al_draw_line(900, 0, 900, 100, COLOR_barritas, 5);
    al_draw_line(1050, 0, 1050, 100, COLOR_barritas, 5);
    al_draw_line(1200, 0, 1200, 100, COLOR_barritas, 5);
    al_draw_line(900, 100, 1200, 100, COLOR_barritas, 5);

    /*
    al_draw_scaled_bitmap(COIN, 0, 0, 500, 500, 980, 52, 40, 40, 0);
    al_draw_scaled_bitmap(CLOVER_VIDAS, 0, 0, 500, 500, 450, 50, 50, 50, 0);

     */


    //abajo
    //al_draw_filled_rectangle(0, 500, 400, 600, al_map_rgba(0, 0, 0, 100));//SI HAY MENSAJES, IMPRIMIR
    al_draw_filled_rectangle(500, 550, 700, 600, al_map_rgba(0, 0, 0, 200));
    al_draw_filled_rectangle(900, 500, 1200, 600, al_map_rgba(0, 0, 0, 200));


    //Texto POINTS
    /*
    al_draw_text(fuente_pixeled_16, al_map_rgb(255, 255, 255), 50, 3, ALLEGRO_ALIGN_LEFT, POINTS_barra_vida);//BARRA DE VIDA
    al_draw_text(fuente_pixeled_16, al_map_rgb(255, 255, 255), 50, 50, ALLEGRO_ALIGN_LEFT, POINTS_barra_infeccion);//BARRA DE INFECCION

    al_draw_text(fuente_pixeled_23, al_map_rgb(0, 255, 102), 450, 15, ALLEGRO_ALIGN_CENTER, POINTS_string_VIDAS);//vidas
    al_draw_text(fuente_pixeled_27, al_map_rgb(0, 255, 122), 450, 60, ALLEGRO_ALIGN_RIGHT, itoa(POINTS_vidas, POINTS_itoa_vidas, 10));//POINTS Vidas iota
    al_draw_text(fuente_pixeled_27, al_map_rgb(40, 245, 255), 600, 5,  ALLEGRO_ALIGN_CENTER, POINTS_string_OBJETOS);//objetos
    //al_draw_text(fuente_pixeled_16, al_map_rgb(127, 165, 255), 700, 5, ALLEGRO_ALIGN_LEFT, POINTS_string_BOMBAS);//POINTS_BOMBAS
    //al_draw_text(fuente_pixeled_23, al_map_rgb(203, 181, 0), 800, 5, ALLEGRO_ALIGN_CENTER, POINTS_string_CORONACOINS);//CORONACOINS
    //al_draw_text(fuente_pixeled_23, al_map_rgb(203, 181, 0), 850, 30, ALLEGRO_ALIGN_CENTER, POINTS_string_CORONACOINS_2);//CORONACOINS
    //al_draw_text(fuente_pixeled_27, al_map_rgb(255, 255, 0), 800, 60, ALLEGRO_ALIGN_CENTER, itoa(POINTS_corona_coins, POINTS_itoa_coronacoins, 10));//CORONACOINS itoa
    al_draw_text(fuente_pixeled_23, al_map_rgb(255, 255, 0), 950, 5, ALLEGRO_ALIGN_CENTER, POINTS_string_CLOVERCOINS);
    al_draw_text(fuente_pixeled_23, al_map_rgb(255, 255, 0), 1000, 30, ALLEGRO_ALIGN_CENTER, POINTS_string_CLOVERCOINS_2);
    al_draw_text(fuente_pixeled_27, al_map_rgb(255, 255, 0), 950, 60, ALLEGRO_ALIGN_CENTER, itoa(POINTS_clover_coins, POINTS_itoa_clovercoins, 10));
    al_draw_text(fuente_pixeled_23, al_map_rgb(255, 255, 255), 1125, 15, ALLEGRO_ALIGN_CENTER, POINTS_string_PUNTAJE);
    al_draw_text(fuente_pixeled_27, al_map_rgb(255, 255, 255), 1125, 60, ALLEGRO_ALIGN_CENTER, itoa(POINTS_puntaje, POINTS_itoa_puntaje, 10));
    al_draw_text(fuente_pixeled_27, al_map_rgb(255, 255, 255), 600, 560, ALLEGRO_ALIGN_CENTER, opcion[num_mundos-1]);
    //al_draw_text(fuente2, al_map_rgb(0, 0, 0), 600, 510, ALLEGRO_ALIGN_CENTER, opcion[num_mundos-1]);

    //al_draw_scaled_bitmap(CLOVER_VIDAS, 0, 0, 1000, 500, )
    al_draw_text(fuente_pixeled_27, al_map_rgb(255, 255, 255), 925, 510, ALLEGRO_ALIGN_CENTER, "1");
    al_draw_text(fuente_pixeled_27, al_map_rgb(255, 255, 255), 925, 560, ALLEGRO_ALIGN_CENTER, "2");
    al_draw_text(fuente_pixeled_27, al_map_rgb(255, 255, 255), 1075, 510, ALLEGRO_ALIGN_CENTER, "3");

    al_draw_line(900, 500, 900, 600, al_map_rgb(255, 4, 78), 5);
    al_draw_line(1050, 500, 1050, 600, al_map_rgb(255, 4, 78), 5);
    al_draw_line(900, 500, 1200, 500, al_map_rgb(255, 4, 78), 5);
    al_draw_line(900, 550, 1200, 550, al_map_rgb(255, 4, 78), 5);
*/

    //DIBUJO_COVIS();
    //DIBUJO_indicadores_VACUNAS();
    DIBUJO_barras_sanidad(25, 20, 0);
    DIBUJO_SPRITES_personaje();
    //DIBUJO_COINS();
    //DIBUJO_COVI_SIMPLES();
}

int verificador_mover_CAMARA()//CAMBIANTE
{
    int capturado_recuadro_camara = 0;
    if(HITBOX_PANTALLA.PERSONAJE_abajo_1_X  >= 600 && KEY_HOLD_flecha_derecha == 1 && switch_mover_CAMARA_derecha == 1)
    {
        posicion_CAMARA_COORDENADAS += movimiento_CAMINAR_valor;
        HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_X += movimiento_CAMINAR_valor;
        capturado_recuadro_camara = 1;
        COINS_CAMARA += movimiento_CAMINAR_valor;
    }
    if(HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_X <= 600 && KEY_HOLD_flecha_izquierda == 1 && switch_mover_CAMARA_izquierda == 1)
    {
        posicion_CAMARA_COORDENADAS -= movimiento_CAMINAR_valor;
        HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_X -= movimiento_CAMINAR_valor;
        capturado_recuadro_camara = 1;
        COINS_CAMARA -= movimiento_CAMINAR_valor;
    }
    return capturado_recuadro_camara;
}
void verificador_LIMITE_COLISION_LADO_IZQ()
{
    if((HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_X != LIMITE_COLISION_LADO_IZQ_COORDENADAS && HITBOX_LIMBO.PERSONAJE_2_X != LIMITE_COLISION_ESQUINA_ARRIBA_IZQ_COORDENADAS) && HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_X != LIMITE_MAIN_COLISION_LADO_IZQ_COORDENADAS && verificador_mover_CAMARA() != 1)
    {
        HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_X -= movimiento_CAMINAR_valor;
        HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_X -= movimiento_CAMINAR_valor;
        SPRITES_posicion_personaje_COORDENADAS_X -= movimiento_CAMINAR_valor;
    }
}
void verificador_LIMITE_COLISION_LADO_DER() {
    if ((HITBOX_LIMBO.PERSONAJE_abajo_1_X != LIMITE_COLISION_LADO_DER_COORDENADAS && HITBOX_LIMBO.PERSONAJE_3_X != LIMITE_COLISION_ESQUINA_ARRIBA_DER_COORDENADAS) && (HITBOX_PANTALLA.PERSONAJE_abajo_1_X) != LIMITE_MAIN_COLISION_LADO_DER_COORDENADAS && verificador_mover_CAMARA() != 1) {
        HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_X += movimiento_CAMINAR_valor;
        HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_X += movimiento_CAMINAR_valor;
        SPRITES_posicion_personaje_COORDENADAS_X += movimiento_CAMINAR_valor;
    }
}
void verificador_LIMITES_MAPA()
{
    if(posicion_CAMARA_COORDENADAS <= LIMITE_MAPA_IZQUIERDA)
        switch_mover_CAMARA_izquierda = 0;
    if(posicion_CAMARA_COORDENADAS + 1200 >= LIMITE_MAPA_DERECHA)
        switch_mover_CAMARA_derecha = 0;
}
void verificador_LIMITE_COLISION_ARRIBA()
{
    if(HITBOX_PANTALLA.PERSONAJE_2_Y <= LIMITE_COLISION_ARRIBA_COORDENADAS)//CAMBIAR A LIMBO (EN GENERAL, TODAS LAS COLISIONES TIENEN QUE ESTAR RELACIONADAS CON LIMBO
        ESTADO_SALTO = 0;
}

void eventos_gravedad ()
{
    float MULTIPLICADOR_gravedad = 0.1; //ACELERACION DE LA GRAVEDAD
    //Flotando Y subiendo

    if (ESTADO_SALTO == 3) //FLOTANDO
    {
        //al_play_sample(salto_sonido, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        POSICION_final_salto = HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y - movimiento_SALTO_valor; //LIMBO
        ESTADO_SALTO = 4;
    }
    if (ESTADO_SALTO == 4)//SUBIENDO
    {
        verificador_LIMITE_COLISION_ARRIBA();
        if (HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y >= POSICION_final_salto + (movimiento_SALTO_valor*0.75)) //aplicador de aceleracion
            MULTIPLICADOR_impulso_arriba = 1.7;

        else if (HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y >= POSICION_final_salto + (movimiento_SALTO_valor*0.35))
            MULTIPLICADOR_impulso_arriba = 1.3;//1.5

        else if (HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y >= POSICION_final_salto + (movimiento_SALTO_valor*0.15))
            MULTIPLICADOR_impulso_arriba = 0.8;//1.1

        else if (HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y >= POSICION_final_salto + (movimiento_SALTO_valor*0.025))
            MULTIPLICADOR_impulso_arriba = 0.3;//0.8

        else if (HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y >= POSICION_final_salto + (movimiento_SALTO_valor*0.015))
            MULTIPLICADOR_impulso_arriba = 0.1;//0.5

        if (HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y != POSICION_final_salto && TIME_FLOATING % 2 == 0) //****************TIEMPO EN EL QUE SE EFECTUA EL SALTO
        {
            HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_Y -= 10 * MULTIPLICADOR_impulso_arriba;//******************************VALOR DE FRAMES entre PISO y SALTO
            HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y -= 10 * MULTIPLICADOR_impulso_arriba;//******************************VALOR DE FRAMES entre PISO y SALTO
            SPRITES_posicion_personaje_COORDENADAS_Y -= 10 * MULTIPLICADOR_impulso_arriba;//******************************VALOR DE FRAMES entre PISO y SALTO
        }
        if (HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y <= POSICION_final_salto) {

            HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y = POSICION_final_salto;

            POSICION_final_salto = 0;
            ESTADO_SALTO = 0;
        }
    }

    //Caida ****COORDENADAS****
    if(ESTADO_SALTO == 0)
    {
        if (TIME_FALLING <= 6)
            MULTIPLICADOR_gravedad = 0.1;

        else if (TIME_FALLING <= 14 && TIME_FALLING > 6 )
            MULTIPLICADOR_gravedad = 0.3;

        else if (TIME_FALLING <= 22 && TIME_FALLING > 14)
            MULTIPLICADOR_gravedad = 0.5;

        else if (TIME_FALLING <= 28 && TIME_FALLING > 22)
            MULTIPLICADOR_gravedad = 0.8;

        else if (TIME_FALLING > 28)
        {
            MULTIPLICADOR_gravedad = 1.0;
            //al_play_sample(salto_sonido, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        }

        if (HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y < LIMITE_piso_COORDENADAS && TIME_FALLING % 1 == 0)//VELOCIDAD DE CAIDA
        {
            HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_Y += MULTIPLICADOR_gravedad * GRAVEDAD;//************VALOR DE CAMBIO DE FRAMES (GRAVEDAD)
            HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y += MULTIPLICADOR_gravedad * GRAVEDAD;//************VALOR DE CAMBIO DE FRAMES (GRAVEDAD)
            SPRITES_posicion_personaje_COORDENADAS_Y += MULTIPLICADOR_gravedad * GRAVEDAD;//************VALOR DE CAMBIO DE FRAMES (GRAVEDAD)
            printf("Q\n");
        }
    }

    //Dejar de Flotar
    //Suelo
    if (HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y >= LIMITE_piso_COORDENADAS && ESTADO_SALTO != 4)
    {
        ESTADO_SALTO = 1;

        HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_Y = LIMITE_piso_COORDENADAS;
        HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y = LIMITE_piso_COORDENADAS;
        SPRITES_posicion_personaje_COORDENADAS_Y = LIMITE_piso_COORDENADAS-100;

        MULTIPLICADOR_gravedad = 0.1;
        if (HABILIDAD_doble_salto > -100)
            HABILIDAD_doble_salto = 2;
        if(HABILIDAD_doble_salto < -100)
            HABILIDAD_doble_salto = -100;
    }
}
void actualizar_display(){
    DIBUJO_fondo();
}
void main_camino_igualdad(){
    int fin = 0;
    while(fin != 1)
    {
        HITBOX_PANTALLA.PERSONAJE_abajo_1_X = HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_X+largo_PERSONAJE;
        HITBOX_PANTALLA.PERSONAJE_abajo_1_Y = HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y;

        HITBOX_PANTALLA.PERSONAJE_2_X = HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_X;
        HITBOX_PANTALLA.PERSONAJE_2_Y = HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y-altura_PERSONAJE;

        HITBOX_PANTALLA.PERSONAJE_3_X = HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_X+largo_PERSONAJE;
        HITBOX_PANTALLA.PERSONAJE_3_Y = HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y-altura_PERSONAJE;

        HITBOX_LIMBO.PERSONAJE_abajo_1_X = HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_X+largo_PERSONAJE;
        HITBOX_LIMBO.PERSONAJE_abajo_1_Y = HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_Y;

        HITBOX_LIMBO.PERSONAJE_2_X = HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_X;
        HITBOX_LIMBO.PERSONAJE_2_Y = HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_Y-altura_PERSONAJE;

        HITBOX_LIMBO.PERSONAJE_3_X = HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_X+largo_PERSONAJE;
        HITBOX_LIMBO.PERSONAJE_3_Y = HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_Y-altura_PERSONAJE;

        HITBOX_PANTALLA.CLOVERY_DAMAGE_X = HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_X+25;
        HITBOX_PANTALLA.CLOVERY_DAMAGE_Y = HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y-(altura_PERSONAJE/2);
        HITBOX_PANTALLA.DAMAGE_RADIO = (HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_Y-HITBOX_LIMBO.PERSONAJE_2_Y)/2;

        HITBOX_LIMBO.CLOVERY_ATAQUE_X = HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_X+25;
        HITBOX_LIMBO.CLOVERY_ATAQUE_Y = HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_Y-(altura_PERSONAJE/2);
        HITBOX_LIMBO.ATAQUE_RADIO = 100;

        //Dibujo y SONIDO
        if(al_event_queue_is_empty(cola_eventos))
        {
            actualizar_display();
            al_flip_display();
        }

        //Cerebro del juego
        al_wait_for_event(cola_eventos, &evento);
        if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            //funcion de confirmacion() --TODO
            fin = 1;
        }else if(evento.type == ALLEGRO_EVENT_TIMER){
            if (evento.timer.source == timer_0_01)//0.01 seg (10 = 0.1 && 100 = 1 seg)
            {
                /*
                inc_time_1_sec++;
                inc_time_SPRITES++;
                if (ESTADO_SALTO == 4) //SALTOS
                    TIME_FLOATING++;
                else
                    TIME_FLOATING = 0;
                if (ESTADO_SALTO == 0) //SALTOS
                    TIME_FALLING++;
                else
                    TIME_FALLING = 0;

                if (inc_time_SPRITES >= 54)
                    inc_time_SPRITES = 0;
*/
                //if((KEY_HOLD_flecha_derecha == 0 && KEY_HOLD_flecha_izquierda == 0)/* && ESTADO_SALTO == 1*/)
                  //  inc_time_SPRITES_corriendo = 0;
                //inc_time_SPRITES_corriendo++;
                //if(inc_time_SPRITES_corriendo >= 44)
                 //   inc_time_SPRITES_corriendo = 0;
                //Bloquea los ataques de un covi simple por 2 segundos

            }
            actualizar_display();
        }
        else if(evento.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT){//Evento de que perdiste el foco de la ventana
            //printf("PERDISTE EL FOCO\n");
        }
        else if(evento.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN){//Evento de que retomaste el foco de la ventana
            //printf("RECUPERASTE EL FOCO\n");

        }
        else{//Si no fueron eventos generales de la ventana:
            switch (momento) {
                case 0: //Menu Principal
                    break;
                case 1: //Juego
                    break;
                case 2: //Pausa
                    break;
                case 3:
                    break;
                default:

            }
        }
        switch (evento.type)
        {
                //EVENTOS TECLADO
            case ALLEGRO_EVENT_KEY_DOWN: // detecta 1 vez cuanto se presiona
                            if(evento.keyboard.keycode == ALLEGRO_KEY_UP && (ESTADO_SALTO == 1 || (HABILIDAD_doble_salto > 0 || HABILIDAD_doble_salto == -100)) /*&& KEY_HOLD_flecha_derecha == 0*/)   //Saltos
                            {
                                ESTADO_SALTO = 3;
                                HABILIDAD_doble_salto--;// ☼DOBLE SALTO
                            }
                            if(evento.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                            {
                                KEY_HOLD_flecha_derecha = 1;
                                KEY_PRESS_looking_side = 2;
                            }
                            if(evento.keyboard.keycode == ALLEGRO_KEY_LEFT)
                            {
                                KEY_HOLD_flecha_izquierda = 1;
                                KEY_PRESS_looking_side = 1;
                            }
                            if(evento.keyboard.keycode == ALLEGRO_KEY_DOWN)
                            {
                                KEY_HOLD_flecha_abajo = 1;
                            }
                break;

            case ALLEGRO_EVENT_KEY_CHAR: //detecta muchas veces (basicamente detecta cuando lo mantienes presionado)

                break;
            case ALLEGRO_EVENT_KEY_UP: //detecta 1 vez cuando se suelta
                if(evento.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                {
                    KEY_HOLD_flecha_derecha = 0;
                }
                if(evento.keyboard.keycode == ALLEGRO_KEY_LEFT)
                {
                    KEY_HOLD_flecha_izquierda = 0;
                }
                if(evento.keyboard.keycode == ALLEGRO_KEY_DOWN)
                {
                    KEY_HOLD_flecha_abajo = 0;
                }
                break;
        }
                HITBOX_PANTALLA.MAIN_PERSONAJE_abajo_0_Y = 0;
                HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_Y = 0;

            eventos_gravedad();
            localidad_COORDENADAS = HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_X / 100;
            localidad_COORDENADAS_1 = HITBOX_LIMBO.PERSONAJE_abajo_1_X / 100;
            localidad_COORDENADAS_2_y = HITBOX_LIMBO.PERSONAJE_2_Y / 50;
            localidad_COORDENADAS_3_y = HITBOX_LIMBO.PERSONAJE_3_Y / 50;
            localidad_coordenadas_y = HITBOX_LIMBO.MAIN_PERSONAJE_abajo_0_Y / 50;

            localidad_CENTRO_PERSONAJE_coordenadas_x = HITBOX_LIMBO.CLOVERY_ATAQUE_X /100;
            localidad_CENTRO_PERSONAJE_coordenadas_y = HITBOX_LIMBO.CLOVERY_ATAQUE_Y /100;
    }
}
int main(){
    if(inicializar_allegro()){
        disp = al_create_display(1200, 600);
        al_set_window_title(disp, "Carrera de Igualdad");
        //al_set_display_icon(disp, LOGO);
        cola_eventos = al_create_event_queue();

        timer_0_01 = al_create_timer(0.01);
        timer_0_5 = al_create_timer(0.5);

        al_register_event_source(cola_eventos,al_get_timer_event_source(timer_0_01)); // FUENTE: eventos de tipo temporizador
        al_register_event_source(cola_eventos,al_get_timer_event_source(timer_0_5)); // FUENTE: eventos de tipo temporizador
        al_register_event_source(cola_eventos, al_get_display_event_source(disp)); // FUENTE: eventos de la ventana
        al_register_event_source(cola_eventos, al_get_keyboard_event_source());// FUENTE: eventos del teclado
        al_start_timer(timer_0_01);

        main_camino_igualdad();

        al_destroy_event_queue(cola_eventos);
        al_destroy_timer(timer_0_01);
        al_destroy_display(disp);
    } else {
        if(DEPURACION == 1)
            printf("No se pudo inicializar allegro");
    }
    return 0;
}
int inicializar_allegro() {
    int verif_todo_ok = 1;
    if (!al_init()) {
        printf("No se pudo iniciar Allegro");
    }
    if (!al_init_primitives_addon()) {
        printf("No se iniciaron las primitivas");
        verif_todo_ok = 0;
    }
    if (!al_install_keyboard()) {
        printf("No se instalo el teclado");
        verif_todo_ok = 0;
    }
    if (!al_init_image_addon()) {
        printf("No se inicio image addon");
        verif_todo_ok = 0;
    }
    if (!al_install_audio()) {//SONIDO
        printf("No se cargo el complemento de audio");
        verif_todo_ok = 0;
    }
    if (!al_init_acodec_addon()) {//SONIDO
        printf("No se pudo cargar el complemento de codex");
        verif_todo_ok = 0;
    }
    if (!al_init_font_addon() || !al_init_ttf_addon()) {
        printf("No se pudo cargar el complemento de fuentes");
        verif_todo_ok = 0;
    }
    if (!init_resources()) {
        printf("Error al iniciar los recursos fuente\n");
        verif_todo_ok = 0;
    }
    return verif_todo_ok;
}