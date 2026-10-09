#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#define MAP_WIDTH 60
#define MAP_HEIGHT 25
#define MAX_ENTITIES 50
#define MAX_PARTICLES 200
#define UI_OFFSET_X 63

#define CP_DEFAULT   1
#define CP_WALL      2
#define CP_PLAYER    3
#define CP_ENEMY     4
#define CP_BLOOD     5
#define CP_FLOOR     6
#define CP_ITEM      7
#define CP_TITLE     8
#define CP_UI_BOX    9 
#define CP_EXIT      10
#define CP_ATTACK    11
#define CP_NO_AMMO   12
#define CP_UI_TEXT   13

#define SYM_FLOOR   '.' 
#define SYM_PLAYER  '@'
#define SYM_ENEMY   'E'
#define SYM_CORPSE  'x'
#define SYM_EXIT    '>'
#define SYM_AMMO    'A'

typedef enum {
    STATE_MENU,
    STATE_HELP,
    STATE_PLAY,
    STATE_GAMEOVER,
    STATE_VICTORY,
    STATE_EXIT
} GameStateEnum;

typedef enum {
    WEAPON_KNIFE,
    WEAPON_PISTOL
} WeaponType;

typedef struct {
    int x, y;
} Vec2;

typedef struct {
    float x, y;
    float vx, vy;
    int life;
    char symbol;
    int color;
    int active;
} Particle;

typedef struct {
    Vec2 pos;
    int active;
    int hp;
    int alert_level; 
} Enemy;

typedef struct {
    Vec2 pos;
    int hp;
    int active;
    WeaponType weapon;
    int ammo;
} Player;

typedef struct {
    char grid[MAP_HEIGHT][MAP_WIDTH + 1];
    char blood_layer[MAP_HEIGHT][MAP_WIDTH + 1];
    Player player;
    Enemy enemies[MAX_ENTITIES];
    Particle particles[MAX_PARTICLES];
    int enemy_count;
    int current_level;
    int score;
    int combo;
    int combo_timer;
    
    int attack_anim_timer;
    int attack_mode;

    time_t start_time;
    int elapsed_seconds;
    int timer_running; 
} World;

GameStateEnum current_state = STATE_MENU;
World world;
int screen_h, screen_w;
long frame_count = 0;

const char *TITLE_ART[] = {
    "  _   _  _____  ____  _   _ ",
    " | \\ | || ____|/ __ \\| \\ | |",
    " |  \\| ||  _| | |  | |  \\| |",
    " | . ` || |___| |__| | . ` |",
    " |_|\\_||_____|\\____/|_|\\_|",
    "  V E N D E T T A  2 0 2 5  "
};

const char LEVEL_1[MAP_HEIGHT][MAP_WIDTH + 1] = {
    "############################################################",
    "#..........................................................#",
    "#..############............................................#",
    "#..#..........#.......E....................................#",
    "#..#..........#............................................#",
    "#..#....@.....#............................................#",
    "#..####.###################...#############...##############",
    "#.........................#...#...........#...#............#",
    "#...................E..... ...#.....E.....#...#......A.....#",
    "#.......#######...........#...#...........#...#............#",
    "#.......#.....#...........#...###.......###...#............#",
    "#.......#..E.. ...........#................................#",
    "#.......#.....#............................................#",
    "#.......#######.......................E....................#",
    "#..........................................................#",
    "#..........................................................#",
    "#..##########################.....#######################..#",
    "#..#........................#.....#.....................#..#",
    "#..#..........A.............#.....#..........>..........#..#",
    "#..#.........E.............. .....#.....................#..#",
    "#..#........................#.....#.....................#..#",
    "#..#........................#.....#.....................#..#",
    "#..##########################.....###################...#..#",
    "#..........................................................#",
    "############################################################"
};

