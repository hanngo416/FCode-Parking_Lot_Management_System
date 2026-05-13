#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/types.h"
#include "../include/parking.h"
#include "../include/billing.h"
#include "../include/fileio.h"
#include "../include/utils.h"


void initParkingLot(ParkingLot *p) {
    p->count = 0;
    p->prices[0] = (PriceConfig){MOTO, "Motorbike", 2000, 2000};
    p->prices[1] = (PriceConfig){CAR,  "Car",       5000, 5000};
    p->prices[2] = (PriceConfig){BUS,  "Bus",      10000, 10000};
}

int findVehicleIndex(ParkingLot *p, const char *plate) {
    int i;
    for (i = 0; i < p->count; i++) {
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
        printf("Invalid license plate! Format: SSCC-SSSS (S: number, C: letter)\n");
        return;
    }

    if (isDuplicateLicensePlate(p->list, p->count, plate)) {
        printf("Vehicle already exists!\n");
        return;
    }

    time_t now = time(NULL);
    struct tm *localTime = localtime(&now);
    int hour = localTime->tm_hour;

    if (hour >= 23 || hour < 3) {
        printf("Parking lot is closed from 23:00 to 03:00!\n");
        return;
    }

    type = getInt(
        "Vehicle type (0: motorbike, 1: car, 2: bus): ",
        "Vehicle type must be 0, 1, or 2.",
        "Please enter a valid number.",
        0, 2
    );

    Vehicle *v = &p->list[p->count];
    strncpy(v->licensePlate, plate, sizeof(v->licensePlate) - 1);
    v->licensePlate[sizeof(v->licensePlate) - 1] = '\0';
    v->type      = type;
    v->entryTime = time(NULL);
    v->exitTime  = 0;
    v->fee       = 0;
    v->status    = PARKING;
    p->count++;

    printf("Vehicle added successfully!\n");
    printf("Entry time: %s", ctime(&v->entryTime));
}

void removeVehicle(ParkingLot *p) {
    char plate[20];

    printf("\n--- CHECK OUT VEHICLE ---\n");
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

    v->fee    = calculateFee(*v, p);
    v->status = EXITED;

    printBill(*v);
}

void listVehicles(ParkingLot *p) {
    int count_in_yard = 0;
<<<<<<< HEAD

    printf("\n\033[1;4;36m======= VEHICLE LIST =======\033[0m\n");
    printf("\033[1;37m%-5s | %-15s | %-12s | %-25s\033[0m\n",
           "STT", "LICENSE PLATE", "VEHICLE TYPE", "ENTRY TIME");
    printf("----------------------------------------------------------------------\n");
=======
    printf(LINE "===================== " TITLE "VEHICLE LIST" RESET LINE " =====================" RESET "\n");
    printf("\n\033[1;36m%-5s | %-15s | %-15s | %-25s\033[0m\n",  "STT", "LICENSE PLATE", "VEHICLE TYPE", "ENTRY TIME");    
    printf(LINE "----------------------------------------------------------------------\n");
>>>>>>> 329567bc8c4173603ca3011fd7cb60a8b506d56e

    int i;
    for (i = 0; i < p->count; i++) {
        if (p->list[i].status == PARKING) {
            count_in_yard++;

            const char *typeStr;
            if      (p->list[i].type == MOTO) typeStr = "Motorbike";
            else if (p->list[i].type == CAR)  typeStr = "Car";
            else if (p->list[i].type == BUS)  typeStr = "Bus";
            else                              typeStr = "Other";

            char timeStr[26];
            char *rawTime = ctime(&p->list[i].entryTime);
            if (rawTime != NULL) {
                strncpy(timeStr, rawTime, 24);
                timeStr[24] = '\0';
            } else {
                strcpy(timeStr, "N/A");
            }

<<<<<<< HEAD
            printf("%-5d | %-15s | %-12s | %-25s\n",
                   count_in_yard,
                   p->list[i].licensePlate,
                   typeStr,
                   timeStr);
        }
    }

    printf("----------------------------------------------------------------------\n");
=======
            printf(RESET "%-5d | %-15s | %-15s | %-25s\n", count_in_yard, p->list[i].licensePlate, typeStr, timeStr);
        }
    }

    printf(LINE "----------------------------------------------------------------------\n");
    
    float ratio = (float)(count_in_yard * 100) / MAX_VEHICLES;
    if (count_in_yard == 0) printf(RED "Empty!\n" RESET);
    else if (ratio < 80) 
    {
        printf(YELLOW "Total: %d/3636 " RESET "\n", count_in_yard);
        printf(GREEN "Status: %0.2f%% Available\n" RESET, ratio);
    }
    else if (ratio < 100) 
    {
        printf(YELLOW "Total: %d/3636 " RESET "\n", count_in_yard);
        printf(YELLOW "Status: %0.2f%% Nearly full\n" RESET, ratio);
    }
    else 
    {
        printf(YELLOW "Total: %d/3636 " RESET "\n", count_in_yard);
        printf(RED "Status: %0.2f%% Full\n" RESET, ratio);
    }
}
>>>>>>> 329567bc8c4173603ca3011fd7cb60a8b506d56e

    float ratio = (float)(count_in_yard * 100) / MAX_VEHICLES;
    if      (count_in_yard == 0) printf("\033[1;31mEmpty!\033[0m\n");
    else if (ratio < 80)         printf("\033[1;33mStatus: %.2f%% Normal\033[0m\n",      ratio);
    else if (ratio < 100)        printf("\033[1;31mStatus: %.2f%% Nearly full\033[0m\n", ratio);
    else                         printf("\033[1;31mStatus: %.2f%% Full\033[0m\n",        ratio);
}

