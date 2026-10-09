#include <karel.h>
#include <superkarel.h>

void center();
void go_to_the_center();

void turn_right() {
    turn_left();
    turn_left();
    turn_left();
}

void make_it() {
    step();
    step();
}
void twice_to_center() {
    make_it();
}

void move_again() {
    center();
    step();
}
void turnin_lef() {
    turn_left();
    turn_left();
}
void making_step() {
    step();
}
void center() {
    twice_to_center();
    if (front_is_clear()){
        move_again();
    } else {
        turnin_lef();
        making_step();
    }
}

void make_step_right() {

    turn_right();
    while (front_is_clear())
    {
        step();
    }
    turn_right();
}
void putting_beeepers() {
    while (no_beepers_present()) {
                put_beeper();
                if (front_is_clear()){
                    step();
                    if (front_is_clear()) {
                        step();
                    }
                }
                else {
                    turn_right();
                    if (front_is_clear()) {
                        step();
                    }
                }
            }
}
void cartina_maslom() {
    while (not_facing_north()) {
        turn_left();
    }
    while (front_is_clear())
    {
            step();
    }
    make_step_right();

    for (;;) {
        while (left_is_blocked()) {
            putting_beeepers();
            turn_right();
            if (front_is_clear()) {
                step();
            }
            while(front_is_clear()) {
                if (no_beepers_present()) {
                    put_beeper();
                }
                step();
            }
            turn_right();
            if (front_is_clear()) {
                step();
            }
            while(front_is_clear()) {
                if (no_beepers_present()) {
                    put_beeper();
                }
                step();
            }
            turn_right();
            if (front_is_clear()) {
                step();
            }
            while(front_is_clear()) {
                if (no_beepers_present()) {
                    put_beeper();
                }
                step();
            }
            turn_right();
            if (front_is_clear()) {
                step();
            }
            while(front_is_clear()) {
                if (no_beepers_present()) {
                    put_beeper();
                }
                step();
            }
            turn_right();
            break;

        }
        go_to_the_center();
        while (not_facing_west()) {
            turn_left();
        }
        break;
    }
}

void go_to_the_center() {
    center();
    turn_left();
    center();
}
int main() {
    turn_on("task_5.kw");
    set_step_delay(10);
    
    cartina_maslom();
    for (;;) {
        if (facing_north()) {
            break;
        }
        turn_left();
    }
    turn_off();
    return 0;
}