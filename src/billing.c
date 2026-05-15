#include <stdio.h>
#include <time.h>
#include "../include/billing.h"
#include "../include/types.h"
#include "../include/parking.h"
#include "../include/utils.h"

double calculateFee(Vehicle v, ParkingLot *p) {
    double seconds = difftime(v.exitTime, v.entryTime);
    double hours   = seconds / 3600.0;

    if (hours < 1) hours = 1;

    /* MOTO=0, CAR=1, BUS=2 matches prices[] index directly */
    double fee = hours * p->prices[v.type].price;

    if (hours > 72)
        printf("\n[WARNING] Vehicle parked over 72 hours!\n");

    return fee;
}

void printBill(Vehicle v, ParkingLot *p) {

    const char *typeStr;

    if (v.type == MOTO)
        typeStr = "Motorbike";
    else if (v.type == CAR)
        typeStr = "Car";
    else
        typeStr = "Bus";

    double parkingHours =
        difftime(v.exitTime, v.entryTime) / 3600.0;

    int roundedHours = (int)(parkingHours + 0.5);

    double baseFee = v.fee;
    double surchargePerHour = 5000;
    double overtimeSurcharge = 0;
    double nightSurchargeFee = 0;
    int nightSurchargeHours = 0;

    time_t tempTime = v.entryTime;

    while (tempTime < v.exitTime) {

        struct tm *local = localtime(&tempTime);
        int hour = local->tm_hour;

        if (hour >= 23 || hour < 3) {
            nightSurchargeFee += surchargePerHour;
            nightSurchargeHours++;
        }
        tempTime += 3600;

        if (parkingHours > 72) {
            overtimeSurcharge = 50000;
        }
    }

    double totalFee = baseFee + nightSurchargeFee + overtimeSurcharge;

    printf("\n===== BILL =====\n");

    printf("Plate              : %s\n", v.licensePlate);
    printf("Type               : %s\n", typeStr);

    printf("Entry Time         : %s", ctime(&v.entryTime));
    printf("Exit Time          : %s", ctime(&v.exitTime));

    printf("Parking Hours      : %d hours\n", roundedHours);

    printf("Night Parking Time : %d hours\n", nightSurchargeHours);

    printf("Base Fee           : ");
    printMoney(baseFee);
    printf("\n");

    printf("Night Hours Fee (23h-3h)   : %.0f VND/hour\n",
       surchargePerHour);

    printf("Night Surcharge   : ");
    printMoney(nightSurchargeFee);
    printf("\n");

    printf("72h Surcharge     : ");
    printMoney(overtimeSurcharge);
    printf("\n");

    printf("Total Fee          : ");
    printMoney(totalFee);
    printf("\n");

    printf("========================\n");
}


void updatePrice(ParkingLot *p) {
    printf("\n===== UPDATE PRICE =====\n");
    printf("1. Motorbike (current: %.0f VND/hour)\n", p->prices[MOTO].price);
    printf("2. Car       (current: %.0f VND/hour)\n", p->prices[CAR].price);
    printf("3. Bus       (current: %.0f VND/hour)\n", p->prices[BUS].price);

    int choice = getInt(
        "Choose type: ",
        "Invalid! Please enter 1, 2, or 3.",
        "Error! Please enter a valid number.",
        1, 3
    );

    double newPrice = getDouble(
        "New price (VND/hr): ",
        "Invalid! Price must be greater than 0.",
        0
    );

    int i = choice - 1;
    p->prices[i].price = newPrice;
    printf("Updated %s price to %.0f VND/hr\n",
           p->prices[i].typeName,
           p->prices[i].price);
}