#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/types.h"
#include "../include/report.h"

/*
 * report.c - Module thong ke doanh thu va xuat bao cao (Chuc nang 5 & 9)
 * 
 * Chuc nang 5: viewDailyRevenue()    - Hien thi doanh thu ngay len console
 * Chuc nang 9: exportRevenueReport() - Xuat bao cao doanh thu ra file .txt
 * 
 * Thu vien su dung: stdio.h, stdlib.h, string.h, time.h (thu vien C chuan)
 */

/* Tham chieu den bien toan cuc duoc khai bao trong main.c */
extern Vehicle vehicles[MAX_VEHICLES];
extern int vehicleCount;

/*
 * getVehicleTypeName - Chuyen enum VehicleType sang chuoi hien thi
 */
static const char* getVehicleTypeName(VehicleType type) {
    switch (type) {
        case MOTO: return "Motorbike";
        case CAR:  return "Car";
        case BUS:  return "Bus";
        default:   return "Unknown";
    }
}

/*
 * isSameDay - Kiem tra 2 gia tri time_t co cung ngay hay khong
 * 
 * Chuyen ca 2 time_t sang struct tm (gio dia phuong) roi so sanh
 * ngay/thang/nam. Muc dich: tranh cong don sai doanh thu tu ngay khac.
 * 
 * Luu y: localtime() tra ve con tro tinh (static pointer),
 * nen phai luu ket qua ngay truoc khi goi lan 2.
 */
static int isSameDay(time_t t1, time_t t2) {
    struct tm *tm1 = localtime(&t1);
    int d1 = tm1->tm_mday, m1 = tm1->tm_mon, y1 = tm1->tm_year;

    struct tm *tm2 = localtime(&t2);
    int d2 = tm2->tm_mday, m2 = tm2->tm_mon, y2 = tm2->tm_year;

    return (d1 == d2 && m1 == m2 && y1 == y2);
}

/*
 * viewDailyRevenue - Hien thi thong ke doanh thu trong ngay (Chuc nang 5)
 * 
 * Loc tat ca xe thoa man 2 dieu kien:
 *   - status == 1 (da xuat ben / phien do xe da ket thuc)
 *   - exitTime trung voi ngay hien tai
 * 
 * Xe co status == 0 (dang do) se bi bo qua vi phien do xe
 * chua hoan tat khong duoc tinh vao doanh thu chot ngay.
 */
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

    /* Duyet toan bo mang xe, loc cac phien do xe da hoan tat trong ngay */
    for (i = 0; i < vehicleCount; i++) {
        /* Dieu kien 1: xe phai da xuat ben (status == 1) */
        if (vehicles[i].status != 1) {
            continue;
        }

        /* Dieu kien 2: thoi diem ra (exitTime) phai trung ngay hom nay */
        if (!isSameDay(vehicles[i].exitTime, now)) {
            continue;
        }

        /* Xe thoa man - cong don doanh thu */
        totalRevenue += vehicles[i].fee;
        totalVehiclesOut++;
    }

    /* Hien thi ket qua hoac thong bao trang thai rong */
    if (totalVehiclesOut == 0) {
        printf("  No revenue data for today.\n");
        printf("  No vehicles have checked out yet.\n");
    } else {
        printf("  Vehicles checked out : %d\n", totalVehiclesOut);
        printf("  Total revenue        : %.0f VND\n", totalRevenue);
    }

    printf("========================================\n");
}

/*
 * exportRevenueReport - Xuat bao cao doanh thu ra file .txt (Chuc nang 9)
 * 
 * Tai su dung logic loc cua viewDailyRevenue de trich xuat
 * danh sach chi tiet xe da thanh toan trong ngay.
 * 
 * File xuat ra: data/report_DD_MM_YYYY.txt
 * Dinh dang: van ban can le voi header, bang du lieu, va footer.
 */
void exportRevenueReport(void) {
    time_t now = time(NULL);
    struct tm *today = localtime(&now);
    double totalRevenue = 0.0;
    int totalVehiclesOut = 0;
    char filename[50];
    int i;

    /* Tao ten file tu dong theo ngay hien tai */
    sprintf(filename, "data/report_%02d_%02d_%04d.txt",
            today->tm_mday, today->tm_mon + 1, today->tm_year + 1900);

    /* Mo file de ghi bao cao */
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("ERROR: Cannot create report file!\n");
        printf("       Please check if 'data/' directory exists and is writable.\n");
        return;
    }

    /* ===== Ghi phan tieu de bao cao (Header) ===== */
    fprintf(fp, "===================================================\n");
    fprintf(fp, "       DAILY REVENUE REPORT - F-CODE CLUB\n");
    fprintf(fp, "       FPT University Ho Chi Minh City\n");
    fprintf(fp, "===================================================\n");
    fprintf(fp, "  Date       : %02d/%02d/%04d\n",
            today->tm_mday, today->tm_mon + 1, today->tm_year + 1900);
    fprintf(fp, "  Generated  : %02d:%02d:%02d\n",
            today->tm_hour, today->tm_min, today->tm_sec);
    fprintf(fp, "===================================================\n\n");

    /* ===== Ghi tieu de cac cot du lieu ===== */
    fprintf(fp, "%-5s %-15s %-14s %12s\n", "No.", "License Plate", "Vehicle Type", "Parking Fee");
    fprintf(fp, "---------------------------------------------------\n");

    /* ===== Ghi tung dong du lieu xe da thanh toan ===== */
    int rowNum = 0;
    for (i = 0; i < vehicleCount; i++) {
        /* Cung dieu kien loc nhu viewDailyRevenue */
        if (vehicles[i].status != 1) {
            continue;
        }
        if (!isSameDay(vehicles[i].exitTime, now)) {
            continue;
        }

        rowNum++;
        totalRevenue += vehicles[i].fee;
        totalVehiclesOut++;

        /* Su dung dinh dang can le de cot deu nhau */
        fprintf(fp, "%-5d %-15s %-14s %12.0f\n",
                rowNum,
                vehicles[i].licensePlate,
                getVehicleTypeName(vehicles[i].type),
                vehicles[i].fee);
    }

    /* Truong hop khong co xe nao xuat ben trong ngay */
    if (totalVehiclesOut == 0) {
        fprintf(fp, "  No vehicles checked out today.\n");
    }

    /* ===== Ghi phan chu thich cuoi bao cao (Footer) ===== */
    fprintf(fp, "---------------------------------------------------\n");
    fprintf(fp, "  Total vehicles checked out : %d\n", totalVehiclesOut);
    fprintf(fp, "  Total revenue              : %.0f VND\n", totalRevenue);
    fprintf(fp, "===================================================\n");
    fprintf(fp, "       End of Report - Parking Lot System\n");
    fprintf(fp, "===================================================\n");

    fclose(fp); /* Dong file ngay lap tuc */

    /* In thong bao thanh cong ra console kem duong dan file */
    printf("\n========================================\n");
    printf("  Report exported successfully!\n");
    printf("  File: %s\n", filename);
    printf("  Vehicles: %d | Revenue: %.0f VND\n", totalVehiclesOut, totalRevenue);
    printf("========================================\n");
}
