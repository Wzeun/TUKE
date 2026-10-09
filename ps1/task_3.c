#include <karel.h>
#include <superkarel.h>

void turn_right();
bool make_back();
void get_that_suit();
bool put_beepers_to_build_stair();
void run_for_the_end();

void configure_needed_angle() {
    while (!facing_east()){
        turn_right();
    }
}
int main() {
    turn_on("task_3.kw");
    set_step_delay(10);
    configure_needed_angle();
    run_for_the_end();
    make_back();

    turn_off();
    return 0;
}

void get_that_suit() {
    for (;;)
    {
        if (!front_is_clear()) {
            break;
        }
        step();
        if (beepers_present())
            pick_beeper();
    }
    turn_left();
    turn_left();
    for (;;)
    { 
        if (!front_is_clear()) {
            break;
        }
        step();
    }
    turn_left();
    turn_left();
    for (;;)
    { 
        if (!front_is_clear()) {
            break;
        }
        step();
    }
}
void continue_buildng() {
    if( beepers_present() && beepers_in_bag()) {
                    turn_left();
                    turn_left();
                    step();
                    put_beeper();
     }
       else {
           turn_left();
        turn_left();
     step();
           }
}

void main_check() {
    if (front_is_blocked()){
                if (beepers_in_bag()) {
                    put_beeper();
                }
                    turn_left();
                    turn_left();
            }
            else {
                step();
                if (front_is_clear()) {
                    step();
                    turn_left();
                    turn_left();
                    step();
                    turn_left();
                    turn_left();
                }
                continue_buildng();
            }
}
void ste_and_turn() {
    step();
            turn_right();
}
bool put_beepers_to_build_stair() {
    while (true) {
        while (true) {
            main_check();
            turn_right();
            if (front_is_blocked()){
                return true;
            }
            ste_and_turn();
        }
        break;
    }
}
void last() {
    turn_left();
    put_beepers_to_build_stair();
    turn_left();
}
void on_back() {
    get_that_suit();
    last();
}
void run_for_the_end(){
    on_back();
    if (front_is_clear()){
        step();
    }
    else {
        return;
    }
    turn_left();
    run_for_the_end();
}



void turn_right() {
    turn_left();
    turn_left();
    turn_left();
}
void directed() {

    turn_left();
    turn_left();
    turn_left();
    turn_left();
    turn_left();
    turn_left();
}
bool make_back(){
    directed();
    for (;;) 
    {
        if (front_is_clear()) {
            step();
        } else {
            break;
        }
    }
    turn_right();
    while (true) {
        while (true) {
            while (front_is_clear()){
                if (no_beepers_present()) {
                    step();
                }
                else{
                    directed();
                    while (front_is_clear())
                    {
                        step();
                    }
                    directed();
                    return true;
                }
            }
            directed();
            while (front_is_clear()){
                step();
            }
            turn_left();
            turn_left();

            while (front_is_clear()){
                step();
            }

            turn_left();
            turn_left();

            while (front_is_clear()){
                step();
            }
            turn_left();
            step();
            turn_left();
        }
        break;
    }
}

