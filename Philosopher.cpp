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

int milliseconds = 200;
std::chrono::milliseconds refreshRate(milliseconds);
int refreshesInSecond = 1000/milliseconds;


Philosopher::Philosopher(int id){

    this->id = id;
    this->isRunning = true;
    this->status = "purgatory";
}

Philosopher::~Philosopher(){
    try{
        std::terminate();
    }catch(exception& e){
        throw e;
    }
}

void Philosopher::eat(mutex & _leftChopstick, mutex & _rightChopstick, mutex & _muGui, int leftChopstickNumber, int rightChopstickNumber){

    //Picking up chopsticks, Changing status of philosopher, drawing philosopher
    status = "Picking chopsticks";
    _muGui.lock();
    drawPhilosopher(id, status, 0, 1);
    _muGui.unlock();

    //Time of picking up chopstick is randomised
    //output = min + (rand() % static_cast<int>(max - min + 1))
    int chopstickPickupTime = 1 + (rand() % /*static_cast<int>*/(2 - 1 + 1));

    //Ensures there are no deadlocks
    lock(_leftChopstick, _rightChopstick);

    //Picking up chopstick takes time
    for(int i = 0; i < chopstickPickupTime * refreshesInSecond; i++){
        std::this_thread::sleep_for(refreshRate);
    }

    //http://en.cppreference.com/w/cpp/thread/lock_guard
    lock_guard<mutex> a(_leftChopstick, adopt_lock);


    //Philosopher change in gui
    _muGui.lock();
    drawChopstick(leftChopstickNumber, id, "taken");
    _muGui.unlock();

    //same as above
    for(int i = 0; i < chopstickPickupTime * refreshesInSecond; i++){
        std::this_thread::sleep_for(refreshRate);
    }

    //same as above
    lock_guard<mutex> b(_rightChopstick, adopt_lock);

    //same as above
    _muGui.lock();
    drawChopstick(rightChopstickNumber, id, "taken");
    _muGui.unlock();

    //eating takes time, both mutexes get freed after this bit of code executes ( when thread exits scope)
    status = "eating";
    int eatingTime = 3 + (rand() % /*static_cast<int>*/(6 - 3 + 1));
    for(int i = 0; i < eatingTime * refreshesInSecond; i++){
        std::this_thread::sleep_for(refreshRate);
        _muGui.lock();
        drawPhilosopher(id, status, eatingTime * refreshesInSecond, i);
        _muGui.unlock();
    }

    _muGui.lock();
    drawChopstick(leftChopstickNumber, -1, "free");
    drawChopstick(rightChopstickNumber, -1, "free");
    _muGui.unlock();
}


void Philosopher::meditate(mutex & _muGui){
    status = "meditating";

    _muGui.lock();
    drawPhilosopher(id, status, 0, 1);
    _muGui.unlock();

    int meditateTime = 3 + (rand() % /*static_cast<int>*/(5 - 3 + 1));

    for(int i = 0; i < meditateTime * refreshesInSecond; i++){
        std::this_thread::sleep_for(refreshRate);

        _muGui.lock();
        drawPhilosopher(id, status, meditateTime * refreshesInSecond,  i);
        _muGui.unlock();
    }

    _muGui.lock();
    drawPhilosopher(id, "Done", 0, 0);
    _muGui.unlock();
}

void Philosopher::routine(mutex & _leftChopstick, mutex & _rightChopstick, mutex & _muGui, int leftChopstickNumber, int rightChopstickNumber) {

    _muGui.lock();
    drawPhilosopher(id, status, 0, 0);
    _muGui.unlock();

    while(isRunning){
        meditate(_muGui);
        eat(_leftChopstick, _rightChopstick,_muGui, leftChopstickNumber, rightChopstickNumber);
    }

    _muGui.lock();
    drawPhilosopher(id,"retired", 0, 1);
    _muGui.unlock();
}

void Philosopher::setRunCondition(bool runCondition){
    this->isRunning = runCondition;
}


