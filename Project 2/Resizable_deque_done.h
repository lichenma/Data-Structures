/*****************************************
 * UW User ID:  l67ma
 * Submitted for ECE 250
 * Semester of Submission:  (Winter) 2018
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided  code.
 *****************************************/

#ifndef DYNAMIC_DEQUE_H
#define DYNAMIC_DEQUE_H

#include "Exception.h"

template <typename Type>
class Resizable_deque {
	public:
		Resizable_deque( int = 16 );
		Resizable_deque( Resizable_deque const & );
		Resizable_deque( Resizable_deque && );
		~Resizable_deque();

		Type front() const;
		Type back() const;
		int size() const;
		bool empty() const;
		int capacity() const;

		void swap( Resizable_deque & );
		Resizable_deque &operator=( Resizable_deque const& );
		Resizable_deque &operator=( Resizable_deque && );
		void push_front( Type const & );
		void push_back( Type const & );
		void pop_front();
		void pop_back();
		void clear();

	private:
		Type *new_storage;
		int new_list_head;
		int new_list_tail;
		int new_list_size;
		int new_initial_list_capacity;
		int new_list_capacity;

		// Any private member functions
		//   - helper functions for resizing your array?
		void resize(int list_size);

	// Friends

	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Resizable_deque<T> const & );
};

/////////////////////////////////////////////////////////////////////////
//                   Constructors and Destructors                      //
/////////////////////////////////////////////////////////////////////////

// Constructor
template <typename Type>
Resizable_deque<Type>::Resizable_deque( int n )
//empty initialization
{
	new_list_head=0;
	new_list_tail=0;
	new_list_size=0;
	
	if (n<0){
		throw underflow();
	}
	if (n <=16) {
		
		new_storage = new Type[16];
		new_initial_list_capacity = 16;
		new_list_capacity = 16;
	}
	
	if (n>16) {
		
		new_storage = new Type[n];
	}
}

// Copy Constructor
template <typename Type>
Resizable_deque<Type>::Resizable_deque( Resizable_deque const &deque )
//empty initilization
{
	new_storage = new Type[deque.new_list_capacity];
	new_initial_list_capacity=deque.new_initial_list_capacity;
	new_list_capacity=deque.new_list_capacity;
	new_list_head=deque.new_list_head;
	new_list_tail=deque.new_list_tail;
	new_list_size=deque.new_list_size;
	
	for (int i = deque.new_list_head; i != deque.new_list_tail; i = (i+1) % deque.new_list_capacity) {
		new_storage[i] = deque.new_storage[i];
	}
	new_storage[new_list_tail] = deque.new_storage[deque.new_list_tail];
}

// Move Constructor
template <typename Type>
Resizable_deque<Type>::Resizable_deque( Resizable_deque &&deque )
{
	new_storage = new Type[16];
	new_initial_list_capacity=16;
	new_list_capacity=16;
	new_list_head=0;
	new_list_tail=0;
	new_list_size=0;
	
	new_storage=deque.new_storage;
	deque.new_storage=nullptr;
	new_list_head=deque.new_list_head;
	deque.new_list_head=nullptr;
	new_list_tail=deque.new_list_tail;
	deque.new_list_tail=nullptr;
	new_list_size=deque.new_list_size;
	deque.new_list_size=nullptr;
	new_initial_list_capacity=deque.new_initial_list_capacity;
	deque.new_initial_list_capacity=nullptr;
	new_list_capacity=deque.new_list_capacity;
	deque.new_list_capacity=nullptr;
	
}

// Destructor
template <typename Type>
Resizable_deque<Type>::~Resizable_deque() {
	delete[] new_storage;
}

/////////////////////////////////////////////////////////////////////////
//                     Public Member Functions                         //
/////////////////////////////////////////////////////////////////////////

template <typename Type>
int Resizable_deque<Type>::size() const {
	return new_list_size;
}

template <typename Type>
int Resizable_deque<Type>::capacity() const {
	return new_list_capacity;
}

template <typename Type>
bool Resizable_deque<Type>::empty() const {
	return(new_list_size==0);
}

template <typename  Type>
Type Resizable_deque<Type>::front() const {
	if (empty()) {
		throw underflow();
	}
	else {
		return new_storage[new_list_head];
	}
}

template <typename  Type>
Type Resizable_deque<Type>::back() const {
	if (empty()) {
		throw underflow();
	}
	else {
		return new_storage[new_list_tail];
	}
}

