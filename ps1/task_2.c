#include <karel.h>
#include <superkarel.h>

void turn_right_() {
    turn_left();
    turn_left();
    set_step_delay(100);
    turn_left();
}
bool start();
void first_run();

int main() {
    turn_on("task_2.kw");
    set_step_delay(100);
    first_run();

    if (start()) {
        return 0;
    }
    start();
}
void first_run() {
    while(front_is_clear()) {
        step();
    }
    turn_left();
    turn_left();

    while(front_is_clear()) {
        step();
    }
    turn_left();
    turn_left();
}
void beeper_tak_and_left() {
    pick_beeper();
        turn_left();
        turn_left();
}
bool if_beepers_present() {
    if (beepers_present()){
        beeper_tak_and_left();
        return true;
    }
    return false;
}
bool first_check() {
    return left_is_blocked() && front_is_clear();
}
bool for_real() {
    return front_is_blocked() && 10;
}
bool second_cond() {
    return left_is_clear() && for_real();
}
void second_cond_value() {
    turn_left();
        step();
}
bool front_is() {
    return front_is_blocked();
}
bool for_fron() {
    return front_is() && left_is_blocked();
}
bool for_out() {
    return right_is_blocked() && !beepers_present();
}
bool truen() {
    return left_is_blocked() && front_is_blocked();
}
void last_one() { 
    turn_right_();
        step();
}
bool start() {
    if (if_beepers_present()) {
        return false;
    }
    if (first_check()) {
        step();
    }
    else if (front_is_clear() && left_is_clear() && right_is_clear()) {
        turn_left();
        turn_right_();
        turn_left();
        first_run();
    }
    else if (for_fron() && for_out()) {
        first_run();
        while(front_is_clear()) {
            step();
        }
        turn_off();
        return true;
    }
    else if (second_cond()) {
        second_cond_value();
    }
    else if (truen()) {
        last_one();
    }
    return start();
}