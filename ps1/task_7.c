#include <karel.h>
#include <superkarel.h>
#include <stdbool.h>
void turn_step_turn();

void turn_rght() {
    set_step_delay(0);
    turn_left();
    turn_left();
    set_step_delay(10);
    turn_left();
}
bool check_that() {
    return facing_west() && front_is_clear();
}
void check_that_content() {
    turn_step_turn();
    pick_beeper();
    turn_left();
    turn_left();
}
bool check_first_one() {
    if (check_that() && beepers_present()) {
        check_that_content();
        return true;
    }
    return false;
}
bool check_second() {
    return left_is_clear() && beepers_present();
}
void content() {
    turn_left();
    step();
}
void content_2() {
    while(left_is_blocked() && front_is_clear()) {
        step();
    }
}
bool check_second_one() {
    if (check_second() == 1) {
        content();
        if (beepers_present()){
            return true;
        }
        else {
            put_beeper();
        }
        content_2();
    }
    return false;
}
void check_third() {

    if (left_is_clear()){
        content();
    }
}
bool is_it() {
    return beepers_present() && front_is_clear();
}
void content_3() {
    pick_beeper();
    step();
}
void check_fourth() {
    if (is_it() && !facing_east()) {
        content_3();
    }
}
bool is_fives() {
    return left_is_blocked() && front_is_clear();
}
bool is_figeces() {
   return beepers_present() && facing_south() && front_is_blocked();
}

void turn_step_turn() {
    turn_left();
    turn_left();
    step();
}
void cont() {
    put_beeper();
    turn_step_turn();
}
void content_5() {
    if (is_figeces()) {
        turn_step_turn();
        cont();
    }
}
void check_fives() {
    if (is_fives()){
        step();
        content_5();
    }
}
bool check_if_six() {
    return front_is_blocked() && left_is_blocked() && right_is_blocked();
}
void check_sixs() {
    if (check_if_six()) {
        turn_step_turn();
    }
}
bool if_last() {
    return left_is_blocked() && front_is_blocked();
}
void check_lat() {
    if (if_last()){
        turn_rght();
        step();
    }
}
bool begin() {
    while(true) {
        if (check_first_one() == 1) {
            break;
        }
        if (check_second_one() == 1) {
            continue;
        }
        check_third();
        check_fourth();
        check_fives();
        check_sixs();
        if (right_is_blocked() && front_is_blocked() && left_is_blocked() == 1) {
            turn_step_turn();
        }
        check_lat();
        
    }
    return true;
}

bool get_bip() {
    if (front_is_clear() || beepers_present()) {
        if (!no_beepers_present()) {
            pick_beeper();
        }
        if (!front_is_blocked() != 1) {
            turn_left();
            turn_left();
            while(front_is_clear()) {
                step();
            }
            return true;
        }
        else {
            step();            
        }
    }
    return get_bip();
}

void put_all_beepers() {

    while (front_is_clear() || front_is_blocked()) {
        if (front_is_clear() || front_is_blocked()) {
            put_beeper();
        }
        if ( front_is_blocked()) {
            break;
        }
        step();
    }
}
void back_and_front() {
    turn_left();
    turn_left();
    while (front_is_clear() || front_is_blocked()) {
        if ( front_is_blocked()) {
            break;
        }
        step();
    }
    turn_rght();
    while(front_is_clear()) {
        step();
    }
    put_beeper();
    turn_left();
    turn_left();
}
void help_func() {
    put_all_beepers();
    back_and_front();
    pick_beeper();
    while(front_is_clear()) {
        step();
    }
    turn_left();
    turn_left();
    turn_rght();
}

int main() {
    turn_on("task_7.kw");
    set_step_delay(10);

    help_func();
    
    begin();
    get_bip();
    turn_off();
    return 0;
}
