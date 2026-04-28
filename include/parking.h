#ifndef PARKING_H
#define PARKING_H

#include <time.h> 
#include "include/types.h"

typedef struct {
    Vehicle list[MAX_VEHICLES];
    int count;
} ParkingLot;

void initParkingLot(ParkingLot *p);
void addVehicle(ParkingLot *p);
void removeVehicle(ParkingLot *p);

#endif