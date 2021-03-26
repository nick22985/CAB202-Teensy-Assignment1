void move_player(int key){
    int hx = round(sprite_x(player));
    int hy = round(sprite_y(player));
        if (key == 'a' && hx > 1) {
            sprite_move(player, -1, 0);
        }
        else if (key == 'd' && hx < (screen_width() - 1 - sprite_width(player))) {
            sprite_move(player, +1, 0);
        }
        else if (key == 'w' && hy > 5) {
            sprite_move(player, 0, -1);
        }
    }