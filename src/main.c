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

void main_camino_igualdad(){

}
int main(){
    if(inicializar_allegro()){
        disp = al_create_display(1200, 700);
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
            printf("No se pudo inicializar arreglo");
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
        printf("Error al iniciar los recursos fuentes");
        verif_todo_ok = 0;
    }
    return verif_todo_ok;

}