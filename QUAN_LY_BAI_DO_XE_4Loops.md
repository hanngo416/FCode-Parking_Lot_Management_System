# 🚗 QUẢN LÝ BÃI ĐỖ XE — Tài liệu kỹ thuật đầy đủ
**Nhóm:** 4Loops | **CLB F-Code · ĐH FPT TP.HCM**
**Ngôn ngữ:** C (chuẩn C99) | **Thời gian:** 17/04/2026 – 17/05/2026

---

## Mục lục

1. [Tổng quan hệ thống](#1-tổng-quan-hệ-thống)
2. [Cấu trúc thư mục](#2-cấu-trúc-thư-mục)
3. [Cấu trúc dữ liệu (Structs)](#3-cấu-trúc-dữ-liệu-structs)
4. [Thiết kế từng module](#4-thiết-kế-từng-module)
   - 4.1 [main.c](#41-mainc)
   - 4.2 [parking.h / parking.c](#42-parkingh--parkingc)
   - 4.3 [billing.h / billing.c](#43-billingh--billingc)
   - 4.4 [fileio.h / fileio.c](#44-fileioh--fileioc)
   - 4.5 [report.h / report.c](#45-reporth--reportc)
   - 4.6 [auth.h / auth.c](#46-authh--authc)
   - 4.7 [utils.h / utils.c](#47-utilsh--utilsc)
5. [Luồng xử lý (Flow)](#5-luồng-xử-lý-flow)
6. [Quy tắc tính tiền](#6-quy-tắc-tính-tiền)
7. [Định dạng file lưu trữ](#7-định-dạng-file-lưu-trữ)
8. [Phân công công việc](#8-phân-công-công-việc)
9. [Quy ước Git & Commit](#9-quy-ước-git--commit)
10. [Checklist trước khi nộp](#10-checklist-trước-khi-nộp)
11. [Gợi ý test case](#11-gợi-ý-test-case)
12. [ANSI Color (Tùy chọn)](#12-ansi-color-tùy-chọn)

---

## 1. Tổng quan hệ thống

### Bài toán
Hệ thống quản lý bãi đỗ xe cho ĐH FPT TP.HCM, chạy hoàn toàn trên **terminal**, không GUI, không mạng, không database, chỉ dùng **thư viện C chuẩn**.

### Ràng buộc cứng
| Ràng buộc | Giá trị |
|-----------|---------|
| Sức chứa tối đa | **3636** phương tiện tại một thời điểm |
| Định danh duy nhất | Biển số xe (không trùng) |
| Lưu trữ | File `.dat` / `.txt` — không mất khi tắt |
| Thư viện | Chỉ dùng `stdio.h`, `stdlib.h`, `string.h`, `time.h`, `ctype.h` |

### Chức năng theo mức độ
| Mức độ | Chức năng |
|--------|-----------|
| **Bắt buộc** | Thêm xe vào, cho xe ra, xem danh sách, tìm kiếm, thống kê doanh thu, lưu/đọc file |
| **Nâng cao** | Chỉnh bảng giá, phân quyền Admin/Nhân viên, xuất báo cáo `.txt` |
| **Tùy chọn** | Giao diện màu ANSI |

---

## 2. Cấu trúc thư mục

```
QuanLyBaiDoXe/
│
├── src/
│   ├── main.c          # Menu chính, điều hướng
│   ├── parking.c       # Logic thêm/xóa/tìm xe
│   ├── billing.c       # Tính tiền theo loại xe & giờ
│   ├── fileio.c        # Đọc/ghi file
│   ├── report.c        # Thống kê, xuất báo cáo
│   ├── auth.c          # Phân quyền Admin/Nhân viên
│   └── utils.c         # Hàm dùng chung
│
├── include/
│   ├── parking.h
│   ├── billing.h
│   ├── fileio.h
│   ├── report.h
│   ├── auth.h
│   ├── utils.h
│   └── types.h         # Khai báo struct dùng chung
│
├── data/
│   ├── vehicles.dat    # Dữ liệu xe đang đỗ + lịch sử
│   ├── prices.dat      # Bảng giá (có thể chỉnh)
│   └── users.dat       # Tài khoản Admin/Nhân viên
│
├── reports/
│   └── report_YYYYMMDD.txt   # Báo cáo xuất ra
│
├── Makefile
└── README.md
```

### Makefile gợi ý

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
SRC = src/main.c src/parking.c src/billing.c src/fileio.c src/report.c src/auth.c src/utils.c
OUT = parking

all:
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)
```

---

## 3. Cấu trúc dữ liệu (Structs)

Đặt tất cả struct trong `include/types.h` để các file khác `#include` dùng chung.

```c
#ifndef TYPES_H
#define TYPES_H

#include <time.h>

/* ===== Hằng số toàn cục ===== */
#define MAX_VEHICLES     3636
#define MAX_PLATE_LEN    15
#define MAX_NAME_LEN     30
#define MAX_VEHICLE_TYPES 3
#define DATA_FILE        "data/vehicles.dat"
#define PRICE_FILE       "data/prices.dat"
#define USER_FILE        "data/users.dat"

/* ===== Mã loại xe ===== */
#define TYPE_MOTORBIKE   0   /* Xe máy */
#define TYPE_CAR         1   /* Ô tô */
#define TYPE_TRUCK       2   /* Xe tải */

/* ===== Trạng thái xe ===== */
#define STATUS_PARKED    0   /* Đang đỗ */
#define STATUS_EXITED    1   /* Đã ra */

/* ===== Mức phân quyền ===== */
#define ROLE_ADMIN       0
#define ROLE_STAFF       1

/* ===== Struct: Phương tiện ===== */
typedef struct {
    char    licensePlate[MAX_PLATE_LEN]; /* Biển số xe, VD: 51A-12345 */
    int     vehicleType;                 /* 0=Xe máy, 1=Ô tô, 2=Xe tải */
    time_t  entryTime;                   /* Thời điểm vào (unix timestamp) */
    time_t  exitTime;                    /* Thời điểm ra (0 nếu đang đỗ)  */
    double  fee;                         /* Phí đỗ (0 nếu chưa ra) */
    int     status;                      /* 0=Đang đỗ, 1=Đã ra */
} Vehicle;

/* ===== Struct: Bảng giá ===== */
typedef struct {
    int     vehicleType;                 /* Mã loại xe */
    char    typeName[MAX_NAME_LEN];      /* Tên hiển thị */
    double  pricePerHour;               /* Giá/giờ (VND) */
    double  minimumFee;                 /* Phí tối thiểu (VND) */
} PriceConfig;

/* ===== Struct: Tài khoản người dùng ===== */
typedef struct {
    char username[MAX_NAME_LEN];
    char password[MAX_NAME_LEN];         /* Lưu dạng plain text cho đơn giản */
    int  role;                           /* 0=Admin, 1=Staff */
} User;

/* ===== Struct: Phiên đăng nhập ===== */
typedef struct {
    User currentUser;
    int  isLoggedIn;                     /* 0=Chưa đăng nhập, 1=Đã đăng nhập */
} Session;

#endif /* TYPES_H */
```

---

## 4. Thiết kế từng module

### 4.1 `main.c`

**Vai trò:** Khởi động, đăng nhập, hiển thị menu, điều hướng. **Không chứa logic nghiệp vụ.**

```
Luồng khởi động:
  1. loadAllData()          ← đọc vehicles.dat + prices.dat + users.dat
  2. login()                ← nhập username/password, xác thực
  3. showMainMenu()         ← vòng lặp menu chính
  4. Người dùng chọn → gọi hàm tương ứng
  5. saveAllData()          ← ghi lại file sau mỗi thao tác quan trọng
```

**Sơ đồ menu:**

```
╔══════════════════════════════╗
║   QUẢN LÝ BÃI ĐỖ XE         ║
║   Đăng nhập bằng: [username] ║
╠══════════════════════════════╣
║ 1. Thêm xe vào bãi           ║
║ 2. Cho xe ra bãi             ║
║ 3. Xem danh sách xe đang đỗ  ║
║ 4. Tìm kiếm xe theo biển số  ║
║ 5. Xem thống kê doanh thu    ║
║ 6. Chỉnh bảng giá  [ADMIN]   ║
║ 7. Quản lý tài khoản [ADMIN] ║
║ 8. Xuất báo cáo ngày         ║
║ 0. Đăng xuất / Thoát         ║
╚══════════════════════════════╝
```

**Gợi ý code main.c:**

```c
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "parking.h"
#include "fileio.h"
#include "report.h"
#include "auth.h"
#include "utils.h"

/* Mảng toàn cục (chỉ khai báo ở main.c, truyền con trỏ sang hàm khác) */
static Vehicle     g_vehicles[MAX_VEHICLES];
static int         g_vehicleCount = 0;
static PriceConfig g_prices[MAX_VEHICLE_TYPES];
static Session     g_session;

int main(void) {
    /* 1. Khởi tạo bảng giá mặc định */
    initDefaultPrices(g_prices);

    /* 2. Đọc dữ liệu từ file */
    loadVehicles(g_vehicles, &g_vehicleCount);
    loadPrices(g_prices);

    /* 3. Đăng nhập */
    if (!loginPrompt(&g_session)) {
        printf("Sai thong tin dang nhap. Thoat.\n");
        return 1;
    }

    /* 4. Menu chính */
    int choice;
    do {
        printMainMenu(&g_session);
        choice = getIntInput("Chon chuc nang: ", 0, 8);
        handleMenuChoice(choice, g_vehicles, &g_vehicleCount, g_prices, &g_session);
    } while (choice != 0);

    /* 5. Lưu khi thoát */
    saveVehicles(g_vehicles, g_vehicleCount);
    savePrices(g_prices);

    printf("Tam biet!\n");
    return 0;
}
```

---

### 4.2 `parking.h` / `parking.c`

**Vai trò:** Thêm xe vào, cho xe ra, xem danh sách, tìm kiếm.

#### Header (`parking.h`)

```c
#ifndef PARKING_H
#define PARKING_H

#include "types.h"

/* Thêm xe vào bãi */
int  addVehicle(Vehicle vehicles[], int *count, const PriceConfig prices[]);

/* Cho xe ra bãi, tính tiền, hiển thị hóa đơn */
int  removeVehicle(Vehicle vehicles[], int *count, const PriceConfig prices[]);

/* Hiển thị toàn bộ xe đang đỗ */
void listParkedVehicles(const Vehicle vehicles[], int count);

/* Tìm xe theo biển số (trả về index, -1 nếu không tìm thấy) */
int  findVehicleByPlate(const Vehicle vehicles[], int count, const char *plate);

/* Đếm số xe đang đỗ */
int  countParkedVehicles(const Vehicle vehicles[], int count);

#endif
```

#### Chi tiết hàm `addVehicle`

```
Bước 1: Kiểm tra bãi đầy (countParked >= MAX_VEHICLES)
Bước 2: Nhập biển số — validatePlate() để kiểm tra định dạng
Bước 3: Kiểm tra biển số không trùng với xe ĐANG ĐỖ
Bước 4: Nhập loại xe (0/1/2)
Bước 5: Ghi entryTime = time(NULL), status = STATUS_PARKED, fee = 0, exitTime = 0
Bước 6: Lưu vào mảng, tăng count
Bước 7: saveVehicles() — ghi file ngay lập tức
Bước 8: Thông báo thành công
```

#### Chi tiết hàm `removeVehicle`

```
Bước 1: Nhập biển số cần cho ra
Bước 2: findVehicleByPlate() — tìm trong xe đang đỗ
Bước 3: Nếu không tìm thấy → thông báo lỗi, return
Bước 4: exitTime = time(NULL)
Bước 5: fee = calculateFee(vehicle, prices[vehicle.vehicleType])
Bước 6: status = STATUS_EXITED
Bước 7: printInvoice(vehicle) — in hóa đơn
Bước 8: saveVehicles() — ghi file
```

#### Chi tiết hàm `listParkedVehicles`

```
In bảng: STT | Biển số | Loại xe | Giờ vào | Thời gian đỗ (tính đến hiện tại)
Chỉ hiển thị các xe có status = STATUS_PARKED
Cuối bảng: tổng số xe đang đỗ / sức chứa tối đa
```

#### Hóa đơn mẫu khi xe ra

```
╔══════════════════════════════════╗
║          HÓA ĐƠN ĐỖ XE           ║
╠══════════════════════════════════╣
║ Biển số    : 51A-12345           ║
║ Loại xe    : Ô tô                ║
║ Giờ vào    : 08:30 27/04/2026    ║
║ Giờ ra     : 11:45 27/04/2026    ║
║ Thời gian  : 3 giờ 15 phút       ║
║ Đơn giá    : 5,000 VND/giờ       ║
╠══════════════════════════════════╣
║ TỔNG TIỀN  : 20,000 VND          ║
╚══════════════════════════════════╝
```

---

### 4.3 `billing.h` / `billing.c`

**Vai trò:** Tính tiền đỗ xe theo loại và thời gian.

#### Header (`billing.h`)

```c
#ifndef BILLING_H
#define BILLING_H

#include "types.h"

/* Tính phí đỗ xe */
double calculateFee(const Vehicle *vehicle, const PriceConfig *price);

/* Tính số giờ đỗ (làm tròn lên) */
double calculateHours(time_t entryTime, time_t exitTime);

/* In hóa đơn ra màn hình */
void   printInvoice(const Vehicle *vehicle, const PriceConfig *price);

/* Khởi tạo bảng giá mặc định */
void   initDefaultPrices(PriceConfig prices[]);

#endif
```

#### Logic tính tiền (`calculateFee`)

```c
double calculateFee(const Vehicle *vehicle, const PriceConfig *price) {
    double hours = calculateHours(vehicle->entryTime, vehicle->exitTime);
    double fee   = hours * price->pricePerHour;

    /* Áp dụng phí tối thiểu */
    if (fee < price->minimumFee) {
        fee = price->minimumFee;
    }
    return fee;
}

double calculateHours(time_t entryTime, time_t exitTime) {
    double seconds = difftime(exitTime, entryTime);
    double hours   = seconds / 3600.0;

    /* Làm tròn lên đến 0.5 giờ (nếu muốn đơn giản, làm tròn lên 1 giờ) */
    /* Ví dụ đơn giản: làm tròn lên 1 giờ */
    int fullHours = (int)hours;
    if (hours - fullHours > 0) fullHours++;
    return (double)fullHours;
}
```

#### Bảng giá mặc định

| Loại xe | Tên | Giá/giờ | Phí tối thiểu |
|---------|-----|---------|---------------|
| 0 | Xe máy | 2,000 VND | 2,000 VND |
| 1 | Ô tô | 5,000 VND | 5,000 VND |
| 2 | Xe tải | 10,000 VND | 10,000 VND |

```c
void initDefaultPrices(PriceConfig prices[]) {
    prices[0] = (PriceConfig){TYPE_MOTORBIKE, "Xe may",  2000.0, 2000.0};
    prices[1] = (PriceConfig){TYPE_CAR,       "O to",    5000.0, 5000.0};
    prices[2] = (PriceConfig){TYPE_TRUCK,     "Xe tai", 10000.0, 10000.0};
}
```

---

### 4.4 `fileio.h` / `fileio.c`

**Vai trò:** Đọc và ghi dữ liệu vào file. **Ghi ngay sau mỗi thao tác** để không mất dữ liệu.

#### Header (`fileio.h`)

```c
#ifndef FILEIO_H
#define FILEIO_H

#include "types.h"

/* Đọc/ghi dữ liệu xe */
int  loadVehicles(Vehicle vehicles[], int *count);
int  saveVehicles(const Vehicle vehicles[], int count);

/* Đọc/ghi bảng giá */
int  loadPrices(PriceConfig prices[]);
int  savePrices(const PriceConfig prices[]);

/* Đọc/ghi tài khoản người dùng */
int  loadUsers(User users[], int *count);
int  saveUsers(const User users[], int count);

#endif
```

#### Định dạng file `vehicles.dat` (xem Phần 7)

#### Gợi ý cách ghi an toàn

```c
int saveVehicles(const Vehicle vehicles[], int count) {
    FILE *fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        perror("Khong the mo file de ghi");
        return 0; /* Thất bại */
    }

    fprintf(fp, "%d\n", count);
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s|%d|%lld|%lld|%.2f|%d\n",
            vehicles[i].licensePlate,
            vehicles[i].vehicleType,
            (long long)vehicles[i].entryTime,
            (long long)vehicles[i].exitTime,
            vehicles[i].fee,
            vehicles[i].status);
    }

    fclose(fp);
    return 1; /* Thành công */
}
```

---

### 4.5 `report.h` / `report.c`

**Vai trò:** Thống kê doanh thu, xuất báo cáo ra file `.txt`.

#### Header (`report.h`)

```c
#ifndef REPORT_H
#define REPORT_H

#include "types.h"

/* Hiển thị thống kê doanh thu trong ngày */
void viewDailyRevenue(const Vehicle vehicles[], int count);

/* Xuất báo cáo ngày ra file txt */
int  exportDailyReport(const Vehicle vehicles[], int count);

/* Thống kê theo loại xe */
void revenueByVehicleType(const Vehicle vehicles[], int count);

#endif
```

#### Logic thống kê doanh thu

```
- Lọc tất cả vehicle có status = STATUS_EXITED
- Lọc tiếp theo ngày hôm nay (so sánh exitTime với ngày hiện tại)
- Tính tổng fee
- Phân loại theo vehicleType
- Đếm số lượt mỗi loại xe
```

#### Mẫu báo cáo xuất ra file

```
====================================
    BÁO CÁO DOANH THU NGÀY
    27/04/2026
====================================
Xe máy  : 45 lượt  —   90,000 VND
Ô tô    : 12 lượt  —   60,000 VND
Xe tải  :  3 lượt  —   30,000 VND
------------------------------------
TỔNG    : 60 lượt  —  180,000 VND
====================================
Xuất lúc: 18:30:00 27/04/2026
```

#### Tên file báo cáo

```c
char filename[50];
time_t now = time(NULL);
struct tm *t = localtime(&now);
strftime(filename, sizeof(filename), "reports/report_%Y%m%d.txt", t);
```

---

### 4.6 `auth.h` / `auth.c`

**Vai trò:** Đăng nhập, phân quyền Admin / Nhân viên.

#### Header (`auth.h`)

```c
#ifndef AUTH_H
#define AUTH_H

#include "types.h"

/* Hiển thị màn hình đăng nhập, trả về 1 nếu thành công */
int  loginPrompt(Session *session);

/* Đăng xuất */
void logout(Session *session);

/* Kiểm tra quyền admin */
int  isAdmin(const Session *session);

/* Quản lý tài khoản (chỉ Admin) */
void manageUsers(Session *session);

#endif
```

#### Tài khoản mặc định

```
Admin:    username=admin,  password=admin123
Nhân viên: username=staff1, password=staff123
```

> ⚠️ **Lưu ý:** Password lưu dạng plain text là đủ cho mức độ bài này. Không cần hash.

#### Logic phân quyền

```
Chức năng "Chỉnh bảng giá" → chỉ ROLE_ADMIN mới thấy/dùng được
Chức năng "Quản lý tài khoản" → chỉ ROLE_ADMIN
Các chức năng còn lại → cả Admin và Staff đều dùng được
```

---

### 4.7 `utils.h` / `utils.c`

**Vai trò:** Các hàm tiện ích dùng chung trong toàn bộ project.

#### Header (`utils.h`)

```c
#ifndef UTILS_H
#define UTILS_H

#include "types.h"

/* Nhập số nguyên có giới hạn, tránh lỗi nhập chuỗi */
int    getIntInput(const char *prompt, int min, int max);

/* Nhập chuỗi an toàn (thay thế gets) */
void   getStringInput(const char *prompt, char *buffer, int maxLen);

/* Kiểm tra định dạng biển số xe (cơ bản) */
int    validatePlate(const char *plate);

/* Chuyển vehicleType sang tên hiển thị */
const char *getVehicleTypeName(int vehicleType);

/* Định dạng time_t thành chuỗi "HH:MM DD/MM/YYYY" */
void   formatTime(time_t t, char *buffer, int bufLen);

/* In đường phân cách */
void   printSeparator(char ch, int width);

/* Xóa màn hình */
void   clearScreen(void);

/* Chuyển chuỗi sang chữ hoa (so sánh biển số không phân biệt hoa/thường) */
void   toUpperCase(char *str);

/* Kiểm tra xe có đang đỗ không */
int    isParked(const Vehicle *v);

#endif
```

#### Hàm nhập quan trọng

```c
/* Nhập số nguyên an toàn, xử lý khi người dùng gõ chữ */
int getIntInput(const char *prompt, int min, int max) {
    int value;
    char buffer[50];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &value) == 1) {
                if (value >= min && value <= max) return value;
            }
        }
        printf("Vui long nhap so tu %d den %d.\n", min, max);
    }
}

/* Nhập chuỗi an toàn */
void getStringInput(const char *prompt, char *buffer, int maxLen) {
    printf("%s", prompt);
    if (fgets(buffer, maxLen, stdin) != NULL) {
        /* Xóa ký tự newline cuối */
        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}
```

#### Kiểm tra biển số

```c
/* Định dạng chấp nhận: XX[A-Z]-##### hoặc tương tự
   Kiểm tra cơ bản: không rỗng, độ dài hợp lý, có gạch nối */
int validatePlate(const char *plate) {
    if (plate == NULL) return 0;
    int len = strlen(plate);
    if (len < 5 || len >= MAX_PLATE_LEN) return 0;
    /* Kiểm tra có chứa ký tự '-' */
    int hasDash = 0;
    for (int i = 0; i < len; i++) {
        if (plate[i] == '-') { hasDash = 1; break; }
    }
    return hasDash;
}
```

---

## 5. Luồng xử lý (Flow)

### Luồng thêm xe vào

```
[Người dùng chọn "Thêm xe"]
        │
        ▼
[Bãi có đầy không?] ──── CÓ ──→ [Thông báo: Bãi đầy] → Kết thúc
        │ KHÔNG
        ▼
[Nhập biển số]
        │
        ▼
[validatePlate()] ──── LỖI ──→ [Thông báo: Biển số không hợp lệ] → Nhập lại
        │ HỢP LỆ
        ▼
[Biển số đã tồn tại trong xe ĐANG ĐỖ?] ── CÓ ──→ [Thông báo: Trùng biển số] → Kết thúc
        │ KHÔNG
        ▼
[Chọn loại xe: 0=Xe máy / 1=Ô tô / 2=Xe tải]
        │
        ▼
[Ghi entryTime = now, status = PARKED]
        │
        ▼
[Lưu vào mảng + saveVehicles() ghi file]
        │
        ▼
[Thông báo: Thêm xe thành công]
```

### Luồng cho xe ra

```
[Người dùng chọn "Cho xe ra"]
        │
        ▼
[Nhập biển số]
        │
        ▼
[findVehicleByPlate() trong xe ĐANG ĐỖ]
        │
  Không tìm thấy ──→ [Thông báo: Không có xe này] → Kết thúc
        │ Tìm thấy
        ▼
[exitTime = time(NULL)]
        │
        ▼
[calculateFee(vehicle, price)]
        │
        ▼
[Cập nhật: fee, exitTime, status = EXITED]
        │
        ▼
[printInvoice() — In hóa đơn]
        │
        ▼
[saveVehicles() — Ghi file]
```

---

## 6. Quy tắc tính tiền

```
Thời gian đỗ = exitTime - entryTime  (tính bằng giây)
Số giờ (làm tròn lên nguyên giờ) = ceil(giây / 3600)

Phí = số_giờ × pricePerHour

Nếu phí < minimumFee  →  phí = minimumFee

Ví dụ:
  Xe máy, đỗ 1 giờ 5 phút:
    → ceil(65/60) = 2 giờ
    → 2 × 2,000 = 4,000 VND

  Ô tô, đỗ 20 phút:
    → ceil(20/60) = 1 giờ
    → 1 × 5,000 = 5,000 VND ≥ minimumFee (5,000) ✓
```

---

## 7. Định dạng file lưu trữ

### `data/vehicles.dat`

```
Dòng 1: Tổng số bản ghi (kể cả xe đã ra)
Dòng 2+: mỗi dòng là một xe, phân cách bởi '|'

FORMAT:
<count>
<licensePlate>|<vehicleType>|<entryTime>|<exitTime>|<fee>|<status>

VÍ DỤ:
3
51A-12345|1|1745720400|1745731500|15000.00|1
59B-67890|0|1745724000|0|0.00|0
30C-11111|2|1745725200|1745739600|40000.00|1
```

- `entryTime`, `exitTime`: Unix timestamp (long long)
- `exitTime = 0` nếu xe đang đỗ
- `fee = 0.00` nếu chưa ra

### `data/prices.dat`

```
FORMAT:
<vehicleType>|<typeName>|<pricePerHour>|<minimumFee>

VÍ DỤ:
0|Xe may|2000.00|2000.00
1|O to|5000.00|5000.00
2|Xe tai|10000.00|10000.00
```

### `data/users.dat`

```
FORMAT:
<username>|<password>|<role>

VÍ DỤ:
admin|admin123|0
staff1|staff123|1
```

---

## 8. Phân công công việc

> Phân công mang tính tham khảo, nhóm có thể điều chỉnh tùy năng lực thực tế.

| Module | File | Thành viên gợi ý | Ghi chú |
|--------|------|-----------------|---------|
| Khung chương trình | `main.c`, `types.h` | Cả nhóm | Làm đầu tiên để mọi người build được |
| Thêm/xóa/tìm xe | `parking.h/.c` | Ngô Ngọc Gia Hân | Core logic |
| Tính tiền | `billing.h/.c` | Nguyễn Tố Uyên | Cần test kỹ |
| Đọc/ghi file | `fileio.h/.c` | Trần Thị Thúy Vy | Quan trọng, ảnh hưởng toàn bộ |
| Thống kê, báo cáo | `report.h/.c` | Nguyễn Thái Huy | Dựa trên dữ liệu đã có |
| Tiện ích & Auth | `utils.h/.c`, `auth.h/.c` | Cả nhóm | Làm sớm, mọi người dùng chung |

### Lộ trình thực hiện (gợi ý)

```
Tuần 1 (17/04 – 23/04):
  ✅ Tạo repo GitHub, thống nhất cấu trúc thư mục
  ✅ Viết types.h (struct) — cả nhóm review
  ✅ Viết utils.c (hàm nhập liệu an toàn)
  ✅ Viết fileio.c (đọc/ghi file cơ bản)
  ✅ Viết parking.c — addVehicle, listParkedVehicles

Tuần 2 (24/04 – 30/04):
  ✅ Hoàn thiện parking.c — removeVehicle, findVehicle
  ✅ Viết billing.c — calculateFee, printInvoice
  ✅ Tích hợp main.c, chạy thử end-to-end
  ✅ Test các trường hợp bất thường

Tuần 3 (01/05 – 07/05):
  ✅ Viết report.c — thống kê doanh thu
  ✅ Viết auth.c — phân quyền Admin/Staff
  ✅ Nâng cao: chỉnh bảng giá, xuất báo cáo txt
  ✅ Tùy chọn: thêm màu ANSI

Tuần 4 (08/05 – 17/05):
  ✅ Hoàn thiện, sửa bug
  ✅ Viết README.md cho repo
  ✅ Chuẩn bị demo, ôn vấn đáp
  ✅ Nộp Final Report
```

---

## 9. Quy ước Git & Commit

### Khởi tạo repo

```bash
git init
git remote add origin https://github.com/<username>/QuanLyBaiDoXe.git
git add .
git commit -m "feat: initial project structure"
git push -u origin main
```

### Quy ước tên branch

```
main          — nhánh chính, luôn build được
dev           — nhánh phát triển tổng hợp
feature/xxx   — tính năng mới (VD: feature/add-vehicle)
fix/xxx       — sửa bug (VD: fix/save-file-crash)
```

### Quy ước tên commit

Dùng format: `<type>: <mô tả ngắn gọn bằng tiếng Anh hoặc tiếng Việt>`

| Type | Dùng khi |
|------|---------|
| `feat` | Thêm tính năng mới |
| `fix` | Sửa lỗi |
| `refactor` | Tái cấu trúc code, không đổi chức năng |
| `docs` | Cập nhật tài liệu |
| `test` | Thêm hoặc sửa test |
| `style` | Format code, đặt lại tên biến |
| `chore` | Việc linh tinh (update Makefile, .gitignore) |

**Ví dụ commit tốt:**

```
feat: implement addVehicle with duplicate plate check
feat: calculate fee based on vehicle type and hours
fix: prevent crash when vehicles.dat does not exist
fix: handle newline when reading license plate input
refactor: extract printInvoice into billing.c
docs: add function comments in parking.h
feat(report): export daily revenue to txt file
feat(auth): add role-based menu visibility
```

### `.gitignore`

```
# Binary
parking
*.exe
*.o

# Data files (tuỳ chọn - có thể commit file mẫu)
# data/vehicles.dat
reports/

# Editor
.vscode/
*.swp
```

---

## 10. Checklist trước khi nộp

### Chức năng bắt buộc (30 điểm)
- [ ] Thêm xe vào bãi — kiểm tra biển số trùng, đầy bãi
- [ ] Cho xe ra — tính tiền đúng, in hóa đơn
- [ ] Xem danh sách xe đang đỗ — hiển thị đủ thông tin
- [ ] Tìm kiếm xe theo biển số — tìm cả xe đang đỗ lẫn lịch sử
- [ ] Thống kê doanh thu ngày — lọc đúng theo ngày hôm nay
- [ ] Lưu file — ghi ngay sau thao tác
- [ ] Đọc file — load đúng khi khởi động lại

### Tổ chức code (20 điểm)
- [ ] Chia file đúng — không nhét hết vào `main.c`
- [ ] Header guard (`#ifndef`) đầy đủ trong mọi `.h`
- [ ] Tên hàm/biến bằng tiếng Anh, camelCase hoặc snake_case nhất quán
- [ ] Không có global variable không cần thiết
- [ ] Mỗi hàm chỉ làm một việc (< 50 dòng là lý tưởng)
- [ ] Comment những đoạn phức tạp

### Git (15 điểm)
- [ ] Tất cả thành viên đều có commit
- [ ] Commit đều đặn suốt quá trình (không commit dồn 1 lần)
- [ ] Tên commit có ý nghĩa, theo quy ước

### Demo (20 điểm)
- [ ] Chạy được không crash trong 10 phút
- [ ] Xử lý đầu vào sai (chữ thay vì số, biển số rỗng, ...)
- [ ] Dữ liệu không mất khi tắt và mở lại chương trình

### Vấn đáp (15 điểm)
- [ ] Mỗi thành viên giải thích được phần mình làm
- [ ] Hiểu được tổng thể luồng dữ liệu
- [ ] Giải thích được lý do chọn cấu trúc dữ liệu

---

## 11. Gợi ý test case

### Test thêm xe
| Test | Input | Expected |
|------|-------|----------|
| Bình thường | 51A-12345, Ô tô | Thêm thành công |
| Biển số trùng | 51A-12345 (đã có) | Từ chối, thông báo lỗi |
| Biển số rỗng | "" | Từ chối, nhập lại |
| Biển số không có dấu '-' | 51A12345 | Từ chối |
| Loại xe sai | 5 | Yêu cầu nhập lại |
| Bãi đầy | 3636 xe đang đỗ | Từ chối, thông báo đầy |

### Test cho xe ra
| Test | Input | Expected |
|------|-------|----------|
| Bình thường | Biển số đang đỗ | In hóa đơn, tính tiền đúng |
| Không tìm thấy | Biển số không tồn tại | Thông báo không tìm thấy |
| Xe đã ra rồi | Biển số đã ra | Thông báo xe không ở trong bãi |

### Test tính tiền
| Loại xe | Thời gian đỗ | Tiền mong đợi |
|---------|-------------|--------------|
| Xe máy | 30 phút | 2,000 VND (minimum fee) |
| Xe máy | 1 giờ 5 phút | 4,000 VND (ceil → 2 giờ) |
| Ô tô | 3 giờ đúng | 15,000 VND |
| Xe tải | 2 giờ 59 phút | 30,000 VND (ceil → 3 giờ) |

### Test lưu/đọc file
| Test | Thao tác | Expected |
|------|---------|----------|
| Persistence | Thêm xe → tắt → mở lại | Xe vẫn còn trong danh sách |
| Không có file | Xóa vehicles.dat → mở lại | Không crash, bắt đầu với 0 xe |
| File hỏng | Sửa vehicles.dat sai format | Xử lý gracefully, không crash |

---

## 12. ANSI Color (Tùy chọn)

Nếu muốn thêm màu sắc cho terminal:

```c
/* Định nghĩa trong utils.h */
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_RESET   "\033[0m"

/* Sử dụng */
printf(COLOR_GREEN "Them xe thanh cong!\n" COLOR_RESET);
printf(COLOR_RED "Bien so da ton tai!\n" COLOR_RESET);
printf(COLOR_BOLD COLOR_CYAN "=== DANH SACH XE ===\n" COLOR_RESET);
```

> ⚠️ ANSI color hoạt động trên Linux/macOS. Trên Windows cần bật bằng `SetConsoleMode()` hoặc dùng Windows Terminal.

---

## Liên hệ & Tài nguyên

- **Mentor:** Nguyễn Hoàng Minh
- **Tài liệu tham khảo:**
  - [cppreference.com — time.h](https://en.cppreference.com/w/c/chrono)
  - [cppreference.com — stdio.h](https://en.cppreference.com/w/c/io)
  - [Pro Git Book (tiếng Việt)](https://git-scm.com/book/vi/v2)

---

*Tài liệu này được tạo cho nhóm 4Loops — CLB F-Code ĐH FPT TP.HCM*
*Chúc nhóm code vui và đạt điểm cao! 🚀*
