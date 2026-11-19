#ifndef GIAODICH_H_INCLUDED_
#define GIAODICH_H_INCLUDED_

#include "ConsoleUtil.h"
#include <string>

using namespace std;

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
    GiaoDich(const string& strId,
             int iLoai,
             long long llSoTien,
             const string& strIdDoiTac,
             const string& strTenDoiTac,
             const string& strThoiGian);

    string getId() const;
    int getLoai() const;
    long long getSoTien() const;
    string getIdDoiTac() const;
    string getTenDoiTac() const;
    string getThoiGian() const;

    string layTenLoai() const;
    ConsoleUtil::Mau layMauLoai() const;
    string chuyenThanhDong() const;
    bool docTuDong(const string& strDong);
    void hienThi() const;

    static void hienThiTieuDeBang();
    static string layThoiGianHienTai();

private:
    string _strId;
    int _iLoai;
    long long _llSoTien;
    string _strIdDoiTac;
    string _strTenDoiTac;
    string _strThoiGian;
};

#endif
