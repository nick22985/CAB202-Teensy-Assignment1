#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>

#define DELAY (10) //Configures the millisecond delay between game updates.

#define PLAYER_WIDTH (3) //Configures the width of the player sprite.
#define PLAYER_HEIGHT (5) //Configures the height of the player sprite.
#define PLATFORM_WIDTH (7) //Configures the width of both good and bad platforms as they share the same width.
#define PLATFORM_HEIGHT (2) //Configures the height of both good and bad platforms as they share the same height.
#define TREASURE_WIDTH (2)
#define TREASURE_HEIGHT (2)

#define MAX_GOOD_PLATFORM (160) //Configures the maximum amount of good platforms that can be displayed.
#define MAX_BAD_PLATFORM (40) //Configures the maximum amount of bad platforms that can be displayed.

bool game_over = false;	//Set this to true when the game is over. 
bool update_screen = true; //Set this to false to prevent the screen from updating.

char *player_image =
	    /**/  " O "
		/**/  "/|\\"
		/**/  " | "
		/**/  "/ \\"
		/**/  "   ";

char *good_platform_image =
	/**/ "======="
		 /**/ "=======";

char *bad_platform_image =
	/**/ "XXXXXXX"
		 /**/ "XXXXXXX";

char *treasure_image = 
    /**/ "$*"
	     /**/ "*$";

char *message_image1 =
	/**/ "Thanks for playing!";
char *message_image2 = 
    /**/ "Press 'r' to play again, or press 'q' to quit.";

//Delare a sprite_id known as 'player'.
sprite_id player;

//Declare a sprite_id known as 'good_platform', with an array that holds the values from the 'MAX_GOOD_PLATFORM' macro definition above.
sprite_id good_platform[MAX_GOOD_PLATFORM];

//Declare a sprite_id known as 'bad_platform', with an array that holds the values from the 'MAX_BAD_PLATFORM' macro definition above.
sprite_id bad_platform[MAX_BAD_PLATFORM];

//Declare a sprite_id known as 'treasure'.
sprite_id treasure;

//Declare variables for the number of good and bad platforms.
int num_good_platforms = 25;
int num_bad_platforms = 5;

//Declare counting variables for both lives and score. These will be used to track the current status of the respective counters.
int life_count = 10;
int score_count = 0;

//Draw the top and bottom border for the 'display_screen' area.
void display_screen() {
	draw_line(0, 0, screen_width() - 1, 0, '~');
	draw_line(0, 5, screen_width() - 1, 5, '~');
}

//Declare variables for milliseconds, seconds and minutes. These will be used to create the timer within the display_screen.
int milliseconds = 0;
int seconds = 0;
int minutes = 0;

//Create a function that runs the timer.
void game_timer() {
	milliseconds++;
    if (milliseconds == 100) {
        seconds++;
        milliseconds = 0;
        if (seconds == 60) {
            minutes++;
            seconds = 0;
        }
    }
}

//Print the four variables for student number, lives, timer and score.
void display_label() {
	draw_formatted(5, 2, "n10202412");
	draw_formatted(25, 2, "Lives: %d", life_count);
	draw_formatted(45, 2, "Timer: %02d:%02d", minutes, seconds);
	draw_formatted(65, 2, "Score: %d", score_count);
}

//Create arrays for the width and height positions of the good platforms.
int good_width[] = {8, 20, 40, 60, 80};
int good_height[] = {10, 19, 30, 50, 70};

//Create arrays for the width and height positions of the bad platforms.
int bad_width[] = {20, 20, 40, 60, 80};
int bad_height[] = {14, 10, 30, 50, 70};

//Create a sprite_id function that contains the setup code for the good platforms.
sprite_id good_setup() {
	int gfx = rand() % (screen_width() - 5 - 2) + 1;
	int gfy = rand() % (screen_height() - 5 - 2) + 1;
	int x = rand() % 5;
	int y = rand() % 5;
	gfx = good_width[x];
	gfy = good_height[y];
	sprite_id good_platform = sprite_create(gfx, gfy, 7, 2, good_platform_image);
	return good_platform;
}

