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
//quick note: I am aware that this code does not work for arrays where the size needs to be increased or decreased, the code does pass the test cases which grants it 50%
//however, upon further testing I realize there was a issue with the way I went about the lab. I tried debugging however I could but that was all I managed to come up with, I asked
//around and found other ways to do the lab but did not have enough time to implement it-decided to stick with this version
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
		// Your member variables
		
		int list_head;
		int list_tail;
		int list_size;
		int list_capacity;
		int initial_list_capacity;
		int capacity_factor;
		int* next;
		int* previous;
		int* stack;
		int* temp;
		Type* storage;
		//there are a lot of private variables in my version of the deque as I essentially implemented a linked list with using three arrays, one storing the node value, one
		//storing the value of the next node, and one storing the value of the previous node-a stack array was also formed to find empty nodes

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
// Your initalization list
{
	if (n<=16){ //if the input for the array size is less than 16, the default minimum is 16
		list_head=nullptr;
		list_tail=nullptr;
		list_size=0;
		capacity_factor=1;
		initial_list_capacity=16;
		list_capacity=initial_list_capacity*capacity_factor;
		stack=new int[list_capacity];
		next=new int[list_capacity];
		previous=new int[list_capacity];
		temp=new int[list_capacity];			//initializing the arrays 
		for (int i=0; i<=list_capacity-1;++i){
			stack[i]=list_capacity-1-i;
		}
		storage=new Type[list_capacity]; //initializing the arrays and creating a stack which contains all of the values below the list_capacity as all of the spaces in the array
										 //are free for push_front and push_back 
	}
	else{
		
		list_head=nullptr;
		list_tail=nullptr;
		list_size=0;
		capacity_factor=1;
		initial_list_capacity=n;
		list_capacity=initial_list_capacity*capacity_factor;
		stack=new int[list_capacity];
		next=new int[list_capacity];
		previous=new int[list_capacity];
		temp=new int[list_capacity];			//initializing the arrays
		for (int i=0; i<=capacity()-1;++i){
			stack[i]=list_capacity-1-i;
		}
		storage=new Type[list_capacity]; //initializing the arrays and creating a stack which contains all of the values below the list_capacity as all of the spaces in the array
										 //are free for push_front and push_back 
	}
	
	
}

// Copy Constructor
template <typename Type>
Resizable_deque<Type>::Resizable_deque( Resizable_deque const &deque )
// Your initalization list
{
	list_head=nullptr;
	list_tail=nullptr;
	list_size=0;
	capacity_factor=1;
	initial_list_capacity=deque.initial_list_capacity;
	list_capacity=initial_list_capacity*capacity_factor;
	stack=new int[list_capacity];
	next=new int[list_capacity];
	previous=new int[list_capacity];
	temp=new int[list_capacity];			//initializing the list which will copy the values in deque
	for (int i=0; i<=list_capacity-1;++i){
		stack[i]=list_capacity-1-i;
	}
	storage=new Type[list_capacity];
	
	
	if (deque.empty()) {
		return;
	}

	for (int i=deque.list_head;i!=nullptr;i=deque.next[i]){
		push_back(deque.storage[i]);		//calling push_back to copy the values into the deque
	}
}

// Move Constructor
template <typename Type>
Resizable_deque<Type>::Resizable_deque( Resizable_deque &&deque )
// Your initalization list
{
	
	list_head=nullptr;
	list_tail=nullptr;
	list_size=0;
	capacity_factor=1;
	initial_list_capacity=deque.initial_list_capacity;
	list_capacity=initial_list_capacity*capacity_factor;
	stack=new int[list_capacity];
	next=new int[list_capacity];
	previous=new int[list_capacity];
	temp=new int[list_capacity];
	for (int i=0; i<=list_capacity-1;++i){
		stack[i]=list_capacity-1-i;
	}
	storage=new Type[list_capacity]; //initializng the constuctor which deque will be moved into
	
	list_head=deque.list_head;
	deque.list_head=nullptr;
	list_tail=deque.list_tail;
	deque.list_tail=nullptr;
	list_size=deque.list_size;
	deque.list_size=0;
	list_capacity=deque.list_capacity;
	deque.list_capacity=0;
	initial_list_capacity=deque.initial_list_capacity;
	deque.initial_list_capacity=0;
	capacity_factor=deque.capacity_factor;
	deque.capacity_factor=0;
	next=deque.next;
	deque.next=nullptr;
	previous=deque.previous;
	deque.previous=nullptr;
	stack=deque.stack;
	deque.stack=nullptr;
	storage=deque.storage;
	deque.storage=nullptr;
	temp=deque.temp;
	deque.temp=nullptr;	//moving the values from deque into the newly created array
}

// Destructor
template <typename Type>
Resizable_deque<Type>::~Resizable_deque() {
	clear();
	delete [] storage;
	delete [] stack;
	delete [] next;
	delete [] previous;
	delete [] temp;	//deleting all of the allocated arrays for maintaining memory
}

/////////////////////////////////////////////////////////////////////////
//                     Public Member Functions                         //
/////////////////////////////////////////////////////////////////////////

template <typename Type>
int Resizable_deque<Type>::size() const {

	return list_size;	//returns the size of the list
}

template <typename Type>
int Resizable_deque<Type>::capacity() const {
	return list_capacity; //returns the current capacity of the list
}

template <typename Type>
bool Resizable_deque<Type>::empty() const {
	return (list_size==0);	//returns whether or not the list is empty, is the list size is 0, the list is empty

}

template <typename  Type>
Type Resizable_deque<Type>::front() const {
	if (empty()){
		throw underflow(); //throw underflow for error checking
	}
	
	return storage[list_head];	//returns the first value in the array

}

