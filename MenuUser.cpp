#include "MenuUser.h"
#include "ConsoleUtil.h"
#include "TheTu.h"
#include "QuanLyTaiKhoan.h"
#include "TaiKhoan.h"
#include "GiaoDich.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

namespace {
    const int USER_XEM_THONG_TIN = 1;
    const int USER_NAP_TIEN = 2;
    const int USER_RUT_TIEN = 3;
    const int USER_CHUYEN_TIEN = 4;
    const int USER_XEM_GIAO_DICH = 5;
    const int USER_DOI_MA_PIN = 6;
    const int USER_THOAT = 7;

    // Do dai ma PIN hop le
    const size_t PIN_LENGTH = 6;

    // So du toi thieu de thuc hien giao dich (rut tien, chuyen tien)
    const long long SO_DU_TOI_THIEU = 50000;
    const long long TIEN_CHUYEN_TOI_THIEU = 50000;

    // Doc lua chon tu nguoi dung trong khoang [iMin, iMax]
    int docLuaChon(int iMin, int iMax) {
        while (true) {
            const long long llLuaChon = ConsoleUtil::nhapSoNguyen(" Lua chon: ");
            if (llLuaChon >= iMin && llLuaChon <= iMax) {
                return static_cast<int>(llLuaChon);
            }
            ConsoleUtil::inLoi("Lua chon khong hop le, vui long nhap lai!");
        }
    }
}

// Constructor
MenuUser::MenuUser(QuanLyTheTu& quanLyTheTu, const string& strID)
    : _quanLyTheTu(quanLyTheTu), _strID(strID) {
}

// Vong lap chay Menu User
void MenuUser::chay() {
    bool bTiepTuc = true;

    while (bTiepTuc) {
        ConsoleUtil::xoaManHinh();
        this->hienThiMenu();

        switch (docLuaChon(USER_XEM_THONG_TIN, USER_THOAT)) {
        case USER_XEM_THONG_TIN:
            this->xemThongTin();
            break;

        case USER_NAP_TIEN:
            this->napTien();
            break;

        case USER_RUT_TIEN:
            this->rutTien();
            break;

        case USER_CHUYEN_TIEN:
            this->chuyenTien();
            break;

        case USER_XEM_GIAO_DICH:
            this->xemGiaoDich();
            break;

        case USER_DOI_MA_PIN:
            this->doiMaPin();
            break;

        case USER_THOAT:
            bTiepTuc = false;
            break;
        }
    }
}

// Hien thi Menu User
void MenuUser::hienThiMenu() const {
    ConsoleUtil::veTieuDe("MENU NGUOI DUNG ATM", ConsoleUtil::Mau::XANH_LAM_SANG);
    ConsoleUtil::inLuaChon(" 1. Xem thong tin tai khoan");
    ConsoleUtil::inLuaChon(" 2. Nap tien");
    ConsoleUtil::inLuaChon(" 3. Rut tien");
    ConsoleUtil::inLuaChon(" 4. Chuyen tien");
    ConsoleUtil::inLuaChon(" 5. Xem lich su giao dich");
    ConsoleUtil::inLuaChon(" 6. Doi ma PIN");
    ConsoleUtil::inLuaChon(" 7. Thoat / Dang xuat");
}

// Chuc nang 1: Xem thong tin tai khoan
void MenuUser::xemThongTin() {
    ConsoleUtil::xoaManHinh();
    ConsoleUtil::veTieuDe("THONG TIN TAI KHOAN", ConsoleUtil::Mau::XANH_LAM_SANG);

    QuanLyTaiKhoan quanLyTaiKhoan;
    TaiKhoan tk;

    if (!quanLyTaiKhoan.docFile(_strID, tk)) {
        ConsoleUtil::inLoi("Khong tim thay thong tin tai khoan!");
    }
    else {
        vector<string> thongTin = {
            "Ma so tai khoan (ID) : " + tk.getID(),
            "Ten chu tai khoan    : " + tk.getTenChuTaiKhoan(),
            "So du hien tai       : " + to_string((long long)tk.getSoDu()) + " " + tk.getLoaiTienTe()
        };
        ConsoleUtil::veKhung(thongTin, "THE TAI KHOAN", ConsoleUtil::Mau::VANG_SANG);
    }

    ConsoleUtil::tamDung();
}

