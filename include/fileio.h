#ifndef FILEIO_H
#define FILEIO_H

#include "parking.h"

void loadData(ParkingLot *p);
void saveData(ParkingLot *p);
void logDeletedVehicle(const Vehicle *v);

#endif
