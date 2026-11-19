#include "QuanLyTheTu.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cctype>
#include "ConsoleUtil.h"

using namespace std;

static string trim(const string& strChuoi){
    size_t iDau = strChuoi.find_first_not_of(" \t\r\n");
    if (iDau == string::npos) return "";

    size_t iCuoi = strChuoi.find_last_not_of(" \t\r\n");

    return strChuoi.substr(iDau, iCuoi - iDau + 1);
}

static string layGiaTri(const string& strDong){
    size_t iViTri = strDong.find(':');

    if (iViTri == string::npos) return "";

    return trim(strDong.substr(iViTri + 1));
}

static bool fileDaTonTai(const string& strDuongDan){
    ifstream file(strDuongDan.c_str());
    return file.good();
}

static const long long SO_DU_TOI_THIEU = 50000;

bool QuanLyTheTu::khoiTao10TheMacDinh(const string& strDuongDan) {
    this->_lstDanhSachThe.xoaTatCa();
    this->_mapSoLanNhapSai.clear();

    string dsID[] = {
        "10000000000001", "10000000000002", "10000000000003", "10000000000004", "10000000000005",
        "10000000000006", "10000000000007", "10000000000008", "10000000000009", "10000000000010"
    };
    string dsTen[] = {
        "Nguyen Van A", "Tran Thi B", "Le Van C", "Pham Thi D", "Hoang Van E",
        "Vu Thi F", "Dinh Van G", "Bui Thi H", "Do Van I", "Ngo Thi K"
    };
    const string PIN_MAC_DINH = "123456";
    const long long SO_DU_MAC_DINH = 500000; // 500,000 VND

    for (int i = 0; i < 10; ++i) {
        TheTu theTu(dsID[i], PIN_MAC_DINH, false, true);
        this->_lstDanhSachThe.themCuoi(theTu);
        this->_mapSoLanNhapSai[dsID[i]] = 0;

        string strDuongDanTK = "data/" + dsID[i] + ".txt";
        string strDuongDanLS = "data/LichSu" + dsID[i] + ".txt";

        ofstream fileTK(strDuongDanTK.c_str());
        if (fileTK.is_open()) {
            fileTK << "Ma so tai khoan (ID): " << dsID[i] << '\n';
            fileTK << "Ten chu tai khoan: " << dsTen[i] << '\n';
            fileTK << "So du: " << SO_DU_MAC_DINH << '\n';
            fileTK << "Loai tien: VND\n";
            fileTK.close();
        }

        ofstream fileLS(strDuongDanLS.c_str());
        if (fileLS.is_open()) {
            fileLS.close();
        }
    }

    return this->ghiFile(strDuongDan);
}


bool QuanLyTheTu::docFile(const string& strDuongDan) {
    ifstream file(strDuongDan.c_str());

    if (!file.is_open()) {
        return this->khoiTao10TheMacDinh(strDuongDan);
    }

    this->_lstDanhSachThe.xoaTatCa();
    this->_mapSoLanNhapSai.clear();

    const string DEFAULT_PIN = "123456";

    string strDongID;
    string strDongPIN;

    while (getline(file, strDongID)) {
        if (!getline(file, strDongPIN)) {
            break;
        }

        bool bBiKhoa = false;
        streampos viTriTruoc = file.tellg();
        string strDongTrangThai;

        if (getline(file, strDongTrangThai) && strDongTrangThai.find("Trang thai:") == 0) {
            bBiKhoa = (layGiaTri(strDongTrangThai) == "Da khoa");
        }
        else {
            file.clear();
            file.seekg(viTriTruoc);  
        }

        if (strDongID.find("Ma so tai khoan (ID):") != 0 || strDongPIN.find("Ma PIN:") != 0) {
            continue;
        }

        string strID = layGiaTri(strDongID);
        string strPIN = layGiaTri(strDongPIN);

        if (!TheTu::kiemTraID(strID) || !TheTu::kiemTraDinhDangPIN(strPIN)) {
            continue;
        }
        if (this->tonTai(strID)) {
            continue;
        }

        TheTu theTu(strID, strPIN, bBiKhoa, strPIN == DEFAULT_PIN);
        this->_lstDanhSachThe.themCuoi(theTu);
        this->_mapSoLanNhapSai[strID] = 0;
    }

    file.close();

    if (this->_lstDanhSachThe.rong()) {
        return this->khoiTao10TheMacDinh(strDuongDan);
    }
    return true;
}

