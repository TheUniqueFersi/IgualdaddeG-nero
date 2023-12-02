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

/* ---- termina; ---- */
void main_camino_igualdad();
void actualizar_display();

int KEY_HOLD_RIGHT=0, KEY_HOLD_LEFT=0;
int KEY_PRESS_looking_side = 0;

struct Personaje {
    ALLEGRO_BITMAP * imagen;
    float x;
    //float y;
    float colX;
    //float colY;
    int estado_movimiento;
} personaje;
typedef struct {
    char * mensaje;
    int coordX;
    //int
} MENSAJE;
MENSAJE Mensajes[30] = {{"Te encargas del niño y del quehacer de la casa"},
//                        {"Hazme un sandwich"},
//                        {"Una dama no se comporta con esos modales"},
                        {"(Silbido)\nSolo a los hombres se les ven bien los tatuajes"},
                        {"¿No quieres que te acompañe a tu casa?"},
                        {"¿A que hora sales por el pan?"},
                        //{"A las mujeres no hay que entenderlas, hay que quererlas"},
                        //{"Mejor deja que un hombre lo haga"},
                        {"Deberías ocupar vestido, ese traje te hace ver como un hombre, y que gei"},
                        {"Pero si solo hay secretarias o sistentas"},
                        //{"No te pongas esa camisa, es rosa porque es de niñas"},
                        {"No ocupes aretes, no eres mujer"},
                        {"Los hombres no lloran"},
                        {"Aguantese como los machos"},
                        {"Trabajas igual que una mujer"},
                        {"Trabajos de ingenieros, no de princesitas"},
};
int COLISIONES[150][3];
void inicializar_colisiones(){
    for(int i=0; i<150; i++){
        for(int j=0; j<3; j++)
            COLISIONES[i][j] = 0;
    }
    //COLISIONES[10][0] = 1;
    //COLISIONES[10][1] = 1;//Tipo interactiva
    //COLISIONES[10][1] = 2;//Tipo automatica (pensamiento)
    //COLISIONES[10][1] = 3;//Tipo automatica ()
    //[j = 0] 1 indica que si hay colision
    //[j = 1] INdica el modo como arriba
    //[j = 2] Indica el indice dle arreglo de estructuras

    COLISIONES[4][0] = 1;
    COLISIONES[4][1] = 1;
    COLISIONES[4][2] = 0;

    COLISIONES[5][0] = 1;
    COLISIONES[5][1] = 1;
    COLISIONES[5][2] = 0;

    COLISIONES[6][0] = 1;
    COLISIONES[6][1] = 1;
    COLISIONES[6][2] = 0;


    COLISIONES[13][0] = 1;
    COLISIONES[13][1] = 1;
    COLISIONES[13][2] = 1;

    COLISIONES[14][0] = 1;
    COLISIONES[14][1] = 1;
    COLISIONES[14][2] = 1;

    COLISIONES[15][0] = 1;
    COLISIONES[15][1] = 1;
    COLISIONES[15][2] = 1;


    COLISIONES[17][0] = 1;
    COLISIONES[17][1] = 1;
    COLISIONES[17][2] = 2;

    COLISIONES[18][0] = 1;
    COLISIONES[18][1] = 1;
    COLISIONES[18][2] = 2;


    COLISIONES[19][0] = 1;
    COLISIONES[19][1] = 1;
    COLISIONES[19][2] = 3;

    COLISIONES[20][0] = 1;
    COLISIONES[20][1] = 1;
    COLISIONES[20][2] = 3;

    COLISIONES[21][0] = 1;
    COLISIONES[21][1] = 1;
    COLISIONES[21][2] = 3;


    COLISIONES[28][0] = 1;
    COLISIONES[28][1] = 1;
    COLISIONES[28][2] = 4;

    COLISIONES[29][0] = 1;
    COLISIONES[29][1] = 1;
    COLISIONES[29][2] = 4;


    COLISIONES[31][0] = 1;
    COLISIONES[31][1] = 1;
    COLISIONES[31][2] = 5;

    COLISIONES[32][0] = 1;
    COLISIONES[32][1] = 1;
    COLISIONES[32][2] = 5;


    COLISIONES[53][0] = 1;
    COLISIONES[53][1] = 1;
    COLISIONES[53][2] = 6;

    COLISIONES[54][0] = 1;
    COLISIONES[54][1] = 1;
    COLISIONES[54][2] = 6;


    COLISIONES[65][0] = 1;
    COLISIONES[65][1] = 1;
    COLISIONES[65][2] = 7;

    COLISIONES[66][0] = 1;
    COLISIONES[66][1] = 1;
    COLISIONES[66][2] = 7;


    COLISIONES[71][0] = 1;
    COLISIONES[71][1] = 1;
    COLISIONES[71][2] = 8;

    COLISIONES[72][0] = 1;
    COLISIONES[72][1] = 1;
    COLISIONES[72][2] = 8;


    COLISIONES[79][0] = 1;
    COLISIONES[79][1] = 1;
    COLISIONES[79][2] = 9;

    COLISIONES[80][0] = 1;
    COLISIONES[80][1] = 1;
    COLISIONES[80][2] = 9;


    COLISIONES[86][0] = 1;
    COLISIONES[86][1] = 1;
    COLISIONES[86][2] = 10;

    COLISIONES[87][0] = 1;
    COLISIONES[87][1] = 1;
    COLISIONES[87][2] = 10;
}
int x, colision = 0;
void revisar_colisiones() {
    x=personaje.colX/100;
    //printf("%i, %f\n", x, personaje.colX);
    if(COLISIONES[x][0] == 1){
        //printf("TIENES COLISION CHAVO: %i, %f\n", x, personaje.colX);
        if(colision!=2)
            colision = 1;
    }
    else
        colision = 0;
}
int momento = 1, etapa = 0;

