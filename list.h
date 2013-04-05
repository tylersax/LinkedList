#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <stdexcept>
#include <new>
#include "node.h"
#include "iterator.h"
#include "nosuchobject.h"

using namespace std;

template <typename T>
class List;

template <typename T>
ostream &operator<<(ostream&, const List<T> &);

template <typename T>
class List {
	
	friend ostream &operator<< <>( ostream &, const List<T> & );
	
public:
    List();
    List( const List<T> & ) throw ( bad_alloc );
    ~List();
    void add( unsigned, const T & ) throw ( bad_alloc, out_of_range );
    void addAll( const List<T> & ) throw ( bad_alloc );
    void addAll( unsigned, const List<T> & ) throw ( bad_alloc, out_of_range );
    void addFirst( const T & ) throw ( bad_alloc );
    void addLast( const T & ) throw ( bad_alloc );
    void clear();
    bool contains( const T & ) const;
    bool empty() const;
    int indexOf( const T & ) const;
    T &get( unsigned ) const throw ( out_of_range );
    T &getFirst() const throw ( NoSuchObject );
    T &getLast() const throw ( NoSuchObject );
    ListIterator<T> listIterator();
    ListIterator<T> listIterator( unsigned ) throw ( out_of_range );
    T remove( unsigned ) throw ( out_of_range );
    T removeFirst() throw ( NoSuchObject );
    T removeFirstOccurrence( const T & ) throw ( NoSuchObject );
    T removeLast() throw ( NoSuchObject );
    T removeLastOccurrence( const T & ) throw ( NoSuchObject );
    T set( unsigned, const T & ) throw ( out_of_range );
    unsigned size() const;
    T *toArray() const throw ( bad_alloc );
    const List<T> &operator=( const List<T> & ) throw ( bad_alloc );
    void printInternal(ostream&);
	
private:
    Node<T> *frontPtr, *backPtr;
    unsigned sz;
	Node<T> *getIthNode( unsigned ) const throw ( out_of_range );
    void add( Node<T> *, const T & ) throw ( bad_alloc );
    void initialize();
    
    T remove( Node<T> * );
	
}; // List class

template <typename T>
List<T>::List(){
	initialize();
}

template <typename T>
List<T>::List(const List<T> &list) throw(bad_alloc){
	*this = list;
}               

template <typename T>
void List<T>::add(unsigned index, const T &object) throw (bad_alloc, out_of_range){
	if(index >= sz) throw out_of_range("out of range add(index)");
	
	Node<T> *finder = frontPtr;
	unsigned counter = 0;
	
	if (index == 0) add(finder, object);
	
	else {
		while (finder != 0) {
			counter++;
			finder = finder->getNextPtr();
			if (index == counter) add(finder, object);
		}//while
		sz++;
	}//else
}

template <typename T>
void List<T>::add(Node<T> *current, const T &object) throw(bad_alloc){
	Node<T> *finder = 0;
	Node<T> *helper = 0;
	finder = frontPtr;
	
	if (finder == current){
		finder = new Node<T>(object);
		finder->setNextPtr(frontPtr);
		frontPtr->setPrevPtr(finder);
		frontPtr = finder;
	}//if
	else{
		while (finder != 0){
			if(finder->getNextPtr() == current){
				helper = new Node<T> (object);
				(finder->getNextPtr())->setPrevPtr(helper);
				helper->setNextPtr(finder->getNextPtr());
				finder->setNextPtr(helper);
				helper->setPrevPtr(finder);
			}//if
			else {
				finder = finder->getNextPtr();
			}//else
		}//while
	}//else
	sz++;
}

template <typename T>
void List<T>::addAll(const List<T> &list) throw (bad_alloc){
	Node<T> *copier = list.frontPtr;
	if (empty()) {
		backPtr = new Node<T> (*copier);
		frontPtr = backPtr;
		copier = copier->getNextPtr(); 
		while(copier != 0){
			backPtr->setNextPtr(new Node<T> (*copier));
			(backPtr->getNextPtr())->setPrevPtr(backPtr);
			backPtr = backPtr->getNextPtr();
			copier = copier->getNextPtr();
		}
	}
	else {
		while(copier != 0){
			backPtr->setNextPtr(new Node<T> (*copier));
			(backPtr->getNextPtr())->setPrevPtr(backPtr);
			backPtr = backPtr->getNextPtr();
			copier = copier->getNextPtr();
		}
	}
	sz = sz + list.sz;
}                

template <typename T>
void List<T>::addAll(unsigned index, const List<T> &list) throw (bad_alloc, out_of_range){
	if (index >= sz) 
		throw out_of_range("out of range -- addall");
	
	if (index == (sz - 1)) 
		addAll(list);
	else {
		Node<T> *copier = list.frontPtr;
		Node<T> *finder = (getIthNode(index)->getPrevPtr());	
		Node<T> *holder = getIthNode(index);
	
		while (copier != 0) {
			finder->setNextPtr(new Node<T> (*copier));
			(finder->getNextPtr())->setPrevPtr(finder);
			finder = finder->getNextPtr();
			copier = copier->getNextPtr();
		}
		finder->setNextPtr(holder);
		holder->setPrevPtr(finder);
		sz += list.sz;
	}   
}

