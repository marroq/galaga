//729110-8 Empresa Comercial Niamaria S.A.

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "allegro5/allegro_image.h"
#include <allegro5/allegro_primitives.h>
#include "allegro5/allegro_native_dialog.h"

const float FPS = 10.0;
const int SCREEN_W = 1000;
const int SCREEN_H = 600;
const int LETTER = 60;
const int REST = 10;
const int CANTDISPAROS = 5;
const int VELOCITY = 15;

enum KEYS {
   KEY_UP, 
   KEY_DOWN, 
   KEY_LEFT, 
   KEY_RIGHT,
   KEY_SPACE
};

typedef struct gamer {
    int x;
    int y;
    ALLEGRO_BITMAP *nave;
} galaga_g;

typedef struct Missil {
    int x;
    int y;
    int sy;
} missil_g;

void drawPlayer(galaga_g *player) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(player->nave, player->x, player->y, 0);
    al_flip_display();
}

void createBullet(galaga_g* player, missil_g *missil, int *disparo, int maxDisparo, int sy) {
    if (*disparo < maxDisparo) {
        missil[*disparo].x = player->x + 15;
        missil[*disparo].y = player->y - 15;
        missil[*disparo].sy = sy;
    }
}

void shutter(galaga_g *player, missil_g *missil, ALLEGRO_BITMAP *bullet, int *disparo, int maxDisparo) {
    bool redraw;
    //short i;
    
    if (*disparo < maxDisparo) {
        while (!redraw) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            
            if (missil[*disparo].y > 20) {
                missil[*disparo].y -= missil[*disparo].sy;   
            } else redraw = true;  
            
            al_draw_bitmap(bullet, missil[*disparo].x, missil[*disparo].y, 0);
            al_draw_bitmap(player->nave, player->x, player->y, 0);
            al_flip_display();   
        }
    }
    
    (*disparo)++;
}

void moveUp(galaga_g *player) {
    /*Delimito el movimiento de la nave hasta la mitad de la pantalla*/
    if (player->y > SCREEN_H/2)
        player->y -= 3;    
}

void moveDown(galaga_g *player) {
    if (player->y <= SCREEN_H - 60)
        player->y += 3;    
}

void moveRight(galaga_g *player) {
    if (player->x <= SCREEN_W-60)
        player->x += 3;
}

void moveLeft(galaga_g *player) {
    if (player->x > 60) 
        player->x -= 3;
}

int showMessage(char* winTitle, char* heading, char* message) {
    return al_show_native_message_box(al_get_current_display(), 
        winTitle, 
        heading, 
        message,
        NULL, 
        ALLEGRO_MESSAGEBOX_ERROR);
}