void desplegarMENSAJE(int indice_Mensaje){
    al_draw_filled_rectangle(100, 100, 1100, 400, negro_transparencia);
    al_draw_text(roboto_regular[30], blanco, 600, 250, ALLEGRO_ALIGN_CENTER, Mensajes[indice_Mensaje].mensaje);
    //printf("XDDD\n");
}
#define VALOR_MOVIMIENTO 2
float distX_pantalla = 0;
float MAX_DIM_IZQ=0, MAX_DIM_DER=1200;
int inc_time_SPRITES = 0;
void SPRITES_personajes(){
    if(momento == 1){
        if(etapa == 0){
            int FLIP_horizontal;
            if (KEY_PRESS_looking_side == 1)
                FLIP_horizontal = ALLEGRO_FLIP_HORIZONTAL;
            else
                FLIP_horizontal = 0;

            if(personaje.estado_movimiento == 1){
                if(personaje.colX>4900){
                    if(inc_time_SPRITES%3 == 0){
                        al_draw_scaled_bitmap(hombre2, 0,0,30,63, personaje.x, 320, 70, 180, FLIP_horizontal);

                    } else if (inc_time_SPRITES%3 == 1){
                        al_draw_scaled_bitmap(hombre1, 0,0,30,63, personaje.x, 320, 70, 180, FLIP_horizontal);

                    } else if (inc_time_SPRITES%3 == 2){
                        al_draw_scaled_bitmap(hombre3, 0,0,30,63, personaje.x, 320, 70, 180, FLIP_horizontal);

                    }
                }
                else{
                    if(inc_time_SPRITES%3 == 0){
                        al_draw_scaled_bitmap(mujer2, 0,0,30,63, personaje.x, 320, 70, 180, FLIP_horizontal);
                    } else if (inc_time_SPRITES%3 == 1){
                        al_draw_scaled_bitmap(mujer1, 0,0,30,63, personaje.x, 320, 70, 180, FLIP_horizontal);

                    } else if (inc_time_SPRITES%3 == 2){
                        al_draw_scaled_bitmap(mujer3, 0,0,30,63, personaje.x, 320, 70, 180, FLIP_horizontal);

                    }
                }
            }
            else{
                if(personaje.colX>4900){
                    al_draw_scaled_bitmap(hombre2, 0,0,30,63, personaje.x, 320, 70, 180, FLIP_horizontal);
                }
                else{
                    al_draw_scaled_bitmap(mujer2, 0,0,30,63, personaje.x, 320, 70, 180, FLIP_horizontal);
                }
            }

        }
    }
}
void actualizar_display(){
    al_draw_scaled_bitmap(fondo1, 0,0,7939,500,distX_pantalla, 0, 7939, 500, 0);
    al_draw_scaled_bitmap(fondo2, 0,0,3239,500,distX_pantalla+7939, 0, 3239, 500, 0);
    al_draw_scaled_bitmap(piso, 0,0,5000,100,distX_pantalla, 500, 5000, 100, 0);
    al_draw_scaled_bitmap(piso, 0,0,5000,100,distX_pantalla+5000, 500, 5000, 100, 0);
    al_draw_scaled_bitmap(piso, 0,0,5000,100,distX_pantalla+10000, 500, 5000, 100, 0);
    //al_draw_filled_rectangle(0,0,1200,150, negro_transparencia);
    //al_draw_scaled_bitmap(fondo2, 0,0,1600,100,distX_pantalla+, 0, 15000, 600, 0);
    //al_draw_filled_rectangle(personaje.x, 400, personaje.x+50, 500, negro_transparencia);
    //120 * 245

    SPRITES_personajes();
    al_draw_filled_rectangle(distX_pantalla+1100, 0, distX_pantalla+1300, 500, gris);
    al_draw_filled_rectangle(distX_pantalla+2500, 0, distX_pantalla+2700, 500, gris);
    al_draw_filled_rectangle(distX_pantalla+3800, 0, distX_pantalla+4000, 500, gris);
    al_draw_filled_rectangle(distX_pantalla+4900, 0, distX_pantalla+5100, 500, gris);
    al_draw_filled_rectangle(distX_pantalla+6200, 0, distX_pantalla+6400, 500, gris);

    al_draw_filled_rectangle(distX_pantalla+7800, 0, distX_pantalla+8000, 500, gris);
    al_draw_filled_rectangle(distX_pantalla+9100, 0, distX_pantalla+9300, 500, gris);

    if(colision==1){
        al_draw_filled_rectangle(400,500,800,600,negro);
        al_draw_text(lexend_bold[20], blanco, 600, 540, ALLEGRO_ALIGN_CENTER, "Presiona X para interactuar");
    }
    if(colision==2){
        desplegarMENSAJE(COLISIONES[x][2]);
        //printf("XEDS");
    }
    //printf("LOOOOOOOOOOOOL: %i\n", colision);
    al_flip_display();
}

