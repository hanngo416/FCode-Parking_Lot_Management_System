#ifndef PARKING_H
#define PARKING_H

#include <time.h>
#include "types.h"

typedef struct {
    Vehicle list[MAX_VEHICLES];
    int count;
    PriceConfig prices[MAX_TYPES];
} ParkingLot;

int findVehicleIndex(ParkingLot *p, const char *plate);
void initParkingLot(ParkingLot *p);
void addVehicle(ParkingLot *p);
void showParkedVehicles(ParkingLot *p);
void removeVehicle(ParkingLot *p);
void deleteVehicle(ParkingLot *p); // Admin permanent delete
void listVehicles(ParkingLot *p);
void searchVehicle(ParkingLot *p);

#endif