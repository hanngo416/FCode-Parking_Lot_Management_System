#ifndef PARKING_H
#define PARKING_H

#include <time.h>
#include "types.h"

typedef struct {
    Vehicle list[MAX_VEHICLES];
    int count;
} ParkingLot;

int findVehicleIndex(ParkingLot *p, const char *plate); //update
void initParkingLot(ParkingLot *p);
void addVehicle(ParkingLot *p);
void removeVehicle(ParkingLot *p);
void listVehicles(ParkingLot *p);
void searchVehicle(ParkingLot *p);

#endif