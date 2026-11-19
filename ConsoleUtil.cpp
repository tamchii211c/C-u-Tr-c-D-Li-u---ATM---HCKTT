#include "ConsoleUtil.h"

#define NOMINMAX
#include <windows.h>
#include <conio.h>

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cctype>

namespace ConsoleUtil {
    void khoiTaoConsole() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }
        SetConsoleOutputCP(65001); 
    }

    void xoaManHinh() {
        std::cout << "\033[2J\033[H";
        std::cout.flush();
    }

    void datMau(Mau mauChu, Mau mauNen) {
        int fgCode = 37;
        switch (mauChu) {
            case Mau::DEN:             fgCode = 30; break;
            case Mau::DO:              fgCode = 31; break;
            case Mau::XANH_LA:         fgCode = 32; break;
            case Mau::VANG:            fgCode = 33; break;
            case Mau::XANH_DUONG:      fgCode = 34; break;
            case Mau::TIM:             fgCode = 35; break;
            case Mau::XANH_LAM:        fgCode = 36; break;
            case Mau::TRANG:           fgCode = 37; break;
            case Mau::DO_SANG:         fgCode = 91; break;
            case Mau::XANH_LA_SANG:    fgCode = 92; break;
            case Mau::VANG_SANG:       fgCode = 93; break;
            case Mau::XANH_DUONG_SANG: fgCode = 94; break;
            case Mau::TIM_SANG:        fgCode = 95; break;
            case Mau::XANH_LAM_SANG:   fgCode = 96; break;
            case Mau::TRANG_SANG:      fgCode = 97; break;
            default:                   fgCode = 39; break;
        }

        int bgCode = 49;
        switch (mauNen) {
            case Mau::DEN:             bgCode = 40; break;
            case Mau::DO:              bgCode = 41; break;
            case Mau::XANH_LA:         bgCode = 42; break;
            case Mau::VANG:            bgCode = 43; break;
            case Mau::XANH_DUONG:      bgCode = 44; break;
            case Mau::TIM:             bgCode = 45; break;
            case Mau::XANH_LAM:        bgCode = 46; break;
            case Mau::TRANG:           bgCode = 47; break;
            default:                   bgCode = 49; break;
        }

        std::cout << "\033[" << fgCode << ";" << bgCode << "m";
    }

    void datLaiMau() {
        std::cout << "\033[0m";
    }

    int layChieuRongConsole() {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int columns = 80;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        }
        return columns > 0 ? columns : 80;
    }

    void inMau(const std::string& noiDung, Mau mau, bool xuongDong) {
        datMau(mau);
        std::cout << noiDung;
        datLaiMau();
        if (xuongDong) {
            std::cout << "\n";
        }
    }

    void inCanGiua(const std::string& noiDung, Mau mau) {
        int width = layChieuRongConsole();
        int pad = (width - static_cast<int>(noiDung.length())) / 2;
        if (pad < 0) pad = 0;

        std::cout << std::string(pad, ' ');
        inMau(noiDung, mau, true);
    }

    void inCanPhai(const std::string& noiDung, Mau mau) {
        int width = layChieuRongConsole();
        int pad = width - static_cast<int>(noiDung.length());
        if (pad < 0) pad = 0;

        std::cout << std::string(pad, ' ');
        inMau(noiDung, mau, true);
    }

    void veTieuDe(const std::string& tieuDe, Mau mau) {
        int width = layChieuRongConsole();
        int boxWidth = std::min(width - 4, 70);
        if (boxWidth < 30) boxWidth = 30;

        int padLeft = (width - boxWidth) / 2;
        if (padLeft < 0) padLeft = 0;
        std::string indent(padLeft, ' ');

        datMau(mau);
        std::cout << indent << "+" << std::string(boxWidth - 2, '=') << "+\n";

        std::string titleStr = tieuDe;
        if (static_cast<int>(titleStr.length()) > boxWidth - 4) {
            titleStr = titleStr.substr(0, boxWidth - 7) + "...";
        }
        int textPadLeft = (boxWidth - 2 - static_cast<int>(titleStr.length())) / 2;
        int textPadRight = boxWidth - 2 - static_cast<int>(titleStr.length()) - textPadLeft;

        std::cout << indent << "|";
        datLaiMau();
        std::cout << std::string(textPadLeft, ' ');
        datMau(Mau::TRANG_SANG);
        std::cout << titleStr;
        datMau(mau);
        std::cout << std::string(textPadRight, ' ') << "|\n";

        std::cout << indent << "+" << std::string(boxWidth - 2, '=') << "+\n";
        datLaiMau();
    }

    void veKhung(const std::vector<std::string>& cacDong, const std::string& tieuDe, Mau mauVien) {
        int maxLen = tieuDe.length() > 0 ? static_cast<int>(tieuDe.length()) + 4 : 0;
        for (const auto& line : cacDong) {
            if (static_cast<int>(line.length()) > maxLen) {
                maxLen = static_cast<int>(line.length());
            }
        }

        int boxWidth = maxLen + 6;
        int consoleWidth = layChieuRongConsole();
        if (boxWidth > consoleWidth - 2) boxWidth = consoleWidth - 2;

        int padLeft = (consoleWidth - boxWidth) / 2;
        if (padLeft < 0) padLeft = 0;
        std::string indent(padLeft, ' ');

        datMau(mauVien);
        std::cout << indent << "+";
        if (!tieuDe.empty()) {
            std::cout << "[ " << tieuDe << " ]" << std::string(boxWidth - 6 - tieuDe.length(), '-');
        } else {
            std::cout << std::string(boxWidth - 2, '-');
        }
        std::cout << "+\n";

        for (const auto& line : cacDong) {
            std::string text = line;
            if (static_cast<int>(text.length()) > boxWidth - 4) {
                text = text.substr(0, boxWidth - 7) + "...";
            }
            int rightSpace = boxWidth - 4 - static_cast<int>(text.length());

            datMau(mauVien);
            std::cout << indent << "| ";

            size_t pos = text.find(':');
            if (pos != std::string::npos) {
                std::string strNhan = text.substr(0, pos + 1); // Phần nhãn (gồm dấu ':') -> Xanh lá sáng
                std::string strGiaTri = text.substr(pos + 1);   // Phần giá trị -> Trắng sáng
                datMau(Mau::XANH_LA_SANG);
                std::cout << strNhan;
                datMau(Mau::TRANG_SANG);
                std::cout << strGiaTri;
            } else {
                datLaiMau();
                std::cout << text;
            }

            datMau(mauVien);
            std::cout << std::string(rightSpace, ' ') << " |\n";
        }

        std::cout << indent << "+" << std::string(boxWidth - 2, '-') << "+\n";
        datLaiMau();
    }

    void tamDung(const std::string& loiNhan) {
        std::cout << "\n";
        inCanGiua(loiNhan, Mau::VANG_SANG);
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::string nhapMatKhau(const std::string& loiNhan, size_t doDaiToiDa, bool chiChuSo) {
        std::cout << loiNhan;
        std::string pass = "";
        char ch;
        while ((ch = _getch()) != 13) {
            if (ch == 8) { 
                if (!pass.empty()) {
                    pass.pop_back();
                    std::cout << "\b \b";
                }
            } else if (ch >= 32 && ch <= 126) {
                if (chiChuSo && !std::isdigit(static_cast<unsigned char>(ch))) {
                    continue;
                }
                if (doDaiToiDa > 0 && pass.length() >= doDaiToiDa) {
                    continue;
                }
                pass.push_back(ch);
                std::cout << '*';
            }
        }
        std::cout << "\n";
    
        return pass;
    }

    void hienThongBao(const std::string& noiDung, LoaiThongBao loai) {
        std::cout << "\n";
        switch (loai) {
            case LoaiThongBao::THANH_CONG:
                inCanGiua("[ THANH CONG ] " + noiDung, Mau::XANH_LA_SANG);
                break;
            case LoaiThongBao::LOI:
                inCanGiua("[ LOI ] " + noiDung, Mau::DO_SANG);
                break;
            case LoaiThongBao::CANH_BAO:
                inCanGiua("[ CANH BAO ] " + noiDung, Mau::VANG_SANG);
                break;
            case LoaiThongBao::THONG_TIN:
                inCanGiua("[ THONG TIN ] " + noiDung, Mau::XANH_LAM_SANG);
                break;
        }
        std::cout << "\n";
    }

    void inLoi(const std::string& noiDung) {
        hienThongBao(noiDung, LoaiThongBao::LOI);
    }

    void inThanhCong(const std::string& noiDung) {
        hienThongBao(noiDung, LoaiThongBao::THANH_CONG);
    }

    void inThongBao(const std::string& noiDung) {
        hienThongBao(noiDung, LoaiThongBao::THONG_TIN);
    }

    void inLuaChon(const std::string& noiDung) {
        inMau(noiDung, Mau::VANG_SANG, true);
    }

    long long nhapSoNguyen(const std::string& loiNhan) {
        if (!loiNhan.empty()) {
            std::cout << loiNhan;
        }
        long long val;
        while (!(std::cin >> val)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            inLoi("Gia tri nhap vao phai la so nguyen!");
            if (!loiNhan.empty()) {
                std::cout << loiNhan;
            }
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return val;
    }

    std::string nhapDong(const std::string& loiNhan) {
        if (!loiNhan.empty()) {
            std::cout << loiNhan;
        }
        std::string str;
        std::getline(std::cin, str);
        return str;
    }

    bool xacNhan(const std::string& loiNhan) {
        std::cout << loiNhan << " (Y/N): ";
        char ch;
        std::cin >> ch;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return (ch == 'y' || ch == 'Y');
    }

} // namespace ConsoleUtil
