#ifndef CONSOLE_UTIL_H
#define CONSOLE_UTIL_H

#include <iostream>
#include <string>
#include <vector>

namespace ConsoleUtil {

    // Danh sach cac mau sac console co ban
    enum class Mau {
        MAC_DINH = 0,
        DEN,
        DO,
        XANH_LA,
        VANG,
        XANH_DUONG,
        TIM,
        XANH_LAM,
        TRANG,
        DO_SANG,
        XANH_LA_SANG,
        VANG_SANG,
        XANH_DUONG_SANG,
        TIM_SANG,
        XANH_LAM_SANG,
        TRANG_SANG
    };

    // Loai thong bao (Thanh cong, Loi, Canh bao, Thong tin)
    enum class LoaiThongBao {
        THANH_CONG,
        LOI,
        CANH_BAO,
        THONG_TIN
    };

    // --- Khoi tao & Cau hinh ---
    // Khoi tao console (kich hoat ANSI mode tren Windows 10+, UTF-8)
    void khoiTaoConsole();

    // --- Thao tac voi Man hinh ---
    void xoaManHinh();

    // --- Thiet lap Mau sac ---
    void datMau(Mau mauChu, Mau mauNen = Mau::MAC_DINH);
    void datLaiMau();

    // --- Dinh dang Van ban & Can le ---
    int layChieuRongConsole();
    void inCanGiua(const std::string& noiDung, Mau mau = Mau::MAC_DINH);
    void inCanPhai(const std::string& noiDung, Mau mau = Mau::MAC_DINH);
    void inMau(const std::string& noiDung, Mau mau, bool xuongDong = true);

    // --- Khung & Giao dien (UI Elements) ---
    // Ve Tieu de Banner dang hop dep mat
    void veTieuDe(const std::string& tieuDe, Mau mau = Mau::XANH_LAM_SANG);

    // Ve khung hop chua noi dung
    void veKhung(const std::vector<std::string>& cacDong, const std::string& tieuDe = "", Mau mauVien = Mau::XANH_LAM);

    // --- Tien ich Nhap xuat (Input Utilities) ---
    // Dung man hinh va doi nguoi dung nhan Enter
    void tamDung(const std::string& loiNhan = "Nhan Enter de tiep tuc...");

    // Nhap mat khau / ma PIN an ki tu bang '*'
    std::string nhapMatKhau(const std::string& loiNhan = "Nhap ma PIN: ", size_t doDaiToiDa = 0, bool chiChuSo = false);

    // Hien thi thong bao dinh dang dep
    void hienThongBao(const std::string& noiDung, LoaiThongBao loai);
    void inLoi(const std::string& noiDung);
    void inThanhCong(const std::string& noiDung);
    void inThongBao(const std::string& noiDung);
    void inLuaChon(const std::string& noiDung);

    // Nhap du lieu an toan
    long long nhapSoNguyen(const std::string& loiNhan = "");
    std::string nhapDong(const std::string& loiNhan = "");
    bool xacNhan(const std::string& loiNhan = "");

} // namespace ConsoleUtil

#endif // CONSOLE_UTIL_H
