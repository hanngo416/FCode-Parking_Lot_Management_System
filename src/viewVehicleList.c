#include <stdio.h>
#include <time.h>
#include <string.h>
#include "../include/types.h" 

extern Vehicle list[];       // Dùng extern để máy hiểu biến nằm ở main.c
extern int total_vehicles;

#define MAX_SLOTS 3636

void loadData() {
    FILE *f = fopen("data/input.txt", "r"); 
    if (f == NULL) {
        printf("Khong tim thay file du lieu!\n");
        return;
    }
    total_vehicles = 0; 
    while (fscanf(f, "%s %d %ld %d", 
                  list[total_vehicles].licensePlate, 
                  &list[total_vehicles].type, 
                  (long *)&list[total_vehicles].entryTime, 
                  &list[total_vehicles].status) != EOF) {
        total_vehicles++;
    }
    fclose(f);
}



extern Vehicle list[];
extern int total_vehicles;

void viewList() {
    int count_in_yard = 0;
    printf("\n\033[1;4;36m======= DANH SACH XE DANG TRONG BAI =======\033[0m\n");
    printf("\033[1;37m%-5s | %-15s | %-10s | %-25s\033[0m\n", "STT", "BIEN SO", "LOAI XE", "GIO VAO");
    printf("------------------------------------------------------------------\n");

    for (int i = 0; i < total_vehicles; i++) {
        if (list[i].status == 0) { 
            count_in_yard++;
            
            char *typeStr;
            if (list[i].type == MOTO) typeStr = "Xe may";
            else if (list[i].type == CAR) typeStr = "O to";
            else if (list[i].type == BUS) typeStr = "Xe buyt";
            else typeStr = "Khac";

            char timeStr[26];
            char *rawTime = ctime((const time_t *)&list[i].entryTime);
            if (rawTime != NULL) {
                for(int j = 0; j < 24; j++) timeStr[j] = rawTime[j];
                timeStr[24] = '\0';
            } else {
                strcpy(timeStr, "N/A");
            }

            printf("%-5d | %-15s | %-10s | %-25s\n", count_in_yard, list[i].licensePlate, typeStr, timeStr);
        }
    }

    printf("------------------------------------------------------------------\n");
    float ratio = (float)(count_in_yard * 100) / MAX_SLOTS;
    if (count_in_yard == 0) printf("\033[1;31mBai xe hien dang trong!\033[0m\n");
    else if (ratio < 50) printf("\033[1;32mTrang thai: %.2f%% Con trong \033[0m\n", ratio);
    else if (ratio < 80) printf("\033[1;33mTrang thai: %.2f%% Co the chua \033[0m\n", ratio);
    else if (ratio < 100) printf("\033[1;31mTrang thai: %.2f%% Sap day \033[0m\n", ratio);
    else printf("\033[1;31mTrang thai: %.2f%% Lap day \033[0m\n", ratio);
}
