#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab2012_sprites.h>
#include <cab202_timers.h>
#include <time.h>
#define DELAY (10) 

bool game_over = false; /* Set this to true when game is over */
bool update_screen = true; /* Set to false to prevent screen update. */

char * player_respawn =
/**/	"\\\\ | //"
/**/	"-- * --"
/**/	"-- * --"
/**/	"// | \\\\";

char * player_image =
/**/	" O "
/**/	"/|\\"
/**/	" | "
/**/	"/ \\";

char * player_jump =
/**/	" O "
/**/	"\\|//"
/**/	" | "
/**/	"\\ //";

char * player_image_left =
/**/	" O "
/**/	"\\|\\"
/**/	" | "
/**/	"\\ \\";

char * player_image_right =
/**/	" O "
/**/	"/|/"
/**/	" | "
/**/	"/ /";

sprite_id player;
sprite_id player_left;
sprite_id player_right;

void lives(void) {
    double lives = 10;
    int hit = 0;
    if (hit >= 1) {
        hit = 0;
        lives -= 1;
        hit = 0;
    }
    else {
        NULL;
    }
}
timer_id(ms);
void timer2(void) {
    clock_t start = clock(), diff;
}

double start_time;
void timer(void) {
    start_time = get_current_time();
    int elapsed_time = (int) (get_current_time() - start_time);
    draw_formatted(10,10,"%d:%d", elapsed_time/60, elapsed_time%60);
}

void draw_hud(void) {
    double lives = 10;
    double Time = 0;
    double score = 0;
    //int screenheight = screen_height();
    int screenwidth = screen_width();
    draw_line(0, 0, screenwidth, 0, '~');
    draw_line(0, 4, screenwidth, 4, '~');
    draw_string(1, 2, "n10224378");
    draw_string(15, 2, "Lives:");
    draw_string(30, 2, "Time:");
    draw_string(45, 2, "Score:");
    draw_double(22, 2, lives);
    draw_double(36, 2, Time);
    draw_double(52, 2, score);
}

void setup(void) {
    draw_hud();
    player = sprite_create(20, 20, 3, 4, player_image);
    sprite_draw(player);
}


void process(void) {
    lives();
    timer();
}

int main(void) {
	setup_screen();
	setup();
	show_screen();

	while ( !game_over ) {
		process();

		if ( update_screen ) {
			show_screen();
		}

		timer_pause(DELAY);
	}

	return 0;
}