// Kiem tra so tien hop le
bool MenuUser::kiemTraSoTien(long long llSoTien) const {
    if (llSoTien <= 0) {
        ConsoleUtil::inLoi("So tien phai lon hon 0!");
        return false;
    }
    return true;
}

// Chuc nang 2: Nap tien
void MenuUser::napTien() {
    ConsoleUtil::xoaManHinh();
    ConsoleUtil::veTieuDe("GIAO DICH NAP TIEN", ConsoleUtil::Mau::XANH_LAM_SANG);

    QuanLyTaiKhoan quanLyTaiKhoan;
    TaiKhoan tk;

    if (!quanLyTaiKhoan.docFile(_strID, tk)) {
        ConsoleUtil::inLoi("Khong tim thay tai khoan!");
        ConsoleUtil::tamDung();
        return;
    }

    cout << fixed << setprecision(0);
    cout << "So du hien tai: " << tk.getSoDu()
         << " " << tk.getLoaiTienTe() << '\n';

    long long llSoTien = ConsoleUtil::nhapSoNguyen(
        "Nhap so tien can nap: "
    );

    if (!kiemTraSoTien(llSoTien)) {
        ConsoleUtil::tamDung();
        return;
    }

    if (llSoTien < TIEN_CHUYEN_TOI_THIEU) {
        ConsoleUtil::inLoi("So tien nap toi thieu la 50.000 VND!");
        ConsoleUtil::tamDung();
        return;
    }

    double soDuCu = tk.getSoDu();
    tk.setSoDu(soDuCu + llSoTien);

    // Cap nhat so du
    if (!quanLyTaiKhoan.ghiFile(tk)) {
        ConsoleUtil::inLoi("Khong the cap nhat so du!");
        ConsoleUtil::tamDung();
        return;
    }

    // Tao giao dich nap tien (loai giao dich = 4)
    string strThoiGian = GiaoDich::layThoiGianHienTai();

    GiaoDich giaoDich(
        _strID,
        4,
        llSoTien,
        "",
        "",
        strThoiGian
    );

    // Luu lich su giao dich
    if (!_quanLyGiaoDich.ghiThemChoId(_strID, giaoDich)) {
        tk.setSoDu(soDuCu);
        quanLyTaiKhoan.ghiFile(tk);

        ConsoleUtil::inLoi("Khong the luu lich su giao dich!");
        ConsoleUtil::tamDung();
        return;
    }

    // Thong bao thanh cong
    ConsoleUtil::xoaManHinh();
    ConsoleUtil::inThanhCong("GIAO DICH NAP TIEN THANH CONG!");

    // In hoa don
    vector<string> chiTiet = {
        "Ma giao dich      : " + strThoiGian,
        "Ma tai khoan      : " + _strID,
        "Ten chu tai khoan : " + tk.getTenChuTaiKhoan(),
        "Loai giao dich    : Nap tien",
        "So tien da nap    : " + to_string(llSoTien) + " " + tk.getLoaiTienTe(),
        "So du ban dau     : " + to_string((long long)soDuCu) + " " + tk.getLoaiTienTe(),
        "So du sau giao dich: " + to_string((long long)tk.getSoDu()) + " " + tk.getLoaiTienTe()
    };

    ConsoleUtil::veKhung(
        chiTiet,
        "HOA DON NAP TIEN",
        ConsoleUtil::Mau::XANH_LA_SANG
    );

    ConsoleUtil::tamDung();
}