template <typename T>
void List<T>::addFirst(const T &object) throw (bad_alloc){
	if (empty()) {
		frontPtr = new Node<T>(object);
		backPtr = frontPtr;
		frontPtr->setNextPtr(0);
		frontPtr->setPrevPtr(0);
		sz++;
	}
	else {
		frontPtr->setPrevPtr(new Node<T> (object));
		(frontPtr->getPrevPtr())->setNextPtr(frontPtr);
		frontPtr = frontPtr->getPrevPtr();
		frontPtr->setPrevPtr(0);
		sz++;
	}       
	
}

template <typename T>
void List<T>::addLast(const T &object) throw (bad_alloc){
	if (empty()) {
		backPtr = new Node<T>(object);
		frontPtr = backPtr;
		frontPtr->setNextPtr(0);
		frontPtr->setPrevPtr(0);
		sz++;
	}
	else {
		backPtr->setNextPtr(new Node<T> (object));
		(backPtr->getNextPtr())->setPrevPtr(backPtr);
		backPtr = backPtr->getNextPtr();
		backPtr->setNextPtr(0);
		sz++;
	}
}                

template <typename T>
void List<T>::clear(){
	Node<T> *finder = frontPtr->getNextPtr();
	delete frontPtr;
	
	while (finder->getNextPtr() != 0) {
		finder = finder->getNextPtr();
		delete finder->getPrevPtr();
	}
	
	delete finder;
	finder = 0;
	initialize();
}                

template <typename T>
bool List<T>::contains(const T & object) const{
	Node<T> *finder = frontPtr;
	
	while (finder != 0) {
			if (finder->getObject() == object) {
				return true;
			}
			else {
				finder = finder->getNextPtr();
			}
	}
	return false;
}                

template <typename T>
bool List<T>::empty() const{
	if (frontPtr == 0) 
		return true;
	else { 
		return false;
	}
}

template <typename T>
int List<T>::indexOf(const T &object) const{
	Node<T> *finder = frontPtr;
	int index = -1;
	int counter = 0;
	
	while (finder != 0) {
		if (finder->getObject() == object){ 
			index = counter;
			return index;
		}
		else {
			finder = finder->getNextPtr();
			counter++;
		}

		
	}
	return index;
}                

template <typename T>
T & List<T>::get(unsigned index) const throw(out_of_range){
	if (empty() || index >= sz) 
		throw out_of_range("out of range -- get()");
	
	Node<T> *finder = frontPtr;
	unsigned counter = 0;
	
	while (counter < index) {
		finder = finder->getNextPtr();
		counter++;
	}
	return finder->getObject();
}                

template <typename T>
T & List<T>::getFirst() const throw (NoSuchObject){
	if (frontPtr==0) {
		throw NoSuchObject("No Such Object -- getFirst()");
	}
	else {
		return frontPtr->getObject();
	}
}                

template <typename T>
T & List<T>::getLast() const throw (NoSuchObject){
	if (backPtr == 0)
		throw NoSuchObject("NO such object -- getLast()");

	return backPtr->getObject();
}                

template <typename T>
ListIterator<T> List<T>::listIterator(){
	ListIterator<T> iter;
	iter.current=frontPtr;
	iter.atFront = true;
	return iter;
}                

template <typename T>
ListIterator<T> List<T>::listIterator(unsigned index) throw (out_of_range){
	if (index >= sz) throw out_of_range("OUT OF RANGE -- LISTITERATOR");
	
	ListIterator<T> iter;
	iter.current=getIthNode(index);
	return iter;
}                

template <typename T>
T List<T>::remove(unsigned index) throw(out_of_range){
	if (index >= sz) 
		throw out_of_range("out of range -- remove(index)");
	
	if (index == 0) {
		removeFirst();
	}
	
	else if (index == (sz -1)) {
		removeLast();
	}
	else {
		Node<T> *finder = frontPtr;
		T object;
	
		unsigned counter = 0;
	
		while (counter < index) {
			finder = finder->getNextPtr();
			counter++;
		}
	
		object = remove(finder);
		return object;
	}
}                

template <typename T>
T List<T>::remove (Node<T>* ptr){
	if (ptr->getNextPtr() == 0) {
		return removeLast();
	}
	
	else if (ptr->getPrevPtr() == 0) {
		return removeFirst();
	}
	
	else {
		Node<T> *next = ptr->getNextPtr();
		Node<T> *prev = ptr->getPrevPtr();
		T object = ptr->getObject();
	
		next->setPrevPtr(prev);
		prev->setNextPtr(next);
	
		delete ptr;
		sz--;
		
		return object;
	}
	
}