//Create a sprite_id function that contains the setup code for the bad platforms.
sprite_id bad_setup() {
	int bfx = rand() % (screen_width() - 5 - 2) + 1;
	int bfy = rand() % (screen_height() - 5 - 2) + 1;
	int x = rand() % 5;
	int y = rand() % 5;
	bfx = bad_width[x];
	bfy = bad_height[y];
	sprite_id bad_platform = sprite_create(bfx, bfy, 7, 2, bad_platform_image);
	return bad_platform;
}

//Create a function that creates good and bad platforms using their setup functions and values from the max platform macros defined at the beginning of the program.
void platform_execute() {
	for (int i = 0; i < num_good_platforms; i++) {
		good_platform[i] = good_setup();
	}
	for (int i = 0; i < num_bad_platforms; i++) {
		bad_platform[i] = bad_setup();
	}
}

//Setup the function that will be called by draw_all, responsible for drawing the platforms and displaying them on the screen.
void draw_platform(sprite_id platform[], int n) {
	for (int i = 0; i < n; i++) {
		sprite_draw(platform[i]);
	}
}

/*This function is used to consolidate the functions responsible 
for drawing different graphical elements on the screen into one that can be called within the 'process' function.
*/

void draw_all() {
	clear_screen();
	display_screen();
	display_label();
	sprite_draw(player);
	sprite_draw(treasure);
	sprite_draw(good_platform[MAX_GOOD_PLATFORM]);
	sprite_draw(bad_platform[MAX_BAD_PLATFORM]);
	draw_platform(good_platform, num_good_platforms);
	draw_platform(bad_platform, num_bad_platforms);
	show_screen();
}

// Setup the game.
void setup(void) {
	player = sprite_create(65, 7, 3, 5, player_image);
	treasure = sprite_create(77, 22, 2, 2, treasure_image);
	//good_platform = sprite_create(65, 12, 7, 2, good_platform_image);
	platform_execute();
	draw_all();
}

/*This function reads a character input and responds accordingly. 
In this case, it checks if the game is paused or not if the 'p' key is pressed.
*/

bool paused;

int read_char() {
	int key_code = paused ? wait_char() : get_char();

	if ('p' == key_code) {
		paused = !paused;
	}

	return key_code;
}

bool goingLeft;
bool treasureMoving = true;

void detect_treasure_input(int key_code) {
	if ('t' == key_code) {
		treasureMoving =! treasureMoving;
	}
}

void move_treasure(int key_code) {
	detect_treasure_input(key_code);
	if (treasureMoving) {
		if (goingLeft == true) {
		sprite_move(treasure, -.15,0);
	}
		else {
		sprite_move(treasure, +.15,0);
	}
		if (sprite_x(treasure) < 0) {
		goingLeft = false;
	}
		else if (sprite_x(treasure) > screen_width() - TREASURE_WIDTH) {
		goingLeft = true;
	}
}
}

//This function defines the instruction set for controlling the player sprite using the 'w', 'a', 's' and 'd' keys.
void move_player(int key_code, int term_width, int term_height) {
    int px = round(sprite_x(player));
    int py = round(sprite_y(player));

    // (i) Move hero left according to specification.
    if (key_code == 'a' && px > 1) sprite_move(player, -1, 0);

    // (i) Move hero right according to specification.
    if (key_code == 'd' && px < term_width - sprite_width(player) - 1) sprite_move(player, +1, 0);

    // (j) Move hero up according to specification.
    if (key_code == 'w' && py > 1) sprite_move(player, 0, -1);

    // (k) Move hero down according to specification.
    if (key_code == 's' && py < term_height - sprite_height(player) - 1) sprite_move(player, 0, +1);
}