// Chuc nang 3: Rut tien
void MenuUser::rutTien() {
    ConsoleUtil::xoaManHinh();
    ConsoleUtil::veTieuDe("GIAO DICH RUT TIEN", ConsoleUtil::Mau::XANH_LAM_SANG);

    QuanLyTaiKhoan quanLyTaiKhoan;
    TaiKhoan tk;

    if (!quanLyTaiKhoan.docFile(_strID, tk)) {
        ConsoleUtil::inLoi("Khong tim thay tai khoan!");
        ConsoleUtil::tamDung();
        return;
    }

    ConsoleUtil::inMau("So du hien tai: " + to_string((long long)tk.getSoDu()) + " " + tk.getLoaiTienTe(), ConsoleUtil::Mau::XANH_LA_SANG);

    long long llSoTien = ConsoleUtil::nhapSoNguyen("\nNhap so tien can rut: ");

    if (!kiemTraSoTien(llSoTien)) {
        ConsoleUtil::tamDung();
        return;
    }

    // Kiem tra so tien rut toi thieu
    if (llSoTien < 50000) {
        ConsoleUtil::inLoi("So tien rut toi thieu la 50.000 VND!");
        ConsoleUtil::tamDung();
        return;
    }

    if (tk.getSoDu() - llSoTien < SO_DU_TOI_THIEU) {
        ConsoleUtil::inLoi(
            "Giao dich that bai! So du sau khi rut phai con it nhat 50.000 VND!"
        );
        ConsoleUtil::tamDung();
        return;
    }

    double soDuCu = tk.getSoDu();
    tk.setSoDu(soDuCu - llSoTien);

    if (!quanLyTaiKhoan.ghiFile(tk)) {
        ConsoleUtil::inLoi("Khong the cap nhat so du!");
        ConsoleUtil::tamDung();
        return;
    }

    GiaoDich giaoDich(_strID, 1, llSoTien, "", "", GiaoDich::layThoiGianHienTai());

    if (!_quanLyGiaoDich.ghiThemChoId(_strID, giaoDich)) {
        tk.setSoDu(soDuCu);
        quanLyTaiKhoan.ghiFile(tk);
        ConsoleUtil::inLoi("Khong the luu lich su giao dich!");
        ConsoleUtil::tamDung();
        return;
    }

    ConsoleUtil::inThanhCong("Rut tien thanh cong!");

    vector<string> ketQua = {
        "So tien da rut : " + to_string(llSoTien) + " " + tk.getLoaiTienTe(),
        "So du con lai  : " + to_string((long long)tk.getSoDu()) + " " + tk.getLoaiTienTe()
    };

    ConsoleUtil::veKhung(ketQua, "HOA DON GIAO DICH", ConsoleUtil::Mau::XANH_LA_SANG);

    ConsoleUtil::tamDung();
}

