//
// Created by jupiter on 14.03.18.
//

#include <ncurses.h>
#include <mutex>
#include "DrawGui.h"

using std::mutex;
using std::to_string;

void initGui(){

    srand(time(NULL));
    initscr();
    start_color();

    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);

    attron(A_BOLD);
    attron(COLOR_PAIR(1));

    int rows = 0, collumns = 0;
    char projectTitle[] = "Five(or more) philosophers";

    getmaxyx( stdscr, rows, collumns);
    mvprintw(0,(collumns / 2) - sizeof(projectTitle) / 2, projectTitle);
    refresh();
}

void drawPhilosopher(int philosopherId, string philosopherStatus, float outOf, float doneThisMany){

    string clean = "                              ";

    attron(COLOR_PAIR(2));
    mvprintw(1 + philosopherId * 4, 0, ("Philosopher no. " + to_string(philosopherId)).c_str());
    mvprintw(2 + philosopherId * 4, 0, clean.c_str());
    mvprintw(2 + philosopherId * 4, 0, ("Status: " + philosopherStatus).c_str());
    mvprintw(3 + philosopherId * 4, 0, clean.c_str());
    mvprintw(3 + philosopherId * 4, 0, ("Progress: " + to_string(doneThisMany/outOf) + " percent").c_str());

    refresh();
}

void drawChopstick(int chopstickId, int philosopherId, string chopstickStatus){

    attron(COLOR_PAIR(2));
    string clean = "                              ";
    int rows = 0, collumns = 0;
    getmaxyx( stdscr, rows, collumns);

    string cn = "Chopstick no. " + to_string(chopstickId);

    mvprintw(1 + chopstickId * 4, rows, ("Chopstick no. " + to_string(chopstickId)).c_str());
    mvprintw(2 + chopstickId * 4, rows, clean.c_str());
    if(philosopherId == -1){
        mvprintw(2 + chopstickId * 4, rows, ("Status: " + chopstickStatus + "").c_str());
    }else{
        mvprintw(2 + chopstickId * 4, rows, ("Status: " + chopstickStatus + " by philosopher" +  to_string(philosopherId)).c_str());
    }



    refresh();
}


