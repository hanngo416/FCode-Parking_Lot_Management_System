#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/utils.h"

void removeNewLine(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

int getInt(const char *messageInfo,const char *messageErrorOutOfRange,const char *messageErrorNumber,int min, int max) {
    char buffer[100];
    int number;
    char extra;
    while (1) {
        printf("%s", messageInfo);
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d %c", &number, &extra) != 1) {
            printf("%s\n", messageErrorNumber);
            continue;
        }
        if (number < min || number > max) {
            printf("%s\n", messageErrorOutOfRange);
            continue;
        }
        return number;
    }
}
int getIntUpdate(const char *messageInfo,const char *messageErrorOutOfRange,const char *messageErrorNumber,int min, int max,int oldValue) {
    char buffer[100];
    int number;
    char extra;
    while (1) {
        printf("%s", messageInfo);
        fgets(buffer, sizeof(buffer), stdin);
        removeNewLine(buffer);
        if (strlen(buffer) == 0) {
            return oldValue;
        }
        if (sscanf(buffer, "%d %c", &number, &extra) != 1) {
            printf("%s\n", messageErrorNumber);
            continue;
        }
        if (number < min || number > max) {
            printf("%s\n", messageErrorOutOfRange);
            continue;
        }
        return number;
    }
}

double getDouble(const char *messageInfo, const char *messageError,double min) {
    char buffer[100];
    double number;
    char extra;
    while (1) {
        printf("%s", messageInfo);
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%lf %c", &number, &extra) != 1) {
            printf("%s\n", messageError);
            continue;
        }
        if (number < min) {
            printf("%s\n", messageError);
            continue;
        }
        return number;
    }
}
void getString(const char *messageInfo,char *buffer,int maxLen) {
    while (1) {
        printf("%s", messageInfo);
        fgets(buffer, maxLen, stdin);
        removeNewLine(buffer);

        if (strlen(buffer) == 0) {
            printf("Input cannot be empty.\n");
            continue;
        }
        return;
    }
}
void getStringUpdate(const char *messageInfo,
                     char *buffer,
                     int maxLen,
                     const char *oldValue) {
    printf("%s", messageInfo);
    fgets(buffer, maxLen, stdin);
    removeNewLine(buffer);

    if (strlen(buffer) == 0) {
        strcpy(buffer, oldValue);
    }
}
void toUpperCase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}
int isEmpty(const char *str) {
    return str == NULL || strlen(str) == 0;
}
int isValidLicensePlate(const char *plate) {
    if (isEmpty(plate)) {
        return 0;
    }

    if (strlen(plate) != 10) {
        return 0;
    }

    if (!isdigit(plate[0]) || !isdigit(plate[1])) {
        return 0;
    }

    if (!isalpha(plate[2]) || !isalpha(plate[3])) {
        return 0;
    }

    if (plate[4] != '-') {
        return 0;
    }

    for (int i = 5; i < 10; i++) {
        if (!isdigit(plate[i])) {
            return 0;
        }
    }

    return 1;
}

int isDuplicateLicensePlate(Vehicle vehicles[], int count, const char *plate) {
    for (int i = 0; i < count; i++) {
        if (vehicles[i].status == 0 &&
            strcmp(vehicles[i].licensePlate, plate) == 0) {
            return 1;
        }
    }

    return 0;
}

int isValidVehicleType(int type) {
    return type == 0 || type == 1 || type == 2;
}

int isValidStatus(int status) {
    return status == 0 || status == 1;
}
int parseDateTime(const char *dateStr, time_t *result) {
    int day, month, year, hour, minute;
    char extra;

    if (isEmpty(dateStr)) {
        return 0;
    }

    if (sscanf(dateStr, "%d/%d/%d %d:%d %c",
               &day, &month, &year, &hour, &minute, &extra) != 5) {
        return 0;
    }

    if (day < 1 || day > 31 ||
        month < 1 || month > 12 ||
        year < 1900 ||
        hour < 0 || hour > 23 ||
        minute < 0 || minute > 59) {
        return 0;
    }

    struct tm timeInfo = {0};
    timeInfo.tm_mday = day;
    timeInfo.tm_mon = month - 1;
    timeInfo.tm_year = year - 1900;
    timeInfo.tm_hour = hour;
    timeInfo.tm_min = minute;
    timeInfo.tm_sec = 0;
    timeInfo.tm_isdst = -1;

    time_t convertedTime = mktime(&timeInfo);

    if (convertedTime == -1) {
        return 0;
    }

    if (timeInfo.tm_mday != day ||
        timeInfo.tm_mon != month - 1 ||
        timeInfo.tm_year != year - 1900 ||
        timeInfo.tm_hour != hour ||
        timeInfo.tm_min != minute) {
        return 0;
    }

    *result = convertedTime;
    return 1;
}

int isValidEntryTime(time_t entryTime) {
    return entryTime > 0;
}

int isValidEntryExitTime(time_t entryTime, time_t exitTime) {
    return entryTime > 0 && exitTime > entryTime;
}
int isValidUsername(const char *username) {
    int len;

    if (isEmpty(username)) {
        return 0;
    }

    len = strlen(username);

    if (len < 2 || len > 25) {
        return 0;
    }

    for (int i = 0; i < len; i++) {
        if (!isalnum(username[i])) {
            return 0;
        }
    }

    return 1;
}
int isValidPassword(const char *password) {
    int len;
    int hasUpper = 0;
    int hasSpecial = 0;

    if (isEmpty(password)) {
        return 0;
    }

    len = strlen(password);

    if (len < 8) {
        return 0;
    }

    for (int i = 0; i < len; i++) {
        if (isupper(password[i])) {
            hasUpper = 1;
        }

        if (!isalnum(password[i])) {
            hasSpecial = 1;
        }
    }

    return hasUpper && hasSpecial;
}
//trong quá trình làm nếu cần thêm hàm validate nào thì nhắn c 