float MITAD_MAPA = 550;
float calcular_valor_nuevo_posicion(float * coord, float valor_de_movimiento){
    float retorno=0;
    if(valor_de_movimiento > 0){
        if((*coord + valor_de_movimiento) > MAX_DIM_DER){
            float dif = MAX_DIM_DER - *coord;
            *coord = MAX_DIM_DER-50;
            personaje.colX += dif;
        } else{
            if((*coord + valor_de_movimiento) >= MITAD_MAPA){
                float dife = MITAD_MAPA - *coord;
                *coord = MITAD_MAPA;
                personaje.colX += dife;
                distX_pantalla -= dife;
            }
            else if((*coord + valor_de_movimiento) < MITAD_MAPA){
                *coord += valor_de_movimiento;
                personaje.colX += valor_de_movimiento;
            }
            if((*coord) == MITAD_MAPA){
                //*coord += valor_de_movimiento;

                personaje.colX += valor_de_movimiento;
                distX_pantalla -= valor_de_movimiento;
            }
        }
        /*
        if(personaje.colX > 13800){
            distX_pantalla = -13800;
            //personaje
        }
         */
    } else{
        if((*coord + valor_de_movimiento) < MAX_DIM_IZQ) {
            float dif = -*coord;
            *coord = MAX_DIM_IZQ;
            personaje.colX += dif;
        } else {
            *coord += valor_de_movimiento;
            personaje.colX += valor_de_movimiento;
        }
    }
}

