#include <stdio.h>
#include <stdlib.h>
#include "include/types.h"
#include "include/fileio.h"
#include "include/report.h"

Vehicle vehicles[MAX_VEHICLES];
int vehicleCount = 0;

int main() {
    int choice;

    loadData();

    do {
        printf("\n========================================\n");
        printf("       PARKING LOT MANAGEMENT       \n");
        printf("========================================\n");
        printf("1. Add a vehicle\n");
        printf("2. Remove a vehicle\n");
        printf("3. View parked vehicles\n");
        printf("4. Search vehicle by license plate\n");
        printf("5. View daily revenue report\n");
        printf("6. Save data\n");
        printf("7. Export revenue report (.txt)\n");
        printf("0. Exit\n");
        printf("========================================\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            printf("Invalid choice. Please enter a number!\n");
            continue;
        }

        switch (choice) {
            case 1:
                printf("1. Add a vehicle\n");
                break;
            case 2:
                printf("2. Remove a vehicle\n");
                break;
            case 3:
                printf("3. View parked vehicles\n");
                break;
            case 4:
                printf("4. Search vehicle by license plate\n");
                break;
            case 5:
                viewDailyRevenue();
                break;
            case 6:
                saveData();
                break;
            case 7:
                exportRevenueReport();
                break;
            case 0:
                saveData();
                printf("Exited the program.\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 0);

    return 0;
}