const char LEVEL_2[MAP_HEIGHT][MAP_WIDTH + 1] = {
    "############################################################",
    "#@.........#...........#.......................#...........#",
    "#####............E.....#.....#############.....#.....E.....#",
    "#..........#...........#.....#...........#.....#...........#",
    "#.....##################.....#.....A.....#.....#######.....#",
    "#............................#...........#.................#",
    "#############.....############.....#######.....#############",
    "#...........#.....#................#.................#.....#",
    "#.....E.....#.....#.......E........#.......................#",
    "#...........#.....#................#######...........#.....#",
    "#######.....#.....####......########.....#.....#######.....#",
    "#...........#............................#.....#...........#",
    "#.....#######............................#.....#.....E.....#",
    "#.....#...........#########..............#.....#...........#",
    "#...........E.....#.......#.....##########.....#############",
    "#.................#...E...#.....#........#.................#",
    "#.....#############.......#.........E....#.................#",
    "#.........................#..............###########.......#",
    "#####################.....#.....#..................#.......#",
    "#.........#.........#.....#.....##########.........#...E...#",
    "#....A....#....E....#.....#..............#....A....#.......#",
    "#.........#.........#.....#..............#.........#.......#",
    "#####.....####...####.....############...###########...#####",
    "#....................................#.................>...#",
    "############################################################"
};

int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

float distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void spawn_blood(int x, int y, int amount) {
    for (int i = 0; i < amount; i++) {
        int idx = -1;
        for (int j = 0; j < MAX_PARTICLES; j++) {
            if (!world.particles[j].active) {
                idx = j;
                break;
            }
        }
        if (idx != -1) {
            world.particles[idx].active = 1;
            world.particles[idx].x = (float)x;
            world.particles[idx].y = (float)y;
            float angle = (float)(rand() % 360) * 3.14 / 180.0;
            float speed = (float)(rand() % 10) / 10.0;
            world.particles[idx].vx = cos(angle) * speed;
            world.particles[idx].vy = sin(angle) * speed;
            world.particles[idx].life = random_int(5, 15);
            world.particles[idx].symbol = '.'; 
            world.particles[idx].color = CP_BLOOD;
        }
    }
}

void update_particles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (world.particles[i].active) {
            Particle *p = &world.particles[i];
            p->x += p->vx;
            p->y += p->vy;
            p->life--;

            int ix = (int)p->x;
            int iy = (int)p->y;
            
            if (ix >= 0 && ix < MAP_WIDTH && iy >= 0 && iy < MAP_HEIGHT) {
                if (world.grid[iy][ix] == '#') {
                    p->active = 0; 
                } else {
                    if (p->life <= 0) {
                        p->active = 0;
                        if (world.blood_layer[iy][ix] != SYM_CORPSE)
                            world.blood_layer[iy][ix] = ',';
                    }
                }
            } else {
                p->active = 0;
            }
        }
    }
}

void init_level(int level_num) {
    world.current_level = level_num;
    world.enemy_count = 0;
    world.combo = 0;
    world.combo_timer = 0;
    world.attack_anim_timer = 0;
    
    world.start_time = time(NULL);
    world.elapsed_seconds = 0;
    world.timer_running = 1;

    for(int i=0; i<MAX_PARTICLES; i++) world.particles[i].active = 0;

    const char (*src)[MAP_WIDTH + 1] = (level_num == 1) ? LEVEL_1 : LEVEL_2;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            char c = src[y][x];
            world.grid[y][x] = c;
            world.blood_layer[y][x] = 0;

            if (c == SYM_PLAYER) {
                world.player.pos.x = x;
                world.player.pos.y = y;
                world.player.hp = 1;
                world.player.active = 1;
                world.player.weapon = WEAPON_PISTOL;
                world.player.ammo = 6; 
                world.grid[y][x] = SYM_FLOOR;
            } else if (c == SYM_ENEMY) {
                if (world.enemy_count < MAX_ENTITIES) {
                    Enemy *e = &world.enemies[world.enemy_count++];
                    e->pos.x = x;
                    e->pos.y = y;
                    e->hp = 1;
                    e->active = 1;
                    e->alert_level = 0;
                }
                world.grid[y][x] = SYM_FLOOR;
            }
        }
    }
}

