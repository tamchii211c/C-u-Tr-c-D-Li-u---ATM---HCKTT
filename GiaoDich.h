#ifndef GIAODICH_H_INCLUDED_
#define GIAODICH_H_INCLUDED_

#include "ConsoleUtil.h"
#include <string>

/**********************************************************
* @Description Mot giao dich trong lich su cua mot tai khoan
*              (ID chu so huu, loai, so tien, doi tac, thoi gian)
* @attention Loai giao dich la so nguyen: 1 = rut tien, 2 = chuyen tien,
*            3 = nhan tien
*            Dinh dang mot dong trong file LichSu[ID].txt:
*            ID|Loai|SoTien|IdDoiTac|TenDoiTac|ThoiGian
*            (rut tien khong co doi tac thi hai truong doi tac de trong)
**********************************************************/
class GiaoDich{
public:
    GiaoDich();
    GiaoDich(const std::string& strId,
             int iLoai,
             long long llSoTien,
             const std::string& strIdDoiTac,
             const std::string& strTenDoiTac,
             const std::string& strThoiGian);

    std::string getId() const;
    int getLoai() const;
    long long getSoTien() const;
    std::string getIdDoiTac() const;
    std::string getTenDoiTac() const;
    std::string getThoiGian() const;

    std::string layTenLoai() const;
    ConsoleUtil::Mau layMauLoai() const;
    std::string chuyenThanhDong() const;
    bool docTuDong(const std::string& strDong);
    void hienThi() const;

    static void hienThiTieuDeBang();
    static std::string layThoiGianHienTai();

private:
    std::string _strId;
    int _iLoai;
    long long _llSoTien;
    std::string _strIdDoiTac;
    std::string _strTenDoiTac;
    std::string _strThoiGian;
};

#endif