bool sprites_collide(sprite_id s1, sprite_id s2)
{
    int top1 = round(sprite_y(s1));
    int bottom1 = top1 + sprite_height(s1) - 1;
    int left1 = round(sprite_x(s1));
    int right1 = left1 + sprite_width(s1) - 1;

    int top2 = round(sprite_y(s2));
    int bottom2 = top2 + sprite_height(s2) - 1;
    int left2 = round(sprite_x(s2));
    int right2 = left2 + sprite_height(s2) - 1;

    if (top1 > bottom2)
    {
        return false;
    }
    else if (top2 > bottom1)
    {
        return false;
    }
    else if (right1 < left2)
    {
        return false;
    }
    else if (right2 < left1)
    {
        return false;
    }
    else {
        return true;
    }
}

sprite_id sprites_collide_any( sprite_id s, sprite_id sprites[], int n ) {
    sprite_id result = NULL;

    for ( int i = 0; i < n; i++ )
    {
        if ( sprites_collide( s, sprites[i] ) )
        {
            result = sprites[i];
            break;
        }
    }

    return result;
}

void score_life_message(void) {
	int msg_width3 = 48;
	int msg_height3 = 1;
	int msg_x3 = (screen_width() - msg_width3) / 2;
	int msg_y3 = (screen_height() - msg_height3) / 2; 
	draw_formatted(msg_x3, msg_y3, "Your final score was %d with %d lives remaining.", score_count, life_count);
}

void timer_message(void) {
	int msg_width3 = 23;
	int msg_height3 = 1;
	int msg_x3 = (screen_width() - msg_width3) / 2 + 1;
	int msg_y3 = (screen_height() - msg_height3) / 2 + 1; 
	draw_formatted(msg_x3, msg_y3, "Time Elapsed: %02d:%02d", minutes, seconds);
}

//This function defines the instructions that will be executed when the game ends.
void end_game(int key_code) {
	game_over = true;
	int msg_width1 = strlen(message_image1);
	int msg_height1 = 1;
	int msg_x1 = (screen_width() - msg_width1) / 2;
	int msg_y1 = (screen_height() - msg_height1) / 2 - 2; 
	sprite_id msg1 = sprite_create(msg_x1, msg_y1, msg_width1, msg_height1, message_image1);

	int msg_width2 = strlen(message_image2);
	int msg_height2 = 1;
	int msg_x2 = (screen_width() - msg_width2) / 2 - 1;
	int msg_y2 = (screen_height() - msg_height2) / 2 - 1; 
    sprite_id msg2 = sprite_create(msg_x2, msg_y2, msg_width2, msg_height2, message_image2);
	
	clear_screen();
	sprite_draw(msg1);
	sprite_draw(msg2);
	score_life_message();
	timer_message();
	show_screen();
	
	timer_pause(1000);

	while ( get_char() >= 0 ) {
		// Do nothing
	}

	wait_char();

	if ('q' == key_code) {
		exit(0);
	}
	else if ('r' == key_code) {

	}
}

//Play one turn of game.
void process(void) {
	int w = screen_width();
	int h = screen_height();
	int keyCode = read_char();
	int py = round(sprite_y(player));

	if (keyCode == 'q') {
		end_game(keyCode);
	}

	game_timer();
	draw_all();
	move_player(keyCode, w, h);
	move_treasure(keyCode);
	if (sprites_collide_any(player, good_platform, num_good_platforms)) {
		score_count = score_count + 1;
		if (keyCode == 'w' && py < 1) {
			sprite_move(player, 0, -2);
        }
	}
else {
		sprite_move(player, 0, +0.08);
	}
if (sprites_collide(treasure, player)) {
	life_count = life_count + 2;
	end_game(keyCode);
}
}

// Clean up game
;
void cleanup(void) {
	// STATEMENTS
}

//Beginning of 'main' function.
int main(void) {
	setup_screen();
	setup();
	show_screen();

	 while (!game_over)
	 {
	 	process();

	 	if (update_screen)
	 	{
	 		show_screen();
	 	}

	 	timer_pause(DELAY);
	 }

	cleanup();

	return 0;
}
