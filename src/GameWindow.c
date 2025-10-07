#include "GameWindow.h"

bool draw = false;
int window = 1;

const char *title = "Final Project Team 5th";

static char const *filename;

// ALLEGRO Variables
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_SAMPLE *song = NULL;
ALLEGRO_SAMPLE *song1 = NULL;
ALLEGRO_SAMPLE *song2 = NULL;
ALLEGRO_SAMPLE *song3 = NULL;
ALLEGRO_SAMPLE_INSTANCE *sample_instance;
ALLEGRO_SAMPLE_INSTANCE *sample_instance1;
ALLEGRO_SAMPLE_INSTANCE *sample_instance2;
ALLEGRO_SAMPLE_INSTANCE *sample_instance3;
ALLEGRO_EVENT event;
ALLEGRO_VIDEO* video ;




int Game_establish() {
    msg = 0;

    init_video();
    video_begin();
    while( 1 ){
        al_wait_for_event(event_queue, &event);
        if( event.type == ALLEGRO_EVENT_TIMER ) {
            video_display(video);
        } else if( event.type == ALLEGRO_EVENT_DISPLAY_CLOSE ) {
            al_close_video(video);
            break;
        } else if( event.type == ALLEGRO_EVENT_VIDEO_FINISHED ) {
            break;
        }

    }
    destroy_video();

    game_init();
    game_begin();

    while ( msg != GAME_TERMINATE ) {
        msg = game_run();
        if ( msg == GAME_TERMINATE )
            printf( "Game Over\n" );

    }

    game_destroy();
    return 0;
}


void video_display(ALLEGRO_VIDEO *video) {
    ALLEGRO_BITMAP *frame = al_get_video_frame(video);
    // check if we get the frame successfully
    // Note the this code is necessary
    if ( !frame )
        return;
    // Display the frame.
    // rescale the frame into the size of screen
    al_draw_scaled_bitmap(frame,
                          // the rescale position of the frame
                          0, 0,
                          // the width and height you want to rescale
                          al_get_bitmap_width(frame),
                          al_get_bitmap_height(frame),
                          // the position of result image
                          0, 0,
                          // the width and height of result image
                          al_get_display_width(display),
                          al_get_display_height(display), 0);
    al_flip_display();
}


void init_video(){
    al_init();
    al_init_video_addon();
    al_install_audio();
    fps = al_create_timer(1.0 / 60);
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
    display = al_create_display(WIDTH, HEIGHT);
    al_set_window_position(display, window_x, window_y);
    // linear interpolation for scaling images
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    filename = "./video/StartGuide.ogv";
    printf("reading video.....\n");
    video = al_open_video(filename);
    if( video )
        printf("read video succeed\n");
    else
        printf("read video fail!!!!\n");
    event_queue = al_create_event_queue();
    // register video event
    ALLEGRO_EVENT_SOURCE *temp = al_get_video_event_source(video);
    al_register_event_source(event_queue, temp);
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(fps));
}
void video_begin(){
    al_reserve_samples(1);
    al_start_video(video, al_get_default_mixer());
    al_start_timer(fps);
}
void destroy_video(){
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
}

void game_init() {
    printf( "Game Initializing...\n" );
    al_init();
    // init audio
    al_install_audio();
    al_init_acodec_addon();
    // Create display
    display = al_create_display(WIDTH, HEIGHT);
    // create event queue
    event_queue = al_create_event_queue();
    // Initialize Allegro settings
    al_set_window_position(display, window_x, window_y);
    al_set_window_title(display, title);
    al_init_primitives_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    al_init_image_addon(); // initialize the image addon
    al_init_acodec_addon(); // initialize acodec addon
    al_install_keyboard(); // install keyboard event
    al_install_mouse();    // install mouse event
    al_install_audio();    // install audio event
    // Register event
    al_register_event_source(event_queue, al_get_display_event_source( display ));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    fps = al_create_timer(1.0/FPS);
    al_register_event_source(event_queue, al_get_timer_event_source( fps )) ;
    // initialize the icon on the display
    ALLEGRO_BITMAP *icon = al_load_bitmap("./image/icon.jpg");
    al_set_display_icon(display, icon);
}