int is_solid(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return 1;
    return (world.grid[y][x] == '#');
}

void attack_enemy(int idx) {
    if (!world.enemies[idx].active) return;
    
    world.enemies[idx].active = 0;
    spawn_blood(world.enemies[idx].pos.x, world.enemies[idx].pos.y, 10);
    world.blood_layer[world.enemies[idx].pos.y][world.enemies[idx].pos.x] = SYM_CORPSE;
    
    world.score += 100 + (world.combo * 50);
    world.combo++;
    world.combo_timer = 40; 
}

void player_attack() {
    world.attack_anim_timer = 3; 
    
    float range = 0;
    if (world.player.weapon == WEAPON_KNIFE) range = 2.0;
    if (world.player.weapon == WEAPON_PISTOL) range = 15.0;

    if (world.player.weapon == WEAPON_PISTOL) {
        if (world.player.ammo > 0) {
            world.player.ammo--;
            world.attack_mode = 1; 
        } else {
            world.attack_mode = 0; 
            return; 
        }
    } else {
        world.attack_mode = 1; 
    }

    int target_idx = -1;
    float min_dist = 999.0;

    for (int i = 0; i < world.enemy_count; i++) {
        if (!world.enemies[i].active) continue;
        
        float d = distance(world.player.pos.x, world.player.pos.y, 
                           world.enemies[i].pos.x, world.enemies[i].pos.y);
        
        if (d <= range && d < min_dist) {
            int clear_shot = 1;
            float dx = (world.enemies[i].pos.x - world.player.pos.x) / d;
            float dy = (world.enemies[i].pos.y - world.player.pos.y) / d;
            float cx = world.player.pos.x + 0.5;
            float cy = world.player.pos.y + 0.5;
            
            for(int s=0; s<(int)d; s++) {
                if (is_solid((int)cx, (int)cy)) { clear_shot = 0; break; }
                cx += dx; cy += dy;
            }

            if (clear_shot) {
                min_dist = d;
                target_idx = i;
            }
        }
    }

    if (target_idx != -1) {
        attack_enemy(target_idx);
    }
}

void update_ai() {
    if (frame_count % 4 != 0) return; 

    for (int i = 0; i < world.enemy_count; i++) {
        Enemy *e = &world.enemies[i];
        if (!e->active) continue;

        float dist = distance(e->pos.x, e->pos.y, world.player.pos.x, world.player.pos.y);

        int see = 0;
        if (dist < 10.0) {
            int clear_los = 1;
            float dx = (world.player.pos.x - e->pos.x) / dist;
            float dy = (world.player.pos.y - e->pos.y) / dist;
            float cx = e->pos.x + 0.5;
            float cy = e->pos.y + 0.5;
            
            for(int s=0; s<(int)dist; s++) {
                if (is_solid((int)cx, (int)cy)) { clear_los = 0; break; }
                cx += dx; cy += dy;
            }
            if (clear_los) see = 1;
        }

        if (see) e->alert_level = 2;

        if (e->alert_level == 2) {
            int nx = e->pos.x;
            int ny = e->pos.y;
            
            if (world.player.pos.x > e->pos.x) nx++;
            else if (world.player.pos.x < e->pos.x) nx--;
            
            if (world.player.pos.y > e->pos.y) ny++;
            else if (world.player.pos.y < e->pos.y) ny--;

            if (!is_solid(nx, ny)) {
                int occupied = 0;
                for(int j=0; j<world.enemy_count; j++) {
                    if (i!=j && world.enemies[j].active && world.enemies[j].pos.x == nx && world.enemies[j].pos.y == ny) occupied = 1;
                }
                if (!occupied) {
                    e->pos.x = nx;
                    e->pos.y = ny;
                }
            }
            
            if (dist < 1.1) {
                world.timer_running = 0; 
                current_state = STATE_GAMEOVER;
            }
        }
    }
}

