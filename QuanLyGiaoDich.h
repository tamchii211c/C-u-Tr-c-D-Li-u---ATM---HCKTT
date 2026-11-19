#ifndef QUANLYGIAODICH_H_INCLUDED_
#define QUANLYGIAODICH_H_INCLUDED_

#include <string>

#include "GiaoDich.h"
#include "LinkedList.h"

/**********************************************************
* @Description Quan ly lich su giao dich bang danh sach lien ket.
*              Danh sach trong bo nho la lich su cua MOT tai khoan
*              (tai bang docFile); file duoc ghi / doc theo tung ID.
**********************************************************/
class QuanLyGiaoDich {
public:
    bool docFile(const std::string& strId);
    void themGiaoDich(const GiaoDich& giaoDich);
    bool ghiThemChoId(const std::string& strId, const GiaoDich& giaoDich) const;
    bool taoFileLichSu(const std::string& strId) const;
    void hienThiTatCa() const;
    int kichThuoc() const;

private:
    LinkedList<GiaoDich> _danhSach;

    static std::string layDuongDan(const std::string& strId);
};

#endif
