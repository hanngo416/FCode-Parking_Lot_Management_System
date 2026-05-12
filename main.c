#include <stdio.h>

#include "include/types.h"
#include "include/parking.h"
#include "include/fileio.h"
#include "include/report.h"
#include "include/utils.h"
#include "include/auth.h"

int main() {
    ParkingLot parkingLot;
    Account accounts[MAX_ACCOUNTS];

    int accountCount = 0;
    int currentIndex = -1;
    int choice;

    initParkingLot(&parkingLot);
    loadData(&parkingLot);

    loadAccounts(accounts, &accountCount);
    initDefaultAdmin(accounts, &accountCount);

    while (currentIndex == -1) {
        printf("\n========== ACCOUNT MENU ==========\n");
        printf("1. Login\n");
        printf("0. Exit\n");

        choice = getInt("Choose: ",
                        "Please enter 0 or 1.",
                        "Invalid input.",
                        0, 1);

        if (choice == 1) {
            currentIndex = login(accounts, accountCount);
        } 
        else {
            saveData(&parkingLot);
            saveAccounts(accounts, accountCount);
            printf("Exit program.\n");
            return 0;
        }
    }

    do {
        int role = accounts[currentIndex].role;

        printf("\n========== PARKING LOT MANAGEMENT ==========\n");
        printf("User: %s | Role: %s\n",
               accounts[currentIndex].username,
               role == ROLE_ADMIN ? "Admin" : "Staff");

        printMenuByRole(role);

        choice = getInt("Choose: ",
                        "Invalid menu option.",
                        "Invalid input.",
                        0, (role == ROLE_ADMIN ? 12 : 4));

        if (!checkPermission(role, choice)) {
            if (choice != 0) {
                printf("Access denied. Staff only can use options 1-4.\n");
                continue;
            }
        }

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
                saveData(&parkingLot);
                printf("Data saved successfully.\n");
                break;

            case 7: 
                printf("Edit price list by vehicle type (Coming soon).\n");
                break;

            case 8: 
                exportRevenueReport(&parkingLot);
                break;

            case 9: 
                updateOwnAccount(accounts, accountCount, currentIndex);
                saveAccounts(accounts, accountCount);
                break;

            case 10:
                createStaffAccount(accounts, &accountCount);
                saveAccounts(accounts, accountCount);
                break;

            case 11: 
                promoteToAdmin(accounts, accountCount);
                saveAccounts(accounts, accountCount);
                break;

            case 12: 
                listAccounts(accounts, accountCount);
                break;

            case 0:
                saveData(&parkingLot);
                saveAccounts(accounts, accountCount);
                printf("Exit program.\n");
                break;
        }

    } while (choice != 0);

    return 0;
}