template <typename  Type>
Type Resizable_deque<Type>::back() const {
	
	if (empty()){
		throw underflow(); //throw underflow for error checking
	}
	return storage[list_tail]; //returns the last value in the array
}

template <typename Type>
void Resizable_deque<Type>::swap( Resizable_deque<Type> &deque ) {
	// Swap the member variables
	//     std::swap( variable, deque.variable );
	std::swap(list_head,deque.list_head);
	std::swap(list_tail,deque.list_tail);
	std::swap(list_size,deque.list_size);
	std::swap(list_capacity,deque.list_capacity);
	std::swap(initial_list_capacity,deque.initial_list_capacity);
	std::swap(capacity_factor,deque.capacity_factor);
	std::swap(next,deque.next);
	std::swap(previous,deque.previous);
	std::swap(stack,deque.stack);
	std::swap(storage,deque.storage);
	std::swap(temp,deque.temp); //swapping the private variables
	
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
	if (list_size==capacity()){
		capacity_factor=capacity_factor*2;
		
	}
	
	if (list_head==nullptr){			//if the list is empty, the new node is the list_head and the list_tail
		list_head=stack[0];				//assigning the first value on the stack to the list_head, the stack indicates which nodes are open
		list_tail=stack[0];				//if the list is empty list_head is equal to the list_tail
		storage[list_head]=obj;
		next[list_head]=nullptr;
		previous[list_head]=nullptr;
		list_size++;
		
		for (int i=0; i<=capacity()-list_size-1;++i){
			stack[i]=stack[i+1];		
		}
		stack[capacity()-list_size]=nullptr;	//removing the first value from the stack after it was assigned
		
	}
	else {
		next[stack[0]]=list_head;
		previous[list_head]=stack[0];			//placing stack[0] in front of the original list_head
		previous[stack[0]]=nullptr;
		list_head=stack[0];						//setting list_head to the value of stack[0]
		storage[list_head]=obj;
		list_size++;
		
		for (int i=0; i<=capacity()-list_size-1;++i){
			stack[i]=stack[i+1];				
		}
		stack[capacity()-list_size]=nullptr;	//removing the first value from the stack after it was assigned
	}
}

template <typename Type>
void Resizable_deque<Type>::push_back( Type const &obj ) {
	if (list_size==capacity()){
		capacity_factor=capacity_factor*2;			//double capacity if the list_size equals capacity, I haven't managed to get this to work yet through my efforts
	}
	
	if (list_tail==nullptr){
		list_head=stack[0];
		list_tail=stack[0];				//if the list is empty, stack[0] is an available node and is assigned to list_head, list_tail
		storage[list_tail]=obj;
		next[list_tail]=nullptr;
		previous[list_tail]=nullptr;
		list_size++;
		
		for (int i=0; i<=capacity()-list_size-1;++i){
			stack[i]=stack[i+1];
		}
		stack[capacity()-list_size]=nullptr;	//removing the first value from the stack after it was assigned
	}
		
	else {
		previous[stack[0]]=list_tail;
		next[list_tail]=stack[0];		//placing the new node behind the original list_tail
		next[stack[0]]=nullptr;
		list_tail=stack[0];				//assigning list_tail to the first available node on the stack
		list_head=list_head;
		storage[list_tail]=obj;
		list_size++;
		
		for (int i=0; i<=capacity()-list_size-1;++i){
			stack[i]=stack[i+1];
		}
		stack[capacity()-list_size]=nullptr;	//removing the first value from the stack after it was assigned
	}
}

template <typename Type>
void Resizable_deque<Type>::pop_front() {
	
	if (empty()){
		throw underflow();	//throw underflow for error checking
	}
		
	if ((list_size-1)==0.25*list_capacity&&list_capacity>initial_list_capacity){
		capacity_factor=capacity_factor*0.5; //decrement the capacity is the list size is less than 1/4 the list_capacity-haven't managed to implement it
	}
	
	for (int i=0; i<=capacity()-list_size-1;++i){
			stack[i+1]=stack[i];
		}
	stack[0]=list_head;
	list_head=next[list_head]; //assigning the list_head as the node after the list_head
	list_size--;
	
}

template <typename Type>
void Resizable_deque<Type>::pop_back() {
	
	if (empty()){
		throw underflow();	//throw underflow for error checking
	}
	
	if ((list_size-1)==0.25*list_capacity&&list_capacity>initial_list_capacity){
		capacity_factor=capacity_factor*0.5; //decrement the capacity is the list size is less than 1/4 the list_capacity-haven't managed to implement it
	}
		
	for (int i=0; i<=capacity()-list_size-1;++i){
			stack[i+1]=stack[i];
		}
	stack[0]=list_tail;
	list_tail=previous[list_tail]; //assigning list_tail to the node before the original list_tail
	list_size--;
}

template <typename Type>
void Resizable_deque<Type>::clear() {
	list_head=nullptr;
	list_tail=nullptr;
	list_size=0;
	capacity_factor=1;
	list_capacity=initial_list_capacity*capacity_factor; //capacity factor was changed back to 1 for changing list_size, however, I am aware of the method we are supposed to use
														 //but my endeavours in implementing this method were unfruitful
	for (int i=0; i<=capacity()-1;++i){
			stack[i]=nullptr;
			next[i]=nullptr;
			storage[i]=nullptr; //assigning null pointer to all the values in the array for reset
	}
}
/////////////////////////////////////////////////////////////////////////
//                      Private member functions                       //
/////////////////////////////////////////////////////////////////////////

// Enter any private member functios (helper functions) here


/////////////////////////////////////////////////////////////////////////
//                               Friends                               //
/////////////////////////////////////////////////////////////////////////

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator<<( std::ostream &out, Resizable_deque<T> const &list ) {
	out << "not yet implemented";

	return out;
}

#endif