// Chuc nang 4: Chuyen tien
void MenuUser::chuyenTien() {
    ConsoleUtil::xoaManHinh();
    ConsoleUtil::veTieuDe("GIAO DICH CHUYEN TIEN", ConsoleUtil::Mau::XANH_LAM_SANG);

    QuanLyTaiKhoan quanLyTaiKhoan;
    TaiKhoan tkGui, tkNhan;

    // 1. Doc tai khoan nguoi gui
    if (!quanLyTaiKhoan.docFile(_strID, tkGui)) {
        ConsoleUtil::inLoi("Khong tim thay tai khoan nguoi gui!");
        ConsoleUtil::tamDung();
        return;
    }

    ConsoleUtil::inMau(
        "So du hien tai: " + to_string((long long)tkGui.getSoDu())
        + " " + tkGui.getLoaiTienTe(),
        ConsoleUtil::Mau::XANH_LA_SANG
    );

    string strIDNhan = ConsoleUtil::nhapDong("\nNhap ID nguoi nhan: ");

    if (strIDNhan == _strID) {
        ConsoleUtil::inLoi("Khong the chuyen tien cho chinh minh!");
        ConsoleUtil::tamDung();
        return;
    }

    if (!quanLyTaiKhoan.docFile(strIDNhan, tkNhan)) {
        ConsoleUtil::inLoi("Tai khoan nguoi nhan khong ton tai!");
        ConsoleUtil::tamDung();
        return;
    }

    if (tkGui.getLoaiTienTe() != tkNhan.getLoaiTienTe()) {
        ConsoleUtil::inLoi("Hai tai khoan khong cung loai tien te!");
        ConsoleUtil::tamDung();
        return;
    }

    long long llSoTien = ConsoleUtil::nhapSoNguyen(
        "Nhap so tien can chuyen: "
    );

    if (!kiemTraSoTien(llSoTien)) {
        ConsoleUtil::tamDung();
        return;
    }

    if (llSoTien < TIEN_CHUYEN_TOI_THIEU) {
        ConsoleUtil::inLoi("So tien chuyen toi thieu la 50.000 VND!");
        ConsoleUtil::tamDung();
        return;
    }

    if (tkGui.getSoDu() - llSoTien < SO_DU_TOI_THIEU) {
        ConsoleUtil::inLoi(
            "So du sau khi chuyen phai con it nhat 50.000 VND!"
        );
        ConsoleUtil::tamDung();
        return;
    }

    vector<string> chiTietXacNhan = {
        "Nguoi gui       : " + tkGui.getTenChuTaiKhoan(),
        "Nguoi nhan      : " + tkNhan.getTenChuTaiKhoan(),
        "ID nguoi nhan   : " + strIDNhan,
        "So tien chuyen  : " + to_string(llSoTien) + " " + tkGui.getLoaiTienTe(),
        "So du sau GD    : " + to_string(static_cast<long long>(tkGui.getSoDu() - llSoTien)) + " " + tkGui.getLoaiTienTe()
    };
    ConsoleUtil::veKhung(chiTietXacNhan, "XAC NHAN GIAO DICH", ConsoleUtil::Mau::XANH_LAM_SANG);

char cXacNhan;
    do {
        cout << "\nBan co chac chan muon chuyen tien? (Y/N): ";

        cin >> cXacNhan;
        cin.ignore(10000, '\n');

        cXacNhan = toupper(static_cast<unsigned char>(cXacNhan));

        if (cXacNhan != 'Y' && cXacNhan != 'N') {
            ConsoleUtil::inLoi("Chi duoc nhap Y hoac N!");
        }

    } while (cXacNhan != 'Y' && cXacNhan != 'N');

    if (cXacNhan == 'N') {
        ConsoleUtil::inThongBao("Giao dich da duoc huy!");
        ConsoleUtil::tamDung();
        return;
    }

    if (cXacNhan == 'Y') {
        double soDuGuiCu = tkGui.getSoDu();
        double soDuNhanCu = tkNhan.getSoDu();

        tkGui.setSoDu(soDuGuiCu - llSoTien);
        tkNhan.setSoDu(soDuNhanCu + llSoTien);

        // Cap nhat so du nguoi gui
        if (!quanLyTaiKhoan.ghiFile(tkGui)) {
            ConsoleUtil::inLoi("Khong the cap nhat tai khoan nguoi gui!");
            ConsoleUtil::tamDung();
            return;
        }

        // Cap nhat so du nguoi nhan
        if (!quanLyTaiKhoan.ghiFile(tkNhan)) {
            tkGui.setSoDu(soDuGuiCu);
            quanLyTaiKhoan.ghiFile(tkGui);

            ConsoleUtil::inLoi("Khong the cap nhat tai khoan nguoi nhan!");
            ConsoleUtil::tamDung();
            return;
        }

        // Tao giao dich
        string strThoiGian = GiaoDich::layThoiGianHienTai();

        GiaoDich gdGui(
            _strID, 2, llSoTien, strIDNhan,
            tkNhan.getTenChuTaiKhoan(), strThoiGian
        );

        GiaoDich gdNhan(
            strIDNhan, 3, llSoTien, _strID,
            tkGui.getTenChuTaiKhoan(), strThoiGian
        );

        // Luu lich su giao dich
        bool bLuuGui = _quanLyGiaoDich.ghiThemChoId(_strID, gdGui);

        QuanLyGiaoDich lichSuNguoiNhan;
        bool bLuuNhan = lichSuNguoiNhan.ghiThemChoId(strIDNhan, gdNhan);

        if (!bLuuGui || !bLuuNhan) {
            tkGui.setSoDu(soDuGuiCu);
            tkNhan.setSoDu(soDuNhanCu);

            quanLyTaiKhoan.ghiFile(tkGui);
            quanLyTaiKhoan.ghiFile(tkNhan);

            ConsoleUtil::inLoi("Khong the luu lich su giao dich!");
            ConsoleUtil::tamDung();
            return;
        }

        // Thong bao giao dich thanh cong
        ConsoleUtil::xoaManHinh();
        ConsoleUtil::inThanhCong("GIAO DICH CHUYEN TIEN THANH CONG!");

        // In hoa don
        vector<string> chiTiet = {
            "Ma giao dich      : " + strThoiGian,
            "Nguoi gui         : " + tkGui.getTenChuTaiKhoan(),
            "ID nguoi gui      : " + _strID,
            "Nguoi nhan        : " + tkNhan.getTenChuTaiKhoan(),
            "ID nguoi nhan     : " + strIDNhan,
            "So tien da chuyen : " + to_string(llSoTien) + " " + tkGui.getLoaiTienTe(),
            "So du con lai     : " + to_string((long long)tkGui.getSoDu()) + " " + tkGui.getLoaiTienTe()
        };

        ConsoleUtil::veKhung(
            chiTiet,
            "HOA DON CHUYEN TIEN",
            ConsoleUtil::Mau::XANH_LA_SANG
        );

        ConsoleUtil::tamDung();
    }
}

