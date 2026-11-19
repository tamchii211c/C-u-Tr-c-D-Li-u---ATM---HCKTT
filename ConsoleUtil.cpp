#include "ConsoleUtil.h"

#define NOMINMAX
#include <windows.h>
#include <conio.h>

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

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
        cout << "\033[2J\033[H";
        cout.flush();
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

        cout << "\033[" << fgCode << ";" << bgCode << "m";
    }

    void datLaiMau() {
        cout << "\033[0m";
    }

    int layChieuRongConsole() {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int columns = 80;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        }
        return columns > 0 ? columns : 80;
    }

    void inMau(const string& noiDung, Mau mau, bool xuongDong) {
        datMau(mau);
        cout << noiDung;
        datLaiMau();
        if (xuongDong) {
            cout << "\n";
        }
    }

    void inCanGiua(const string& noiDung, Mau mau) {
        int width = layChieuRongConsole();
        int pad = (width - static_cast<int>(noiDung.length())) / 2;
        if (pad < 0) pad = 0;

        cout << string(pad, ' ');
        inMau(noiDung, mau, true);
    }

    void inCanPhai(const string& noiDung, Mau mau) {
        int width = layChieuRongConsole();
        int pad = width - static_cast<int>(noiDung.length());
        if (pad < 0) pad = 0;

        cout << string(pad, ' ');
        inMau(noiDung, mau, true);
    }

    void veTieuDe(const string& tieuDe, Mau mau) {
        int width = layChieuRongConsole();
        int boxWidth = min(width - 4, 70);
        if (boxWidth < 30) boxWidth = 30;

        int padLeft = (width - boxWidth) / 2;
        if (padLeft < 0) padLeft = 0;
        string indent(padLeft, ' ');

        datMau(mau);
        cout << indent << "+" << string(boxWidth - 2, '=') << "+\n";

        string titleStr = tieuDe;
        if (static_cast<int>(titleStr.length()) > boxWidth - 4) {
            titleStr = titleStr.substr(0, boxWidth - 7) + "...";
        }
        int textPadLeft = (boxWidth - 2 - static_cast<int>(titleStr.length())) / 2;
        int textPadRight = boxWidth - 2 - static_cast<int>(titleStr.length()) - textPadLeft;

        cout << indent << "|";
        datLaiMau();
        cout << string(textPadLeft, ' ');
        datMau(Mau::TRANG_SANG);
        cout << titleStr;
        datMau(mau);
        cout << string(textPadRight, ' ') << "|\n";

        cout << indent << "+" << string(boxWidth - 2, '=') << "+\n";
        datLaiMau();
    }

    void veKhung(const vector<string>& cacDong, const string& tieuDe, Mau mauVien) {
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
        string indent(padLeft, ' ');

        datMau(mauVien);
        cout << indent << "+";
        if (!tieuDe.empty()) {
            cout << "[ " << tieuDe << " ]" << string(boxWidth - 6 - tieuDe.length(), '-');
        } else {
            cout << string(boxWidth - 2, '-');
        }
        cout << "+\n";

        for (const auto& line : cacDong) {
            string text = line;
            if (static_cast<int>(text.length()) > boxWidth - 4) {
                text = text.substr(0, boxWidth - 7) + "...";
            }
            int rightSpace = boxWidth - 4 - static_cast<int>(text.length());

            datMau(mauVien);
            cout << indent << "| ";

            size_t pos = text.find(':');
            if (pos != string::npos) {
                string strNhan = text.substr(0, pos + 1); // Phần nhãn (gồm dấu ':') -> Xanh lá sáng
                string strGiaTri = text.substr(pos + 1);   // Phần giá trị -> Trắng sáng
                datMau(Mau::XANH_LA_SANG);
                cout << strNhan;
                datMau(Mau::TRANG_SANG);
                cout << strGiaTri;
            } else {
                datLaiMau();
                cout << text;
            }

            datMau(mauVien);
            cout << string(rightSpace, ' ') << " |\n";
        }

        cout << indent << "+" << string(boxWidth - 2, '-') << "+\n";
        datLaiMau();
    }

    void tamDung(const string& loiNhan) {
        cout << "\n";
        inCanGiua(loiNhan, Mau::VANG_SANG);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string nhapMatKhau(const string& loiNhan, size_t doDaiToiDa, bool chiChuSo) {
        cout << loiNhan;
        string pass = "";
        char ch;
        while ((ch = _getch()) != 13) {
            if (ch == 8) { 
                if (!pass.empty()) {
                    pass.pop_back();
                    cout << "\b \b";
                }
            } else if (ch >= 32 && ch <= 126) {
                if (chiChuSo && !isdigit(static_cast<unsigned char>(ch))) {
                    continue;
                }
                if (doDaiToiDa > 0 && pass.length() >= doDaiToiDa) {
                    continue;
                }
                pass.push_back(ch);
                cout << '*';
            }
        }
        cout << "\n";
    
        return pass;
    }

    void hienThongBao(const string& noiDung, LoaiThongBao loai) {
        cout << "\n";
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
        cout << "\n";
    }

    void inLoi(const string& noiDung) {
        hienThongBao(noiDung, LoaiThongBao::LOI);
    }

    void inThanhCong(const string& noiDung) {
        hienThongBao(noiDung, LoaiThongBao::THANH_CONG);
    }

    void inThongBao(const string& noiDung) {
        hienThongBao(noiDung, LoaiThongBao::THONG_TIN);
    }

    void inLuaChon(const string& noiDung) {
        inMau(noiDung, Mau::VANG_SANG, true);
    }

    long long nhapSoNguyen(const string& loiNhan) {
        if (!loiNhan.empty()) {
            cout << loiNhan;
        }
        long long val;
        while (!(cin >> val)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            inLoi("Gia tri nhap vao phai la so nguyen!");
            if (!loiNhan.empty()) {
                cout << loiNhan;
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return val;
    }

    string nhapDong(const string& loiNhan) {
        if (!loiNhan.empty()) {
            cout << loiNhan;
        }
        string str;
        getline(cin, str);
        return str;
    }

    bool xacNhan(const string& loiNhan) {
        cout << loiNhan << " (Y/N): ";
        char ch;
        cin >> ch;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return (ch == 'y' || ch == 'Y');
    }

} // namespace ConsoleUtil
