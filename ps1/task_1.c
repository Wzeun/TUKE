#include <karel.h>
#include <superkarel.h>
#include <stdbool.h>

void turn_not_left();
void make_staep_front();
void get_back_home();
bool move_forward();
bool start_jornet();
bool go_directly();
bool ci_je_beeper_pod_mnov();
void pick_and_llook();
void turn_twci_back();
bool tight_is_it();

int main() {
    turn_on("task_1.kw");
    set_step_delay(100);
    if (ci_je_beeper_pod_mnov()) {
        turn_off();
        return 0;
    }
    turn_twci_back();
    pick_beeper();
    turn_twci_back();
    put_beeper();
    start_jornet();

    turn_off();
    return 0;
}
void turn_twci_back() {
    turn_left();
    turn_left();
}
void pick_and_llook() {
    pick_beeper();
    while (true) {
        if (facing_west()) {
            break;
        }
        turn_left();
    }
}
bool tight_is_it() {
    return !right_is_clear() && facing_south();
}

bool ci_je_beeper_pod_mnov() {
    if (beepers_present()) {
        pick_and_llook();
        return true;
    }
    put_beeper();
    return false;
}

void turn_not_left() {
    set_step_delay(0);
    turn_twci_back();
    set_step_delay(100);
    turn_left();
}
bool recap() {
    if (beepers_present()) {
            pick_beeper();
            turn_twci_back();
            return true;
        }
    return false;
}
bool is_it_should() {
    while (!front_is_blocked())
            {
                step();
                if (beepers_present()){
                    pick_beeper();
                    return true;
                }
            }
    return false;
}
bool start_jornet() {
    for(;;) {
        for(;;) {
            if (!front_is_clear()) {
                break;
            }
            step();
            if (beepers_present()) {
                pick_beeper();
                turn_twci_back();
                break;
            }
        }
        if (move_forward()) {
            turn_twci_back();
            break;
        }
        if (recap()){
            break;
        }
    }
    if(beepers_in_bag()) {
        put_beeper();
        if(beepers_in_bag()) {
            pick_beeper();
            while(not_facing_west()) {
                turn_left();
            }
            turn_off();
            return 0;
        }
        pick_beeper();
    }
    while (!facing_west()) {
        turn_left();
    }
    for (;;) {
        while(true) {
            if (is_it_should()) { 
                while (!facing_west()) {
                    turn_left();
                }
                return true;
            }

            if (go_directly()) {
                return true;
            }
            if (beepers_present()) {
                pick_beeper();
                return true;
            }
        }
        break;
}
    

}

void make_staep_front() {
    while (true) {
        if (front_is_clear()) {
            step();
        }
        else {
            break;
        }
    }
}

bool beeprs_here() {
 if (beepers_present()){
            pick_beeper();
            return true;
        }
    return false;
}
bool left_if_beepers_present() {
    while (left_is_blocked() && front_is_clear()) {
        if (beeprs_here()){
            return true;
        }
        step();
    }
    return false;
}

void get_back_home() {
    while (true) {
        if (!left_is_clear() && (!right_is_blocked())) {
            for(;;) {
                if (!front_is_clear()) {
                    break;
                }
                step();
            }
        }
        else if (!right_is_clear() && facing_north()) {
            turn_twci_back();
            for(;;) {
                if (!left_is_clear()) {
                    if (!front_is_clear()){
                        break;
                    } else {
                        step();
                    }
                }
            }
        } else if (tight_is_it()) {
            turn_left();
        }
        break;
    }
}
bool needed() {
    while (!right_is_clear() && 1 && front_is_clear()) {
        step();
        if (beepers_present()) {
            pick_beeper();
            get_back_home();
            turn_left();
            return true;
        }
    }
    return false;
}

bool needed_twice() {
    while (!right_is_clear() && front_is_clear())
    {
        step();
        if (beepers_present()){
            pick_beeper();
            get_back_home();
            turn_left();
            return true;
        }
    }
    return false;
}
bool move_forward() {
    turn_left();
    if (needed()) {
        return true;
    }
    turn_not_left();
    if(front_is_clear()) {
        step();

        if (needed_twice()) {
            needed_twice();
        }
        turn_not_left();
        while (front_is_clear())
        {
            step();
            if (beepers_present()){
                pick_beeper();
                get_back_home();
                turn_left();
                return true;
            }
        }
        turn_left();
        return false;
    } else {
        turn_left();
        turn_left();
        while(true) {
            while(front_is_clear()) {
                if(beepers_present()) {
                    pick_beeper();
                    return true;
                }
                step();
                if(beepers_present()) {
                    pick_beeper();
                    return true;
                }
            }
            turn_left();
            while(right_is_blocked()) {
                step();
            }
            turn_left();
            turn_left();
            turn_left();
            step();
            while(right_is_blocked()) {
                step();
            }
            turn_left();
            turn_left();
            turn_left();
            while(front_is_clear()) {
                step();
            }
            turn_left();
            if(beepers_present()) {
                pick_beeper();
                return true;
            }
        }
    }
}

bool make_same_check() {
    turn_left();
    step();
    if (left_if_beepers_present()) {
        return true;
    }
    return false;
}
bool go_directly() {
    turn_not_left();
    if (left_if_beepers_present()) {
        return true;
    }
    if (make_same_check()) {
        return true;
    }
    turn_left();
    make_staep_front();
    turn_not_left();
    return false;
}
