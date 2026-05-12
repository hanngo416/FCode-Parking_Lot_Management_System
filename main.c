#include <stdio.h>
#include <stdlib.h>
#include "include/types.h"
#include "include/parking.h" 
#include "include/fileio.h"  
#include "include/report.h"
#include "include/utils.h"  

int main() {
    ParkingLot parkingLot;
    initParkingLot(&parkingLot);
    loadData(&parkingLot);       

    int choice;

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
        printf("7. Edit price list by vehicle type\n");
        printf("8. Export revenue report (.txt)\n");
        printf("0. Exit\n");
        printf("========================================\n");
        
        choice = getInt("Enter your choice: ",
                        "Please enter a number from 0 to 8.",
                        "Invalid input. Please enter a number.",
                        0, 8);

        switch (choice) {
            case 1:
                addVehicle(&parkingLot);
                break;
            case 2:
                removeVehicle(&parkingLot);
                break;
            case 3:
                listVehicles(&parkingLot);
                break;
            case 4:
                searchVehicle(&parkingLot);
                break;
            case 5:
                viewDailyRevenue(&parkingLot);
                break;
            case 6:
                saveData(&parkingLot, "Manual force save by User");                
                break;
            case 7:
                printf("Adjust price list by vehicle type.\n");
                break;
            case 8:
                exportRevenueReport(&parkingLot);
                break;
            case 0:
                saveData(&parkingLot, "Auto-save on system exit");
                printf("Exit the program.\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 0);

    return 0;
}