template <typename T>
T List<T>::removeFirst() throw (NoSuchObject){
	if (frontPtr == 0) 
                throw NoSuchObject("no such object -- removeLast()");

	T object;
	
	if (frontPtr->getNextPtr() == 0) {
		object = frontPtr->getObject();
		delete frontPtr;
		initialize();
		return object;
	}
	else {
		object = frontPtr->getObject();
		frontPtr = frontPtr->getNextPtr();
		delete frontPtr->getPrevPtr();
		frontPtr->setPrevPtr(0);
		sz--;
		return object;
	}
}                

template <typename T>
T List<T>::removeFirstOccurrence(const T &object) throw(NoSuchObject){
	if (!contains(object)) 
		throw NoSuchObject("No such object -- removeFirstOccurrance()");
	
	Node<T>* finder = getIthNode(indexOf(object));
	
	T outobject;
	outobject = remove(finder);
	return outobject;
}  

template <typename T>
T List<T>::removeLast() throw (NoSuchObject){
	if (backPtr == 0) 
		throw NoSuchObject("no such object -- removeLast()");

	T object;
	
	if (backPtr->getPrevPtr() == 0) {
		object = backPtr->getObject();
		delete backPtr;
		initialize();
		return object;
	}
	else {
		object = backPtr->getObject();
		backPtr = backPtr->getPrevPtr();
		delete backPtr->getNextPtr();
		backPtr->setNextPtr(0);
		sz--;
		return object;
	}
}                

template <typename T>
T List<T>::removeLastOccurrence(const T &object) throw(NoSuchObject){
	if (!contains(object)) 
		throw NoSuchObject("No such object -- removeFirstOccurrance()");
	
	Node<T>* finder = backPtr;
	
	bool found = false;
	while (found == false) {
		if (finder->getObject() == object){ 
			found = true;
		}
		else {
			finder = finder->getPrevPtr();
		}
	}
	
	T outobject=remove(finder);
	
	return outobject;
} 

template <typename T>
T List<T>::set(unsigned index, const T& inobject) throw (out_of_range){
	T outobject;
	
	if (index >= sz)
		throw out_of_range("out of range -- set()");
	
	Node<T>* finder = getIthNode(index);
	outobject = finder->getObject();
	
	finder->setObject(inobject);
	
	return outobject;
}                

template <typename T>
unsigned List<T>::size() const{
	return sz;
}                

template <typename T>
T* List<T>::toArray() const throw(bad_alloc){
	T* array[sz];
	Node<T>* finder = frontPtr;
	
	for (unsigned i=0; i<sz; i++) {
		array[i] = &(finder->getObject());
		finder = finder->getNextPtr();
	}
	return array[sz];
}                

template <typename T>
const List<T>& List<T>::operator= (const List<T> &s) throw (bad_alloc){
	if (&s != this){
		if(!empty()) clear();
		sz = s.sz;
		Node<T> *sptr = s.frontPtr;
		Node<T> *last = 0;
		while (sptr != 0){
			if(last == 0){
				frontPtr = new Node<T>(*sptr);
				last = frontPtr;
			}
			else{
				last->setNextPtr(new Node<T> (*sptr));
				last = last->getNextPtr();
			}//else
			sptr = sptr->getNextPtr();
		}//while
	}//if
	return *this;
}              

template <typename T>
ostream & operator<<( ostream & out, const List<T>& list){
	cout << "["<<(list.getIthNode(0))->getObject();
	for (unsigned i=1; i<list.sz; i++) {
		out<<", "<<(list.getIthNode(i))->getObject();
	}
	cout <<"]"<<endl;
	return out;
}

template <typename T>
void List<T>::printInternal(ostream &){
	Node<T> *current = frontPtr;
	int i=0;
	cout<<"front: "<<frontPtr<<endl;
	while (current !=0){
		cout<<"Node: "<<i<<"   contents: "<<current->getObject()<<endl;
		//cout<<"Address: "<<current <<endl;
		//cout<<"next ptr: "<<current->getNextPtr()<<endl;
		current = current->getNextPtr();
		i++;
	}//while
	cout << "size: "<<sz<<endl;
}                

template <typename T>
List<T>::~List(){
	clear();
	initialize();
}

template <typename T>
void List<T>::initialize(){
	frontPtr = 0;
	backPtr = 0;
	sz = 0;
}

template <typename T>
Node<T>* List<T>::getIthNode(unsigned index) const throw (out_of_range){
	if (index >= sz){
		throw out_of_range("out of range getIthNode");
	}
	if (index == 0) {
		return frontPtr;
	}
	
	Node<T> *finder = frontPtr;
	
	for(unsigned i=0; i<index; i++){
		finder = finder->getNextPtr();
	}
	
	return finder;
}


#endif