template <typename Type>
void Resizable_deque<Type>::swap( Resizable_deque<Type> &deque ) {
	std::swap(new_storage, deque.new_storage);
	std::swap(new_list_head, deque.new_list_head);
	std::swap(new_list_tail, deque.new_list_tail);
	std::swap(new_list_size, deque.new_list_size);
	std::swap(new_initial_list_capacity, deque.new_initial_list_capacity);
	std::swap(new_list_capacity, deque.new_list_capacity);
}

template <typename Type>
Resizable_deque<Type> &Resizable_deque<Type>::operator=( Resizable_deque<Type> const &rhs ) {
	// This is done for you...
	Resizable_deque<Type> copy( rhs );
	swap( copy );

	return *this;
}

template <typename Type>
Resizable_deque<Type> &Resizable_deque<Type>::operator=( Resizable_deque<Type> &&rhs ) {
	// This is done for you...
	swap( rhs );

	return *this;
}

template <typename Type>
void Resizable_deque<Type>::push_front( Type const &obj ) {
	
	if (new_list_size == new_list_capacity) {
		resize(new_list_capacity*2);
		push_front(obj);
	}
	
	else if (empty()) {
		new_storage[new_list_head] = obj;
		new_list_size++;
	}
	else if (!empty()){
		int new_front = (new_list_capacity + (new_list_head - 1)) % new_list_capacity;
		new_storage[new_front] = obj;
		new_list_head = new_front;
		new_list_size++;
	}
}

template <typename Type>
void Resizable_deque<Type>::push_back( Type const &obj ) {
	
	if (new_list_size >= new_list_capacity) {
		resize(2 * new_list_capacity);
		push_back(obj);
	}
	else if (empty()) {
		new_storage[new_list_tail] = obj;
		new_list_size++;
	}
	
	
	
	else if (!empty()) {
		int new_back = (new_list_capacity + (new_list_tail + 1)) % new_list_capacity;
		new_storage[new_back] = obj;
		new_list_tail = new_back;
		new_list_size++;
	}
}

template <typename Type>
void Resizable_deque<Type>::pop_front() {

	if (new_list_head != new_list_tail) {
		new_list_head = ++new_list_head % new_list_capacity;
		new_list_size--;
	} 
	else if (new_list_size == 1) {
		new_list_size--;
	} 
	else {
		throw underflow();
	}

	if (new_list_size <= (new_list_capacity / 4) && new_list_capacity != new_initial_list_capacity) {
		resize(new_list_capacity / 2);
	}
}

template <typename Type>
void Resizable_deque<Type>::pop_back() {

	if (new_list_tail != new_list_head) {
		new_list_tail = ((--new_list_tail % new_list_capacity) + new_list_capacity) % new_list_capacity;
		new_list_size--;
	}
	else if (new_list_size == 1) {
		new_list_size--;
	}
	else {
		throw underflow();
	}

	if (new_list_size <= (new_list_capacity / 4) && new_list_capacity > new_initial_list_capacity) {
		resize(new_list_capacity / 2);
	}
}

template <typename Type>
void Resizable_deque<Type>::clear() {
	Type* clear = new Type[new_initial_list_capacity];
	auto tmp = new_storage;
	new_storage = clear;

	new_list_capacity = new_initial_list_capacity;
	new_list_head = 0;
	new_list_tail = 0;
	new_list_size = 0;

	delete[] tmp;
}

/////////////////////////////////////////////////////////////////////////
//                      Private member functions                       //
/////////////////////////////////////////////////////////////////////////

template <typename Type>
void Resizable_deque<Type>::resize(int list_size) {
	Type* new_array = new Type[list_size];

	int count = 0;
	for (int i = new_list_head; i != new_list_tail; i = (i+1) % new_list_capacity) {
		new_array[count] = new_storage[i];
		count++;
	}
	new_array[count] = new_storage[new_list_tail];

	auto tmp = new_storage;
	new_storage = new_array;
	delete[] tmp;

	new_list_head = 0;
	new_list_tail = count;
	new_list_capacity = list_size;
}


/////////////////////////////////////////////////////////////////////////
//                               Friends                               //
/////////////////////////////////////////////////////////////////////////

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator<<(std::ostream &out, Resizable_deque<T> const &list) {
	out << "not yet implemented";


	return out;
}

#endif
