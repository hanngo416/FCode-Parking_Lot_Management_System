#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
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
        printf(RED "Parking lot is full!\n" RESET);
        return;
    }

    char plate[20];
    int type;

    printf(LINE "\n===================== " TITLE "ADD VEHICLE" RESET LINE " =====================" RESET "\n");

    getString("Enter license plate: ", plate, sizeof(plate));
    toUpperCase(plate);
    printf("License plate: %s\n", plate);

    if (!isValidLicensePlate(plate)) {
        printf("Invalid license plate! Format: SSCC-SSSSS (S: number, C: letter)\n");
        return;
    }

    if (isDuplicateLicensePlate(p->list, p->count, plate)) {
        printf(RED "Vehicle already exists!\n" RESET);
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

    printf(GREEN "Vehicle added successfully!\n" RESET);
    printf("Entry time: %s", ctime(&v->entryTime));
}

void removeVehicle(ParkingLot *p) {
    char plate[20];

    printf("\n--- CHECK OUT VEHICLE ---\n");
    listVehicles(p);
    if (p->count == 0) {
         printf("No vehicles in parking lot.\n");
         return;
    }
    getString("Enter license plate: ", plate, sizeof(plate));
    toUpperCase(plate);

    if (!isValidLicensePlate(plate)) {
        printf(RED "Invalid license plate!\n" RESET);
        return;
    }

    int idx = findVehicleIndex(p, plate);
    if (idx == -1) {
        printf(RED "Vehicle not found or already exited!\n" RESET);
        return;
    }

    Vehicle *v = &p->list[idx];
    v->exitTime = time(NULL);

    if (v->exitTime <= v->entryTime) {
        printf(RED "Time error!\n" RESET);
        return;
    }

    v->fee    = calculateFee(*v, p);
    v->status = EXITED;

    printBill(*v);
}

void listVehicles(ParkingLot *p) {
    int choice;
    printf(LINE "\n===================== " TITLE "DISPLAY OPTIONS" RESET LINE " =====================" RESET "\n");
    printf("1. Vehicles currently in yard\n");
    printf("2. Vehicles already left\n");
    printf("3. All transaction history\n");
    printf(LINE "------------------------------------------------------------------------------\n");
    printf(YELLOW "Enter your choice: " RESET);
    scanf("%d", &choice);
    while(getchar() != '\n'); 

    int count_in_yard = 0;
    time_t currentTime = time(NULL); 

    printf(LINE "\n===================== " TITLE "VEHICLE LIST" RESET LINE " =====================" RESET "\n");

    if (choice == 3) {
        printf("\033[1;36m%-5s | %-15s | %-15s | %-10s | %-25s\033[0m\n", 
               "STT", "LICENSE PLATE", "VEHICLE TYPE", "STATUS", "ENTRY TIME");
    } else {
        printf("\033[1;36m%-5s | %-15s | %-15s | %-25s\033[0m\n", 
               "STT", "LICENSE PLATE", "VEHICLE TYPE", "ENTRY TIME");
    }
    printf(LINE "------------------------------------------------------------------------------\n");

    int i;
    for (i = 0; i < p->count; i++) {
        if (p->list[i].status == PARKING) {
            count_in_yard++;

            bool isOver72h = false;
            if (p->list[i].status == PARKING) {
                double seconds = difftime(currentTime, p->list[i].entryTime);
                if (seconds >= 72 * 3600) isOver72h = true;
            }

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
            } else { strcpy(timeStr, "N/A"); }


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


    
    if      (count_in_yard == 0) printf("\033[1;31mEmpty!\033[0m\n");
    else if (ratio < 80)         printf("\033[1;33mStatus: %.2f%% Normal\033[0m\n",      ratio);
    else if (ratio < 100)        printf("\033[1;31mStatus: %.2f%% Nearly full\033[0m\n", ratio);
    else                         printf("\033[1;31mStatus: %.2f%% Full\033[0m\n",        ratio);

}
void searchVehicle(ParkingLot *p) {
    char key[15];
    int found_count = 0;

    printf(LINE "===================== " TITLE "SEARCH VEHICLE" RESET LINE " =====================" RESET "\n\n");
    getString("Enter license plate keyword: ", key, sizeof(key));

    printf(LINE "===================== " TITLE "SUGGESTED RESULTS" RESET LINE " =====================" RESET "\n");
    printf("\n\033[1;36m%-5s | %-15s | %-15s | %-10s | %-25s\033[0m\n",  "STT", "LICENSE PLATE", "VEHICLE TYPE", "STATUS", "ENTRY TIME");    
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

            printf(RESET "%-5d | %-15s | %-15s | %-10s | %-25s\n",
                   found_count,
                   p->list[i].licensePlate,
                   typeStr,
                   (p->list[i].status == PARKING ? "In Yard" : "Exited"),
                   timeStr);
        }
    }

    if (found_count == 0)
        printf("No vehicle found matching '%s'.\n", key);
    }

