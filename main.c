#include <stdio.h>
#include <stdlib.h>
#include "include/types.h"
#include "include/parking.h"
#include "include/billing.h"
#include "include/utils.h"

int main() {
    ParkingLot parkingLot;
    initParkingLot(&parkingLot);
    loadData(&parkingLot);       //ham loadData em de day cho cai viewlist no chay nha

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
        printf("7. Adjust prices [ADMIN]\n");
        printf("8. Management [ADMIN]\n");
        printf("9. Export daily report\n");
        printf("0. Exit\n");
        printf("========================================\n");

        choice = getInt("Enter your choice: ",
                        "Please enter a number from 0 to 9.",
                        "Invalid input. Please enter a number.",
                        0, 9);

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
                printf("View daily revenue report.\n");
                break;
            case 6:
                printf("Save data.\n");
                break;
            case 7:
                printf("Adjust prices [ADMIN].\n");
                break;
            case 8:
                printf("Management [ADMIN].\n");
                break;
            case 9:
                printf("Export daily report.\n");
                break;
            case 0:
                printf("Exit the program.\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 0);

    return 0;
}