void game_begin() {

    // Load sound
    song = al_load_sample("./sound/back_music.wav");
    song1 = al_load_sample("./sound/hello.wav");
    song2 = al_load_sample("./sound/victory.wav");
    song3 = al_load_sample("./sound./fail.wav");
    al_reserve_samples(20);
    sample_instance = al_create_sample_instance(song);
    sample_instance1 = al_create_sample_instance(song1);// battle bgm
    sample_instance2 = al_create_sample_instance(song2);
    sample_instance3 = al_create_sample_instance(song3);
    // Loop the song until the display closes
    al_set_sample_instance_playmode(sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_playmode(sample_instance2, ALLEGRO_PLAYMODE_ONCE);
    al_set_sample_instance_playmode(sample_instance3, ALLEGRO_PLAYMODE_ONCE);
        //  al_set_sample_instance_playmode(sample_instance1, ALLEGRO_PLAYMODE_LOOP);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(sample_instance1, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(sample_instance2, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(sample_instance3, al_get_default_mixer());
        //al_attach_sample_instance_to_mixer(sample_instance1, al_get_default_mixer());
    // set the volume of instance
    al_set_sample_instance_gain(sample_instance, 1) ;
    al_play_sample_instance(sample_instance);
    al_set_sample_instance_gain(sample_instance1, 1) ;
    //al_play_sample_instance(sample_instance1);
    al_start_timer(fps);
    // initialize the menu before entering the loop
    menu_init();
    //learn_init();
    //victory_init();
    //fail_init();

}

void game_update(){
    if( judge_next_window == 3 ){
        if(window == 1){
            menu_destroy;
            learn_init();
        }
        window = 3; //說明頁面
    }
    else if( judge_next_window ==1 ){
        if(window == 3){  //window==3 遊戲說明頁面
            learn_destroy();
            game_scene_init();
            judge_next_window = 0;
            window = 2;
            // Stop the current song
            al_stop_sample_instance(sample_instance);

            // Attach and play the new song
            al_attach_sample_instance_to_mixer(sample_instance1, al_get_default_mixer());
            al_set_sample_instance_gain(sample_instance1, 1);
            al_play_sample_instance(sample_instance1);
        }
        else if( window == 1 ){
            // not back menu anymore, therefore destroy it
            menu_destroy();
            // initialize next scene
            game_scene_init();
            judge_next_window = 0;
            window = 2;
            // Stop the current song
            al_stop_sample_instance(sample_instance);

            // Attach and play the new song
            al_attach_sample_instance_to_mixer(sample_instance1, al_get_default_mixer());
            al_set_sample_instance_gain(sample_instance1, 1);
            al_play_sample_instance(sample_instance1);
        }
        else if(window == 4){  //window==4  victory頁面
            victory_destroy();
            game_scene_init();
            judge_next_window = 0;
            //window = 2;
            // Stop the current song
            //al_stop_sample_instance(sample_instance);
            if (al_get_sample_instance_playing(sample_instance1)) {
                al_stop_sample_instance(sample_instance1);
            }

            // Attach and play the new song
            al_attach_sample_instance_to_mixer(sample_instance2, al_get_default_mixer());
            al_set_sample_instance_gain(sample_instance2, 1);
            al_play_sample_instance(sample_instance2);
        }
        else if(window == 5){  //window==5 fail頁面
            fail_destroy();
            game_scene_init();
            judge_next_window=0;
            //window = 2;
            // Stop the current song
            //al_stop_sample_instance(sample_instance);
            //al_stop_sample_instance(sample_instance1);
            if (al_get_sample_instance_playing(sample_instance1)) {
                al_stop_sample_instance(sample_instance1);
            }

            // Attach and play the new song
            al_attach_sample_instance_to_mixer(sample_instance3, al_get_default_mixer());
            al_set_sample_instance_gain(sample_instance3, 1);
            al_play_sample_instance(sample_instance3);
        }
    }

    else if(judge_next_window == 4){
        victory_init();
        window =4 ;
    }
    else if(judge_next_window == 5){
        fail_init();
        window = 5;
    }
    if (al_get_sample_instance_playing(sample_instance1) && window == 4 || window == 5) {
        al_stop_sample_instance(sample_instance1);
        if(window == 4)
        {
            al_attach_sample_instance_to_mixer(sample_instance2, al_get_default_mixer());
            al_set_sample_instance_gain(sample_instance2, 1);
            al_play_sample_instance(sample_instance2);
            al_stop_sample_instance(sample_instance2);
        }
        else if(window == 5)
        {
            al_attach_sample_instance_to_mixer(sample_instance3, al_get_default_mixer());
            al_set_sample_instance_gain(sample_instance3, 1);
            al_play_sample_instance(sample_instance3);
            //al_stop_sample_instance(sample_instance3);
        }
    }

    if( window == 2 ){
        charater_update();
        bullet_update();
        if(update_boss())
        {

        }
    }
}
int process_event(){
    // Request the event
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);
    // process the event of other component
    if( window == 1 ){
        menu_process(event);}
    else if( window ==3 ){
        learn_process(event);
    }
    else if( window == 2 ){
        charater_process(event);
    }
    else if(window == 4){
        victory_process(event);
    }
    else if(window == 5){
        fail_process(event);
    }

    // Shutdown our program
    if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        return GAME_TERMINATE;
        /*
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
        {
            return GAME_TERMINATE;
        }
    }
    */
    else if(event.type == ALLEGRO_EVENT_TIMER)
        if(event.timer.source == fps)
            draw = true;
    if(draw) game_update();
    return 0;
}
void game_draw(){
    if( window == 1 ){
        menu_draw();
    }else if( window == 2 ){
        game_scene_draw();
    }
    else if(window == 3){
        learn_draw();
    }
    else if(window == 4){
        victory_draw();
    }
    else if(window == 5){
        fail_draw();
    }
    al_flip_display();
}
int game_run() {
    int error = 0;
    if( draw ){
        game_draw();
        draw = false;
    }
    if ( !al_is_event_queue_empty(event_queue) ) {
        error = process_event();
    }
    if(msg == GAME_TERMINATE)
    {
        game_destroy();
        return GAME_TERMINATE;
    }
    return error;
}

void game_destroy() {
    // Make sure you destroy all things
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_sample_instance(sample_instance);
    al_destroy_sample_instance(sample_instance1);
    al_destroy_sample_instance(sample_instance2);
    al_destroy_sample_instance(sample_instance3);
    game_scene_destroy();
}