int main(int argc, char **argv) {
    /*Declaración de punteros y variables*/
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_SAMPLE *music = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_BITMAP *bullet = NULL;
    
    int key[4] = {0, 0, 0, 0};
    bool exit = true;
    
    /*Inicializar todos los componentes de allegro*/
    if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
    }
    
    if(!al_install_audio()){
        fprintf(stderr, "failed to initialize audio!\n");
        return -1;
    }
    
    if(!al_init_acodec_addon()){
        fprintf(stderr, "failed to initialize audio codecs!\n");
        return -1;
    }
    
    if (!al_reserve_samples(1)){
        fprintf(stderr, "failed to reserve samples!\n");
        return -1;
    }
    
    if (!al_init_font_addon()) {
        fprintf(stderr, "failed to initialize font!\n");
        return -1;
    }
    
    if (!al_init_ttf_addon()) {
        fprintf(stderr, "failed to initialize ttf!\n");
        return -1;
    }
    
    if(!al_init_image_addon()) {
        fprintf(stderr, "failed to initialize al_init_image_addon!");
        return -1;
    }
    
    if(!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        return -1;
    }
    
    if(!al_install_mouse()) {
        fprintf(stderr, "failed to initialize the mouse!\n");
        return -1;
    }
    
    if(!al_init_primitives_addon()){
        fprintf(stderr, "failed to initialize the primitives addon!\n");
        return -1;
    }
    
    music = al_load_sample("space.wav");
    if (!music){
        printf( "Audio clip sample not loaded!\n" ); 
        return -1;
    }
    
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        fprintf(stderr, "failed to create timer!\n");
        al_destroy_sample(music);
        return -1;
    }
    
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);
        al_destroy_sample(music);
        return -1;
    }
    
    event_queue = al_create_event_queue();
    if(!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_timer(timer);
        al_destroy_sample(music);
        al_destroy_display(display);
        return -1;
    }
    
    font = al_load_ttf_font("galafont.ttf",LETTER,0);
    if (!font){
        fprintf(stderr, "Could not load 'pirulen.ttf'.\n");
        al_destroy_timer(timer);
        al_destroy_sample(music);
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        return -1;
    }
    
    galaga_g *player = (galaga_g *)malloc(sizeof(galaga_g));
    player->nave = al_load_bitmap("nave.png");
    player->x = SCREEN_W/2;
    player->y = SCREEN_H-60;
    if(!player->nave) {
        fprintf(stderr,"Failed to load nave image!");
        al_destroy_timer(timer);
        al_destroy_sample(music);
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        return 0;
    }
    
    missil_g *missil = (missil_g*)malloc(CANTDISPAROS * sizeof(missil_g));
    bullet = al_load_bitmap("shot.png");
    if (!bullet) {
        fprintf(stderr,"Failed to load shot image!");
        al_destroy_timer(timer);
        al_destroy_sample(music);
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        al_destroy_bitmap(player->nave);
        return 0;
    }
    
    /*Valores default de la pantalla y demas componentes*/
    al_inhibit_screensaver(1);
    al_set_window_title(display, "GALAGA - CIENCIAS DE LA COMPUTACION III");
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    
    al_set_target_bitmap(al_get_backbuffer(display));
    //al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    
    //showMessage("GALAGA","Iniciando...","Empezamos en 10 segundos");
    drawPlayer(player);
    al_draw_text(font, al_map_rgb(255,255,255), SCREEN_W/2, 10, ALLEGRO_ALIGN_CENTRE, "galaga");
    
    srand(time(NULL));
    al_start_timer(timer);
    
    al_play_sample(music, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    
    //al_rest(REST);
    ALLEGRO_EVENT ev;
    int sCount = 0;
    
    while(exit) {
        al_wait_for_event(event_queue, &ev);
        
        if(ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE:
                    exit = false;
                    break;
                case ALLEGRO_KEY_UP:
                    moveUp(player);
                    break;
                case ALLEGRO_KEY_DOWN:
                    moveDown(player);
                    break;
                case ALLEGRO_KEY_LEFT:
                    moveLeft(player);    
                    break;
                case ALLEGRO_KEY_RIGHT:
                    moveRight(player);
                    break;
                case ALLEGRO_KEY_SPACE:
                    createBullet(player, missil, &sCount, CANTDISPAROS, VELOCITY);
                    shutter(player, missil, bullet, &sCount, CANTDISPAROS);
                    break;
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    moveUp(player);
                    break;
                case ALLEGRO_KEY_DOWN:
                    moveDown(player);
                    break;
                case ALLEGRO_KEY_LEFT:
                    moveLeft(player);    
                    break;
                case ALLEGRO_KEY_RIGHT:
                    moveRight(player);
                    break;
            }
        } else if (ev.type == ALLEGRO_EVENT_TIMER) {
            if(key[KEY_UP])
                moveUp(player);
            else if(key[KEY_DOWN])
                moveDown(player);
            else if(key[KEY_LEFT])
                moveLeft(player);
            else if(key[KEY_RIGHT]) 
                moveRight(player);
        }
        
        drawPlayer(player);
    }
    
    /*Limpiar memoria*/
    al_destroy_display(display);
    al_destroy_sample(music);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(player->nave);
    al_destroy_bitmap(bullet);
    free(player);
    free(missil);
    
    return 0;
}

