#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <algorithm>
#include <mutex>
#include "Philosopher.h"
#include "DrawGui.h"
#include <ncurses.h>
#include <future>

using std::move;
using std::vector;
using std::unique_ptr;
using std::thread;
using std::for_each;
using std::mem_fn;
using std::mutex;
using std::ref;

mutex _muGUI;
std::chrono::milliseconds runTimer(10000);
static const int numPhilosophers = 5;

class Chopstick
{
public:
    Chopstick() = default;;
    mutex m;
};

//Sets bool to false which causes all threads to close after they finish they running cycle
void stopThreadsTimer(vector<Philosopher *> philosophers){

    //coment out getch and uncomment timer for it to work for given amount of time
    //for some reason u have to press getch twice for program to close (???)
    //std::this_thread::sleep_for(runTimer);
    getch();

    for(int i = 0; i < philosophers.size(); i++){
       philosophers.at(i)->setRunCondition(false);
    }
}

int main() {

    initGui();
    
    // 5 utencils on the left and right of each philosopher. Use them to acquire locks.
    vector< unique_ptr<Chopstick> > chopsticks(numPhilosophers);

    for (int i = 0; i < numPhilosophers; ++i)
    {
        auto c1 = unique_ptr<Chopstick>(new Chopstick());
        chopsticks[i] = move(c1);
        drawChopstick(i, -1, "free");
    }

    vector<thread> tasks(numPhilosophers);
    vector<Philosopher*> philosophers;

    for(int i = 0; i < numPhilosophers; i++){
        Philosopher *p = new Philosopher(i);
        philosophers.push_back(p);

        if(i == 0){
            tasks[i] = (thread(&Philosopher::routine,
                              p,
                              ref(chopsticks[numPhilosophers - 1].get()->m),
                              ref(chopsticks[i].get()->m),
                              ref(_muGUI),
                              0,
                              numPhilosophers - 1)
            );
        }else{
            tasks[i] = (thread(&Philosopher::routine,
                              p,
                              ref(chopsticks[i - 1].get()->m),
                              ref(chopsticks[i].get()->m),
                              ref(_muGUI),
                              i - 1,
                              i)
            );
        }

    }

    //for_each(tasks.begin(), tasks.end(), mem_fn(&thread::join));

    for(int i = 0; i < tasks.size(); i++){
        stopThreadsTimer(philosophers);
        tasks.at(i).join();
    }

    curs_set(FALSE);
    endwin();

    return 0;
}