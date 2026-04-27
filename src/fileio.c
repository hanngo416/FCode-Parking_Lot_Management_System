#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/fileio.h"

/*
 * fileio.c - Module doc/ghi du lieu xe (Chuc nang 6)
 * 
 * Dinh dang du lieu trong vehicles.dat (phan tach bang '|', moi xe mot dong):
 * bienSo|loaiXe|gioVao|gioRa|phi|trangThai
 * 
 * Vi du:
 * 76AF-03235|1|1714200000|1714210000|50000.00|1
 * 
 * Thu vien su dung: stdio.h, stdlib.h, string.h
 */

#define DATA_FILE "data/vehicles.dat"

/* Tham chieu den bien toan cuc duoc khai bao trong main.c */
extern Vehicle vehicles[MAX_VEHICLES];
extern int vehicleCount;

/*
 * loadData - Nap du lieu xe tu file vao bo nho RAM
 * 
 * Mo file data/vehicles.dat va doc toan bo ban ghi.
 * Neu file khong ton tai (lan chay dau), khoi tao trang thai rong.
 * Kiem tra so luong xe khong vuot qua MAX_VEHICLES (3636).
 */
void loadData(void) {
    FILE *fp = fopen(DATA_FILE, "r");

    if (fp == NULL) {
        /* File chua ton tai - he thong chay lan dau hoac du lieu da bi xoa */
        vehicleCount = 0;
        printf("========================================\n");
        printf("  No saved data found. Starting fresh.\n");
        printf("========================================\n");
        return;
    }

    vehicleCount = 0;

    /* Doc tung dong va phan tich cac truong phan tach bang dau '|' */
    while (vehicleCount < MAX_VEHICLES) {
        int type, status;
        long entry, exit_t;
        double fee;
        char plate[12];

        /* Dinh dang doc: bienSo|loaiXe|gioVao|gioRa|phi|trangThai */
        int result = fscanf(fp, "%11[^|]|%d|%ld|%ld|%lf|%d\n",
                            plate, &type, &entry, &exit_t, &fee, &status);

        if (result == EOF || result != 6) {
            break; /* Het file hoac dong bi loi dinh dang */
        }

        /* Sao chep du lieu da doc vao mang vehicles toan cuc */
        strcpy(vehicles[vehicleCount].licensePlate, plate);
        vehicles[vehicleCount].type = (VehicleType)type;
        vehicles[vehicleCount].entryTime = (time_t)entry;
        vehicles[vehicleCount].exitTime = (time_t)exit_t;
        vehicles[vehicleCount].fee = fee;
        vehicles[vehicleCount].status = status;

        vehicleCount++;
    }

    fclose(fp); /* Dong file ngay lap tuc de giai phong tai nguyen */

    /* Canh bao neu du lieu vuot qua suc chua toi da */
    if (vehicleCount >= MAX_VEHICLES) {
        printf("WARNING: Maximum capacity (%d) reached during data load!\n", MAX_VEHICLES);
        printf("         Some records may have been skipped.\n");
    }

    printf("========================================\n");
    printf("  Data loaded: %d vehicle(s) found.\n", vehicleCount);
    printf("========================================\n");
}

/*
 * saveData - Ghi toan bo du lieu xe tu RAM xuong file
 * 
 * Ghi de file data/vehicles.dat voi trang thai hien tai cua mang vehicles.
 * Ham nay duoc goi tu dong sau moi thao tac them/xuat xe.
 * File chi duoc mo trong luc ghi va dong ngay - khong bao gio de mo.
 */
void saveData(void) {
    FILE *fp = fopen(DATA_FILE, "w");

    if (fp == NULL) {
        printf("ERROR: Cannot open file for saving! Data may be lost.\n");
        printf("       Please check if 'data/' directory exists.\n");
        return;
    }

    /* Ghi moi xe thanh mot dong, phan tach bang dau '|' */
    int i;
    for (i = 0; i < vehicleCount; i++) {
        fprintf(fp, "%s|%d|%ld|%ld|%.2f|%d\n",
                vehicles[i].licensePlate,
                vehicles[i].type,
                (long)vehicles[i].entryTime,
                (long)vehicles[i].exitTime,
                vehicles[i].fee,
                vehicles[i].status);
    }

    fclose(fp); /* Dong file ngay lap tuc - tranh hong du lieu */

    printf("Data saved successfully. (%d vehicle(s) written)\n", vehicleCount);
}
