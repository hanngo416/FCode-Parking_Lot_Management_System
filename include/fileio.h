#ifndef FILEIO_H
#define FILEIO_H

#include "parking.h"

void loadData(ParkingLot *p);
void saveData(ParkingLot *p, const char *actionMsg);
void logDeletedVehicle(const Vehicle *v);

#endif
