/*****************************************
 * Instructions
 *  - Replace 'uwuserid' with your uWaterloo User ID
 *  - Select the current calendar term and enter the year
 *  - List students with whom you had discussions and who helped you
 *
 * uWaterloo User ID:  l67ma@uwaterloo.ca
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  (Winter) 2018
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *
 * The following is a list of uWaterloo User IDs of those students
 * I had discussions with in preparing this project:
 *    -
 *
 * The following is a list of uWaterloo User IDs of those students
 * who helped me with this project (describe their help; e.g., debugging):
 *    -
 *****************************************/

#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H

#ifndef nullptr
#define nullptr 0
#endif

#include "Exception.h"
#include "ece250.h"

enum bin_state_t { UNOCCUPIED, OCCUPIED, ERASED };

template <typename Type>
class Quadratic_hash_table {
	private:
		int count;
		int counterased;
		int power;
		int array_size;
		int mask;
		Type *array;
		bin_state_t *occupied;

		int hash( Type const & ) const;

	public:
		Quadratic_hash_table( int = 5 );
		~Quadratic_hash_table();
		int size() const;
		int capacity() const;
		double load_factor() const;
		bool empty() const;
		bool member( Type const & ) const;
		Type bin( int ) const;

		void print() const;

		void insert( Type const & );
		bool erase( Type const & );
		void clear();

	// Friends

	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Quadratic_hash_table<T> const & );
};

template <typename Type>
Quadratic_hash_table<Type>::Quadratic_hash_table( int m ):
count( 0 ), power( m ),
array_size( 1 << power ),
mask( array_size - 1 ),
array( new Type[array_size] ),
occupied( new bin_state_t[array_size] ) {

	for ( int i = 0; i < array_size; ++i ) {
		occupied[i] = UNOCCUPIED; //constructor initializing the values of count, array size and setting all
								  //the values of occupied[] to UNOCCUPIED for an empty hash table
	}
	counterased=0; //initializing counterased to 0
}

template <typename Type>
Quadratic_hash_table<Type>::~Quadratic_hash_table() {

	delete[] array;
	delete[] occupied; //destructor removing the two arrays for memory deallocation
}

template <typename Type>
int Quadratic_hash_table<Type>::size() const {
      return count; //returns count which contains the number of elements in the hash table
}

template <typename Type>
int Quadratic_hash_table<Type>::capacity() const {
      return array_size; //return array_size which contains the size of the hash table array
}

template <typename Type>
double Quadratic_hash_table<Type>::load_factor() const {
	  
      return static_cast<double>(count+counterased)/static_cast<double>(array_size); //calculates the ratio of occupied  and erased
	  															  //bins to the total number of bins
}

template <typename Type>
bool Quadratic_hash_table<Type>::empty() const {
      return count==0; //determines if count is 0, if it is, it indicates that the hash table is empty
}

template <typename Type>
bool Quadratic_hash_table<Type>::member(Type const &obj) const {
	int bin=hash(obj); //finding the initial bin number using the hash function
	
	if (empty()){
		return false; //no need to search if the hash table is empty
	}
	
	for (int k=0;k<array_size;++k){
		bin=(bin+k)%array_size; //incrementing until the obj is found or the hash table ends
		if (occupied[bin]==OCCUPIED && array[bin]==obj){ //if the bin is occupied and contains obj we have a match
			return true;
		}
		if (occupied[bin]==UNOCCUPIED){ //once we hit an unoccupied bin we know the object is not in the hash table
			return false;
		}
		if (occupied[bin]==ERASED){ 
			if (array[bin]==obj){ //if the erased value is equal to the object value we know it is not in the hash table
				return false;
			}
		}

	}
	return false;
}

template <typename Type>
Type Quadratic_hash_table<Type>::bin( int n ) const {
      return array[n]; //return the value in the bin at index n
}

template <typename Type>
void Quadratic_hash_table<Type>::print() const {
	//not implemented
}

template <typename Type>
void Quadratic_hash_table<Type>::insert(Type const &obj) {
	if (count==array_size){
		throw overflow(); //throw overflow if the list is full 
	}
	
	
	if (member(obj)!=1){ //if obj is already in the hash table do nothing 
		int bin=hash(obj); //finding the initial bin number using the hash function
		for (int k=0;k<array_size;++k){
			bin=(bin+k)%array_size; //incrementing until an available bin is found
			
	 		if (occupied[bin]==UNOCCUPIED){ //if the bin is available for insertion
				occupied[bin]=OCCUPIED; //this bin is now occupied 
				array[bin]=obj; //storing the obj in the available bin
				count++; //incrementing count
				break; //break if obj has been inserted into a bin
			}
			else if (occupied[bin]==ERASED){
				occupied[bin]=OCCUPIED; //this bin is now occupied 
				array[bin]=obj; //storing the obj in the available bin
				count++; //incrementing count
				counterased--; //decrementing the count for erased bins
				break; //break if obj has been inserted into a bin
			}
			
		}
	}
}

template <typename Type>
bool Quadratic_hash_table<Type>::erase(Type const &obj) {
	int bin=hash(obj); //finding the initial bin number using the hash function
	
	if (empty()){
		return false; //if empty there is nothing to erase
	}
	for (int k=0;k<array_size;++k){
		bin=(bin+k)%array_size; //incrementing until obj is found or the hash table ends
		if (occupied[bin]==OCCUPIED && array[bin]==obj){ //if the bin is occupied and contains obj we have a match
			occupied[bin]=ERASED; //the bin is now erased
			count--; //decrementing count
			counterased++; //incrementing the counter for bins that are erased
			return true; //return true if obj has been erased
		}
		if (occupied[bin]==UNOCCUPIED){//once we hit a bin that isn't occupied we know the object is not in the hash table
			return false;
		}
		if (occupied[bin]==ERASED){
			if (array[bin]==obj){ //if the erased value is equal to the object being inserted we know it is not in the hash table
				return false;
			}
		}
	}
	return false; //return false if obj has not been found 	
}

template <typename Type>
void Quadratic_hash_table<Type>::clear() {
	for (int k=0;k<array_size;++k){ //looping through the array
		occupied[k]=UNOCCUPIED; //assigning all of the bins to unoccupied 
	}
	count=0; //once cleared, there are no elements left in the hash table
	counterased=0; //resetting counterased as well
	
}

template <typename Type>
int Quadratic_hash_table<Type>::hash(Type const &obj) const {
	int i= static_cast<int>(obj);
	int initial_bin=(i%array_size); //hashing function 
	if (initial_bin<0){
		initial_bin=initial_bin+array_size; //add the array size if the hash value is negative
	}
	return initial_bin;
}

// Your implementation here

template <typename T>
std::ostream &operator<<( std::ostream &out, Quadratic_hash_table<T> const &hash ) {
	for ( int i = 0; i < hash.capacity(); ++i ) {
		if ( hash.occupied[i] == UNOCCUPIED ) {
			out << "- ";
		} else if ( hash.occupied[i] == ERASED ) {
			out << "x ";
		} else {
			out << hash.array[i] << ' ';
		}
	}

	return out;
}

#endif
