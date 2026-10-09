#include <karel.h>
#include <superkarel.h>
#include <stdbool.h>

void turn_not_left();
bool turn_west();
bool turn_south();
bool turn_north();
bool turn_east();
bool start_turning();
bool should_we_change_dir();
void making_turn_that_way();

int main() {
    turn_on("task_6.kw");
    set_step_delay(50);
    
    while (front_is_clear() || front_is_blocked() || left_is_blocked() || right_is_blocked() || right_is_clear() || left_is_clear()) {
        if (start_turning()){
            continue;
        } else {
            break;
        }
    }
    turn_off();
    return 0;
}


void turn_not_left() {
    turn_left();
    turn_left();
    set_step_delay(50);
    turn_left();
    set_step_delay(50);
}
void making_turn_that_way() {
    while (!facing_north())
    {
        turn_left();
    }
}
void making_turn_that_double() {
    while (!facing_west())
    {
        turn_left();
    }
}

void making_turn_that_thriple() {
    while (!facing_south()) {
        turn_left();
    }
}
void make_fortuh() {
    while (!facing_east())
        {
            turn_left();
        }
}
bool turn_west() {
    pick_beeper();

    if (beepers_present()){
        pick_beeper();
    } else {
        making_turn_that_way();
        return true;
    }
    return false;
}
bool turn_south() {
    if (beepers_present()){
        pick_beeper();
    } else {
        making_turn_that_double();
        return true;
    }
return false;
}
bool turn_north() {
    if (beepers_present()){
        pick_beeper();
    } else {
        making_turn_that_thriple();
        return true;
    }
    return false;
}
bool turn_east() {

    if (beepers_present()){
        pick_beeper();
    } else {   
        make_fortuh();
        return true;
    }
    return false;
}
bool should_we_change_dir() {
    if (turn_west())
        return true;
    if (turn_south())
        return true;
    if (turn_north())
        return true;
    if (turn_east())
        return true;
    return false;
}
bool start_turning() {
    while(no_beepers_present()) {
        step();
    }
    if (should_we_change_dir()) {
        return start_turning();
    } else {
        return false;
    }

}
