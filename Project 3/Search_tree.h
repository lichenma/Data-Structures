/*****************************************
 * UW User ID:  l67ma
 * Submitted for ECE 250
 * Semester of Submission:  (Winter) 2018
 *
 * By  submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

#ifndef RPOVDGBQN9TIEO3P
#define RPOVDGBQN9TIEO3P

#include "Exception.h"
#include "ece250.h"
#include <cassert>

template <typename Type>
class Search_tree {
	public:
		class Iterator;

	private:
		class Node {
			public:
				Type node_value;
				int tree_height;

				// The left and right sub-trees
				Node *left_tree;
				Node *right_tree;

				// Hint as to how you can create your iterator
				// Point to the previous and next nodes in linear order
				Node *previous_node;
				Node *next_node;

				// Member functions
				Node( Type const & = Type() );

				void update_height();

				int height() const;
				bool is_leaf() const;
				Node *front();
				Node *back();
				Node *find( Type const &obj );

				void clear();
				bool insert( Type const &obj, Node *&to_this );
				bool erase( Type const &obj, Node *&to_this );
				
				void rotateR(Node*node,Node*&y); 
				void rotateL(Node*node,Node*&x);
				void rotateLR(Node*node,Node*&y);
				void rotateRL(Node*node,Node*&x);
				//created the rotate functions to deal with the rotations in order to maintain the AVL tree
		};

		Node *root_node;
		int tree_size;

		// Hint as to how to start your linked list of the nodes in order 
		Node *front_sentinel;
		Node *back_sentinel;

	public:
		class Iterator {
			private:
				Search_tree *containing_tree;
				Node *current_node;
				bool is_end;

				// The constructor is private so that only the search tree can create an iterator
				Iterator( Search_tree *tree, Node *starting_node );

			public:
				// DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
				Type operator*() const;
				Iterator &operator++();
				Iterator &operator--();
				bool operator==( Iterator const &rhs ) const;
				bool operator!=( Iterator const &rhs ) const;

			// Make the search tree a friend so that it can call the constructor
			friend class Search_tree;
		};

		// DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
		Search_tree();
		~Search_tree();

		bool empty() const;
		int size() const;
		int height() const;

		Type front() const;
		Type back() const;

		Iterator begin();
		Iterator end();
		Iterator rbegin();
		Iterator rend();
		Iterator find( Type const & );

		void clear();
		bool insert( Type const & );
		bool erase( Type const & );
		
		

	// Friends

	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Search_tree<T> const & );
};

//////////////////////////////////////////////////////////////////////
//                Search Tree Public Member Functions               //
//////////////////////////////////////////////////////////////////////

// The initialization of the front and back sentinels is a hint
template <typename Type>
Search_tree<Type>::Search_tree():
root_node( nullptr ),
tree_size( 0 ),
front_sentinel( new Search_tree::Node( Type() ) ),
back_sentinel( new Search_tree::Node( Type() ) ) {
	front_sentinel->next_node = back_sentinel;
	back_sentinel->previous_node = front_sentinel;
	front_sentinel->previous_node=nullptr;
	back_sentinel->next_node=nullptr;
}

template <typename Type>
Search_tree<Type>::~Search_tree() {
	clear();  // might as well use it...
	delete front_sentinel;
	delete back_sentinel;
}

template <typename Type>
bool Search_tree<Type>::empty() const {
	return ( root_node == nullptr );
}

template <typename Type>
int Search_tree<Type>::size() const {
	return tree_size;
}

template <typename Type>
int Search_tree<Type>::height() const {
	return root_node->height();
}

template <typename Type>
Type Search_tree<Type>::front() const {
	if ( empty() ) {
		throw underflow();
	}

	return root_node->front()->node_value;
}

template <typename Type>
Type Search_tree<Type>::back() const {
	if ( empty() ) {
		throw underflow();
	}

	return root_node->back()->node_value;
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::begin() {
	return empty() ? Iterator( this, back_sentinel ) : Iterator( this, root_node->front() );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::end() {
	return Iterator( this, back_sentinel );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::rbegin() {
	return empty() ? Iterator( this, front_sentinel ) : Iterator( this, root_node->back() );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::rend() {
	return Iterator( this, front_sentinel );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::find( Type const &obj ) {
	if ( empty() ) {
		return Iterator( this, back_sentinel );
	}

	typename Search_tree<Type>::Node *search_result = root_node->find( obj );

	if ( search_result == nullptr ) {
		return Iterator( this, back_sentinel );
	} else {
		return Iterator( this, search_result );
	}
}

template <typename Type>
void Search_tree<Type>::clear() {
	if ( !empty() ) {
		root_node->clear();
		root_node = nullptr;
		tree_size = 0;
	}

	// Reinitialize the sentinels
	front_sentinel->next_node = back_sentinel;
	back_sentinel->previous_node = front_sentinel;
}

template <typename Type>
bool Search_tree<Type>::insert( Type const &obj ) {
	if ( empty() ) {
		root_node = new Search_tree::Node( obj );
		tree_size = 1;
	
		root_node->next_node=back_sentinel;
		root_node->previous_node=front_sentinel;
		front_sentinel->next_node=root_node;
		back_sentinel->previous_node=root_node;
		//inserting the first node, the root node, into the linked list as well 

		return true;
	} else if ( root_node->insert( obj, root_node ) ) {
		++tree_size;
		return true;
	} else {
		return false;
	}
}

template <typename Type>
bool Search_tree<Type>::erase( Type const &obj ) {
	if ( !empty() && root_node->erase( obj, root_node ) ) {
		--tree_size;
		return true;
	} else {
		return false;
	}
}

//////////////////////////////////////////////////////////////////////
//                   Node Public Member Functions                   //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Search_tree<Type>::Node::Node( Type const &obj ):
node_value( obj ),
left_tree( nullptr ),
right_tree( nullptr ),
next_node( nullptr ),
previous_node( nullptr ),
tree_height( 0 ) {
	// does nothing
}

template <typename Type>
void Search_tree<Type>::Node::update_height() {
	tree_height = std::max( left_tree->height(), right_tree->height() ) + 1;
}

template <typename Type>
int Search_tree<Type>::Node::height() const {
	return ( this == nullptr ) ? -1 : tree_height;
}

// Return true if the current node is a leaf node, false otherwise
template <typename Type>
bool Search_tree<Type>::Node::is_leaf() const {
	return ( (left_tree == nullptr) && (right_tree == nullptr) );
}

// Return a pointer to the front node
template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::front() {
	return ( left_tree == nullptr ) ? this : left_tree->front();
}

// Return a pointer to the back node
template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::back() {
	return ( right_tree == nullptr ) ? this : right_tree->back();
}

template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::find( Type const &obj ) {
	if ( obj == node_value ) {
		return this;
	} else if ( obj < node_value ) {
		return (left_tree == nullptr) ? nullptr : left_tree->find( obj );
	} else {
		return ( right_tree == nullptr ) ? nullptr : right_tree->find( obj );
	}
}

// Recursively clear the tree
template <typename Type>
void Search_tree<Type>::Node::clear() {
	if ( left_tree != nullptr ) {
		left_tree->clear();
	}

	if ( right_tree != nullptr ) {
		right_tree->clear();
	}

	delete this;
}

template <typename Type>
bool Search_tree<Type>::Node::insert( Type const &obj, Search_tree<Type>::Node *&to_this ) {
	
	if ( obj < node_value ) {
		if ( left_tree == nullptr ) {
			left_tree = new Search_tree<Type>::Node( obj );
		
			previous_node->next_node=left_tree;
			left_tree->previous_node=previous_node;
			left_tree->next_node=to_this; //linked list is setup so that the values go from minimum to maximum and since the value is placed in the left_tree, the value is placed 
										  //ahead of its ancestor in the linked list
			previous_node=left_tree;
			//setting up the linked list for when there is already another entry in the linked list	
			
			update_height();
			
			return true;
		} else {
			if ( left_tree->insert( obj, left_tree ) ) {
				if(right_tree==nullptr||left_tree->height()-right_tree->height()==2){ //if there is a difference in height between the left and right tree which would cause an avl
																					  //imbalance, then call the rotate functions for the two cases
					if(obj<left_tree->node_value){ 
						//LeftLeft Case
						rotateR(this,to_this); //calling the rotateR function
					}
					else{
						//LeftRight Case
						rotateLR(this,to_this); //calling the rotateLR function
					}
				}
				update_height();
				return true;
			} else {
				return false;
			}
		}
	} else if ( obj > node_value ) {
		if ( right_tree == nullptr ) {
			right_tree = new Search_tree<Type>::Node( obj );
			
			next_node->previous_node=right_tree;
			right_tree->previous_node=to_this; //linked list is setup so that the values go from minimum to maximum and since the value is placed in the right_tree, the value is placed 
										  	   //after its ancestor in the linked list
			right_tree->next_node=next_node;
			next_node=right_tree;	
			update_height();	
			//setting up the linked list for when there is already another entry in the linked list
			
			return true;
		} else {
			if ( right_tree->insert( obj, right_tree ) ) {
				if(left_tree==nullptr||right_tree->height()-left_tree->height()==2){ //if there is a difference in height between the left and right tree which would cause an avl
																					  //imbalance, then call the rotate functions for the two cases
					if (obj>right_tree->node_value){
						//RightRight Case
						rotateL(this,to_this); //calling the rotateL function
					}
					else{
						//RightLeft Case
						rotateRL(this,to_this); //calling the rotateRL function
					}
				}
				update_height();
				return true;
			} else {
				return false;
			}
		}
	} else {
		return false;
	}
}

template <typename Type>
bool Search_tree<Type>::Node::erase( Type const &obj, Search_tree<Type>::Node *&to_this ) {
	if ( obj < node_value ) {
		if ( left_tree == nullptr ) {
			return false;
		} else {
			if ( left_tree->erase( obj, left_tree ) ) {
				update_height();
				if(right_tree->height()-left_tree->height()==2){ //if there is a difference in height between the left and right tree which would cause an avl
																 //imbalance, then call the rotate functions for the two cases
					if(right_tree->right_tree->height()-left_tree->height()>0){
						//RightRight Case
						rotateL(this,to_this);
					}
					else{
						//RightLeft Case
						rotateRL(this,to_this);
					}
				}
				
				return true;
			}

			return false;
		}
	} else if ( obj > node_value ) {
		if ( right_tree == nullptr ) {
			return false; 
		} else {
			if ( right_tree->erase( obj, right_tree ) ) {
				update_height();
				if(left_tree->height()-right_tree->height()==2){ //if there is a difference in height between the left and right tree which would cause an avl
																 //imbalance, then call the rotate functions for the two cases
					if(left_tree->left_tree->height()-right_tree->height()>0){
						//LeftLeft Case
						rotateR(this,to_this);
					}
					else{
						//LeftRight
						rotateLR(this,to_this);
					}
				}
				
				return true;
			}

			return false;
		}
	} else {
		assert( obj == node_value );

		if ( is_leaf() ) {
			next_node->previous_node=previous_node;
			previous_node->next_node=next_node;
			to_this = nullptr;
			delete this;
		} else if ( left_tree == nullptr ) {
			next_node->previous_node=previous_node;
			previous_node->next_node=next_node;
			to_this = right_tree;
			delete this;
		} else if ( right_tree == nullptr ) {
			next_node->previous_node=previous_node;
			previous_node->next_node=next_node;
			to_this = left_tree;
			delete this;
		} else {
			next_node->previous_node=previous_node;
			previous_node->next_node=next_node;
			node_value = right_tree->front()->node_value;
			right_tree->erase( node_value, right_tree );
			update_height();
			//disconnecting the linked list values that are erased from the avl tree
		}

		return true;
	}
}

template <typename Type>
void Search_tree<Type>::Node::rotateR(Node*node,Node*&y) {
	Node*x=node->left_tree;
	Node*T2=x->right_tree;
	//declaring the variables to be moved 
	x->right_tree=node;
	y=x;
	node->left_tree=T2;
	//assigning the variables such that avl balance is maintained 
	
	node->tree_height = std::max( node->left_tree->height(), node->right_tree->height() ) + 1;
	y->tree_height = std::max( y->left_tree->height(), y->right_tree->height() ) + 1;
	x->left_tree->update_height();
	x->right_tree->update_height();
	x->update_height();
	//updating the heights after the rotation has been completed
	
}

template <typename Type>
void Search_tree<Type>::Node::rotateL(Node*node,Node*&x) {
	Node*y=node->right_tree;
	Node*T2=y->left_tree;
	//declaring the variables to be moved
	y->left_tree=node;
	x=y;
	node->right_tree=T2;
	//assigning the variables such that avl balance is maintained
	node->tree_height = std::max( node->left_tree->height(), node->right_tree->height() ) + 1;
	x->tree_height = std::max( x->left_tree->height(), x->right_tree->height() ) + 1;
	y->left_tree->update_height();
	y->right_tree->update_height();
	y->update_height();
	//updating the heights after the rotation has been completed
}

template <typename Type>
void Search_tree<Type>::Node::rotateLR(Node*node,Node*&x) {
	Node*b=node->left_tree;
	Node*d=b->right_tree;
	Node*dl=d->left_tree;
	Node*dr=d->right_tree;
	//declaring the variables to be moved
	d->left_tree=b;
	d->right_tree=node;
	x=d;
	b->right_tree=dl;
	node->left_tree=dr;
	//assigning the variables such that avl balance is maintained
	node->tree_height = std::max( node->left_tree->height(), node->right_tree->height() ) + 1;
	x->tree_height = std::max( x->left_tree->height(), x->right_tree->height() ) + 1;
	d->right_tree->update_height();
	d->left_tree->update_height();
	d->update_height();
	//updating the heights after the rotation has been completed

}

template <typename Type>
void Search_tree<Type>::Node::rotateRL(Node*node,Node*&x) {
	Node*b=node->right_tree;
	Node*d=b->left_tree;
	Node*dl=d->left_tree;
	Node*dr=d->right_tree;
	//declaring the variables to be moved
	d->right_tree=b;
	d->left_tree=node;
	x=d;
	b->left_tree=dl;
	node->right_tree=dr;
	//assigning the variables such that avl balance is maintained
	node->tree_height = std::max( node->left_tree->height(), node->right_tree->height() ) + 1;
	x->tree_height = std::max( x->left_tree->height(), x->right_tree->height() ) + 1;
	d->right_tree->update_height();
	d->left_tree->update_height();
	d->update_height();
	//updating the heights after the rotation has been completed
}


//////////////////////////////////////////////////////////////////////
//                   Iterator Private Constructor                   //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Search_tree<Type>::Iterator::Iterator( Search_tree<Type> *tree, typename Search_tree<Type>::Node *starting_node ):
containing_tree( tree ),
current_node( starting_node ) {
	// This is done for you...
	// Does nothing...
}

//////////////////////////////////////////////////////////////////////
//                 Iterator Public Member Functions                 //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Type Search_tree<Type>::Iterator::operator*() const {
	// This is done for you...
	return current_node->node_value;
}

template <typename Type>
typename Search_tree<Type>::Iterator &Search_tree<Type>::Iterator::operator++() {
	// Update the current node to the node containing the next higher value
	// If we are already at end do nothing
	if (current_node->next_node!=nullptr){
		current_node=current_node->next_node;
	}
	// Your implementation here, do not change the return value

	return *this;
}

template <typename Type>
typename Search_tree<Type>::Iterator &Search_tree<Type>::Iterator::operator--() {
	// Update the current node to the node containing the next smaller value
	// If we are already at either rend, do nothing
	if (current_node->previous_node!=nullptr){
		current_node=current_node->previous_node;
	}
	// Your implementation here, do not change the return value

	return *this;
}

template <typename Type>
bool Search_tree<Type>::Iterator::operator==( typename Search_tree<Type>::Iterator const &rhs ) const {
	// This is done for you...
	return ( current_node == rhs.current_node );
}

template <typename Type>
bool Search_tree<Type>::Iterator::operator!=( typename Search_tree<Type>::Iterator const &rhs ) const {
	// This is done for you...
	return ( current_node != rhs.current_node );
}

//////////////////////////////////////////////////////////////////////
//                            Friends                               //
//////////////////////////////////////////////////////////////////////

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator<<( std::ostream &out, Search_tree<T> const &list ) {
	out << "not yet implemented";

	return out;
}

#endif
