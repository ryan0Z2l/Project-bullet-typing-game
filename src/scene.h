#include "global.h"
#include "charater.h"

void menu_init();
void menu_process(ALLEGRO_EVENT event);
void menu_draw();
void menu_destroy();

void game_scene_init();
void game_scene_draw();
void game_scene_destroy();

void learn_init();
void learn_process(ALLEGRO_EVENT event);
void learn_draw();
void learn_destroy();

void victory_init();
void victory_process(ALLEGRO_EVENT event);
void victory_draw();
void victory_destroy();

void fail_init();
void fail_process(ALLEGRO_EVENT event);
void fail_draw();
void fail_destroy();
