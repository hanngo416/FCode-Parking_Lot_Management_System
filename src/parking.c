#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/types.h"
#include "../include/parking.h"
#include "../include/billing.h"
#include "../include/utils.h"

void initParkingLot(ParkingLot *p) {
    p->count = 0;
}


int findVehicleIndex(ParkingLot *p, const char *plate) {
    for (int i = 0; i < p->count; i++) {
        if (p->list[i].status == PARKING &&
            strcmp(p->list[i].licensePlate, plate) == 0) {
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

    getString("Enter license plate: ", plate, sizeof(plate));
    if (!isValidLicensePlate(plate)) {
        printf("Invalid license plate! Format: SSCC-SSSS( S: number, C: letter)\n");
        return;
    }

    if (isDuplicateLicensePlate(p->list, p->count, plate)) {
        printf("Vehicle already exists!\n");
        return;
    }

    type = getInt("Vehicle type (0: motorbike, 1: car, 2: bus): ",
                  "Vehicle type must be 0, 1, or 2.",
                  "Please enter a valid number.",
                  0, 2);

    Vehicle *v = &p->list[p->count];

    strncpy(v->licensePlate, plate, sizeof(v->licensePlate) - 1);
    v->licensePlate[sizeof(v->licensePlate) - 1] = '\0';
    v->type = type;
    v->entryTime = time(NULL);
    v->exitTime = 0;
    v->fee = 0;
    v->status = PARKING;

    p->count++;

    printf("Vehicle added successfully!\n");
    printf("Entry time: %s", ctime(&v->entryTime));
}


void removeVehicle(ParkingLot *p) {
    char plate[20];

    printf("\n--- REMOVE VEHICLE ---\n");
    getString("Enter license plate: ", plate, sizeof(plate));

    if (!isValidLicensePlate(plate)) {
        printf("Invalid license plate!\n");
        return;
    }

    int idx = findVehicleIndex(p, plate);

    if (idx == -1) {
        printf("Vehicle not found or already exited!\n");
        return;
    }

    Vehicle *v = &p->list[idx];

    v->exitTime = time(NULL);

    if (v->exitTime <= v->entryTime) {
        printf("Time error!\n");
        return;
    }

    v->fee = calculateFee(*v);
    v->status = EXITED;

    printf("\n--- BILL ---\n");
    printf("Plate: %s\n", v->licensePlate);

    if (v->type == MOTO) printf("Type: Motorbike\n");
    else if (v->type == CAR) printf("Type: Car\n");
    else printf("Type: Bus\n");

    printf("Entry: %s", ctime(&v->entryTime));
    printf("Exit : %s", ctime(&v->exitTime));
    printf("Fee  : %.0f VND\n", v->fee);
}

void listVehicles(ParkingLot *p) {
    printf("\n--- VEHICLES IN PARKING ---\n");

    int found = 0;

    for (int i = 0; i < p->count; i++) {
        if (p->list[i].status == PARKING) {
            found = 1;

            printf("%d. %s | ", i + 1, p->list[i].licensePlate);

            if (p->list[i].type == MOTO) printf("Motorbike");
            else if (p->list[i].type == CAR) printf("Car");
            else printf("Bus");

            printf(" | Entry: %s", ctime(&p->list[i].entryTime));
        }
    }

    if (!found) {
        printf("No vehicles in parking.\n");
    }
}