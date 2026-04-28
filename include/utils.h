#ifndef UTILS_H
#define UTILS_H
#include "types.h"
int getInt(const char *messageInfo,const char *messageErrorOutOfRange,const char *messageErrorNumber,int min, int max);

int getIntUpdate(const char *messageInfo,const char *messageErrorOutOfRange,const char *messageErrorNumber,int min, int max,int oldValue);

double getDouble(const char *messageInfo,const char *messageError,double min);

void getString(const char *messageInfo,char *buffer,int maxLen);

void getStringUpdate(const char *messageInfo,char *buffer,int maxLen,const char *oldValue);
void toUpperCase(char *str);
int isEmpty(const char *str);

int isValidLicensePlate(const char *plate);
int isDuplicateLicensePlate(Vehicle vehicles[], int count, const char *plate);

int isValidVehicleType(int type);
int isValidStatus(int status);

int parseDateTime(const char *dateStr, time_t *result);
int isValidEntryExitTime(time_t entryTime, time_t exitTime);
int isValidEntryTime(time_t entryTime);
int isValidUsername(const char *username);
int isValidPassword(const char *password);
#endif
