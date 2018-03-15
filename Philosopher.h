//
// Created by jupiter on 14.03.18.
//

#ifndef FIVE_PHILOSOPHERS_PHILISOPHER_H
#define FIVE_PHILOSOPHERS_PHILISOPHER_H

#include <mutex>
using std::string;
using std::mutex;


class Philosopher
{
private:
    int id;
    bool isRunning;
    string status;
    void eat(mutex & _muLeftChopstick, mutex & _muRightChopstick, mutex & _muGui, int leftChopstickNumber, int rightChopstickNumber);
    void meditate(mutex & _muGui);

public:
    explicit Philosopher(int id);
    ~Philosopher();
    void routine(mutex & _muLeftChopstick, mutex & _muRightChopstick, mutex & _muGui, int leftChopstickNumber, int rightChopstickNumber);
    void setRunCondition(bool runCondition);
};

#endif //FIVE_PHILOSOPHERS_PHILISOPHER_H