void MOVIMIENTO_PERSONAJE(){
    if(KEY_HOLD_LEFT){
        calcular_valor_nuevo_posicion(&personaje.x, -VALOR_MOVIMIENTO);
    }
    if(KEY_HOLD_RIGHT) {
        calcular_valor_nuevo_posicion(&personaje.x, VALOR_MOVIMIENTO);
    }
}

void re_inicializar_variables(){
    personaje.estado_movimiento = 0; //quieto
    personaje.x = 0;
    personaje.colX = 0;
    distX_pantalla = 0;
}
void main_camino_igualdad(){
    int fin = 0;
    inicializar_colisiones();
    re_inicializar_variables();
    //al_clear_to_color(al_map_rgb(141,67,255));
    while(fin!=1){
        if(al_event_queue_is_empty(cola_eventos)){
            //pantalla_requiere_actualizacion=0;
            actualizar_display();
        }
        else if(!al_event_queue_is_empty(cola_eventos)){
            //pantalla_requiere_actualizacion = 1;
        }

        //EVENTOS SUCEDIENDO:
        //printf("Momento: %i ", momento);
        //printf("Nombre: %s\n", nombre);
        al_wait_for_event(cola_eventos, &evento);
        if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            //funcion de confirmacion() --TODO
            fin = 1;
        }else if(evento.type == ALLEGRO_EVENT_TIMER){
            //actualizar_display();
            if(evento.timer.source == timer_0_5){
                inc_time_SPRITES++;
                if(inc_time_SPRITES == 10000)
                    inc_time_SPRITES = 0;
            }
        }
        else if(evento.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT){//Evento de que perdiste el foco de la ventana
            //printf("PERDISTE EL FOCO\n");
        }
        else if(evento.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN){//Evento de que retomaste el foco de la ventana
            //printf("RECUPERASTE EL FOCO\n");

        }
        else{//Si no fueron eventos generales de la ventana:
            /*
            switch (nivel) {

            }*/
            switch (evento.type) {
                case ALLEGRO_EVENT_KEY_DOWN:
                    if(evento.keyboard.keycode == ALLEGRO_KEY_LEFT){
                        if(KEY_HOLD_RIGHT != 1){
                            KEY_HOLD_LEFT = 1;
                            KEY_PRESS_looking_side = 1;
                            personaje.estado_movimiento=1;
                        }
                    } else if(evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                        if(KEY_HOLD_LEFT != 1){
                            KEY_HOLD_RIGHT = 1;
                            KEY_PRESS_looking_side = 2;
                            personaje.estado_movimiento=1;
                        }
                    } else if (evento.keyboard.keycode == ALLEGRO_KEY_X){
                        if(colision == 1){
                            colision = 2;
                        }
                    } else if (evento.keyboard.keycode == ALLEGRO_KEY_C){
                        if(colision == 2){
                            colision = 0;
                        }
                    }
                    break;
                case ALLEGRO_EVENT_KEY_UP:
                    if(evento.keyboard.keycode == ALLEGRO_KEY_LEFT){
                        KEY_HOLD_LEFT = 0;
                        personaje.estado_movimiento=0;
                    } else if(evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                        KEY_HOLD_RIGHT = 0;
                        personaje.estado_movimiento=0;
                    }
                    break;
            }
        }
        MOVIMIENTO_PERSONAJE();
        revisar_colisiones();
        //distX_pantalla = ;
        //printf("x: %f, Coord: %f\n", personaje.x, personaje.colX);
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
        al_start_timer(timer_0_5);

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