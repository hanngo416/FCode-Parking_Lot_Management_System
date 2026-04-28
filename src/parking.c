#include <stdio.h>
#include <time.h>
#include "include/types.h"
#include "include/parking.h"
#include "include/billing.h"

int myStrCmp(char *a, char *b) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return (a[i] == '\0' && b[i] == '\0');
}

void myStrCpy(char *dest, char *src) {
    int i = 0;
    while (src[i]) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void initParkingLot(ParkingLot *p) {
    p->count = 0;
}

int findVehicleIndex(ParkingLot *p, char plate[]) {
    for (int i = 0; i < p->count; i++) {
        if (p->list[i].status == PARKING &&
            myStrCmp(p->list[i].licensePlate, plate)) {
            return i;
        }
    }
    return -1;
}

void addVehicle(ParkingLot *p) {
    if (p->count >= MAX_VEHICLES) {
        printf("Parking lot is full!\n");
        return;
    }

    char plate[20];
    int type;

    printf("\n--- ADD VEHICLE ---\n");
    printf("Enter license plate: ");
    scanf("%19s", plate);

    if (findVehicleIndex(p, plate) != -1) {
        printf("Vehicle already exists!\n");
        return;
    }

    printf("Vehicle type (0: motorbike, 1: car, 2: truck): ");
    if (scanf("%d", &type) != 1 || type < 0 || type > 2) {
        printf("Invalid vehicle type!\n");
        return;
    }

    Vehicle *v = &p->list[p->count];

    myStrCpy(v->licensePlate, plate);
    v->type = type;
    v->entryTime = time(NULL);
    v->exitTime = 0;
    v->fee = 0;
    v->status = PARKING;

    p->count++;

    printf("Vehicle added successfully!\n");
}
void removeVehicle(ParkingLot *p) {
    char plate[20];

    printf("\n--- REMOVE VEHICLE ---\n");
    printf("Enter license plate: ");
    scanf("%19s", plate);

    int idx = findVehicleIndex(p, plate);

    if (idx == -1) {
        printf("Vehicle not found!\n");
        return;
    }

    Vehicle *v = &p->list[idx];

    v->exitTime = time(NULL);
    v->fee = calculateFee(*v);
    v->status = EXITED;

    printBill(*v);
}