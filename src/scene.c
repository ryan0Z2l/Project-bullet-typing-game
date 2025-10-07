#include "scene.h"

ALLEGRO_FONT *font = NULL;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_BITMAP *menu = NULL;
ALLEGRO_BITMAP *learn = NULL;
ALLEGRO_BITMAP *attack_frame = NULL; //攻擊卷軸：336*114
ALLEGRO_BITMAP *victory = NULL;
ALLEGRO_BITMAP *fail = NULL;

// function of menu
void menu_init(){
    //font = al_load_ttf_font("./font/pirulen.ttf",12,0);
    menu = al_load_bitmap("./image/start.png");
}
void menu_process(ALLEGRO_EVENT event){
    if( event.type == ALLEGRO_EVENT_KEY_UP ){
        if( event.keyboard.keycode == ALLEGRO_KEY_ENTER )
            judge_next_window = 1;  //進入遊戲
        if( event.keyboard.keycode == ALLEGRO_KEY_SPACE ){
            judge_next_window = 3;  //進入說明頁面
        }


    }
}


void menu_draw(){
    al_draw_bitmap(menu,0, 0, 0);
    //al_clear_to_color(al_map_rgb(100,100,100));
    //al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+200 , ALLEGRO_ALIGN_CENTRE, "Press 'Enter' to start");
    //al_draw_rectangle(WIDTH/2-150, 510, WIDTH/2+150, 550, al_map_rgb(255, 255, 255), 0);
}
void menu_destroy(){
    al_destroy_font(font);
    al_destroy_bitmap(menu); //
}

void learn_init(){
    learn = al_load_bitmap("./image/learn.png");
}
void learn_process(ALLEGRO_EVENT event){
    if( event.type == ALLEGRO_EVENT_KEY_UP )
    if( event.keyboard.keycode == ALLEGRO_KEY_ENTER )
            judge_next_window = 1;
}
void learn_draw(){
    al_draw_bitmap(learn, 0, 0 , 0);
}

void learn_destroy(){
    al_destroy_bitmap(learn);
}

// function of game_scene
void game_scene_init(){
    character_init();
    background = al_load_bitmap("./image/stage.jpg");
    attack_frame = al_load_bitmap("./image/attack_frame.png");
    boss_init();
    bulletset_init();
}
void game_scene_draw(){
    al_draw_bitmap(background, 0, 0, 0);
    al_draw_bitmap(attack_frame, (WIDTH-al_get_bitmap_width(attack_frame))/2,(HEIGHT-al_get_bitmap_height(attack_frame)), 0);
    character_draw();
    boss_draw();
    bullet_draw();
    boss_health_draw();
    chara_health_draw();
    hint_text_draw();
}
void game_scene_destroy(){
    al_destroy_bitmap(background);
    character_destory();
    boss_destroy();
    bullet_destroy();
}
void victory_init(){
    victory = al_load_bitmap("./image/victory.png");
}
void victory_process(ALLEGRO_EVENT event){
    if( event.type == ALLEGRO_EVENT_KEY_UP )
    if( event.keyboard.keycode == ALLEGRO_KEY_ENTER )
        judge_next_window = 1;
}
void victory_draw(){
    al_draw_bitmap(victory, 0, 0 , 0);
}

void victory_destroy(){
    al_destroy_bitmap(victory);
}

void fail_init(){
    fail = al_load_bitmap("./image/fail.png");
}
void fail_process(ALLEGRO_EVENT event){
    if( event.type == ALLEGRO_EVENT_KEY_UP )
    if( event.keyboard.keycode == ALLEGRO_KEY_ENTER )
            judge_next_window = 1;
}
void fail_draw(){
    al_draw_bitmap(fail, 0, 0 , 0);
}

void fail_destroy(){
    al_destroy_bitmap(fail);
}
