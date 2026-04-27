#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/fileio.h"


#define DATA_FILE "data/vehicles.dat"

extern Vehicle vehicles[MAX_VEHICLES];
extern int vehicleCount;

void loadData(void) {
    FILE *fp = fopen(DATA_FILE, "r");

    if (fp == NULL) {
        vehicleCount = 0;
        printf("========================================\n");
        printf("  No saved data found. Starting fresh.\n");
        printf("========================================\n");
        return;
    }

    vehicleCount = 0;

    while (vehicleCount < MAX_VEHICLES) {
        int type, status;
        long entry, exit_t;
        double fee;
        char plate[12];

        int result = fscanf(fp, "%11[^|]|%d|%ld|%ld|%lf|%d\n",
                            plate, &type, &entry, &exit_t, &fee, &status);

        if (result == EOF || result != 6) {
            break; 
        }

        strcpy(vehicles[vehicleCount].licensePlate, plate);
        vehicles[vehicleCount].type = (VehicleType)type;
        vehicles[vehicleCount].entryTime = (time_t)entry;
        vehicles[vehicleCount].exitTime = (time_t)exit_t;
        vehicles[vehicleCount].fee = fee;
        vehicles[vehicleCount].status = status;

        vehicleCount++;
    }

    fclose(fp);

    if (vehicleCount >= MAX_VEHICLES) {
        printf("WARNING: Maximum capacity (%d) reached during data load!\n", MAX_VEHICLES);
        printf("         Some records may have been skipped.\n");
    }

    printf("========================================\n");
    printf("  Data loaded: %d vehicle(s) found.\n", vehicleCount);
    printf("========================================\n");
}

void saveData(void) {
    FILE *fp = fopen(DATA_FILE, "w");

    if (fp == NULL) {
        printf("ERROR: Cannot open file for saving! Data may be lost.\n");
        printf("       Please check if 'data/' directory exists.\n");
        return;
    }

    int i;
    for (i = 0; i < vehicleCount; i++) {
        fprintf(fp, "%s|%d|%ld|%ld|%.2f|%d\n",
                vehicles[i].licensePlate,
                vehicles[i].type,
                (long)vehicles[i].entryTime,
                (long)vehicles[i].exitTime,
                vehicles[i].fee,
                vehicles[i].status);
    }

    fclose(fp);

    printf("Data saved successfully. (%d vehicle(s) written)\n", vehicleCount);
}
