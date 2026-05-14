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
<<<<<<< HEAD
        long entry, exit_t;
        double fee;
        char plate[20];

        int result = fscanf(fp, "%19[^|]|%d|%ld|%ld|%lf|%d\n",
=======
        long long entry, exit_t;
        double fee;
        char plate[20];

        int result = fscanf(fp, "%11[^|]|%d|%lld|%lld|%lf|%d\n",
>>>>>>> 329567bc8c4173603ca3011fd7cb60a8b506d56e
                            plate, &type, &entry, &exit_t, &fee, &status);

        if (result == EOF || result != 6) {
            break;
        }

<<<<<<< HEAD
        strcpy(p->list[p->count].licensePlate, plate);
=======
        strncpy(p->list[p->count].licensePlate, plate, 11);
        p->list[p->count].licensePlate[11] = '\0';
>>>>>>> 329567bc8c4173603ca3011fd7cb60a8b506d56e
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

void saveData(ParkingLot *p, const char *actionMsg) {
    FILE *fp = fopen(DATA_FILE, "w");

    if (fp == NULL) {
        printf("ERROR: Cannot open %s for saving!\n", DATA_FILE);
        return;
    }

    int i;
    for (i = 0; i < p->count; i++) {
<<<<<<< HEAD
        fprintf(fp, "%s|%d|%ld|%ld|%.2f|%d\n",
                p->list[i].licensePlate,
                p->list[i].type,
                (long)p->list[i].entryTime,
                (long)p->list[i].exitTime,
=======
        fprintf(fp, "%s|%d|%lld|%lld|%.2f|%d\n",
                p->list[i].licensePlate,
                p->list[i].type,
                (long long)p->list[i].entryTime,
                (long long)p->list[i].exitTime,
>>>>>>> 329567bc8c4173603ca3011fd7cb60a8b506d56e
                p->list[i].fee,
                p->list[i].status);
    }

    fclose(fp);

    printf("[System Sync] %s - Total: %d records saved to %s\n", actionMsg, p->count, DATA_FILE);
}
<<<<<<< HEAD
=======

void logDeletedVehicle(const Vehicle *v) {
    FILE *fp = fopen("data/deleted_vehicles.dat", "a");
    if (fp == NULL) {
        printf("ERROR: Cannot open deleted_vehicles.dat for logging!\n");
        return;
    }

    fprintf(fp, "%s|%d|%lld|%lld|%.2f|%d\n",
            v->licensePlate,
            v->type,
            (long long)v->entryTime,
            (long long)v->exitTime,
            v->fee,
            v->status);

    fclose(fp);
}
>>>>>>> 329567bc8c4173603ca3011fd7cb60a8b506d56e
