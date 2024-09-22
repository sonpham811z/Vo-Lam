Đề thi Võ Lâm Truyền Kỳ của trường Đại học Công Nghệ Thông Tin - Đại Học Quốc Gia TP-HCM

[image](https://github.com/user-attachments/assets/0b38f963-6be2-4181-bdc5-788b7f59c381)

# Hướng dẫn Setup SDL2 trên Visual Studio 32-bit

## Mục lục
1. [Yêu cầu hệ thống](#yêu-cầu-hệ-thống)
2. [Tải SDL2](#tải-sdl2)
3. [Cài đặt SDL2 trong Visual Studio](#cài-đặt-sdl2-trong-visual-studio)
    - [Thêm đường dẫn Include và Lib](#thêm-đường-dẫn-include-và-lib)
    - [Thêm tệp `.lib` vào dự án](#thêm-tệp-lib-vào-dự-án)
    - [Sao chép tệp DLL](#sao-chép-tệp-dll)
4. [Viết chương trình SDL cơ bản](#viết-chương-trình-sdl-cơ-bản)
5. [Chạy chương trình](#chạy-chương-trình)

## Yêu cầu hệ thống
- Visual Studio (bản 2017 trở lên) - 32-bit
- SDL2 (Simple DirectMedia Layer) - phiên bản 32-bit
- Windows (32-bit)

## Tải SDL2
1. Truy cập trang chính thức của SDL2: [https://libsdl.org](https://libsdl.org)
2. Tải phiên bản SDL2 mới nhất cho Windows (32-bit).
   - Chọn `SDL2-devel-2.x.x-VC.zip` cho Visual Studio.
3. Giải nén tệp đã tải về vào một thư mục, ví dụ: `C:\SDL2`.

## Cài đặt SDL2 trong Visual Studio

### Thêm đường dẫn Include và Lib
1. Mở Visual Studio và tạo một dự án mới:
   - Chọn **File > New > Project**, sau đó chọn **Empty Project**.
2. Nhấp chuột phải vào dự án của bạn trong Solution Explorer và chọn **Properties**.
3. Đi đến **Configuration Properties > VC++ Directories**.
4. Tại **Include Directories**, thêm đường dẫn đến thư mục `include` của SDL2:
   - Ví dụ: `C:\SDL2\include`.
5. Tại **Library Directories**, thêm đường dẫn đến thư mục `lib\x86` của SDL2:
   - Ví dụ: `C:\SDL2\lib\x86`.

### Thêm tệp `.lib` vào dự án
1. Đi đến **Configuration Properties > Linker > Input**.
2. Tại **Additional Dependencies**, thêm các thư viện sau:
. Nhấn **OK** để lưu các thiết lập.
### Sao chép tệp DLL
1. Trong thư mục `C:\SDL2\lib\x86`, tìm tệp `SDL2.dll`.
2. Sao chép tệp `SDL2.dll` vào thư mục nơi chương trình của bạn sẽ được biên dịch (thường là thư mục `Debug` hoặc `Release`).

[video setup](https://www.youtube.com/watch?v=tUQ9lkTWF1Y&list=PLR7NDiX0QsfTIEQUeYCfc2MyCquX0ig9V&index=2)
