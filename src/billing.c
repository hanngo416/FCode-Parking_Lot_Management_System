#include <stdio.h>
#include <time.h>
#include "include/billing.h"
#include "include/types.h"

double calculateFee(Vehicle v) {
    double seconds = difftime(v.exitTime, v.entryTime);
    double hours = seconds / 3600.0;

    if (hours < 1) hours = 1;

    if (v.type == MOTO) return hours * 2000;
    if (v.type == CAR) return hours * 5000;
    return hours * 10000; // TRUCK
}

void printBill(Vehicle v) {
    double seconds = difftime(v.exitTime, v.entryTime);

    printf("\n===== BILL =====\n");
    printf("Plate: %s\n", v.licensePlate);
    printf("Time: %.0f seconds\n", seconds);
    printf("Fee: %.2lf VND\n", v.fee);
}