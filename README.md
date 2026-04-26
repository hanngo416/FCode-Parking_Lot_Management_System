# QUẢN LÝ BÃI ĐỖ XE - Nhóm 4Loops

Đây là kho lưu trữ mã nguồn cho đồ án **Quản lý bãi đỗ xe** (viết bằng ngôn ngữ C).
Dự án được dựng base sẵn để nhóm **4Loops** có thể làm việc chung thông qua Git mà không bị conflict.

## 1. Cấu trúc thư mục

```text
📦 Parking_Lot_Management_System
 ┣ 📂 data        # Nơi chứa các file .dat hoặc .txt lưu trữ dữ liệu
 ┣ 📂 include     # Nơi chứa các file Header (.h) khai báo Struct và nguyên mẫu hàm
 ┃ ┗ 📜 types.h   # (Đã có) Chứa định nghĩa struct Vehicle, PriceConfig, enum VehicleType
 ┣ 📂 src         # Nơi chứa mã nguồn logic (.c) của từng chức năng
 ┣ 📜 .gitignore  # Cấu hình bỏ qua các file rác khi đẩy lên Git (không được xóa)
 ┣ 📜 main.c      # Vòng lặp Menu chính của chương trình
 ┗ 📜 Parking_Lot_Management_System.dev  # File project để mở bằng Dev-C++
```

## 2. Cách chạy dự án trên máy cá nhân

1. Clone toàn bộ code từ nhánh dev về máy
2. Mở thư mục code, click đúp vào file `Parking_Lot_Management_System.dev` để phần mềm **Dev-C++** tự động mở dự án.
3. Trong Dev-C++, nhấn **F11 (Compile & Run)** để chạy chương trình. (Lưu ý: Chạy bằng file .dev, không compile riêng lẻ file `main.c`).

## 3. Quy tắc làm việc nhóm (Coding Convention)

Để dự án trơn tru và đạt điểm tối đa phần "Tổ chức code và Git", cả nhóm cần tuân thủ:

- **Ngôn ngữ code**: Đặt tên biến, tên hàm 100% bằng **Tiếng Anh** (vd: `vehicleCount`, `addVehicle()`). Không dùng tiếng Việt để đặt tên biến.
- **In ra màn hình**: Các câu `printf` hiển thị ra giao diện terminal dùng Tiếng Anh
- **Không code dồn vào `main.c`**: File `main.c` chỉ dùng để hiển thị Menu. Chức năng nào thì tạo file `.c` và `.h` riêng cho chức năng đó ở thư mục tương ứng.
- **Quy tắc Git**:
  - **Không ai được commit thẳng vào branch `main`**.
  - Mỗi người sẽ tạo 1 branch riêng theo tên ( ví dụ: GH). Làm xong tiến hành Push nên báo với leader để review code trước khi gộp (Merge) vào nhánh `dev`.

