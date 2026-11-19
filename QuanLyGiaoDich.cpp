#include "QuanLyGiaoDich.h"

#include <fstream>
#include <iostream>

using namespace std;

namespace {
	// File lich su nam o data/LichSu[ID].txt
	const string THU_MUC_DATA = "data/";
	const string TIEN_TO_LICH_SU = "LichSu";
	const string DUOI_FILE = ".txt";
}

/***********************************************2***********
* @Description Doc lich su giao dich cua tai khoan strId vao danh sach lien ket
*
* @return true neu doc thanh cong
* @attention Danh sach cu bi xoa truoc khi doc. Neu file chua ton tai thi
*            tao file rong va coi nhu lich su trong. Dong sai dinh dang bi bo qua.
**********************************************************/
bool QuanLyGiaoDich::docFile(const string& strId){
	this->_danhSach.xoaTatCa();

	ifstream fileLichSu(QuanLyGiaoDich::layDuongDan(strId));
	if (!fileLichSu.is_open()){
		return this->taoFileLichSu(strId);
	}

	string strDong;
	while (getline(fileLichSu, strDong)){
		if (strDong.empty()){
			continue;
		}

		GiaoDich giaoDich;
		if (giaoDich.docTuDong(strDong)){
			this->_danhSach.themCuoi(giaoDich);
		}
	}

	fileLichSu.close();
	return true;
}

/**********************************************************
* @Description Them giao dich vao danh sach lien ket (chi trong bo nho)
* @attention Muon luu xuong file phai goi them ghiThemChoId
**********************************************************/
void QuanLyGiaoDich::themGiaoDich(const GiaoDich& giaoDich){
	this->_danhSach.themCuoi(giaoDich);
}

/**********************************************************
* @Description Ghi them mot giao dich vao cuoi file LichSu[ID].txt
*
* @return true neu ghi thanh cong
* @attention Tu tao file neu chua co. Dung duoc cho tai khoan khac tai khoan
*            dang tai trong danh sach (vi du ben nhan tien khi chuyen tien).
**********************************************************/
bool QuanLyGiaoDich::ghiThemChoId(
    const string& strId,
    const GiaoDich& giaoDich
) const {

    string strDuongDan = QuanLyGiaoDich::layDuongDan(strId);

    ofstream fileLichSu(strDuongDan, ios::app);

    if (!fileLichSu.is_open()) {
        cerr << "Loi: Khong mo duoc file: "
             << strDuongDan << '\n';
        return false;
    }

    fileLichSu << giaoDich.chuyenThanhDong() << '\n';

    if (!fileLichSu.good()) {
        cerr << "Loi: Ghi giao dich that bai!\n";
        fileLichSu.close();
        return false;
    }

    fileLichSu.close();

    return true;
}
/**********************************************************
* @Description Tao file LichSu[ID].txt rong neu chua ton tai
*
* @return true neu file ton tai sau khi goi
* @attention Dung khi Admin them tai khoan moi
**********************************************************/
bool QuanLyGiaoDich::taoFileLichSu(
    const string& strId
) const {

    string strDuongDan = QuanLyGiaoDich::layDuongDan(strId);

    ofstream fileLichSu(strDuongDan, ios::app);

    if (!fileLichSu.is_open()) {
        return false;
    }

    fileLichSu.close();
    return true;
}
/**********************************************************
* @Description In toan bo lich su giao dich dang tai trong danh sach
**********************************************************/
void QuanLyGiaoDich::hienThiTatCa() const{
	if (this->_danhSach.rong()){
		cout << " Chua co giao dich nao.\n";
		return;
	}

	GiaoDich::hienThiTieuDeBang();
	for (Node<GiaoDich>* pNode = this->_danhSach.getHead(); pNode != nullptr; pNode = pNode->getNext()){
		pNode->getData().hienThi();
	}
}

int QuanLyGiaoDich::kichThuoc() const{
	return this->_danhSach.kichThuoc();
}

/**********************************************************
* @Description Tao duong dan data/LichSu[ID].txt
**********************************************************/
string QuanLyGiaoDich::layDuongDan(const string& strId){
	return THU_MUC_DATA + TIEN_TO_LICH_SU + strId + DUOI_FILE;
}