void update_game() {
    if (world.timer_running) {
        world.elapsed_seconds = (int)(time(NULL) - world.start_time);
    }

    if (world.combo_timer > 0) world.combo_timer--;
    else world.combo = 0;
    
    if (world.attack_anim_timer > 0) world.attack_anim_timer--;

    update_particles();
    update_ai();
}

void init_curses() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(30); 

    start_color();
    
    init_pair(CP_DEFAULT, COLOR_WHITE, COLOR_BLACK);
    init_pair(CP_WALL, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CP_PLAYER, COLOR_CYAN, COLOR_BLACK);
    init_pair(CP_ENEMY, COLOR_WHITE, COLOR_RED);
    init_pair(CP_BLOOD, COLOR_RED, COLOR_BLACK);
    init_pair(CP_FLOOR, COLOR_BLUE, COLOR_BLACK);
    init_pair(CP_ITEM, COLOR_YELLOW, COLOR_BLACK);
    init_pair(CP_TITLE, COLOR_CYAN, COLOR_MAGENTA);
    init_pair(CP_UI_BOX, COLOR_CYAN, COLOR_BLACK); 
    init_pair(CP_EXIT, COLOR_GREEN, COLOR_BLACK);
    init_pair(CP_ATTACK, COLOR_YELLOW, COLOR_RED);
    init_pair(CP_NO_AMMO, COLOR_BLACK, COLOR_WHITE);
    init_pair(CP_UI_TEXT, COLOR_WHITE, COLOR_BLACK);
}

void draw_ui_box(int x, int y, int w, int h) {
    attron(COLOR_PAIR(CP_UI_BOX));
    mvhline(y, x, ACS_HLINE, w);
    mvhline(y + h, x, ACS_HLINE, w);
    mvvline(y, x, ACS_VLINE, h);
    mvvline(y, x + w, ACS_VLINE, h);
    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(y, x + w, ACS_URCORNER);
    mvaddch(y + h, x, ACS_LLCORNER);
    mvaddch(y + h, x + w, ACS_LRCORNER);
    attroff(COLOR_PAIR(CP_UI_BOX));
}

void draw_game_ui() {
    int x = UI_OFFSET_X;
    
    draw_ui_box(x - 2, 0, 22, MAP_HEIGHT);
    
    attron(COLOR_PAIR(CP_TITLE) | A_BOLD);
    mvprintw(2, x, " LEVEL: %d ", world.current_level);
    attroff(COLOR_PAIR(CP_TITLE) | A_BOLD);
    
    int min = world.elapsed_seconds / 60;
    int sec = world.elapsed_seconds % 60;
    attron(COLOR_PAIR(CP_UI_TEXT));
    mvprintw(4, x, " TIME: %02d:%02d", min, sec);
    
    mvprintw(6, x, " SCORE: %06d", world.score);
    attroff(COLOR_PAIR(CP_UI_TEXT));
    
    attron(COLOR_PAIR(CP_UI_BOX));
    mvhline(8, x-1, ACS_HLINE, 20);
    attroff(COLOR_PAIR(CP_UI_BOX));

    attron(COLOR_PAIR(CP_UI_TEXT) | A_BOLD);
    mvprintw(10, x, " WEAPON:");
    attroff(COLOR_PAIR(CP_UI_TEXT) | A_BOLD);

    if (world.player.weapon == WEAPON_PISTOL) {
        attron(COLOR_PAIR(CP_ITEM) | A_BOLD);
        mvprintw(11, x + 2, "PISTOL");
        attroff(COLOR_PAIR(CP_ITEM) | A_BOLD);
        
        mvprintw(12, x, " AMMO:");
        if (world.player.ammo > 0) {
            attron(COLOR_PAIR(CP_ITEM) | A_BOLD);
            mvprintw(12, x + 7, "%d / 6", world.player.ammo);
            attroff(COLOR_PAIR(CP_ITEM) | A_BOLD);
        } else {
            attron(COLOR_PAIR(CP_BLOOD) | A_BLINK | A_BOLD);
            mvprintw(12, x + 7, "EMPTY!");
            attroff(COLOR_PAIR(CP_BLOOD) | A_BLINK | A_BOLD);
        }
    } else {
        attron(COLOR_PAIR(CP_TITLE));
        mvprintw(11, x + 2, "KNIFE");
        attroff(COLOR_PAIR(CP_TITLE));
        mvprintw(12, x, " AMMO: --");
    }

    int enemies_left = 0;
    for(int i=0; i<world.enemy_count; i++) if(world.enemies[i].active) enemies_left++;
    
    attron(COLOR_PAIR(CP_UI_TEXT));
    mvprintw(15, x, " TARGETS: %d ", enemies_left);
    attroff(COLOR_PAIR(CP_UI_TEXT));
    
    if (world.combo > 1) {
        attron(COLOR_PAIR(CP_TITLE) | A_BLINK);
        mvprintw(17, x, " COMBO x%d", world.combo);
        attroff(COLOR_PAIR(CP_TITLE) | A_BLINK);
    }

    if (enemies_left == 0) {
        attron(COLOR_PAIR(CP_EXIT) | A_BLINK | A_BOLD);
        mvprintw(20, x, " EXIT OPEN! ");
        attroff(COLOR_PAIR(CP_EXIT) | A_BLINK | A_BOLD);
    }
    
    attron(A_DIM);
    mvprintw(MAP_HEIGHT - 2, x, " [ESC] MENU");
    attroff(A_DIM);
}

