//
// Created by jupiter on 14.03.18.
//

#ifndef FIVE_PHILOSOPHERS_DRAWGUI_H
#define FIVE_PHILOSOPHERS_DRAWGUI_H

using std::string;

void initGui();
void drawPhilosopher(int philosopherId, string philosopherStatus, float outOf, float doneThisMany);
void drawChopstick(int chopstickId, int philosopherId, string chopstickStatus);

#endif //FIVE_PHILOSOPHERS_DRAWGUI_H
