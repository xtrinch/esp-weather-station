#ifndef __PING_H
#define __PING_H

#define PING_INTERVAL 60 // seconds

#include "common.h"
#include "station.h"

bool checkIfShouldPing();
bool sendPing();

#endif