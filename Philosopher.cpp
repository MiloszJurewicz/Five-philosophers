//
// Created by jupiter on 14.03.18.
//

#include <iostream>
#include <thread>
#include "Philosopher.h"
#include "DrawGui.h"

using std::lock_guard;
using std::cout;
using std::to_string;
using std::adopt_lock;
using std::endl;
using std::exception;
using std::thread;

std::chrono::milliseconds TIMEOUT(200);

Philosopher::Philosopher(int id){

    this->id = id;
}

Philosopher::~Philosopher(){
    try{
        std::terminate();
    }catch(exception& e){
        throw e;
    }
}

void Philosopher::eat(mutex & _leftChopstick, mutex & _rightChopstick, mutex & _muGui, int leftChopstickNumber, int rightChopstickNumber){

    //Picking up chopsticks, Changing status of philosopher
    status = "Picking chopsticks";
    _muGui.lock();
    drawPhilosopher(id, status, 0, 0);
    _muGui.unlock();

    //Time of picking up, chopstick is randomised to somewhat avoid starvation
    //output = min + (rand() % static_cast<int>(max - min + 1))
    int chopstickPickupTime = 1 + (rand() % static_cast<int>(2 - 1 + 1));

    //Ensures there are no deadlocks
    lock(_leftChopstick, _rightChopstick);

    //Picking up chopstick takes time
    for(int i = 0; i < chopstickPickupTime * 5; i++){
        std::this_thread::sleep_for(TIMEOUT);
    }

    //http://en.cppreference.com/w/cpp/thread/lock_guard
    lock_guard<mutex> a(_leftChopstick, adopt_lock);

    _muGui.lock();
    drawChopstick(leftChopstickNumber, id, "taken");
    _muGui.unlock();


    /*string sl = "   Philosopher " + to_string(id) + " picked " + to_string(leftChopstickNumber) + " chopstick.\n";
    cout << sl.c_str();*/

    //same as above
    for(int i = 0; i < chopstickPickupTime * 5; i++){
        std::this_thread::sleep_for(TIMEOUT);
    }

    //same as above
    lock_guard<mutex> b(_rightChopstick, adopt_lock);

    _muGui.lock();
    drawChopstick(rightChopstickNumber, id, "taken");
    _muGui.unlock();

    /*string sr = "   Philosopher " + to_string(id) + " picked " + to_string(rightChopstickNumber) + " chopstick.\n";
    cout << sr.c_str();*/

    //eating takes time, both mutexes get freed after this bit of code executes ( when thread exits scope)
    status = "eating";
    int eatingTime = 4 + (rand() % static_cast<int>(10 - 4 + 1));
    for(int i = 0; i < eatingTime * 5; i++){
        std::this_thread::sleep_for(TIMEOUT);
        _muGui.lock();
        drawPhilosopher(id, status, eatingTime * 5, i);
        _muGui.unlock();
    }

    _muGui.lock();
    drawChopstick(leftChopstickNumber, -1, "free");
    drawChopstick(rightChopstickNumber, -1, "free");
    _muGui.unlock();

    /*string pe = "Philosopher: " + to_string(id) + ", eats.\n";
    cout << pe;*/
}


void Philosopher::meditate(mutex & _muGui){
    status = "meditating";

    _muGui.lock();
    drawPhilosopher(id, status, 0, 0);
    _muGui.unlock();

    int meditateTime = 5 + (rand() % /*static_cast<int>*/(8 - 5 + 1));
    //cout << "Philosopher: " << id << " starts meditation for: " << meditateTime <<" s" << endl;

    for(int i = 0; i < meditateTime * 5; i++){
        std::this_thread::sleep_for(TIMEOUT);

        _muGui.lock();
        drawPhilosopher(id, status, meditateTime * 5,  i);
        _muGui.unlock();
    }

    _muGui.lock();
    drawPhilosopher(id, "Done", 0, 0);
    _muGui.unlock();
}

void Philosopher::routine(mutex & _leftChopstick, mutex & _rightChopstick, mutex & _muGui, int leftChopstickNumber, int rightChopstickNumber) {
    //cout << "thread: " << id << ", is using chopsticks number: "<< leftChopstickNumber << ", and: " << rightChopstickNumber << endl;

    _muGui.lock();
    drawPhilosopher(id, status, 0, 0);
    _muGui.unlock();

    for(int z = 0; z < 3; z++){
        meditate(_muGui);
        eat(_leftChopstick, _rightChopstick,_muGui, leftChopstickNumber, rightChopstickNumber);
    }

}