void draw_world() {
    erase();

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            char cell = world.grid[y][x];
            char blood = world.blood_layer[y][x];
            
            if (cell == '#') {
                attron(COLOR_PAIR(CP_WALL));
                mvaddch(y, x, ACS_CKBOARD);
                attroff(COLOR_PAIR(CP_WALL));
            } else if (cell == SYM_EXIT) {
                attron(COLOR_PAIR(CP_EXIT) | A_BOLD);
                mvaddch(y, x, ACS_DIAMOND); 
                attroff(COLOR_PAIR(CP_EXIT) | A_BOLD);
            } else if (cell == SYM_AMMO) {
                attron(COLOR_PAIR(CP_ITEM) | A_BOLD | A_BLINK);
                mvaddch(y, x, 'A'); 
                attroff(COLOR_PAIR(CP_ITEM) | A_BOLD | A_BLINK);
            } else if (blood != 0) {
                attron(COLOR_PAIR(CP_BLOOD));
                if (blood == SYM_CORPSE) mvaddch(y, x, 'X');
                else mvaddch(y, x, ACS_BULLET); 
                attroff(COLOR_PAIR(CP_BLOOD));
            } else {
                attron(COLOR_PAIR(CP_FLOOR) | A_DIM);
                mvaddch(y, x, ' ');
                attroff(COLOR_PAIR(CP_FLOOR) | A_DIM);
            }
        }
    }

    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (world.particles[i].active) {
            attron(COLOR_PAIR(world.particles[i].color));
            mvaddch((int)world.particles[i].y, (int)world.particles[i].x, '*');
            attroff(COLOR_PAIR(world.particles[i].color));
        }
    }

    for (int i = 0; i < world.enemy_count; i++) {
        if (world.enemies[i].active) {
            attron(COLOR_PAIR(CP_ENEMY) | A_BOLD);
            mvaddch(world.enemies[i].pos.y, world.enemies[i].pos.x, 'E');
            attroff(COLOR_PAIR(CP_ENEMY) | A_BOLD);
        }
    }

    attron(COLOR_PAIR(CP_PLAYER) | A_BOLD);
    
    if (world.attack_anim_timer > 0) {
        if (world.attack_mode == 1) { 
            attron(COLOR_PAIR(CP_ATTACK));
            mvaddch(world.player.pos.y, world.player.pos.x, '@');
            attroff(COLOR_PAIR(CP_ATTACK));
            
            mvaddch(world.player.pos.y+1, world.player.pos.x, '|');
            mvaddch(world.player.pos.y-1, world.player.pos.x, '|');
            mvaddch(world.player.pos.y, world.player.pos.x+1, '-');
            mvaddch(world.player.pos.y, world.player.pos.x-1, '-');
        } else { 
            attron(COLOR_PAIR(CP_NO_AMMO));
            mvaddch(world.player.pos.y, world.player.pos.x, '@');
            attroff(COLOR_PAIR(CP_NO_AMMO));
            mvprintw(world.player.pos.y-1, world.player.pos.x-1, "CLICK");
        }
    } else {
        mvaddch(world.player.pos.y, world.player.pos.x, '@');
    }
    attroff(COLOR_PAIR(CP_PLAYER) | A_BOLD);

    draw_game_ui();
    refresh();
}