//Chức năng 5: Xem giao dịch
void MenuUser::xemGiaoDich() {
    ConsoleUtil::xoaManHinh();
    ConsoleUtil::veTieuDe("LICH SU GIAO DICH", ConsoleUtil::Mau::XANH_LAM_SANG);

    if (!_quanLyGiaoDich.docFile(_strID)) {
        ConsoleUtil::inLoi("Khong doc duoc lich su giao dich!");
    }
    else if (_quanLyGiaoDich.kichThuoc() == 0) {
        ConsoleUtil::inThongBao("Tai khoan chua co giao dich nao!");
    }
    else {
        _quanLyGiaoDich.hienThiTatCa();
    }

    ConsoleUtil::tamDung();
}

// Chuc nang 6: Doi ma PIN
void MenuUser::doiMaPin() {
    ConsoleUtil::xoaManHinh();
    ConsoleUtil::veTieuDe("DOI MA PIN TAI KHOAN", ConsoleUtil::Mau::XANH_LAM_SANG);

    string strPINCu   = ConsoleUtil::nhapMatKhau("Nhap ma PIN cu: ", PIN_LENGTH, true);
    string strPINMoi1 = ConsoleUtil::nhapMatKhau("Nhap ma PIN moi: ", PIN_LENGTH, true);
    string strPINMoi2 = ConsoleUtil::nhapMatKhau("Nhap lai ma PIN moi: ", PIN_LENGTH, true);

    if (_quanLyTheTu.doiPIN(_strID, strPINCu, strPINMoi1, strPINMoi2)) {
        ConsoleUtil::inThanhCong("Doi ma PIN thanh cong!");
    }
    else {
        ConsoleUtil::inLoi("Doi ma PIN that bai! Kiem tra lai ma PIN cu hoac dinh dang PIN moi.");
    }

    ConsoleUtil::tamDung();
}
