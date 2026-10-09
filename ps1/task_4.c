#include <superkarel.h>

#define SPEED 15

void begin() {
    while(!facing_south()) {
        turn_left();
    }
    while(front_is_clear()) {
        step();
    }
    while(!facing_west()) {
        turn_left();
    }
    while(front_is_clear()) {
        step();
    }
    turn_left();
    turn_left();
    
    while(front_is_clear()) {
        step();
    }

    turn_left();
}
int main()
{
    turn_on("task_4.kw");
    set_step_delay(SPEED);
    begin();
    while (true) {
        if (front_is_clear() && beepers_present()) {
            while(front_is_clear()){
                step();
                if (no_beepers_present()) {
                    put_beeper();
                }
            }
            turn_left();
            turn_left();
            while(front_is_clear()) {
                step();
            }
            turn_left();
            turn_left();
            turn_left();
            if(front_is_clear()) {
                step();
            } else {
                break;
            }

            turn_left();

            turn_left();

            turn_left();
        } else if ( front_is_clear() && no_beepers_present()) {
            while(front_is_clear() && no_beepers_present()) {
                step();
            }
            if (beepers_present()) {
                turn_left();
                turn_left();
                while(front_is_clear()) {
                    step();
                    if (!beepers_present()){
                        put_beeper();
                    }
                }
                turn_left();
                turn_left();
                while(front_is_clear()) {
                    step();
                    if (!beepers_present()){
                        put_beeper();
                    }
                }
                turn_left();
                turn_left();
                while(front_is_clear()) {
                    step();
                    if (!beepers_present()){
                        put_beeper();
                    }
                }
                turn_left();
                turn_left();
                turn_left();
                if (front_is_clear()) {
                    step();
                    turn_left();
                    turn_left();
                    turn_left();
                } else {
                    break;
                }
            } else {
                turn_left();
                turn_left();
                while(front_is_clear()) {
                    step();
                }
                turn_left();
                turn_left();
                turn_left();
                if (front_is_clear()) {
                    step();
                    turn_left();
                    turn_left();
                    turn_left();
                } else {
                    break;
                }
            }
        }
    }
    turn_left();
    turn_left();
    while(front_is_clear()) {
        step();
    }
    turn_off();
    return 0;
}
