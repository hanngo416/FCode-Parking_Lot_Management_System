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
    int choice;
    printf(TITLE "\n------------------------------ DISPLAY OPTIONS ------------------------------\n" RESET);
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

    for (int i = 0; i < p->count; i++) {
        bool shouldPrint = false;
        if (choice == 1 && p->list[i].status == PARKING) shouldPrint = true;
        else if (choice == 2 && p->list[i].status == EXITED) shouldPrint = true;
        else if (choice == 3) shouldPrint = true;

        if (shouldPrint) { 
            count_in_yard++;

            bool isOver72h = false;
            if (p->list[i].status == PARKING) {
                double seconds = difftime(currentTime, p->list[i].entryTime);
                if (seconds >= 72 * 3600) isOver72h = true;
            }

            const char *typeStr;
            if (p->list[i].type == MOTO) typeStr = "Motorbike";
            else if (p->list[i].type == CAR) typeStr = "Car";
            else if (p->list[i].type == BUS) typeStr = "Bus";
            else typeStr = "Other";

            char timeStr[26];
            char *rawTime = ctime(&p->list[i].entryTime);
            if (rawTime != NULL) {
                strncpy(timeStr, rawTime, 24);
                timeStr[24] = '\0';
            } else { strcpy(timeStr, "N/A"); }

            const char *rowColor = isOver72h ? RED : RESET;

            if (choice == 3) {
                const char *statusStr = (p->list[i].status == PARKING) ? "In Yard" : "Exited";
                printf("%s%-5d | %-15s | %-15s | %-10s | %-25s%s\n", 
                       rowColor, count_in_yard, p->list[i].licensePlate, typeStr, statusStr, timeStr, (isOver72h ? " (Over 72h!)" : ""));
            } else {
                printf("%s%-5d | %-15s | %-15s | %-25s%s\n", 
                       rowColor, count_in_yard, p->list[i].licensePlate, typeStr, timeStr, (isOver72h ? " (Over 72h!)" : ""));
            }
            printf(RESET); 
        }
    }
}

void searchVehicle(ParkingLot *p) {
    char key[15];
    int found_count = 0;

    printf(LINE "===================== " TITLE "SEARCH VEHICLE" RESET LINE " =====================" RESET "\n\n");
    getString("Enter license plate keyword: ", key, sizeof(key));

    printf(LINE "===================== " TITLE "SUGGESTED RESULTS" RESET LINE " =====================" RESET "\n");
    printf("\n\033[1;36m%-5s | %-15s | %-15s | %-10s | %-25s\033[0m\n",  "STT", "LICENSE PLATE", "VEHICLE TYPE", "STATUS", "ENTRY TIME");    
    printf("-------------------------------------------------------------------------------------\n");

    for (int i = 0; i < p->count; i++) {
        if (strstr(p->list[i].licensePlate, key) != NULL) {
            found_count++;

            const char *typeStr;
            if (p->list[i].type == MOTO) typeStr = "Motorbike";
            else if (p->list[i].type == CAR) typeStr = "Car";
            else if (p->list[i].type == BUS) typeStr = "Bus";
            else typeStr = "Other";

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

    if (found_count == 0) {
        printf(RED "No vehicle found matching '%s'.\n" RESET, key);
    }
}

void deleteVehicle(ParkingLot *p) {
    char plate[20];
    printf("\n" LINE "================ " TITLE "DELETE VEHICLE (ADMIN ONLY)" RESET LINE " ================" RESET "\n");
    getString("Enter license plate to permanently delete: ", plate, sizeof(plate));

    int targetIdx = -1;
    for (int i = 0; i < p->count; i++) {
        if (strcmp(p->list[i].licensePlate, plate) == 0) {
            targetIdx = i;
            break;
        }
    }

    if (targetIdx == -1) {
        printf(RED "Error: Vehicle '%s' not found in the system!\n" RESET, plate);
        return;
    }

    logDeletedVehicle(&p->list[targetIdx]);

    for (int i = targetIdx; i < p->count - 1; i++) {
        p->list[i] = p->list[i + 1];
    }
    p->count--;

    printf(GREEN "Vehicle '%s' has been permanently deleted and logged.\n" RESET, plate);
    
    saveData(p, "Admin permanently deleted a vehicle");
}