#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "allegro5/allegro_image.h"
#include <allegro5/allegro_primitives.h>
#include "allegro5/allegro_native_dialog.h"

const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;

enum KEYS {
   KEY_UP, 
   KEY_DOWN, 
   KEY_LEFT, 
   KEY_RIGHT
};

typedef struct gamer {
    int x;
    int y; 
    ALLEGRO_BITMAP *nave;
} galaga_g;

void drawPlayer(galaga_g *player) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(player->nave, player->x, player->y, 0);
    al_flip_display();
}

void moveUp(galaga_g *player) {
    player->y -= 2;
}

void moveDown(galaga_g *player) {
    player->y += 2;
}

void moveRight(galaga_g *player) {
    player->x += 2;
}

void moveLeft(galaga_g *player) {
    player->x -= 2;
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
    
    bool redraw = true;
    bool key[4] = {false, false, false, false};
    
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
    
    display = al_create_display(1000, 600);
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
    
    font = al_load_ttf_font("pirulen.ttf",20,0);
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
    player->x = 500;
    player->y = 540;
    if(!player->nave) {
        fprintf(stderr,"Failed to load image!");
        al_destroy_timer(timer);
        al_destroy_sample(music);
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        return 0;
    }
    
    /*Valores default de la pantalla y demas componentes*/
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_set_target_bitmap(al_get_backbuffer(display));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    drawPlayer(player);
    al_draw_text(font, al_map_rgb(255,255,255), 500, 10,ALLEGRO_ALIGN_CENTRE, "galaga!");
    al_play_sample(music, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    al_inhibit_screensaver(1);
    al_set_window_title(display, "GALAGA - CIENCIAS DE LA COMPUTACION III");
    al_start_timer(timer);
    al_flip_display();
    
    //Por el momento esto para revisar todos
    al_rest(60.0);
    
    /*Limpiar memoria*/
    al_destroy_display(display);
    al_destroy_sample(music);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(player->nave);
    
    return 0;
}
