#include <stdio.h>
#include <time.h>
#include <string.h>
#include "../include/types.h" 

extern Vehicle list[];
extern int total_vehicles;

void searchVehicle() {
    char key[15];
    int found = 0;
    printf("\n\033[1;4;36m======= SEARCHING VEHICLE =======\033[0m\n");
    printf("Enter search keyword >> ");
    scanf("%s", key);

    printf("\n\033[1;37m%-5s | %-15s | %-10s | %-25s | %-12s\033[0m\n", 
           "STT", "LICENSE PLATE", "VEHICLE TYPE", "ENTRY TIME", "STATUS");
    printf("------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < total_vehicles; i++) {
        if (strstr(list[i].licensePlate, key) != NULL) {
            found++;
            
            char *typeStr;
            if (list[i].type == MOTO) typeStr = "Motocycle";
            else if (list[i].type == CAR) typeStr = "Car";
            else if (list[i].type == BUS) typeStr = "Bus";
            else typeStr = "Other";

            char timeStr[26];
            char *rawTime = ctime(&list[i].entryTime);
            if (rawTime != NULL) {
                for(int j = 0; j < 24; j++) timeStr[j] = rawTime[j];
                timeStr[24] = '\0';
            } else {
                strcpy(timeStr, "N/A");
            }

            printf("%-5d | %-15s | %-10s | %-25s | %s\n", 
                   found, list[i].licensePlate, typeStr, timeStr,
                   (list[i].status == 0 ? "\033[1;32mCurrently parked\033[0m" : "\033[1;31mAlready exited\033[0m"));
        }
    }
    if (found == 0) printf("\033[1;31m[LOI] Vehicle not found!\033[0m\n");
}
