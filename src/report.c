#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/types.h"
#include "../include/report.h"

extern Vehicle vehicles[MAX_VEHICLES];
extern int vehicleCount;

static const char* getVehicleTypeName(VehicleType type) {
    switch (type) {
        case MOTO: return "Motorbike";
        case CAR:  return "Car";
        case BUS:  return "Bus";
        default:   return "Unknown";
    }
}

static int isSameDay(time_t t1, time_t t2) {
    struct tm *tm1 = localtime(&t1);
    int d1 = tm1->tm_mday, m1 = tm1->tm_mon, y1 = tm1->tm_year;

    struct tm *tm2 = localtime(&t2);
    int d2 = tm2->tm_mday, m2 = tm2->tm_mon, y2 = tm2->tm_year;

    return (d1 == d2 && m1 == m2 && y1 == y2);
}

void viewDailyRevenue(void) {
    time_t now = time(NULL);
    struct tm *today = localtime(&now);
    double totalRevenue = 0.0;
    int totalVehiclesOut = 0;
    int i;

    printf("\n========================================\n");
    printf("     DAILY REVENUE STATISTICS\n");
    printf("========================================\n");
    printf("  Date: %02d/%02d/%04d\n",
           today->tm_mday, today->tm_mon + 1, today->tm_year + 1900);
    printf("----------------------------------------\n");

    for (i = 0; i < vehicleCount; i++) {
        if (vehicles[i].status != 1) {
            continue;
        }

        if (!isSameDay(vehicles[i].exitTime, now)) {
            continue;
        }

        totalRevenue += vehicles[i].fee;
        totalVehiclesOut++;
    }

    if (totalVehiclesOut == 0) {
        printf("  No revenue data for today.\n");
        printf("  No vehicles have checked out yet.\n");
    } else {
        printf("  Vehicles checked out : %d\n", totalVehiclesOut);
        printf("  Total revenue        : %.0f VND\n", totalRevenue);
    }

    printf("========================================\n");
}

void exportRevenueReport(void) {
    time_t now = time(NULL);
    struct tm *today = localtime(&now);
    double totalRevenue = 0.0;
    int totalVehiclesOut = 0;
    char filename[50];
    int i;

    sprintf(filename, "data/report_%02d_%02d_%04d.txt",
            today->tm_mday, today->tm_mon + 1, today->tm_year + 1900);

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("ERROR: Cannot create report file!\n");
        printf("       Please check if 'data/' directory exists and is writable.\n");
        return;
    }

    fprintf(fp, "===================================================\n");
    fprintf(fp, "       DAILY REVENUE REPORT - F-CODE CLUB\n");
    fprintf(fp, "       FPT University Ho Chi Minh City\n");
    fprintf(fp, "===================================================\n");
    fprintf(fp, "  Date       : %02d/%02d/%04d\n",
            today->tm_mday, today->tm_mon + 1, today->tm_year + 1900);
    fprintf(fp, "  Generated  : %02d:%02d:%02d\n",
            today->tm_hour, today->tm_min, today->tm_sec);
    fprintf(fp, "===================================================\n\n");

    fprintf(fp, "%-5s %-15s %-14s %12s\n", "No.", "License Plate", "Vehicle Type", "Parking Fee");
    fprintf(fp, "---------------------------------------------------\n");

    int rowNum = 0;
    for (i = 0; i < vehicleCount; i++) {
        if (vehicles[i].status != 1) {
            continue;
        }
        if (!isSameDay(vehicles[i].exitTime, now)) {
            continue;
        }

        rowNum++;
        totalRevenue += vehicles[i].fee;
        totalVehiclesOut++;

        fprintf(fp, "%-5d %-15s %-14s %12.0f\n",
                rowNum,
                vehicles[i].licensePlate,
                getVehicleTypeName(vehicles[i].type),
                vehicles[i].fee);
    }

    if (totalVehiclesOut == 0) {
        fprintf(fp, "  No vehicles checked out today.\n");
    }

    fprintf(fp, "---------------------------------------------------\n");
    fprintf(fp, "  Total vehicles checked out : %d\n", totalVehiclesOut);
    fprintf(fp, "  Total revenue              : %.0f VND\n", totalRevenue);
    fprintf(fp, "===================================================\n");
    fprintf(fp, "       End of Report - Parking Lot System\n");
    fprintf(fp, "===================================================\n");

    fclose(fp);
    printf("\n========================================\n");
    printf("  Report exported successfully!\n");
    printf("  File: %s\n", filename);
    printf("  Vehicles: %d | Revenue: %.0f VND\n", totalVehiclesOut, totalRevenue);
    printf("========================================\n");
}
