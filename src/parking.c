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

    saveData(p, "Auto-save after Check-in");
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

    saveData(p, "Auto-save after Check-out");
}

void listVehicles(ParkingLot *p) {
    int count_in_yard = 0;
    printf(LINE "===================== " TITLE "VEHICLE LIST" RESET LINE " =====================" RESET "\n");
    printf("\n\033[1;36m%-5s | %-15s | %-15s | %-25s\033[0m\n",  "STT", "LICENSE PLATE", "VEHICLE TYPE", "ENTRY TIME");    
    printf(LINE "----------------------------------------------------------------------\n");

    for (int i = 0; i < p->count; i++) {
        if (p->list[i].status == PARKING) { 
            count_in_yard++;

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


void searchVehicle(ParkingLot *p) {
    char key[15];
    int found_count = 0;

    printf(LINE "===================== " TITLE "SEARCH VEHICLE" RESET LINE " =====================" RESET "\n");
    getString("Enter license plate keyword: ", key, sizeof(key));

    printf("\n\033[1;36m%-5s | %-15s | %-15s | %-25s\033[0m\n",  "STT", "LICENSE PLATE", "VEHICLE TYPE", "ENTRY TIME");    
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

            printf(RESET "%-5d | %-15s | %-15s | %-25s | %s\n",
                   found_count,
                   p->list[i].licensePlate,
                   typeStr,
                   timeStr,
                   (p->list[i].status == PARKING ? "Currently parked" : "Already exited"));
        }
    }

    if (found_count == 0) {
        printf("No vehicle found matching '%s'.\n", key);
    }
}

void deleteVehicle(ParkingLot *p) {
    printf("\n" LINE "=================== " TITLE "DELETE VEHICLE (ADMIN ONLY)" RESET LINE " ===================" RESET "\n");

    if (p->count == 0) {
        printf(YELLOW "  System is currently empty. No vehicles to delete.\n" RESET);
        return;
    }

    printf("\n" TITLE "--- AVAILABLE VEHICLES IN SYSTEM ---" RESET "\n");
    printf("\033[1;36m %-5s | %-15s | %-15s | %-18s\033[0m\n", "STT", "LICENSE PLATE", "VEHICLE TYPE", "STATUS");
    printf(LINE "-------------------------------------------------------------------\n" RESET);

    for (int i = 0; i < p->count; i++) {
        const char *typeStr;
        if (p->list[i].type == MOTO) typeStr = "Motorbike";
        else if (p->list[i].type == CAR) typeStr = "Car";
        else if (p->list[i].type == BUS) typeStr = "Bus";
        else typeStr = "Other";

        printf(RESET " %-5d | %-15s | %-15s | %-18s\n",
               i + 1,
               p->list[i].licensePlate,
               typeStr,
               p->list[i].status == PARKING ? "Currently parked" : "Already exited");
    }
    printf(LINE "-------------------------------------------------------------------\n" RESET);

    char plate[20];
    int targetIdx = -1;

    while (1) {
        getString("\nEnter license plate to permanently delete (or '0' to cancel): ", plate, sizeof(plate));
        
        if (strcmp(plate, "0") == 0) {
            printf(YELLOW "Deletion cancelled. Returning to menu...\n" RESET);
            return;
        }

        targetIdx = -1;
        for (int i = 0; i < p->count; i++) {
            if (strcmp(p->list[i].licensePlate, plate) == 0) {
                targetIdx = i;
                break;
            }
        }

        if (targetIdx != -1) {
            break; 
        }
        
        printf(RED "Error: Vehicle '%s' not found! Please check the list and try again.\n" RESET, plate);
    }

    logDeletedVehicle(&p->list[targetIdx]);

    for (int i = targetIdx; i < p->count - 1; i++) {
        p->list[i] = p->list[i + 1];
    }
    p->count--;

    printf(GREEN "Vehicle '%s' has been permanently deleted and logged.\n" RESET, plate);
    saveData(p, "Admin permanently deleted a vehicle");
}