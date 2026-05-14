#ifndef AUTH_H
#define AUTH_H

#define MAX_ACCOUNTS 100
#define ROLE_ADMIN 1
#define ROLE_STAFF 2

typedef struct {
    char username[30];
    char password[30];
    int role;
} Account;

void initDefaultAdmin(Account accounts[], int *accountCount);

int login(Account accounts[], int accountCount);

void printMenuByRole(int role);
int checkPermission(int role, int choice);

void updateOwnAccount(Account accounts[], int accountCount, int currentIndex);
void createStaffAccount(Account accounts[], int *accountCount);
void promoteToAdmin(Account accounts[], int accountCount);
void listAccounts(Account accounts[], int accountCount);

int findAccountByUsername(Account accounts[], int accountCount, char username[]);

void loadAccounts(Account accounts[], int *accountCount);
void saveAccounts(Account accounts[], int accountCount);

#endif