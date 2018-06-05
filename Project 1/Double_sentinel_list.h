/*****************************************
 * UW User ID:  l67ma
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  (Winter) 2018
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

#ifndef DOUBLE_SENTINEL_LIST_H
#define DOUBLE_SENTINEL_LIST_H

#include <iostream>
#include "Exception.h"

template <typename Type>
class Double_sentinel_list {
	public:
		class Double_node {
			public:
				Double_node( Type const & = Type(), Double_node * = nullptr, Double_node * = nullptr );

				Type value() const;
				Double_node *previous() const;
				Double_node *next() const;

				Type         node_value;
				Double_node *previous_node;
				Double_node *next_node;
		};

		Double_sentinel_list();
		Double_sentinel_list( Double_sentinel_list const & );
		Double_sentinel_list( Double_sentinel_list && );
		~Double_sentinel_list();

		// Accessors

		int size() const;
		bool empty() const;

		Type front() const;
		Type back() const;

		Double_node *begin() const;
		Double_node *end() const;
		Double_node *rbegin() const;
		Double_node *rend() const;

		Double_node *find( Type const & ) const;
		int count( Type const & ) const;

		// Mutators

		void swap( Double_sentinel_list & );
		Double_sentinel_list &operator=( Double_sentinel_list );
		Double_sentinel_list &operator=( Double_sentinel_list && );

		void push_front( Type const & );
		void push_back( Type const & );

		void pop_front();
		void pop_back();

		int erase( Type const & );

	private:
		Double_node *list_head;
		Double_node *list_tail;
		int list_size;

		// List any additional private member functions you author here
	// Friends

	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Double_sentinel_list<T> const & );
};

/////////////////////////////////////////////////////////////////////////
//                      Public member functions                        //
/////////////////////////////////////////////////////////////////////////

template <typename Type>
Double_sentinel_list<Type>::Double_sentinel_list():
// Updated the initialization list here
list_head(new Double_node()),
list_tail(new Double_node()),
list_size(0)
{	
	list_head->next_node=list_tail;
	list_tail->previous_node=list_head;

}

template <typename Type>
Double_sentinel_list<Type>::Double_sentinel_list( Double_sentinel_list<Type> const &list ):
// Updated the initialization list here
list_head( new Double_node() ),
list_tail( new Double_node() ),
list_size( 0 )
{
	list_head->next_node=list_tail;
	list_tail->previous_node=list_head;
	
	if (list.empty()) {
		return;
	}
	
	for (Double_node*ptr=list.begin();ptr!=list.end();ptr=ptr->next_node){
		push_back(ptr->node_value);
	}

}

template <typename Type>
Double_sentinel_list<Type>::Double_sentinel_list( Double_sentinel_list<Type> &&list ):
// Updated the initialization list here
list_head( new Double_node() ),
list_tail( new Double_node() ),
list_size( 0 )
{
	
	
	list_head->next_node=list_tail;
	list_tail->previous_node=list_head;
	
	list_head=list.list_head;
	list.list_head=nullptr;
	list_tail=list.list_tail;
	list.list_tail=nullptr;
	list_size=list.list_size;
	list.list_size=0;
	
	
	
	/*
	while (!empty()){
		push_back(list.begin()->node_value);
		list.pop_front();
	}
	*/
	
}

template <typename Type>
Double_sentinel_list<Type>::~Double_sentinel_list() {
	while(!empty()){
		pop_front();
	}
	delete list_head;
	delete list_tail;
}

template <typename Type>
int Double_sentinel_list<Type>::size() const {
	int count=0;
	for (Double_node *ptr=begin();ptr!=end();ptr=ptr->next_node){
		++count;
	}
	return count;
}

template <typename Type>
bool Double_sentinel_list<Type>::empty() const {
	if (list_head==nullptr||list_tail==nullptr){
		return 1;
	}
	return (list_head->next_node==list_tail); // This returns true if the node after the front sentinel node is empty indicating that the list is empty
}

template <typename Type>
Type Double_sentinel_list<Type>::front() const {
	
	if (empty()){
		throw underflow();
	}
	
	return begin()->value(); // This returns the value of the next node the front sentinel node points to
}

