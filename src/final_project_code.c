#include "charater.h"
#include "math.h"
#define PI 3.14159265
// the state of character
enum {STOP = 0, MOVE, ATK};
typedef struct character
{
    int x, y; // the position of image
    int width, height; // the width and height of image
    bool dir; // left: false, right: true
    int state; // the state of character
    ALLEGRO_BITMAP *img_move[2];
    ALLEGRO_BITMAP *img_atk[2];
    ALLEGRO_SAMPLE_INSTANCE *atk_Sound;
    int anime; // counting the time of animation
    int anime_time; // indicate how long the animation
}Character;
Character chara;
//=================================================================
typedef struct boss
{
    int x, y; // the position of image
    int width, height; // the width and height of image
    bool dir; // left: false, right: true
    int state; // the state of character
    ALLEGRO_BITMAP *img;
    //ALLEGRO_BITMAP *img_atk[2];
    //ALLEGRO_SAMPLE_INSTANCE *atk_Sound;
    int anime; // counting the time of animation
    int anime_time; // indicate how long the animation
}Boss;
Boss boss;
//=======================================================
//=======================================================
typedef struct bullet
{
    ALLEGRO_BITMAP *img_bullet;
    int posx, posy;
    double speedx, speedy;
    int width, height;
    int damage;
}Bullet;

Bullet bullet;

struct Bulletset
{
    Bullet set[10000];
    unsigned int size;
}bullet_set;

void bulletset_init()
{
    char temp2[50];
    bullet_set.size = 0;
    sprintf(temp2, "./image/resized_bullet_boss.png");
    bullet.img_bullet = al_load_bitmap(temp2);
}
ALLEGRO_SAMPLE *sample = NULL;
//===================================================================
void boss_init()
{
    char temp[50];
    sprintf(temp, "./image/resized_boss_charc.png");
    boss.img = al_load_bitmap(temp);
    boss.width = al_get_bitmap_width(boss.img);
    boss.height = al_get_bitmap_height(boss.img);
    boss.x = 160;
    boss.y = 0;
    boss.anime = 0;
    boss.anime_time = 30;
}
//===================================================================
void character_init(){
    // load character images
    for(int i = 1 ; i <= 2 ; i++){
        char temp[50];
        sprintf( temp, "./image/char_move%d.png", i );
        chara.img_move[i-1] = al_load_bitmap(temp);
    }
    for(int i = 1 ; i <= 2 ; i++){
        char temp[50];
        sprintf( temp, "./image/char_atk%d.png", i );
        chara.img_atk[i-1] = al_load_bitmap(temp);
    }

    // load effective sound
    sample = al_load_sample("./sound/atk_sound.wav");
    chara.atk_Sound  = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(chara.atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.atk_Sound, al_get_default_mixer());

    // initial the geometric information of character
    chara.width = al_get_bitmap_width(chara.img_move[0]);
    chara.height = al_get_bitmap_height(chara.img_move[0]);
    chara.x = WIDTH/2;
    chara.y = HEIGHT/2;
    chara.dir = false;

    // initial the animation component
    chara.state = STOP;
    chara.anime = 0;
    chara.anime_time = 30;

}
void charater_process(ALLEGRO_EVENT event){
    // process the animation
    if( event.type == ALLEGRO_EVENT_TIMER ){
        if( event.timer.source == fps ){
            chara.anime++;
            chara.anime %= chara.anime_time;
        }
    // process the keyboard event
    }else if( event.type == ALLEGRO_EVENT_KEY_DOWN ){
        key_state[event.keyboard.keycode] = true;
        chara.anime = 0;
    }else if( event.type == ALLEGRO_EVENT_KEY_UP ){
        key_state[event.keyboard.keycode] = false;
    }
}
void charater_update(){
    // use the idea of finite state machine to deal with different state
    if( key_state[ALLEGRO_KEY_W] ){
        chara.y -= 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_A] ){
        chara.dir = false;
        chara.x -= 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_S] ){
        chara.y += 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_D] ){
        chara.dir = true;
        chara.x += 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_SPACE] ){
        chara.state = ATK;
    }else if( chara.anime == chara.anime_time-1 ){
        chara.anime = 0;
        chara.state = STOP;
    }else if ( chara.anime == 0 ){
        chara.state = STOP;
    }
}
//========================================================