void draw_menu(int selection) {
    clear();
    int mid_x = COLS / 2;
    int mid_y = LINES / 2;

    attron(COLOR_PAIR(CP_TITLE) | A_BOLD);
    for (int i = 0; i < 6; i++) {
        mvprintw(mid_y - 10 + i, mid_x - 15, "%s", TITLE_ART[i]);
    }
    attroff(COLOR_PAIR(CP_TITLE) | A_BOLD);
    
    draw_ui_box(mid_x - 20, mid_y - 2, 40, 10);

    const char *options[] = {"NEW GAME", "CONTROLS", "QUIT"};
    
    for (int i = 0; i < 3; i++) {
        if (i == selection) {
            attron(COLOR_PAIR(CP_PLAYER) | A_REVERSE); 
            mvprintw(mid_y + 2 + (i * 2), mid_x - 5, " %s ", options[i]);
            attroff(COLOR_PAIR(CP_PLAYER) | A_REVERSE);
        } else {
            mvprintw(mid_y + 2 + (i * 2), mid_x - 5, " %s ", options[i]);
        }
    }
    
    mvprintw(LINES - 2, 2, "PLEASE USE ENGLISH KEYBOARD LAYOUT!");
    refresh();
}

void draw_help() {
    clear();
    draw_ui_box(2, 2, 50, 20);
    attron(A_BOLD);
    mvprintw(4, 5, "CONTROLS:");
    mvprintw(6, 5, "W, A, S, D  - Move (OR ARROWS)");
    mvprintw(7, 5, "SPACE       - Attack / Shoot");
    mvprintw(8, 5, "TAB or 1    - Switch Weapon");
    mvprintw(9, 5, "R           - Restart Level");
    mvprintw(10, 5,"ESC         - Menu");
    
    mvprintw(13, 5, "MECHANICS:");
    mvprintw(14, 5, "Pistol: Long range, Limited Ammo");
    mvprintw(15, 5, "Knife:  Short range, Infinite");
    mvprintw(16, 5, "Pick up 'A' for Ammo.");
    attroff(A_BOLD);
    
    mvprintw(20, 5, "Press ANY KEY to return");
    refresh();
}

void draw_gameover() {
    bkgd(COLOR_PAIR(CP_BLOOD));
    clear();
    attron(A_BOLD);
    draw_ui_box(COLS/2 - 15, LINES/2 - 5, 30, 10);
    mvprintw(LINES/2 - 2, COLS/2 - 4, "YOU DIED");
    mvprintw(LINES/2, COLS/2 - 9, "Time: %02d:%02d", world.elapsed_seconds/60, world.elapsed_seconds%60);
    mvprintw(LINES/2 + 2, COLS/2 - 8, "[R]etry  [M]enu");
    attroff(A_BOLD);
    refresh();
}

void draw_victory() {
    bkgd(COLOR_PAIR(CP_EXIT));
    clear();
    attron(A_BOLD);
    draw_ui_box(COLS/2 - 15, LINES/2 - 5, 30, 10);
    mvprintw(LINES/2 - 2, COLS/2 - 4, "VICTORY!");
    mvprintw(LINES/2, COLS/2 - 9, "Time: %02d:%02d", world.elapsed_seconds/60, world.elapsed_seconds%60);
    mvprintw(LINES/2 + 2, COLS/2 - 10, "Press [M] for Menu");
    attroff(A_BOLD);
    refresh();
}

