#include <stdio.h>
#include <string.h>
#include "../include/auth.h"
#include "../include/utils.h"

void initDefaultAdmin(Account accounts[], int *accountCount) {

    if (*accountCount == 0) {

        strcpy(accounts[0].username, "admin");
        strcpy(accounts[0].password, "Admin123");

        accounts[0].role = ROLE_ADMIN;

        *accountCount = 1;
    }
}

int login(Account accounts[], int accountCount) {

    char username[30];
    char password[30];

    printf("\n===== LOGIN =====\n");

    int index = -1;
    while (index == -1) {
        getString("Enter username: ", username, sizeof(username));

        
        index = findAccountByUsername(accounts, accountCount, username);

        if (index == -1) {
            printf("Error: Username '%s' not found. Please try again.\n", username);
        }
    }

   
    while (1) {
        getString("Enter password: ", password, sizeof(password));

        if (strcmp(password, accounts[index].password) == 0) {
            printf("Login successfully!\n");
            return index;
        } else {
            printf("Error: Invalid password. Please try again.\n");
        }
    }
}

void printMenuByRole(int role) {
 
    printf("\n========================================\n");
    printf("       PARKING LOT MANAGEMENT\n");
    printf("========================================\n");

    printf("1. Add a vehicle\n");
    printf("2. Remove a vehicle\n");
    printf("3. View parked vehicles\n");
    printf("4. Search vehicle by license plate\n");

    if (role == ROLE_ADMIN) {

        printf("5. View daily revenue report\n");
        printf("6. Save data\n");
        printf("7. Edit price list by vehicle type\n");
        printf("8. Export revenue report (.txt)\n");

        printf("9. Change my username/password\n");
        printf("10. Create staff account\n");
        printf("11. Promote staff to admin\n");
        printf("12. View account list\n");
    }

    printf("0. Exit\n");

    printf("========================================\n");
}

int checkPermission(int role, int choice) {

    if (role == ROLE_ADMIN) {
        return 1;
    }

    if (role == ROLE_STAFF) {

        if (choice >= 1 && choice <= 4) {
            return 1;
        }

        return 0;
    }

    return 0;
}

void updateOwnAccount(Account accounts[], int accountCount, int currentIndex) {

    char newPassword[30];

    printf("\n===== CHANGE PASSWORD =====\n");
    printf("Current account: %s\n", accounts[currentIndex].username);

    while (1) {
        getString("Enter new password: ", newPassword, sizeof(newPassword));
        if (isValidPassword(newPassword)) break;
    }

    strcpy(accounts[currentIndex].password, newPassword);

    printf("Password updated successfully.\n");
}

int findAccountByUsername(Account accounts[], int accountCount, char username[]) {

    for (int i = 0; i < accountCount; i++) {

        if (strcmp(accounts[i].username, username) == 0) {
            return i;
        }
    }

    return -1;
}

void createStaffAccount(Account accounts[], int *accountCount) {

    if (*accountCount >= MAX_ACCOUNTS) {

        printf("Account list is full.\n");

        return;
    }

    char username[30];
    char password[30];

    printf("\n===== CREATE STAFF ACCOUNT =====\n");

    while (1) {
        getString("Enter username: ", username, sizeof(username));
        if (isValidUsername(username)) break;
    }

    if (findAccountByUsername(accounts, *accountCount, username) != -1) {

        printf("Username already exists.\n");

        return;
    }

    while (1) {
        getString("Enter password: ", password, sizeof(password));
        if (isValidPassword(password)) break;
    }

    strcpy(accounts[*accountCount].username, username);
    strcpy(accounts[*accountCount].password, password);

    accounts[*accountCount].role = ROLE_STAFF;

    (*accountCount)++;

    printf("Staff account created successfully.\n");
}

void promoteToAdmin(Account accounts[], int accountCount) {

    char username[30];

    printf("\n===== PROMOTE STAFF TO ADMIN =====\n");

    getString("Enter username: ", username, sizeof(username));

    int index = findAccountByUsername(accounts, accountCount, username);

    if (index == -1) {

        printf("Account not found.\n");

        return;
    }

    accounts[index].role = ROLE_ADMIN;

    printf("Account promoted to admin successfully.\n");
}

void listAccounts(Account accounts[], int accountCount) {

    printf("\n===== ACCOUNT LIST =====\n");

    for (int i = 0; i < accountCount; i++) {

        printf("Username: %s\n", accounts[i].username);

        if (accounts[i].role == ROLE_ADMIN) {
            printf("Role: ADMIN\n");
        } else {
            printf("Role: STAFF\n");
        }

        printf("------------------------\n");
    }
}

void saveAccounts(Account accounts[], int accountCount) {

    FILE *file = fopen("data/accounts.dat", "wb");

    if (file == NULL) {

        printf("Cannot open account file.\n");

        return;
    }

    fwrite(&accountCount, sizeof(int), 1, file);

    fwrite(accounts, sizeof(Account), accountCount, file);

    fclose(file);
}

void loadAccounts(Account accounts[], int *accountCount) {

    FILE *file = fopen("data/accounts.dat", "rb");

    if (file == NULL) {

        *accountCount = 0;

        return;
    }

    fread(accountCount, sizeof(int), 1, file);

    fread(accounts, sizeof(Account), *accountCount, file);

    fclose(file);
}