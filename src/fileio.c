#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/parking.h"
#include "../include/fileio.h"

#define DATA_FILE "data/vehicles.dat"

void loadData(ParkingLot *p) {
    FILE *fp = fopen(DATA_FILE, "r");

    if (fp == NULL) {
        p->count = 0;
        printf("========================================\n");
        printf("  No saved data found in vehicles.dat. Starting fresh.\n");
        printf("========================================\n");
        return;
    }

    p->count = 0;

    while (p->count < MAX_VEHICLES) {
        int type, status;
        long entry, exit_t;
        double fee;
        char plate[20];

        int result = fscanf(fp, "%19[^|]|%d|%ld|%ld|%lf|%d\n",
                            plate, &type, &entry, &exit_t, &fee, &status);

        if (result == EOF || result != 6) {
            break;
        }

        strcpy(p->list[p->count].licensePlate, plate);
        p->list[p->count].type = (VehicleType)type;
        p->list[p->count].entryTime = (time_t)entry;
        p->list[p->count].exitTime = (time_t)exit_t;
        p->list[p->count].fee = fee;
        p->list[p->count].status = status;

        p->count++;
    }

    fclose(fp);

    printf("========================================\n");
    printf("  System Data loaded: %d vehicle(s) found.\n", p->count);
    printf("========================================\n");
}

void saveData(ParkingLot *p) {
    FILE *fp = fopen(DATA_FILE, "w");

    if (fp == NULL) {
        printf("ERROR: Cannot open %s for saving!\n", DATA_FILE);
        return;
    }

    int i;
    for (i = 0; i < p->count; i++) {
        fprintf(fp, "%s|%d|%ld|%ld|%.2f|%d\n",
                p->list[i].licensePlate,
                p->list[i].type,
                (long)p->list[i].entryTime,
                (long)p->list[i].exitTime,
                p->list[i].fee,
                p->list[i].status);
    }

    fclose(fp);
    printf("System data saved successfully to %s. (%d records)\n", DATA_FILE, p->count);
}