bool update_enemy()
{
    int i;
    static int boss_timer = 0;
    if(boss_timer != 0)
    {
        Bullet new_bullet;
		//new_bullet.posx = boss.x + boss.width/2.0 - 17.0;
		//new_bullet.posy = boss.y + boss.height/2.0 - 17.0;
		new_bullet.posx = boss.x;
		new_bullet.posy = boss.y;
		int balls_number = 10;
		for(i=0;i<balls_number;i++)
        {
			new_bullet.speedx = 10 * 0.7 * sin(2.0*PI*(double)(i)/(double)balls_number);
			new_bullet.speedy = 10 * 0.7 * cos(2.0*PI*(double)(i)/(double)balls_number);
			bulletset_push(&bullet_set, new_bullet);
		}
    }
    boss_timer++;
    return false;
}
//========================================================
void boss_draw()
{
    al_draw_bitmap(boss.img, boss.x, boss.y, 0);
}
//========================================================
void bulletset_push(struct Bulletset* ptr, Bullet now)
{
    ptr->set[ptr->size++] = now;
}
//=========================================================
//=========================================================
void bullet_update()
{
    int i, j;
    for(i=0; i<bullet_set.size; i++)
    {
        bullet_set.set[i].posx -= (double)(-2)*bullet_set.set[i].speedx;
        bullet_set.set[i].posy -= (double)(-2)*bullet_set.set[i].speedy;
    }
    int bullet_set_new_size = 0;
    for(i=0; i<bullet_set.size; i++)
    {

    }
    bullet_set.size = (unsigned int) bullet_set_new_size;

}
//=========================================================
void bullet_draw()
{
    for(int i=0; i<bullet_set.size; i++)
    {
        al_draw_bitmap(bullet.img_bullet, bullet_set.set[i].posx, bullet_set.set[i].posy, 0);
    }
}
//=========================================================
void character_draw(){
    // with the state, draw corresponding image
    if( chara.state == STOP ){
        if( chara.dir )
            al_draw_bitmap(chara.img_move[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
        else
            al_draw_bitmap(chara.img_move[0], chara.x, chara.y, 0);
    }else if( chara.state == MOVE ){
        if( chara.dir ){
            if( chara.anime < chara.anime_time/2 ){
                al_draw_bitmap(chara.img_move[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }else{
                al_draw_bitmap(chara.img_move[1], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }
        }else{
            if( chara.anime < chara.anime_time/2 ){
                al_draw_bitmap(chara.img_move[0], chara.x, chara.y, 0);
            }else{
                al_draw_bitmap(chara.img_move[1], chara.x, chara.y, 0);
            }
        }
    }else if( chara.state == ATK ){
        if( chara.dir ){
            if( chara.anime < chara.anime_time/2 ){
                al_draw_bitmap(chara.img_atk[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }else{
                al_draw_bitmap(chara.img_atk[1], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
                al_play_sample_instance(chara.atk_Sound);
            }
        }else{
            if( chara.anime < chara.anime_time/2 ){
                al_draw_bitmap(chara.img_atk[0], chara.x, chara.y, 0);
            }else{
                al_draw_bitmap(chara.img_atk[1], chara.x, chara.y, 0);
                al_play_sample_instance(chara.atk_Sound);
            }
        }
    }
}
void character_destory(){
    al_destroy_bitmap(chara.img_atk[0]);
    al_destroy_bitmap(chara.img_atk[1]);
    al_destroy_bitmap(chara.img_move[0]);
    al_destroy_bitmap(chara.img_move[1]);
    al_destroy_sample_instance(chara.atk_Sound);
}
void boss_destroy()
{
    al_destroy_bitmap(boss.img);
}
void bullet_destroy()
{
    al_destroy_bitmap(bullet.img_bullet);
}
