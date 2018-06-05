/*****************************************
 * Instructions
 *  - Replace 'uwuserid' with your uWaterloo User ID
 *  - Select the current calendar term and enter the year
 *  - List students with whom you had discussions and who helped you
 *
 * uWaterloo User ID:  l67ma @uwaterloo.ca
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
 *    -20651352
 *
 * The following is a list of uWaterloo User IDs of those students
 * who helped me with this project (describe their help; e.g., debugging):
 *    -
 *****************************************/

#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H

#ifndef nullptr
#define nullptr 0
#endif

#include <iostream>
#include <limits>
#include <limits.h>
#include <stdio.h>

// include whatever classes you want


class Weighted_graph {
	private:
		// your implementation here
		//  you can add both private member variables and private member functions
		int count;
		int edge_total;
		int * degree_count;
		//int ** neighbor;
		double **matrix;
		static const double INF;
		//int * closest;
		bool * visited;
		//double * distance2;
		//int * previous;
		double * dist;
		bool changed;

	public:
		Weighted_graph( int = 50 );
		~Weighted_graph();

		int degree( int ) const;
		int edge_count() const;
		double adjacent( int, int ) const;
		double distance( int, int );

		void insert( int, int, double );

	// Friends

	friend std::ostream &operator<<( std::ostream &, Weighted_graph const & );
};


const double Weighted_graph::INF = std::numeric_limits<double>::infinity();

// Your implementation here

// You can modify this function however you want:  it will not be tested


Weighted_graph::Weighted_graph( int n ):
count(n),edge_total(0),degree_count(new int[n]),visited(new bool[n]),dist(new double[n]) {
	
	if (n<=0){ //default number of vertices if n is less than or equal to zero is 1
		n=1;
	}
	count=n; //initializing count to the number of vertices in the graph
	changed=true; //initialzing changed to true
 	for (int i=0;i<n;++i){
		degree_count[i]=0;
		//previous[i]=-1;
		//distance2[i]=INF;
		//closest[i]=-1;
		visited[i]=false;
		dist[i]=INF; //constructing the degree_count, visited and dist matrices used in the functions 
	}
	
	matrix=new double*[n];

	for (int i=0;i<n;++i){
		matrix[i]=new double[n]; //constructing 2d array which contains the graph and the distances between vertices
		//neighbor[i]=new int[n];
	}
	
	for ( int i = 0; i < n; ++i ) {
		for (int j=0; j<n; ++j){
			matrix[i][j]=INF; //setting up the default distance between vertices to be infinite
		}
		matrix[i][i]=0; //the default distance between a vertex and itself is 0 
	}

}


Weighted_graph::~Weighted_graph() {
	for (int i=0;i<count;++i){
		delete[] matrix[i];
		//delete[] neighbor[i];
	}
	
	delete[] matrix; 
	delete[] dist;
	//delete[] previous;
	//delete[] distance2;
	//delete[] neighbor;
	delete[] visited;
	//delete[] closest;
	delete[] degree_count;
	edge_total=0;
	changed=true;
	count=0; //destructor used to reset all the values and delete all the matrices used

}


int Weighted_graph::degree(int n) const {
	if (n<0||n>count-1){ //throw illegal argument exception if the argument does not correspond to an existing vertex
		//throw illegal_argument(); 
	}
      return degree_count[n]; //return the degree of vertex n
}


int Weighted_graph::edge_count() const {
      return edge_total; //return the total number of edgues in the graph
}


double Weighted_graph::adjacent(int m,int n) const {
	
	if (m<0||m>count-1||n<0||n>count-1){ //throw illegal argument exception if the arguments do not correspond to existing vertices
	  	//throw illegal_argument(); 
	}
	  
    if (m==n){ //if matricies are the same return 0
    	return 0;
	} 
	  return matrix[m][n]; //return the distance between two vertices
}


