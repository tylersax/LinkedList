#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <new>

using namespace std;

template <typename T>
class Node {
	private:
		T object;
		Node<T> *nextPtr;	  
		Node<T> *prevPtr;

	public: 
		Node (const T &object =Node());
		void setObject(const T&);
		T & getObject();
		void setNextPtr(Node<T> *nextPtr);
		void setPrevPtr(Node<T> *prevPtr);
		Node<T>* getNextPtr() const;
		Node<T>* getPrevPtr() const;
};

template <typename T>
Node<T>::Node(const T &object){
	this->object = object;
	this->nextPtr = 0;
}

template <typename T>
void Node <T>::setObject(const T &object){
	this->object=object;
}

template <typename T>
T& Node <T>::getObject(){
	return object;
}

template <typename T>
void Node<T>::setNextPtr(Node<T> *nextPtr){
	this->nextPtr=nextPtr;
}

template <typename T>
Node<T>* Node<T>::getNextPtr() const{
	return nextPtr;
}

template <typename T>
Node<T>* Node<T>::getPrevPtr() const{
        return prevPtr;
}

template <typename T>
void Node<T>::setPrevPtr(Node<T> *prevPtr){
        this->prevPtr=prevPtr;
}

#endif
