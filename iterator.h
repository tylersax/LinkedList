/*
 *  iterator.h
 *  072 p4
 *
 *  Created by Tyler on 4/14/11.
 *  Copyright 2011 Georgetown University. All rights reserved.
 *
 */

#ifndef ITERATOR_H
#define ITERATOR_H

#include "nosuchobject.h"
#include "list.h"

using namespace std;

template <typename> class List;

template <typename T>
class ListIterator
{
	
	friend class List<T>;
	
public:
    ListIterator();
    void add( const T &object );
    bool hasNext() const;
    bool hasPrevious() const;
    T &next() throw ( NoSuchObject );
    T &previous() throw ( NoSuchObject );
    void set( const T & );
    void printInternal() const;
	
private:
    Node<T> *current;
    bool atFront, atEnd;
	
}; // ListIterator class

template <typename T>
ListIterator<T>::ListIterator(){
	atFront=false;
	atEnd=false;
	current=0;
}

template <typename T>
void ListIterator<T>::add( const T &object ){
	
	
}

template <typename T>
bool ListIterator<T>::hasNext() const{
	if(atEnd == false) return true;
	else{ 
		return false;
	}
	
}

template <typename T>
bool ListIterator<T>::hasPrevious() const{
	if (atFront == false) {
		return true;
	}
	else {
		return false;
	}

	
}

template <typename T>
T& ListIterator<T>::next() throw ( NoSuchObject ){
	if (hasNext() == false){
		throw NoSuchObject ("NO SUCH OBJECT -- next()");
	}
	else if (hasPrevious() == false){
		T &object = current->getObject();
		atFront = false;
		return object;
	}
		
	else{
		current = current->getNextPtr();
		if (current->getNextPtr() == 0) {
			atEnd = true;
		}
		return current->getObject();
	}
}

template <typename T>
T& ListIterator<T>::previous() throw ( NoSuchObject ){
	if (hasPrevious() == false){
		throw NoSuchObject ("NO SUCH OBJECT -- previous()");
	}
	else{
		current = current->getPrevPtr();
		return current->getObject();
	}
}

template <typename T>
void ListIterator<T>::set( const T & object){
	current->setObject(object);
	
}

template <typename T>
void ListIterator<T>::printInternal() const{
	
	
}

#endif



