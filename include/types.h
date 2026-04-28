#ifndef TYPES_H
#define TYPES_H

#include <time.h>

#define MAX_VEHICLES 3636
#define PARKING 0
#define EXITED 1

// Enum cho loại xe
typedef enum {
    MOTO = 0,
    CAR = 1,
    BUS = 2
} VehicleType;

// Struct thông tin phương tiện
typedef struct {
    char licensePlate[12];
    VehicleType type;
    time_t entryTime;
    time_t exitTime;
    double fee;
    int status; // 0: Đang đỗ, 1: Đã ra
} Vehicle;

// Struct bảng giá đỗ xe
typedef struct {
    VehicleType type;
    char typeName[20];
    double price;
} PriceConfig;

#endif
