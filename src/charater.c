#include "charater.h"
#include<math.h>
#include<time.h>
#include <stdlib.h>
#include<string.h>
#include<windows.h>
#define PI 3.14159265
// the state of character
enum {STOP = 0, MOVE, ATK};
char rand_text[100] = "";
// =====================================================
unsigned long long current_timestamp() {
    FILETIME ft;
    ULARGE_INTEGER uli;
    GetSystemTimeAsFileTime(&ft);
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    return uli.QuadPart / 10000;
}

unsigned int bossrandom() {
    unsigned long long seed = current_timestamp();
    return (unsigned int)(seed ^ (seed >> 32));
}
// ======================================================
typedef struct character
{
    int x, y; // the position of image
    int width, height; // the width and height of image
    bool dir; // left: false, right: true
    bool open_area;// true if open
    int state; // the state of character
    char text_input[100][100];
    ALLEGRO_BITMAP *img_move[4];
    ALLEGRO_BITMAP *img_atk[2];
    ALLEGRO_FONT *char_font;
    ALLEGRO_SAMPLE_INSTANCE *atk_Sound;
    ALLEGRO_SAMPLE_INSTANCE *hit_Sound;
    int anime; // counting the time of animation
    int anime_time; // indicate how long the animation
    int blood;
    int MAX_BLOOD;
}Character;

