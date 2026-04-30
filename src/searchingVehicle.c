#include <stdio.h>
#include <time.h>
#include <string.h>
#include "../include/parking.h"
#include "../include/utils.h"

void searchVehicle(ParkingLot *p) {
    char key[15];
    int found = 0;

    printf("\n======= SEARCH VEHICLE =======\n");
    getString("Enter license plate keyword: ", key, sizeof(key));

    printf("\n%-5s | %-15s | %-10s | %-25s | %s\n",
           "STT", "LICENSE PLATE", "VEHICLE TYPE", "ENTRY TIME", "STATUS");
    printf("------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < p->count; i++) {
        if (strstr(p->list[i].licensePlate, key) != NULL) {
            found++;
            
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

            printf("%-5d | %-15s | %-10s | %-25s | %s\n",
                   found,
                   p->list[i].licensePlate,
                   typeStr,
                   timeStr,
                   (p->list[i].status == PARKING ? "Currently parked" : "Already exited"));
        }
    }

    if (found == 0) {
        printf("No vehicle found matching '%s'.\n", key);
    }
}
