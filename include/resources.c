#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
ALLEGRO_COLOR principal_pale_chestnut;

ALLEGRO_FONT * lexend_regular[60];
ALLEGRO_FONT * lexend_bold[60];
ALLEGRO_FONT * lexend_thin[60];
ALLEGRO_FONT * roboto_bold[60];
ALLEGRO_FONT * roboto_italic[60];
ALLEGRO_FONT * roboto_regular[60];
ALLEGRO_FONT * roboto_thin[60];
ALLEGRO_FONT * roboto_black[60];
ALLEGRO_FONT * pixeled[60];
ALLEGRO_FONT * lettering[60];

ALLEGRO_BITMAP *RECORDS;

int init_resources(){
    int inicializado_correctamente=1;

    principal_pale_chestnut = al_map_rgb(222, 186, 192);

    for(int i=0; i<50; i++) {
        lexend_regular[i] = al_load_font("./media/fuentes/lexend/Lexend-Regular.ttf", i, 0);
        lexend_bold[i] = al_load_font("./media/fuentes/lexend/Lexend-Bold.ttf", i, 0);
        lexend_thin[i] = al_load_font("./media/fuentes/lexend/Lexend-Thin.ttf", i, 0);
        roboto_bold[i] = al_load_font("./media/fuentes/roboto/Roboto-Bold.ttf", i, 0);
        roboto_italic[i] = al_load_font("./media/fuentes/roboto/Roboto-Italic.ttf", i, 0);
        roboto_regular[i] = al_load_font("./media/fuentes/roboto/Roboto-Regular.ttf", i, 0);
        roboto_thin[i] = al_load_font("./media/fuentes/roboto/Roboto-Thin.ttf", i, 0);
        roboto_black[i] = al_load_font("./media/fuentes/roboto/Roboto-Black.ttf", i, 0);
        pixeled[i] = al_load_font("./media/fuentes/Pixeled.ttf", i, 0);
        lettering[i] = al_load_font("./media/fuentes/Lettering/lettering.ttf", i, 0);

        if(!lexend_regular[i] || !lexend_bold[i] || !lexend_thin[i]){
            inicializado_correctamente = 0;
            printf("ERROR: %i\n", al_get_errno());
            printf("ERROR: Hubo un problema al cargar las fuentes de lexend\n");
        }
        if(!roboto_regular[i] || !roboto_black[i] || !roboto_bold[i] || !roboto_italic[i] || !roboto_thin[i]){
            inicializado_correctamente = 0;
            printf("ERROR: %i\n", al_get_errno());
            printf("ERROR: Hubo un problema al cargar las fuentes de roboto\n");
        }
        if(!pixeled[i] || !lettering[i]){
            inicializado_correctamente = 0;
            printf("ERROR: %i\n", al_get_errno());
            printf("ERROR: Hubo un problema al cargar las fuentes de pixeled o lettering\n");
        }
    }
    /*
    RECORDS = al_load_bitmap("./media/img/recordatorio_blanco.jpg");
    if(!RECORDS){
        inicializado_correctamente = 0;
        printf("ERROR: %i\n", al_get_errno());
        printf("ERROR: Hubo un problema al cargar las imagenes de img\n");
        printf("\n");
    }
     */
    return inicializado_correctamente;
}