int main() {
    srand(time(NULL));
    init_curses();
    getmaxyx(stdscr, screen_h, screen_w);

    int running = 1;
    int menu_selection = 0;

    while (running) {
        frame_count++;

        int ch = getch(); 
        if (ch >= 'A' && ch <= 'Z') ch = tolower(ch);

        switch (current_state) {
            case STATE_MENU:
                timeout(-1); 
                if (ch == 'w' || ch == KEY_UP) menu_selection--;
                if (ch == 's' || ch == KEY_DOWN) menu_selection++;
                if (menu_selection < 0) menu_selection = 2;
                if (menu_selection > 2) menu_selection = 0;
                
                if (ch == '\n' || ch == ' ') {
                    if (menu_selection == 0) {
                        timeout(30);
                        world.score = 0;
                        init_level(1);
                        current_state = STATE_PLAY;
                    } else if (menu_selection == 1) {
                        timeout(-1);
                        current_state = STATE_HELP;
                    } else {
                        running = 0;
                    }
                }
                draw_menu(menu_selection);
                break;

            case STATE_HELP:
                if (ch != ERR) current_state = STATE_MENU;
                draw_help();
                break;

            case STATE_PLAY:
                if (ch == 27) { 
                    current_state = STATE_MENU;
                    timeout(-1);
                }
                if (ch == 'r') init_level(world.current_level);
                
                if (ch == '\t' || ch == '1') {
                    if (world.player.weapon == WEAPON_PISTOL) world.player.weapon = WEAPON_KNIFE;
                    else world.player.weapon = WEAPON_PISTOL;
                }

                int nx = world.player.pos.x;
                int ny = world.player.pos.y;
                int moved = 0;

                if (ch == 'w' || ch == KEY_UP) { ny--; moved = 1; }
                if (ch == 's' || ch == KEY_DOWN) { ny++; moved = 1; }
                if (ch == 'a' || ch == KEY_LEFT) { nx--; moved = 1; }
                if (ch == 'd' || ch == KEY_RIGHT) { nx++; moved = 1; }
                
                if (moved && !is_solid(nx, ny)) {
                    if (world.grid[ny][nx] == SYM_AMMO) {
                        world.player.ammo += 3;
                        world.grid[ny][nx] = SYM_FLOOR;
                    }

                    world.player.pos.x = nx;
                    world.player.pos.y = ny;
                    
                    if (world.grid[ny][nx] == SYM_EXIT) {
                        int alive = 0;
                        for(int i=0; i<world.enemy_count; i++) 
                            if (world.enemies[i].active) alive++;
                        
                        if (alive == 0) {
                            world.timer_running = 0;
                            if (world.current_level == 1) init_level(2);
                            else {
                                current_state = STATE_VICTORY;
                                timeout(-1);
                            }
                        }
                    }
                }

                if (ch == ' ') player_attack();

                update_game();
                draw_world();
                break;

            case STATE_GAMEOVER:
                timeout(-1); 
                if (ch == 'r') {
                    timeout(30);
                    init_level(world.current_level);
                    bkgd(COLOR_PAIR(CP_DEFAULT));
                    current_state = STATE_PLAY;
                }
                if (ch == 'm') {
                    bkgd(COLOR_PAIR(CP_DEFAULT));
                    current_state = STATE_MENU;
                }
                draw_gameover();
                break;
                
            case STATE_VICTORY:
                 timeout(-1);
                 if (ch == 'm') {
                    bkgd(COLOR_PAIR(CP_DEFAULT));
                    current_state = STATE_MENU;
                }
                draw_victory();
                break;

            case STATE_EXIT:
                running = 0;
                break;
        }
    }

    endwin();
    return 0;
}