template <typename Type>
Type Double_sentinel_list<Type>::back() const {
	
	if (empty()){
		throw underflow();
	}
	
	return rbegin()->value(); // This returns the value of the previous node the sentinel node points to
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::begin() const {

	return list_head->next_node;
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::end() const {

	return list_tail;
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::rbegin() const {

	return list_tail->previous_node;
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::rend() const {

	return list_head;
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::find( Type const &obj ) const {
	for (Double_node*ptr=begin();ptr!=end();ptr=ptr->next()){
		if (ptr->node_value==obj){
			return ptr;
		}
	}
}

template <typename Type>
int Double_sentinel_list<Type>::count( Type const &obj ) const {
	int node_count=0;
	for (Double_node*ptr=begin();ptr!=end();ptr=ptr->next()){
		if (ptr->node_value==obj){
			++node_count;
		}
	}
	return node_count;
}

template <typename Type>
void Double_sentinel_list<Type>::swap( Double_sentinel_list<Type> &list ) {
	// This is done for you
	std::swap( list_head, list.list_head );
	std::swap( list_tail, list.list_tail );
	std::swap( list_size, list.list_size );
}

template <typename Type>
Double_sentinel_list<Type> &Double_sentinel_list<Type>::operator=( Double_sentinel_list<Type> rhs ) {
	// This is done for you
	swap( rhs );

	return *this;
}

template <typename Type>
Double_sentinel_list<Type> &Double_sentinel_list<Type>::operator=( Double_sentinel_list<Type> &&rhs ) {
	// This is done for you
	swap( rhs );

	return *this;
}

template <typename Type>
void Double_sentinel_list<Type>::push_front( Type const &obj ) {
	if (empty()) {
		Double_node*temp=new Double_node();
		temp->node_value=obj;
		temp->next_node=list_tail;
		temp->previous_node=list_head;
		list_head->next_node=temp;
		list_tail->previous_node=temp;
	} 
	
	else {
	
		Double_node *temp=new Double_node;
		temp->node_value=obj;
		temp->next_node=list_head->next_node;
		list_head->next_node=temp;
		temp->previous_node=list_head;
		list_head->next_node->next_node->previous_node=temp;
	}
}

template <typename Type>
void Double_sentinel_list<Type>::push_back( Type const &obj ) {
		if (empty()) {
		Double_node *temp=new Double_node;
		temp->node_value=obj;
		temp->previous_node=list_head;
		temp->next_node=list_tail;
		list_tail->previous_node=temp;
		list_head->next_node=temp;
	} 
	
	else {

		Double_node *temp=new Double_node;
		temp->node_value=obj;
		temp->previous_node=list_tail->previous_node;
		list_tail->previous_node=temp;
		temp->next_node=list_tail;
		list_tail->previous_node->previous_node->next_node=temp;
	}
}

template <typename Type>
void Double_sentinel_list<Type>::pop_front() {
		
	if (empty()){
		throw underflow();
	}
	
	else{
		Double_node *tmp=list_head->next_node;
		list_head->next_node=tmp->next_node;
		
		
		for (Double_node *ptr=rbegin();ptr!=rend();ptr=ptr->previous_node){
			if (ptr->previous_node->previous_node==rend()){
				ptr->previous_node=ptr->previous_node->previous_node;
			}
		}
		delete tmp;
	}
	
}

template <typename Type>
void Double_sentinel_list<Type>::pop_back() {
	if (empty()){
		throw underflow();
	}
	else{
		Double_node *tmp=list_tail->previous_node;
		list_tail->previous_node=tmp->previous_node;
		
		for (Double_node *ptr=begin();ptr!=end();ptr=ptr->next_node){
			if (ptr->next_node->next_node==end()){
				ptr->next_node=ptr->next_node->next_node;
			}
		}
		delete tmp;
	}
}

template <typename Type>
int Double_sentinel_list<Type>::erase( Type const &obj ) {
	int node_count=0;
	for (Double_node *ptr=begin();ptr!=end();ptr=ptr->next_node){
		
		if (ptr->node_value==obj){
			ptr=ptr->previous_node;
			Double_node*tmp=ptr->next_node;
			ptr->next_node=tmp->next_node;
			
			++node_count;
			
			for (Double_node *ptr2=rbegin();ptr2!=rend();ptr2=ptr2->previous_node){
				if (obj==ptr2->node_value){
			
					ptr2->next_node->previous_node=ptr2->previous_node;
			
				}
			}	
			delete tmp;
			
		}
		
	}

	return node_count; 
}

template <typename Type>
Double_sentinel_list<Type>::Double_node::Double_node(
	Type const &nv,
	typename Double_sentinel_list<Type>::Double_node *pn,
	typename Double_sentinel_list<Type>::Double_node *nn ):
// Updated the initialization list here
node_value( nv ), // This assigns 'node_value' the default value of Type
previous_node(pn),
next_node(nn)
{
	//empty constructor
}

template <typename Type>
Type Double_sentinel_list<Type>::Double_node::value() const {
	// Enter your implementation here
	return node_value; // This returns a default value of Type
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::Double_node::previous() const {
	// Enter your implementation here
	return previous_node;
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::Double_node::next() const {
	// Enter your implementation here
	return next_node;
}

/////////////////////////////////////////////////////////////////////////
//                      Private member functions                       //
/////////////////////////////////////////////////////////////////////////

// If you author any additional private member functions, include them here

/////////////////////////////////////////////////////////////////////////
//                               Friends                               //
/////////////////////////////////////////////////////////////////////////

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator<<( std::ostream &out, Double_sentinel_list<T> const &list ) {
	out << "head";

	for ( typename Double_sentinel_list<T>::Double_node *ptr = list.rend(); ptr != nullptr; ptr = ptr->next() ) {
		if ( ptr == list.rend() || ptr == list.end() ) {
			out << "->S";
		} else {
			out << "->" << ptr->value();
		}
	}

	out << "->0" << std::endl << "tail";

	for ( typename Double_sentinel_list<T>::Double_node *ptr = list.end(); ptr != nullptr; ptr = ptr->previous() ) {
		if ( ptr == list.rend() || ptr == list.end() ) {
			out << "->S";
		} else {
			out << "->" << ptr->value();
		}
	}

	out << "->0";

	return out;
}

#endif