Character chara;
//=================================================================
typedef struct boss
{
    int x, y;
    int width, height;
    bool dir;
    int state;
    ALLEGRO_BITMAP *img;
    ALLEGRO_BITMAP *hit_image;
    //ALLEGRO_BITMAP *img_atk[2];
    ALLEGRO_SAMPLE_INSTANCE *boss_atk_sound;                     //Boss_struct_setting
    int anime;
    int anime_time;
    int bullet_timer;
    int hit_timer;
    bool is_shooting;
    int bullet_delay;
    int target_x;
    int target_y;
    double health;
    double MAX_HEALTH;
    bool fliped;
    bool is_jumping;
    bool is_hit;
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
ALLEGRO_SAMPLE *sample2 = NULL;
ALLEGRO_SAMPLE *sample3 = NULL;

//===================================================================
void boss_init()
{
    char temp[50];
    char temp2[50];
    sprintf(temp, "./image/resized_boss_charc.png");
    sprintf(temp2, "./image/boss_hit.png");
    boss.img = al_load_bitmap(temp);
    boss.hit_image = al_load_bitmap(temp2);
    boss.width = al_get_bitmap_width(boss.img);
    boss.height = al_get_bitmap_height(boss.img);
    boss.x = (WIDTH/2) - 70;
    boss.y = 20;
    boss.anime = 0;
    boss.anime_time = 30;
    boss.bullet_timer = 0;
    boss.hit_timer = 0;
    boss.is_shooting = false;
    boss.bullet_delay = 20;
    boss.MAX_HEALTH = 300;
    boss.health = 300;
    boss.is_jumping = false;
    boss.is_hit = false;

    // load boss sound;
    sample2 = al_load_sample("./sound/boss_attack_sound.wav");
    boss.boss_atk_sound = al_create_sample_instance(sample2);
    al_set_sample_instance_playmode(boss.boss_atk_sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(boss.boss_atk_sound, al_get_default_mixer());

}
//===================================================================
void character_init(){
    // load character images
    for(int i = 1 ; i <= 4 ; i++){
        char temp[50];
        sprintf( temp, "./image/char_move_%d.png", i );
        chara.img_move[i-1] = al_load_bitmap(temp);
    }
    for(int i = 1 ; i <= 2 ; i++){
        char temp[50];
        sprintf( temp, "./image/char_atk%d.png", i );
        chara.img_atk[i-1] = al_load_bitmap(temp);
    }
    // load font types
        al_init_font_addon();
        al_init_ttf_addon();
        chara.char_font = al_load_ttf_font("./font/DePixelBreitFett.ttf",20,0);
    // load effective sound
    sample = al_load_sample("./sound/atk_sound.wav");
    chara.atk_Sound  = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(chara.atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.atk_Sound, al_get_default_mixer());

    sample3 = al_load_sample("./sound/atkl_sound1.wav");
    chara.hit_Sound = al_create_sample_instance(sample3);
    al_set_sample_instance_playmode(chara.hit_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.hit_Sound, al_get_default_mixer());

    // initial the geometric information of character
    chara.width = al_get_bitmap_width(chara.img_move[0]);
    chara.height = al_get_bitmap_height(chara.img_move[0]);
    chara.x = WIDTH/2;
    chara.y = HEIGHT/2;
    chara.dir = false;
    chara.open_area = false;
    chara.text_input[0][0]="";//clear the buffer
    // initial the animation component
    chara.state = STOP;
    chara.anime = 0;
    chara.anime_time = 30;
    chara.blood = 100;
    chara.MAX_BLOOD = 100;

}
void charater_process(ALLEGRO_EVENT event){
    // process the animation
    if( event.type == ALLEGRO_EVENT_TIMER ){
        if( event.timer.source == fps ){
            chara.anime++;
            chara.anime %= chara.anime_time;
        }
    // process the keyboard event
    }
    else if( event.type == ALLEGRO_EVENT_KEY_DOWN )
    {
        key_state[event.keyboard.keycode] = true;
        chara.anime = 0;
    }
     else if ((event.type == ALLEGRO_EVENT_KEY_CHAR)&&chara.open_area)
        {
            al_draw_text(chara.char_font,al_map_rgb(255,215,0),chara.x,chara.y-20,0,chara.text_input);
                // Append the input character to text_input
                char ch = event.keyboard.unichar;
                char temp [100];
                strcpy(temp, &ch);
                strcat(chara.text_input,temp);
        }

    else if( event.type == ALLEGRO_EVENT_KEY_UP ){
        key_state[event.keyboard.keycode] = false;
    }
}
void charater_update(){
    // use the idea of finite state machine to deal with different state
    if( key_state[ALLEGRO_KEY_UP] ){
        chara.y -= 5;
        //chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_LEFT] ){
        chara.dir = false;
        chara.x -= 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_DOWN] ){
        chara.y += 5;
        //chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_RIGHT] ){
        chara.dir = true;
        chara.x += 5;
        chara.state = MOVE;
    }
    else if( key_state[ALLEGRO_KEY_ENTER] ){
        if(chara.open_area) check_text_correct(chara.text_input[0]);
        chara.open_area = true;
        al_play_sample_instance(chara.atk_Sound);
        memset(chara.text_input,0,sizeof(chara.text_input[0]));
        hint_text_choose();
    }
    else if(key_state[ALLEGRO_KEY_BACKSPACE])
        {
        chara.open_area = false;
        memset(chara.text_input,0,sizeof(chara.text_input[0]));
        }
    else if( chara.anime == chara.anime_time-1 ){
        chara.anime = 0;
        chara.state = STOP;
    }else if ( chara.anime == 0 ){
        chara.state = STOP;
    }
}
//========================================================

bool update_boss()
{
    int i;
    static int boss_timer = 0;
    boss.bullet_timer++;
    //boss.health -= 1;
    int initial = boss.y;
    if(boss.is_hit && boss.hit_timer <= 15)
    {
        boss.hit_timer++;
    }
    else
    {
        boss.is_hit = false;
        boss.hit_timer = 0;
    }
    if(boss.health == 0)
    {
        judge_next_window = 4;
    }

    if(boss.bullet_timer == 120)
    {
        boss.target_x = bossrandom() % (WIDTH - boss.width + 30);
        boss.target_y = bossrandom() % (HEIGHT/2 - boss.height + 30);
    }
    if(boss.bullet_timer >= 180)
    {
        boss.is_shooting = true;
        boss.bullet_timer = 0;
    }
    if(boss.is_shooting && boss.bullet_timer < 60)
    {
        Bullet new_bullet;
		//new_bullet.posx = boss.x + boss.width/2.0 - 17.0;
		//new_bullet.posy = boss.y + boss.height/2.0 - 17.0;
		new_bullet.posx = boss.x;
		new_bullet.posy = boss.y;
		int balls_number = 5;
		for(i=0;i<balls_number;i++)
        {
            if(boss.bullet_timer % boss.bullet_delay == 0)
            {
                new_bullet.speedx = 5 * 0.5 * sin(2.0*PI*(double)(i)/(double)balls_number);
                new_bullet.speedy = 5 * 0.5 * cos(2.0*PI*(double)(i)/(double)balls_number);
                bulletset_push(&bullet_set, new_bullet);
            }
		}
		al_play_sample_instance(boss.boss_atk_sound);
		if(!boss.is_jumping)
        {
            boss.is_jumping = true;
        }
    }
    else
    {
        boss.is_shooting = false;
        boss.is_jumping = false;
        boss.y = initial;
        if(boss.x < boss.target_x)
        {
            boss.x++;
            boss.fliped = true;
        }
        else if(boss.x > boss.target_x)
        {
            boss.x--;
            boss.fliped = false;
        }
        if(boss.y < boss.target_y)
        {
            boss.y++;
        }
        else if(boss.y > boss.target_y)
        {
            boss.y--;
        }

        if(boss.x < 0) boss.x = 0;
        else if (boss.x + boss.width > WIDTH) boss.x = WIDTH - boss.width;
        if(boss.y < 30) boss.y = 30;
        else if (boss.y + boss.height > HEIGHT) boss.y = HEIGHT - boss.height;
    }
    return false;
}
//========================================================
void boss_draw()
{
    if(boss.fliped)
    {
        //al_draw_bitmap(boss.img, boss.x, boss.y, ALLEGRO_FLIP_HORIZONTAL);
        if(boss.is_hit)
        {
            al_draw_bitmap(boss.img, boss.x, boss.y, ALLEGRO_FLIP_HORIZONTAL);
            al_draw_bitmap(boss.hit_image, boss.x-90, boss.y-100, 0);
        }
        else{
            al_draw_bitmap(boss.img, boss.x, boss.y, ALLEGRO_FLIP_HORIZONTAL);
        }
    }
    else
    {
        //al_draw_bitmap(boss.img, boss.x, boss.y, 0);
        if(boss.is_hit)
        {
            al_draw_bitmap(boss.img, boss.x, boss.y, 0);
            al_draw_bitmap(boss.hit_image, boss.x-90, boss.y-100, 0);
        }
        else
        {
            al_draw_bitmap(boss.img, boss.x, boss.y, 0);
        }
    }
}
//=========================================================
void boss_health_draw()
{
    double bar_width = 150;
    double bar_height = 10;
    int bar_x = boss.x - 30;
    int bar_y = boss.y - 20;
    double health_show = boss.health;
    double filled_number = (health_show / boss.MAX_HEALTH) * bar_width;

    if(boss.health > 0)
    {
        al_draw_rectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, al_map_rgb(255, 0, 0), 2);
        al_draw_filled_rectangle(bar_x, bar_y, bar_x + filled_number, bar_y + bar_height, al_map_rgb(255, 0, 0));
    }
    else if(boss.health <= 0)
    {
        al_draw_rectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, al_map_rgb(255, 0, 0), 2);
    }
}
//========================================================
void chara_health_draw()
{
    double bar_width = 150;
    double bar_height = 10;
    int bar_x = chara.x - 30;
    int bar_y = chara.y + 60;
    double health_show = chara.blood;
    double filled_number = (health_show / chara.MAX_BLOOD) * bar_width;

    if(chara.blood > 0)
    {
        al_draw_rectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, al_map_rgb(255, 0, 0), 2);
        al_draw_filled_rectangle(bar_x, bar_y, bar_x + filled_number, bar_y + bar_height, al_map_rgb(255, 0, 0));
    }
    else if(chara.blood <= 0)
    {
        al_draw_rectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, al_map_rgb(255, 0, 0), 2);
    }
}
//========================================================
void bulletset_push(struct Bulletset* ptr, Bullet now)
{
    ptr->set[ptr->size++] = now;
}
//=========================================================
void bullet_update()
{
    int i, j;
    for(i=0; i<bullet_set.size; i++)
    {
        bullet_set.set[i].posx -= (double)(-2)*bullet_set.set[i].speedx;
        bullet_set.set[i].posy -= (double)(-2)*bullet_set.set[i].speedy;

        if(bullet_set.set[i].posx < 0 || bullet_set.set[i].posx > WIDTH || bullet_set.set[i].posy < 0 || bullet_set.set[i].posy > HEIGHT)
        {
            for(int j=i; j < bullet_set.size - 1; j++)
            {
                bullet_set.set[j] = bullet_set.set[j+1];
            }
            bullet_set.size--;
            i--;
        }
        else
        {
            if( bullet_set.set[i].posx < chara.x + chara.width &&
                bullet_set.set[i].posx + bullet_set.set[i].width > chara.x &&
                bullet_set.set[i].posy < chara.y + chara.height &&
                bullet_set.set[i].posy + bullet_set.set[i].height > chara.y)
                {
                    al_attach_sample_instance_to_mixer(chara.hit_Sound, al_get_default_mixer());
                    al_set_sample_instance_gain(chara.hit_Sound, 1);
                    al_play_sample_instance(chara.hit_Sound);
                    chara.blood -= 1;

                    if(chara.blood <= 0)
                    {
                        judge_next_window = 5;
                    }
                }
        }
    }

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
    if( chara.state == STOP )
    {
        //change into input
        if(chara.open_area)
            {

        if( chara.dir )
        {
            al_draw_bitmap(chara.img_atk[0],chara.x-220,chara.y-230,0);
            al_draw_bitmap(chara.img_move[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
        }
        else
        {
            al_draw_bitmap(chara.img_atk[0],chara.x-220,chara.y-230,0);
            al_draw_bitmap(chara.img_move[0], chara.x, chara.y, 0);
        }

        al_draw_text(chara.char_font,al_map_rgb(255,100,255),chara.x,chara.y-20,0,chara.text_input);
            }
        else
        {
        if( chara.dir )
            al_draw_bitmap(chara.img_move[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
        else
            al_draw_bitmap(chara.img_move[0], chara.x, chara.y, 0);
        }

    }

    else if( chara.state == MOVE )
        {
            if(chara.open_area)
            {

        if( chara.dir )
                {                    // right
              al_draw_bitmap(chara.img_atk[0],chara.x-220,chara.y-230,0);
             al_draw_bitmap(chara.img_move[2], chara.x, chara.y, 0);
                }
        else if(!chara.dir)
                {                   //left
            al_draw_bitmap(chara.img_atk[0],chara.x-220,chara.y-230,0);
            al_draw_bitmap(chara.img_move[1], chara.x, chara.y, 0);
                }

            al_draw_text(chara.char_font,al_map_rgb(255,0,255),chara.x,chara.y-20,0,chara.text_input);
            }

        else
        {
        if( chara.dir )
            {                    // right

             al_draw_bitmap(chara.img_move[2], chara.x, chara.y, 0);
            }
        else
            {                   //left
            al_draw_bitmap(chara.img_move[1], chara.x, chara.y, 0);
            }

        }
        }
}
//===================================================================== NEW
void hint_text_choose()
{
    char str[20][100]={"avada kedavra","wingardium leviosa",
    "sectumsempra","riddikulus","lumos","expelliarmus","imperio","petrificus totalus","expecto patronum","crucio"};// declare all the string we need to type
    // randomly choose a string
    srand(time(NULL));
    int randomIndex = rand() % 10;
    const char* randomStr = str[randomIndex];
    strcpy(rand_text,randomStr);
}
void hint_text_draw()
{
    al_draw_text(chara.char_font,al_map_rgb(163,106,20),400,725,ALLEGRO_ALIGN_CENTER,rand_text);
}
void check_text_correct(char *player_input)
{
    if(strncmp(chara.text_input[0], rand_text, strlen(rand_text)) == 0)
    {
        boss.is_hit = true;
        boss.health -= 150;
    }
}
//=====================================================================
void character_destory(){
    al_destroy_bitmap(chara.img_atk[0]);
    al_destroy_bitmap(chara.img_atk[1]);
    al_destroy_bitmap(chara.img_move[0]);
    al_destroy_bitmap(chara.img_move[1]);
    al_destroy_sample_instance(chara.atk_Sound);
    al_destroy_sample_instance(chara.hit_Sound);

}
void boss_destroy()
{
    al_destroy_bitmap(boss.img);
    al_destroy_bitmap(boss.hit_image);
    al_destroy_sample_instance(boss.boss_atk_sound);
    al_destroy_sample(sample2);
}
void bullet_destroy()
{
    al_destroy_bitmap(bullet.img_bullet);
}
