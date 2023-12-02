#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
ALLEGRO_COLOR negro_transparencia;

ALLEGRO_FONT * lexend_regular[60];
ALLEGRO_FONT * lexend_bold[60];
ALLEGRO_FONT * lexend_thin[60];
ALLEGRO_FONT * roboto_bold[60];
ALLEGRO_FONT * roboto_italic[60];
ALLEGRO_FONT * roboto_regular[60];
ALLEGRO_FONT * roboto_thin[60];
ALLEGRO_FONT * roboto_black[60];

ALLEGRO_BITMAP *fondo1, *fondo2, *hombre1, *hombre2, *hombre3, *mujer1, *mujer2, *mujer3, *piso;


int init_resources(){
    int inicializado_correctamente=1;

    negro_transparencia = al_map_rgba(0, 0, 0, 100);

    for(int i=0; i<60; i++) {
        lexend_regular[i] = al_load_font("./media/fuentes/lexend/Lexend-Regular.ttf", i, 0);
        lexend_bold[i] = al_load_font("./media/fuentes/lexend/Lexend-Bold.ttf", i, 0);
        lexend_thin[i] = al_load_font("./media/fuentes/lexend/Lexend-Thin.ttf", i, 0);
        roboto_bold[i] = al_load_font("./media/fuentes/roboto/Roboto-Bold.ttf", i, 0);
        roboto_italic[i] = al_load_font("./media/fuentes/roboto/Roboto-Italic.ttf", i, 0);
        roboto_regular[i] = al_load_font("./media/fuentes/roboto/Roboto-Regular.ttf", i, 0);
        roboto_thin[i] = al_load_font("./media/fuentes/roboto/Roboto-Thin.ttf", i, 0);
        roboto_black[i] = al_load_font("./media/fuentes/roboto/Roboto-Black.ttf", i, 0);

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
    }
    fondo1 = al_load_bitmap("./media/img/fondo1.png");
    fondo2 = al_load_bitmap("./media/img/fondo2.png");
    piso = al_load_bitmap("./media/img/piso5000.png");
    hombre1 = al_load_bitmap("./media/img/main1.1.png");
    hombre2 = al_load_bitmap("./media/img/main1.2.png");
    hombre3 = al_load_bitmap("./media/img/main1.3.png");
    mujer1 = al_load_bitmap("./media/img/main2.1.png");
    mujer2 = al_load_bitmap("./media/img/main2.2.png");
    mujer3 = al_load_bitmap("./media/img/main2.3.png");
    if(!fondo1 || !fondo2 || !piso || !hombre1 || !hombre2 || !hombre3 || !mujer1 || !mujer2 || !mujer3){
        inicializado_correctamente = 0;
        printf("ERROR: %i\n", al_get_errno());
        printf("ERROR: Hubo un problema al cargar las imagenes de img\n");
        printf("\n");
    }
    return inicializado_correctamente;
}
