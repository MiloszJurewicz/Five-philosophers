#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <algorithm>
#include <mutex>
#include "Philosopher.h"
#include "DrawGui.h"
#include <ncurses.h>

using std::move;
using std::vector;
using std::unique_ptr;
using std::thread;
using std::for_each;
using std::mem_fn;
using std::mutex;
using std::ref;

class Chopstick
{
public:
    Chopstick() = default;;
    mutex m;
};

mutex _muGUI;

int main() {

    initGui();

    static const int numPhilosophers = 5;

    // 5 utencils on the left and right of each philosopher. Use them to acquire locks.
    vector< unique_ptr<Chopstick> > chopsticks(numPhilosophers);

    for (int i = 0; i < numPhilosophers; ++i)
    {
        auto c1 = unique_ptr<Chopstick>(new Chopstick());
        chopsticks[i] = move(c1);
        drawChopstick(i, -1, "free");
    }

    vector<thread> tasks(numPhilosophers);
    for(int i = 0; i < numPhilosophers; i++){
        Philosopher *p = new Philosopher(i);
        if(i == 0){
            tasks[i] = (thread(&Philosopher::routine,
                              p,
                              ref(chopsticks[numPhilosophers - 1].get()->m),
                              ref(chopsticks[i].get()->m),
                              ref(_muGUI),
                              0,
                              numPhilosophers - 1)
            );
            //philosopherThread.detach();
        }else{
            tasks[i] = (thread(&Philosopher::routine,
                              p,
                              ref(chopsticks[i - 1].get()->m),
                              ref(chopsticks[i].get()->m),
                              ref(_muGUI),
                              i - 1,
                              i)
            );
            //philosopherThread.detach();
        }

    }

    for_each(tasks.begin(), tasks.end(), mem_fn(&thread::join));

    //thread clock(clockFunc);
    //clock.join();

    curs_set(FALSE);
    endwin();

    return 0;
}