bool QuanLyTheTu::ghiFile(const string& strDuongDan) const{
    ofstream file(strDuongDan.c_str());

    if (!file.is_open()) return false;

    for (Node<TheTu>* pCurrent = this->_lstDanhSachThe.getHead(); pCurrent != nullptr; pCurrent = pCurrent->getNext()){
        const TheTu& theTu = pCurrent->getData();
        file << "Ma so tai khoan (ID): " << theTu.getID() << '\n';
        file << "Ma PIN: " << theTu.getPIN() << '\n';
        file << "Trang thai: " << (theTu.getBiKhoa() ? "Da khoa" : "Hoat dong") << '\n';
    }

    file.close();
    
    return true;
}

void QuanLyTheTu::hienThiDanhSach() const {
    if (this->_lstDanhSachThe.rong()) {
        ConsoleUtil::inThongBao("Danh sach the tu trong!\n");
        return;
    }

    int iSTT = 1;

    for (Node<TheTu>* pCurrent = this->_lstDanhSachThe.getHead();
         pCurrent != nullptr;
         pCurrent = pCurrent->getNext()) {

        const TheTu& theTu = pCurrent->getData();

        // The tu thu: mau tim
        ConsoleUtil::datMau(ConsoleUtil::Mau::TIM);
        cout << "\nThe tu thu " << iSTT++;
        ConsoleUtil::datLaiMau();
        cout << '\n';

        // ID: mau xanh duong
        ConsoleUtil::datMau(ConsoleUtil::Mau::XANH_DUONG);
        cout << "ID: " << theTu.getID();
        ConsoleUtil::datLaiMau();
        cout << '\n';

        cout << "PIN: ******\n";

        cout << "Trang thai: ";

        // Trang thai: do neu bi khoa, xanh la neu hoat dong
        if (theTu.getBiKhoa()) {
            ConsoleUtil::datMau(ConsoleUtil::Mau::DO);
            cout << "Bi khoa";
            ConsoleUtil::datLaiMau();
        } else {
            ConsoleUtil::datMau(ConsoleUtil::Mau::XANH_LA);
            cout << "Hoat dong";
            ConsoleUtil::datLaiMau();
        }

        cout << '\n';
    }
}

TheTu* QuanLyTheTu::timTheoID(const string& strID){
    Node<TheTu>* pCurrent = this->_lstDanhSachThe.getHead();

    while (pCurrent != nullptr){
        TheTu& theTu = pCurrent->getData();

        if (theTu.getID() == strID){
            return &theTu;
        }
        pCurrent = pCurrent->getNext();
    }
    return nullptr;
}

const TheTu* QuanLyTheTu::timTheoID(const string& strID) const{
    Node<TheTu>* pCurrent = this->_lstDanhSachThe.getHead();

    while (pCurrent != nullptr){
        const TheTu& theTu = pCurrent->getData();
        if (theTu.getID() == strID){
            return &theTu;
        }
        pCurrent = pCurrent->getNext();
    }
    return nullptr;
}

bool QuanLyTheTu::tonTai(const string& strID) const{
    return this->timTheoID(strID) != nullptr;
}

bool QuanLyTheTu::themTaiKhoan(const string& strID, const string& strTenTaiKhoan,long long llSoDu,const string& strLoaiTien){
    if (!TheTu::kiemTraID(strID) || strTenTaiKhoan.empty() || strLoaiTien.empty() || llSoDu < SO_DU_TOI_THIEU){
        return false;
    }

    if (this->tonTai(strID)){
        return false;
    }

    const string strDuongDanTaiKhoan = "data/" + strID + ".txt";
    const string strDuongDanLichSu = "data/LichSu" + strID + ".txt";
    if (fileDaTonTai(strDuongDanTaiKhoan) || fileDaTonTai(strDuongDanLichSu)){
        return false;
    }

    ofstream fileTaiKhoan(strDuongDanTaiKhoan.c_str());

    if (!fileTaiKhoan.is_open()){
        return false;
    }

    fileTaiKhoan << "Ma so tai khoan (ID): " << strID << '\n';
    fileTaiKhoan << "Ten chu tai khoan: " << strTenTaiKhoan << '\n';
    fileTaiKhoan << "So du: " << llSoDu << '\n';
    fileTaiKhoan << "Loai tien: " << strLoaiTien << '\n';

    fileTaiKhoan.close();

    ofstream fileLichSu(strDuongDanLichSu.c_str());

    if (!fileLichSu.is_open()){
        remove(strDuongDanTaiKhoan.c_str());
        return false;
    }

    fileLichSu.close();

    TheTu theTu(strID, "123456", false, true);

    this->_lstDanhSachThe.themCuoi(theTu);
    this->_mapSoLanNhapSai[strID] = 0;

    if (!this->ghiFile()){
        int iViTri = 0;
        Node<TheTu>* pCurrent = this->_lstDanhSachThe.getHead();

        while (pCurrent != nullptr){
            if (pCurrent->getData().getID() == strID) break;

            pCurrent = pCurrent->getNext();
            ++iViTri;
        }

        if (pCurrent != nullptr){
            this->_lstDanhSachThe.xoaTheoViTri(iViTri);
        }

        this->_mapSoLanNhapSai.erase(strID);

        remove(strDuongDanTaiKhoan.c_str());
        remove(strDuongDanLichSu.c_str());

        return false;
    }

    return true;
}

