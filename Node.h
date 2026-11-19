//#pragma once
#ifndef NODE_H_INCLUDED_
#define NODE_H_INCLUDED_

/**********************************************************
* @Description Nut cua danh sach lien ket don, chua du lieu kieu T
*              va con tro toi nut ke tiep
* @attention Nut khong cho sao chep; LinkedList chiu trach nhiem
*            cap phat (new) va giai phong (delete) cac nut
**********************************************************/
template <typename T>
class Node{
public:
	explicit Node(const T& data)
		: _data(data), _pNext(nullptr){}

	Node(const Node&) = delete;
	Node& operator=(const Node&) = delete;

	T& getData(){
		return this->_data;
	}

	const T& getData() const{
		return this->_data;
	}

	Node<T>* getNext() const{
		return this->_pNext;
	}

	void setNext(Node<T>* pNext){
		this->_pNext = pNext;
	}

private:
	T _data;
	Node<T>* _pNext;
};

#endif
