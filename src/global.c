#include "global.h"

// variables for global usage
const float FPS = 60.0;
const int WIDTH = 800;
const int HEIGHT = 800;
const int window_x = 300;
const int window_y = 100;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *fps = NULL;
bool key_state[ALLEGRO_KEY_MAX] = {false};
int judge_next_window;