void searchVehicle(ParkingLot *p) {
    char key[15];
    int found_count = 0;

    printf(LINE "===================== " TITLE "SEARCH VEHICLE" RESET LINE " =====================" RESET "\n");
    getString("Enter license plate keyword: ", key, sizeof(key));

<<<<<<< HEAD
    printf("\n%-5s | %-15s | %-12s | %-25s | %s\n",
           "STT", "LICENSE PLATE", "VEHICLE TYPE", "ENTRY TIME", "STATUS");
=======
    printf("\n\033[1;36m%-5s | %-15s | %-15s | %-25s\033[0m\n",  "STT", "LICENSE PLATE", "VEHICLE TYPE", "ENTRY TIME");    
>>>>>>> 329567bc8c4173603ca3011fd7cb60a8b506d56e
    printf("-------------------------------------------------------------------------------------\n");

    int i;
    for (i = 0; i < p->count; i++) {
        if (strstr(p->list[i].licensePlate, key) != NULL) {
            found_count++;

            const char *typeStr;
            if      (p->list[i].type == MOTO) typeStr = "Motorbike";
            else if (p->list[i].type == CAR)  typeStr = "Car";
            else if (p->list[i].type == BUS)  typeStr = "Bus";
            else                              typeStr = "Other";

            char timeStr[26];
            char *rawTime = ctime(&p->list[i].entryTime);
            if (rawTime != NULL) {
                strncpy(timeStr, rawTime, 24);
                timeStr[24] = '\0';
            } else {
                strcpy(timeStr, "N/A");
            }

<<<<<<< HEAD
            printf("%-5d | %-15s | %-12s | %-25s | %s\n",
=======
            printf(RESET "%-5d | %-15s | %-15s | %-25s | %s\n",
>>>>>>> 329567bc8c4173603ca3011fd7cb60a8b506d56e
                   found_count,
                   p->list[i].licensePlate,
                   typeStr,
                   timeStr,
                   (p->list[i].status == PARKING ? "Currently parked" : "Already exited"));
        }
    }

    if (found_count == 0)
        printf("No vehicle found matching '%s'.\n", key);
<<<<<<< HEAD
}
=======
    }
}

>>>>>>> 329567bc8c4173603ca3011fd7cb60a8b506d56e
