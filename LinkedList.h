//#pragma once
#ifndef LINKEDLIST_H_INCLUDED_
#define LINKEDLIST_H_INCLUDED_

#include "Node.h"

/**********************************************************
* @Description Danh sach lien ket don chua cac phan tu kieu T
* @attention Kieu T can co toan tu == neu dung timKiem(const T&)
*            hoac xoa(const T&). Cac nut duoc cap phat bang new
*            trong class nay va giai phong trong destructor.
**********************************************************/
template <typename T>
class LinkedList{
public:
	LinkedList()
		: _pHead(nullptr), _pTail(nullptr), _iSize(0){ }

	LinkedList(const LinkedList& other)
		: _pHead(nullptr), _pTail(nullptr), _iSize(0){
		this->saoChep(other);
	}

	~LinkedList(){
		this->xoaTatCa();
	}

	LinkedList& operator=(const LinkedList& other){
		if (this != &other){
			this->xoaTatCa();
			this->saoChep(other);
		}
		return *this;
	}

	/**********************************************************
	* @Description Them phan tu vao dau danh sach
	**********************************************************/
	void themDau(const T& data){
		Node<T>* pNew = new Node<T>(data);
		pNew->setNext(this->_pHead);
		this->_pHead = pNew;
		if (this->_pTail == nullptr){
			this->_pTail = pNew;
		}
		++this->_iSize;
	}

	/**********************************************************
	* @Description Them phan tu vao cuoi danh sach
	**********************************************************/
	void themCuoi(const T& data){
		Node<T>* pNew = new Node<T>(data);
		if (this->_pTail == nullptr){
			this->_pHead = pNew;
		}
		else{
			this->_pTail->setNext(pNew);
		}
		this->_pTail = pNew;
		++this->_iSize;
	}

	/**********************************************************
	* @Description Xoa phan tu dau tien bang (==) voi data
	*
	* @return true neu tim thay va xoa; false neu khong co
	**********************************************************/
	bool xoa(const T& data){
		return this->xoaTheo([&data](const T& item){
				return item == data;
		});
	}

	/**********************************************************
	* @Description Xoa phan tu dau tien thoa dieu kien pred
	*
	* @return true neu tim thay va xoa; false neu khong co
	* @attention pred la ham/lambda dang bool pred(const T&)
	**********************************************************/
	template <typename Predicate>
	bool xoaTheo(Predicate pred){
		Node<T>* pPrev = nullptr;
		Node<T>* pCurrent = this->_pHead;
		while (pCurrent != nullptr && !pred(pCurrent->getData())){
			pPrev = pCurrent;
			pCurrent = pCurrent->getNext();
		}

		if (pCurrent == nullptr){
			return false;
		}
		this->goNode(pPrev, pCurrent);
		return true;
	}

	/**********************************************************
	* @Description Xoa phan tu tai vi tri iViTri (tinh tu 0)
	*
	* @return false neu vi tri khong hop le
	**********************************************************/
	bool xoaTheoViTri(int iViTri){
		if (iViTri < 0 || iViTri >= this->_iSize){
			return false;
		}

		Node<T>* pPrev = nullptr;
		Node<T>* pCurrent = this->_pHead;
		for (int iIndex = 0; iIndex < iViTri; ++iIndex){
			pPrev = pCurrent;
			pCurrent = pCurrent->getNext();
		}
		this->goNode(pPrev, pCurrent);
		return true;
	}

	/**********************************************************
	* @Description Tim phan tu dau tien bang (==) voi data
	*
	* @return Con tro toi phan tu trong danh sach, nullptr neu khong co
	**********************************************************/
	T* timKiem(const T& data) const{
		return this->timKiemTheo([&data](const T& item){
				return item == data;
		});
	}

	/**********************************************************
	* @Description Tim phan tu dau tien thoa dieu kien pred
	*
	* @return Con tro toi phan tu trong danh sach, nullptr neu khong co
	* @attention Vi du: timKiemTheo([&](const TheTu& t){ return t.getId() == strId; })
	**********************************************************/
	template <typename Predicate>
	T* timKiemTheo(Predicate pred) const{
		for (Node<T>* pCurrent = this->_pHead; pCurrent != nullptr; pCurrent = pCurrent->getNext()){
			if (pred(pCurrent->getData())){
				return &pCurrent->getData();
			}
		}
		return nullptr;
	}

	/**********************************************************
	* @Description Lay phan tu tai vi tri iViTri (tinh tu 0)
	*
	* @return Con tro toi phan tu, nullptr neu vi tri khong hop le
	**********************************************************/
	T* layTai(int iViTri) const{
		if (iViTri < 0 || iViTri >= this->_iSize){
			return nullptr;
		}

		Node<T>* pCurrent = this->_pHead;
		for (int iIndex = 0; iIndex < iViTri; ++iIndex){
			pCurrent = pCurrent->getNext();
		}
		return &pCurrent->getData();
	}

	/**********************************************************
	* @Description Xoa toan bo phan tu, giai phong bo nho
	**********************************************************/
	void xoaTatCa(){
		Node<T>* pCurrent = this->_pHead;
		while (pCurrent != nullptr){
			Node<T>* pNext = pCurrent->getNext();
			delete pCurrent;
			pCurrent = pNext;
		}
		this->_pHead = nullptr;
		this->_pTail = nullptr;
		this->_iSize = 0;
	}

	int kichThuoc() const{
		return this->_iSize;
	}

	bool rong() const{
		return this->_iSize == 0;
	}

	/**********************************************************
	* @Description Lay nut dau de duyet danh sach
	* @attention Duyet: for (Node<T>* p = ds.getHead(); p != nullptr; p = p->getNext())
	**********************************************************/
	Node<T>* getHead() const{
		return this->_pHead;
	}

private:
	Node<T>* _pHead;
	Node<T>* _pTail;
	int _iSize;

	void saoChep(const LinkedList& other){
		for (Node<T>* pCurrent = other._pHead; pCurrent != nullptr; pCurrent = pCurrent->getNext()){
			this->themCuoi(pCurrent->getData());
		}
	}

	/**********************************************************
	* @Description Go pCurrent khoi danh sach (pPrev la nut ngay truoc,
	*              nullptr neu pCurrent la nut dau) roi delete
	**********************************************************/
	void goNode(Node<T>* pPrev, Node<T>* pCurrent){
		if (pPrev == nullptr){
			this->_pHead = pCurrent->getNext();
		}
		else{
			pPrev->setNext(pCurrent->getNext());
		}

		if (pCurrent == this->_pTail){
			this->_pTail = pPrev;
		}

		delete pCurrent;
		--this->_iSize;
	}
};

#endif
