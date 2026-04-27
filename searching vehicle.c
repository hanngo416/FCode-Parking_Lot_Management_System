#include <stdio.h>
#include <time.h>
#include <string.h>

typedef struct { char plate[15]; int type; time_t entry_time; int status; } Vehicle;
extern Vehicle list[];
extern int total_vehicles;

void searchVehicle() {
    char key[15];
    int found = 0;
    printf("\n\033[1;4;36m======= TIM KIEM BIEN SO =======\033[0m\n");
    printf("Nhap ky tu can tim >> ");
    scanf("%s", key);

    printf("\n\033[1;37m%-5s | %-15s | %-10s | %-25s | %-12s\033[0m\n", 
           "STT", "BIEN SO", "LOAI XE", "GIO VAO", "TRANG THAI");
    printf("------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < total_vehicles; i++) {
        if (strstr(list[i].plate, key) != NULL) {
            found++;
            char *typeStr = (list[i].type == 0) ? "Xe may" : (list[i].type == 1 ? "O to" : "Xe tai");
            
            char timeStr[26];
            char *rawTime = ctime(&list[i].entry_time);
            for(int j = 0; j < 24; j++) timeStr[j] = rawTime[j];
            timeStr[24] = '\0';

            printf("%-5d | %-15s | %-10s | %-25s | %s\n", 
                   found, list[i].plate, typeStr, timeStr,
                   (list[i].status == 0 ? "\033[1;32mTrong bai\033[0m" : "\033[1;31mDa ra\033[0m"));
        }
    }
    if (found == 0) printf("\033[1;31m[LOI] Khong tim thay xe!\033[0m\n");
}
