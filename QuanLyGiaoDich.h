#ifndef QUANLYGIAODICH_H_INCLUDED_
#define QUANLYGIAODICH_H_INCLUDED_

#include <string>

#include "GiaoDich.h"
#include "LinkedList.h"

using namespace std;

/**********************************************************
* @Description Quan ly lich su giao dich bang danh sach lien ket.
*              Danh sach trong bo nho la lich su cua MOT tai khoan
*              (tai bang docFile); file duoc ghi / doc theo tung ID.
**********************************************************/
class QuanLyGiaoDich {
public:
    bool docFile(const string& strId);
    void themGiaoDich(const GiaoDich& giaoDich);
    bool ghiThemChoId(const string& strId, const GiaoDich& giaoDich) const;
    bool taoFileLichSu(const string& strId) const;
    void hienThiTatCa() const;
    int kichThuoc() const;

private:
    LinkedList<GiaoDich> _danhSach;

    static string layDuongDan(const string& strId);
};

#endif