bool QuanLyTheTu::xoaTheTu(const string& strID){
    TheTu* pTheTu = this->timTheoID(strID);

    if (pTheTu == nullptr){
        return false;
    }

    TheTu theTuCu = *pTheTu;

    bool bDaXoa = false;
    int iViTri = 0;

    Node<TheTu>* pCurrent = this->_lstDanhSachThe.getHead();

    while (pCurrent != nullptr){
        if (pCurrent->getData().getID() == strID){
            this->_lstDanhSachThe.xoaTheoViTri(iViTri);
            bDaXoa = true;
            break;
        }

        pCurrent = pCurrent->getNext();
        ++iViTri;
    }

    if (!bDaXoa){
        return false;
    }

    if (!this->ghiFile()){
        this->_lstDanhSachThe.themCuoi(theTuCu);
        this->ghiFile();
        return false;
    }

    const string strFileTaiKhoan = "data/" + strID + ".txt";
    const string strFileLichSu = "data/LichSu" + strID + ".txt";

    int iXoaTaiKhoan = remove(strFileTaiKhoan.c_str());
    int iXoaLichSu = remove(strFileLichSu.c_str());
    if (iXoaTaiKhoan != 0 || iXoaLichSu != 0) {
        return false;
    }
    this->_mapSoLanNhapSai.erase(strID);
    return true;
}

bool QuanLyTheTu::moKhoa(const string& strID) {
    TheTu* pTheTu = this->timTheoID(strID);
    if (pTheTu == nullptr) {
        return false;
    }

    pTheTu->moKhoaThe();
    this->_mapSoLanNhapSai[strID] = 0;

    if (!this->ghiFile()) {  
        pTheTu->khoaThe();
        return false;
    }
    return true;
}

bool QuanLyTheTu::kiemTraDangNhap(const string& strID, const string& strPIN){
    const int MAX_LAN_NHAP_SAI = 3;

    TheTu* pTheTu = this->timTheoID(strID);

    if (pTheTu == nullptr || pTheTu->getBiKhoa()){
        ConsoleUtil::datMau(ConsoleUtil::Mau::XANH_DUONG_SANG);
        cout << "The khong ton tai hoac da bi khoa!\n";
        ConsoleUtil::datLaiMau();
        cout << '\n';
        return false;
    }

    if (pTheTu->kiemTraPIN(strPIN)){
        this->_mapSoLanNhapSai[strID] = 0;
        return true;
    }

    ++this->_mapSoLanNhapSai[strID];

    int iSoLanConLai = MAX_LAN_NHAP_SAI - this->_mapSoLanNhapSai[strID];

    if (this->_mapSoLanNhapSai[strID] >= MAX_LAN_NHAP_SAI){
        pTheTu->khoaThe();
        ConsoleUtil::datMau(ConsoleUtil::Mau::DO_SANG);
        cout << "The da bi khoa do nhap sai PIN 3 lan!\n";
        ConsoleUtil::datLaiMau();
        cout << '\n';
        this->ghiFile();
        return false;
    }
    ConsoleUtil::datMau(ConsoleUtil::Mau::DO_SANG);
    cout << "PIN khong chinh xac! Ban con "<< iSoLanConLai << " lan thu.\n";
    ConsoleUtil::datLaiMau();
    cout << '\n';
    return false;
}

bool QuanLyTheTu::doiPIN(const string& strID, const string& strPINCu, const string& strPINMoi1, const string& strPINMoi2){
    TheTu* pTheTu = this->timTheoID(strID);

    if (pTheTu == nullptr) return false;

    string strPINDaLuu = pTheTu->getPIN();
    bool bTrangThaiDoiPIN = pTheTu->getDoiPINLanDau();

    if (!pTheTu->doiPIN(strPINCu, strPINMoi1, strPINMoi2)) return false;

    if (!this->ghiFile()){
        pTheTu->setPIN(strPINDaLuu);
        pTheTu->setDoiPINLanDau(bTrangThaiDoiPIN);
        return false;
    }
    return true;
}

int QuanLyTheTu::getSoLuongThe() const{
    return this->_lstDanhSachThe.kichThuoc();
}