void Weighted_graph::insert(int m,int n, double w) {
	
	if (w<=0||w==INF){ //if the weight is less than or equal to zero or infinity, throw an illegal argument exception
	  	//throw illegal_argument(); 
	}
	  
    if (m==n||m>count-1||n>count-1||m<0||n<0){ //if the vertices do not exist or are equal throw illegal argument exception
    	//throw illegal_argument(); 
	} 
	matrix[n][m]=w;
	matrix[m][n]=w; //setting the edge between vertices m and n 
	edge_total++; //inserting an edge increases the edge total of the graph
	degree_count[m]++; 
	degree_count[n]++; //the degrees of vertices m and n are both increased by 1
	changed=true; //setting the value of changed to true after insert
	/*
	neighbor[m][degree_count[m]]=n;
	neighbor[n][degree_count[n]]=m;

	if (closest[m]==-1){
		closest[m]=n;
	}
	if (closest[n]==-1){
		closest[n]=m;
	}
	
	if (matrix[m][closest[m]]>w){
		closest[m]=n;
	}
	if (matrix[n][closest[n]]>w){
		closest[n]=m;
	}*/
}


double Weighted_graph::distance(int m,int n) {
	
	if (m<0||m>count-1||n<0||n>count-1){ //if the vertices do not exist throw illegal argument exception
    	//throw illegal_argument(); 
	}
	
	if (m==n){ //distance between a vertex and itself is zero 
		return 0;
	}
	 
	if (degree_count[m]==0||degree_count[n]==0){//if vertices are not connected return infinity 
		return INF;
	}
	
	
	
	if (changed==false){ //if the graph has not changed since the last time we called distance we can use the previous dist and visited matrices
		if (dist[n]==0&&visited[m]==true){ //if dist[n] is zero that means that the previous dist matrix contains the distance from n to all other vertices
			return dist[m];
		}
		else if (dist[m]==0&&visited[n]==true){ //if dist[m] is zero that means that the previous dist matrix contains the distance from m to all other vertices
			return dist[n];
		}
		else {
			for (int i=0;i<count;i++){ //otherwise reset the dist and visited matrices and calculate the distance again 
				dist[i]=INF;
				visited[i]=false;
			}
		}
	}
	else if (changed==true){ //if the graph changed since the since the last time we called distance we need to calculate the distances again
		for (int i=0;i<count;i++){
				dist[i]=INF;
				visited[i]=false;
			}
	}
	
	
	//int next;
	//int next2;
	//double distance=0; 
	//double length=0;
	/*
	distance2[m]=0;
	previous[m]=nullptr;
	for (int i=0; i<degree_count[m]-1;i++){
		distance2[neighbor[m][i]]=matrix[m][neighbor[m][i]];
		previous[neighbor[m][i]]=m;
	}
	visited[m]=true;	
	next=closest[m];	
	for (int i=0; i<degree_count[next]-1;i++){
		if (matrix[next][neighbor[next][i]]+matrix[m][neighbor[m][next]]<distance2[neighbor[next][i]]){
			distance2[neighbor[next][i]]=matrix[next][neighbor[next][i]];
			previous[neighbor[m][i]]=next;
		}
	}	
	visited[next]=true;	
	
	if (degree_count[next]>0) {
		next2=1000;
	}
	else {
		return distance;
	}
	
	for (int i=0; i<degree_count[next]-1;i++){
		if (neighbor[next][i]<next2&&included[neighbor[next][i]]!=true){
			next2=neighbor[next][i];
		}
	}*/

	dist[n]=0; //distance to the starting vertex is 0
	
	for (int i=0;i<count-1;i++){
		int min=INF;
		int min_index;
		for(int j=0;j<count;j++){
			if(visited[j]==false && dist[j]<=min){
				min=dist[j];
				min_index=j; //cycle through the distances and calculate the min distance and the corresponding vertex
			}
		}
		visited[min_index]=true; //we will visit the corresponding vertex with the shortest distance
		
		for (int k=0;k<count;k++){
			if (!visited[k]&&matrix[min_index][k]!=INF&&dist[min_index]!=INF && dist[min_index]+matrix[min_index][k]<dist[k]){
				dist[k]=dist[min_index]+matrix[min_index][k]; //comparing the existing distance and the new distance through the 
															  //vertex with the shortest distance 
			}
		}
		
		
	}
	return dist[m]; //return the distance to the other node 
	//note: approach used in distance function was inspired and modified from geek.com 
	changed=false; //after calculating dist and visited in the distance function, setting changed to false
}


std::ostream &operator<<( std::ostream &out, Weighted_graph const &graph ) {
	return out;
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif
