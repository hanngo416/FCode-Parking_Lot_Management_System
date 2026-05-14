#ifndef TYPES_H
#define TYPES_H

#include <time.h>

#define MAX_VEHICLES 3636
#define PARKING 0
#define EXITED 1

typedef enum {
    MOTO = 0,
    CAR = 1,
    BUS = 2
} VehicleType;
typedef struct {
    char licensePlate[12];
    VehicleType type;
    time_t entryTime;
    time_t exitTime;
    double fee;
    int status;
} Vehicle;
typedef struct {
    VehicleType type;
    char typeName[20];
    double price;
} PriceConfig;

#endif
