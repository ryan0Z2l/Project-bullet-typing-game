#include "global.h"

void character_init();
void boss_init();
void bulletset_init();
int timenum();
int random();
void charater_process(ALLEGRO_EVENT event);
void boss_process(ALLEGRO_EVENT event);
void charater_update();
void bullet_update();
void character_draw();
void boss_draw();
void boss_health_draw();
void chara_health_draw();
void bullet_draw();
void bulletset_push();
bool update_boss();
void hint_text_choose();
void hint_text_draw();
void check_text_correct(char *player_input);
void character_destory();
void boss_destroy();
void bullet_destroy();
