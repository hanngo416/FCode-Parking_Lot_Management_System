#include <stdio.h>
#include <time.h>

#define MAX_SLOTS 3636

typedef struct {
    char plate[15];
    int type;
    time_t entry_time;
    int status;
} Vehicle;

Vehicle list[MAX_SLOTS]; 
int total_vehicles = 0; 

void loadData() {
    FILE *f = fopen("parking_data.txt", "r");
    if (f == NULL) {
        printf("\033[1;31m[LOI] Khong tim thay file parking_data.txt!\033[0m\n");
        return;
    }
    total_vehicles = 0; 
    while (fscanf(f, "%s %d %ld %d", 
                  list[total_vehicles].plate, 
                  &list[total_vehicles].type, 
                  (long *)&list[total_vehicles].entry_time, 
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
            char *typeStr = (list[i].type == 0) ? "Xe may" : (list[i].type == 1 ? "O to" : "Xe tai");

            char timeStr[26];
            char *rawTime = ctime((const time_t *)&list[i].entry_time);
            for(int j = 0; j < 24; j++) timeStr[j] = rawTime[j];
            timeStr[24] = '\0';

            printf("%-5d | %-15s | %-10s | %-25s\n", count_in_yard, list[i].plate, typeStr, timeStr);
        }
    }

    printf("------------------------------------------------------------------\n");
    float ratio = (float)(count_in_yard * 100) / MAX_SLOTS;
    if (count_in_yard == 0) printf("\033[1;31mBai xe hien dang trong!\033[0m\n");
    else if (ratio < 50) printf("\033[1;32mTrang thai: %.2f%% Con trong \033[0m\n", ratio);
    else if (ratio < 80) printf("\033[1;33mTrang thai: %.2f%% Co the chua \033[0m\n", ratio);
    else if (ratio < 100) printf("\033[1;31mTrang thai: %.2f%% Sap day \033[0m\n", ratio);
    else printf("\033[1;32mTrang thai: %.2f%% Lap day \033[0m\n", ratio);
}
