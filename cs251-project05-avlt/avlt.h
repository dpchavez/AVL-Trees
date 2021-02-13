/*avlt.h*/

// Threaded AVL tree
/* Student: Daniela Chavez
 * NetID: dchave29
 * Instructors: Joe Hummel && Shanon Reckinger
 * Due date: March 14th, 2020
 * */ 
#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include <cassert>

using namespace std;

template<typename KeyT, typename ValueT>
class avlt
{
private:
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded; // true => Right is a thread, false => non-threaded
    int    Height;     // height of tree rooted at this node
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)
  NODE* newNode; 
  
/*************************************************************************************************/ 
/*TAKEN FROM LAST PROJECT...
 *Inserts the given key into the tree; if the key has already been insert then
  the function returns without changing the tree.
  Time complexity:  O(lgN) on average 
  Will be used for our copy helper and constructor.
 */
  void insert(KeyT key, ValueT value, int height)
  {
    NODE*cur = Root; // We create a new node with the root information
	NODE* prev = nullptr;
	
	// 1. Search for key, return if found:
	while(cur!=nullptr) //Looping through the tree
	{
		if (key == cur->Key)
			return;
		prev = cur;
		if (key < cur->Key) //If the key we are looking for is less then the key at the curent node
			cur = cur->Left; //And then we go left
		
		else
		{   //If the key we are looking for is greater than the key at the curent node
 			if(cur->isThreaded) //Checking if the curent node is threaded
 				break; //If its threaded, then break;
			cur = cur->Right; //If the node is not threaded, then go right
		}
	} //Closing while loop
	
  /*Now that we have checked if the key exsists or not within the tree:
   * If it doesnt, then we add it to the tree, at the corresponding position*/
  
	NODE *n = new NODE; //Creating a new node that will be inserted into the tree
	n->Key = key; //The key of the new node will be the key that doesnt exist in the tree
	n->Value = value; //We are also giving it the value that was set as a parameter
	n->Left = nullptr; //Creating a left node
	n->Right = nullptr; //Creating a right node
	n->Height = height; //Added the height...
	n->isThreaded = true;
	++Size;
	
	if (prev == nullptr) //If the previous node is nul
	{                  //Then we set the Root to N
		Root= n;       //And then we devlare that it is not threaded
		return;
	}
	
	if (key < prev->Key) //If the key is smaller than the previouskey
	{
		n->Right = prev; //we assign the node to the right as the prev
		prev->Left = n; //Assigning the previous left node as the curent node
	}	
	else
	{
		prev->isThreaded = false; //previous node as not threaded
		n->Right = prev->Right; //Node to the right is now the previous node to the right
		prev->Right = n;		
	}
  } //closing insert.
 
/*************************************************************************************************/
//TAKEN FROM LAST PROGRAM
  /*This is my Copy Helper. This fyunction will be used recursively.
   * Will be used to help my copy contructor by copying an existing tree into a new one*/
  void copyHelper(NODE* Other)
  {
	  if(Other == nullptr) //If the existing tree is null
		  return;          // Then just return

	  else{
		  insert(Other->Key, Other->Value, Other->Height); //Useing insert to get all the nodes and values of the existing tree
		  copyHelper(Other->Left); //Copy left nodes from old tree into new tree
		  if (!Other->isThreaded) //If the old tree is not threaded
			  copyHelper(Other->Right); //then copy the right nodes from the pld tree into the new tree
	  } 
  }
/*************************************************************************************************/
  /* This is my Destructor Helper. It will help the Destructor is a member function
   *  which destructs or deletes an object.*/
  void destructorHelper(NODE *cur) //We take the curent node as a parameter
  {
	  if(cur == nullptr) //If the curent node is a null pointer
		  return;        // Then just return
		  
	  destructorHelper(cur->Left); //We call this recursivly
	  if(!cur->isThreaded) //If the curent node is not threaded
		  destructorHelper(cur->Right); //We call the decontructor helper for the right node recursivly

	  delete cur; //Delete the curent node by default
  }
  
  
  /*************************************************************************************************/
  /*This is my Dump Helper. This will help my dump fucntion. This will help my dump fucntion
   * to output the threaded and non threaded nodes*/
 void dumpHelper(NODE* cur, ostream& output)const
 {
	 if (cur == nullptr) //If the curent node is null
		 return;          //Then just return
	 
	 else{
		 dumpHelper(cur->Left, output); //Calling the dump helper 
		 if (cur->isThreaded && cur->Right != nullptr) //if the curent node is threaded and the nide to the right isnt null
			 output<< "(" << cur->Key <<","<<cur->Value << "," << cur->Height <<","  <<cur->Right->Key << ")" << endl; //output key, value, threaded value	 
		 else
		 {  //If the node is not threaded
			 output << "(" << cur->Key <<","<<cur->Value << "," << cur->Height<< ")" << endl; //Print curent key and value
			 dumpHelper(cur->Right, output); //calling the dump helper recursively
		 } 
	 }//Closes else statement 
 }//closes function
  
/*************************************************************************************************/
/* This is my Range Search 
 * This function recursivly searches tbhe left most node until it sreaches the lowest node from the range
 * and then it goes right until it reaches the right most node that is equivalent of the range
 * */
	void helperRangeSearch(KeyT& lower, KeyT& upper, NODE* cur, vector<KeyT> &keys)
	{
		if (cur == nullptr) //If the current node is null, then just return
			return;
		if (cur->Key > lower) //Going left
			helperRangeSearch(lower, upper, cur->Left, keys); //recursivly going left
			
		if (lower <= cur->Key && cur->Key <= upper) {  //pushing values into the vector
			keys.push_back(cur->Key);
		}
		if (!cur->isThreaded && cur->Key < upper)  //Rucursivly going right
			helperRangeSearch(lower, upper, cur->Right, keys);
	}
 /*************************************************************************************************/
/* This gets the height of the left node.
 * I will be using this function to get the height of the left node.
 * I will be using this function in my insert function
 * */
  int heightHelper(NODE* cur)
  {
	  if (cur == nullptr) //If the node is null, return -1
		  return -1;
	 else
		 return cur->Height; //else, return height
  }
/*************************************************************************************************/
/*This function gets the height of the node if it at the right
 * This function also checks if the niode is threaded or not
 * Will be using this function in my insert function as well
 * */
  int heightRight(NODE* cur)
  {
	  if (cur->isThreaded) //If threaed, return -1
		  return -1;
	  else
		  return cur->Right->Height; //if not threaded, get the height of the right
  }

  /*************************************************************************************************/
  /*TAKEN FROM ZYBOOKS
   *This function rotates the node to the right of it. 
   It also checks if its threaded
   *It also rebalances the nodes that were rotated */

void _RightRotate(NODE* Parent, NODE* N)
  {
      
      NODE* L = N->Left;
      NODE* B = L->Right;
		  
 	  if (L->isThreaded)
 		  B = nullptr;
          
      N->Left = B; //Setting the node at the left of the N to be equal to node at the Right of the parent
      L->Right = N; //Setting the node at the right of the parent to be equal to N
  	  L->isThreaded = false;
      
      if (Parent == nullptr)
         Root = L;
      else if (L->Key < Parent->Key)
         Parent->Left = L;
      else
         Parent->Right = L;
    
      N->Height = 1 + max(heightHelper(N->Left), heightRight(N));
      L->Height = 1 + max(heightHelper(L->Left), heightRight(L));
      
  }

/*************************************************************************************************/
/*TAKEN FROM ZYBOOKS
 * This rotates the nodes to the leftit also rebalances the pointers to the nodes to the left and right
 * */
  void _LeftRotate(NODE* Parent, NODE* N)
  {
       // assert(N!=nullptr); //Checking that the aretn isnt a null pointer
      // assert(N->Left != nullptr); //Checking if the node at the left of the parent is a null pointer
      
      NODE* R = N->Right;
      NODE* B = R->Left;
      
      
      R->Left = N; //Setting the node at the left of the N to be equal to node at the Right of the parent
      N->Right = B; //Setting the node at the right of the parent to be equal to N
	  if (B == nullptr) {
		  N->Right = R;
		  N->isThreaded = true;
	  }
      
      if (Parent == nullptr)
         Root = R;
      else if (Parent->Right == N)
         Parent->Right = R;
      else
         Parent->Left = R;
      
      N->Height = 1 + max(heightHelper(N->Left), heightRight(N));
      R->Height = 1 + max(heightHelper(R->Left), heightRight(R));
      
  }

/*************************************************************************************************/
 
public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  
  avlt()
  {
    Root = nullptr;
    Size = 0;
  }


 /*************************************************************************************************/
  // copy constructor 
  // NOTE: makes an exact copy of the "other" tree, such that making the
  // copy requires no rotations.
  //
  avlt (const avlt& other)
  {
    Root = nullptr; 
	Size = 0;
	copyHelper(other.Root); //Using copy helper
  }

/*************************************************************************************************/
  // destructor:
  // Called automatically by system when tree is about to be destroyed;
  // this is our last chance to free any resources / memory used by
  // this tree.

  virtual ~avlt()
  {
	destructorHelper(Root); /* Calling the destructor Helper*/
  }
  
/*************************************************************************************************/
  // operator=
  // Clears "this" tree and then makes a copy of the "other" tree.
  // NOTE: makes an exact copy of the "other" tree, such that making the
  // copy requires no rotations.
  avlt& operator=(const avlt& other)
  {
    destructorHelper(Root); // deletes root
	Root = nullptr; //sets root to null ptr
	Size = 0; //As the root is null, we wet the size to zero
	copyHelper(other.Root); // we copy nodes from existing tree to root

    return *this; //returns tree with new nodes
  }
/*************************************************************************************************/
/*Clear Function
 * Clears the contents of the tree, resetting the tree to empty.
 * */
  void clear()
  {
    destructorHelper(Root); //Calls the destructorfunction to delete the Rot
	Root = nullptr; //We set the root to a null pointer
	Size = 0; //We set the size to zero
  }
/*************************************************************************************************/
  // size:
  // Returns the # of nodes in the tree, 0 if empty.
  // Time complexity:  O(1) 
  int size() const
  {
    return Size;
  }

/*************************************************************************************************/ 
  // height:
  // Returns the height of the tree, -1 if empty.
  // Time complexity:  O(1) 
  int height() const
  {
    if (Root == nullptr)
      return -1;
    else
      return Root->Height;
  }
  
/*************************************************************************************************/
  // search:
  // Searches the tree for the given key, returning true if found
  // and false if not.  If the key is found, the corresponding value
  // is returned via the reference parameter.
  // Time complexity:  O(lgN) on average
 
  bool search(KeyT key, ValueT& value) const
  {
    NODE* cur = Root; //creating cur with the data of Root
	
	while (cur != nullptr) //While cur is not null
	{
		if (key == cur->Key) //If the key is found, then just return
		{
			value = cur->Value; 
			return true;
		}
		
		if (key < cur->Key) //If key is less than the curent key
			cur = cur->Left; // our curent node will be moved to the node on the left
		
		else 
		{
			if(cur->isThreaded)
				cur = nullptr; //if the curent node is threaded, make it a null pointer
			else
				cur = cur->Right; //If cur is not threaded, then assign cur to the node at the right
		}	
	}
	return false; //default
  }

/*************************************************************************************************/
  // range_search
  // Searches the tree for all keys in the range [lower..upper], inclusive.
  // It is assumed that lower <= upper.  The keys are returned in a vector;
  // if no keys are found, then the returned vector is empty.
  // Time complexity: O(lgN + M), where M is the # of keys in the range
  // [lower..upper], inclusive.
  // NOTE: do not simply traverse the entire tree and select the keys
  // that fall within the range.  That would be O(N), and thus invalid.
  // Be smarter, you have the technology.
 
  vector<KeyT> range_search(KeyT lower, KeyT upper)
  {
    vector<KeyT>  keys;
	helperRangeSearch(lower, upper, Root ,keys);
    return keys;
  }
  
/*************************************************************************************************/
  // insert
  // Inserts the given key into the tree; if the key has already been insert then
  // the function returns without changing the tree.  Rotations are performed
  // as necessary to keep the tree balanced according to AVL definition.
  // Time complexity:  O(lgN) worst-case
  
  void insert(KeyT key, ValueT value)
  {
    NODE*cur = Root; // We create a new node with the root information
	stack<NODE*> s;
	
	// 1. Search for key, return if found:
	while(cur!=nullptr) //Looping through the tree
	{
		if (key == cur->Key)
			return;
		s.push(cur); //pushing element into the stack if the key is not equal to the curent key
		if (key < cur->Key) //If the key we are looking for is less then the key at the curent node
			cur = cur->Left; //And then we go left
		
		else
		{   //If the key we are looking for is greater than the key at the curent node
 			if(cur->isThreaded) //Checking if the curent node is threaded
 				break; //If its threaded, then break;
			cur = cur->Right; //If the node is not threaded, then go right
		}
	} //Closing while loop
	
  /*Now that we have checked if the key exsists or not within the tree:
   * If it doesnt, then we add it to the tree, at the corresponding position*/
  
	NODE *n = new NODE; //Creating a new node that will be inserted into the tree
	n->Key = key; //The key of the new node will be the key that doesnt exist in the tree
	n->Value = value; //We are also giving it the value that was set as a parameter
	n->Left = nullptr; //Creating a left node
	n->Right = nullptr; //Creating a right node
	n->Height = 0;
	n->isThreaded = true;
	++Size;
	
	if (s.empty()) //If the previous node is nul
	{                  //Then we set the Root to N
		Root= n;       //And then we devlare that it is not threaded
		return;
	}
	
	cur = s.top();
	
	if (key < cur->Key) //If the key is smaller than the previouskey
	{
		n->Right = cur; //we assign the node to the right as the prev
		cur->Left = n; //Assigning the previous left node as the curent node
	}	
	else
	{
		cur->isThreaded = false; //previous node as not threaded
		n->Right = cur->Right; //Node to the right is now the previous node to the right
		cur->Right = n;		
	}
	while(!s.empty())
	{
	
		cur = s.top(); //We assign the new node to the last emement that was pushed into the stack
		s.pop(); //removing the last element in the stack
		
		 //  let HL = height of cur's left sub-tree (-1 if empty);
		int LH = heightHelper(cur->Left);
      
		//  let HR = height of cur's right sub-tree (-1 if empty);
		int RH = heightRight(cur);
		int hCur = 1 + max(LH, RH);
		
		if (hCur == cur->Height)     //if height didn't change, stop here
			break;
		cur->Height = hCur;         //If the height did change, then assign the  new height to cur
		
		//Creating a balance factor to check if the trea is leaning
		int balanceFactor = LH - RH;  
		
		if (balanceFactor < -1 || balanceFactor > 1) {    //Checking if its leaning
			NODE* parent = nullptr;
			if (!s.empty())
				parent = s.top();
				
			if (LH > RH)
			{
				cout << "Leaning left" << endl;
				if (heightHelper(cur->Left->Left) > heightRight(cur->Left)) // This is a Left-Left case
				{
					cout << "Left Left Case" << endl;
					_RightRotate(parent, cur);
				}
					
			    else {    // Left-Right case
					cout << "left Right Case" << endl;
					_LeftRotate(cur, cur->Left);
					_RightRotate(parent, cur);
				}				
			}
			
			else 
			{
				cout << "Leaning right" << endl;
				if (heightHelper(cur->Right->Left) > heightRight(cur->Right))  //Right Left case
					{
						cout << "Right Left case" << endl;
						_RightRotate(cur, cur->Right);
						_LeftRotate(parent, cur);
					}
				else {
					cout << "Right Right case" << endl; //Right Right case
					_LeftRotate(parent, cur);
				}
			}
		}
	}
  }
/*************************************************************************************************/
  // [] Operator
  // Returns the value for the given key; if the key is not found,
  // the default value ValueT{} is returned.
  // Time complexity:  O(lgN) worst-case
 
  ValueT operator[](KeyT key) const
  {
    NODE* cur = Root;
   
	while (cur != nullptr) //While cur is not null
	{
		if (key == cur->Key) //If the key is found, then just return
			return cur->Value;
		
		if (key < cur->Key) //If key is less than the curent key
			cur = cur->Left; // our curent node will be moved to the node on the left
		
		else 
		{
			if(cur->isThreaded)
				break;//if the curent node is threaded, make it a null pointer
			else{ cur = cur->Right;} //If cur is not threaded, then assign cur to the node at the right
		}	
	}	
	return ValueT{ }; //Returns default value
  }

/*************************************************************************************************/
  // () Operator
  // Finds the key in the tree, and returns the key to the "right".
  // If the right is threaded, this will be the next inorder key.
  // if the right is not threaded, it will be the key of whatever
  // node is immediately to the right.
  //
  // If no such key exists, or there is no key to the "right", the
  // default key value KeyT{} is returned.
  //
  // Time complexity:  O(lgN) worst-case
  //
  KeyT operator()(KeyT key) const
  {
    NODE* cur = Root; // We get all the information from Root
	while (cur != nullptr) //While the tree is not empty
	{
		if(cur->Key == key && cur->Right != nullptr) //If the curent key matches the key we are looking for
			return cur->Right->Key; //And if the node to the right is not null, then return the key to the right
		
		if (cur->Key > key) //If the key we are looking for is less than the key at the curent node
			cur = cur->Left; //Go left
		
		else{
			if(cur->isThreaded) //if the curent node is threaded
				return KeyT{ }; //We return the default key
				
			cur = cur->Right; //If the node is not threaded, then go right
		}	
	}
    return KeyT{ }; //Returns default key value
  }

/*************************************************************************************************/
  // % Operator
  // Returns the height stored in the node that contains key; if key is
  // not found, -1 is returned.
  // Example:  cout << tree%12345 << endl;
  // Time complexity:  O(lgN) worst-case

  int operator%(KeyT key) const
  {
    NODE* cur = Root; //creating cur with the data of Root
	
	while (cur != nullptr) //While cur is not null
	{
		if (key == cur->Key) //If the key is found, then just return			
			return cur->Height;
		
		if (key < cur->Key) //If key is less than the curent key
			cur = cur->Left; // our curent node will be moved to the node on the left
		
		else 
		{
			if(cur->isThreaded)
				break; //if the curent node is threaded, make it a null pointer
			else
				cur = cur->Right; //If cur is not threaded, then assign cur to the node at the right
		}	
	}
    return -1; //default
  }

/*************************************************************************************************/
  // begin
  // Resets internal state for an inorder traversal.  After the 
  // call to begin(), the internal state denotes the first inorder
  // key; this ensure that first call to next() function returns
  // the first inorder key.

  // Space complexity: O(1)
  // Time complexity:  O(lgN) worst-case
  
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;

  void begin()
  {
    newNode= Root; //The node has all the information about the root. We dont want to alter the root
	  if (Root == nullptr) //If the size is equal to zero
		  return; //Just return  
	  else 
	  {
		  while (newNode->Left != nullptr) //And while the node to the left is not a null pointer
		  {
			newNode = newNode->Left; // We keep moving the node to the left until we find the smallest value
		  } //closing while loop
	  }  //Closing else statement
  }

/*************************************************************************************************/
  // next
  // Uses the internal state to return the next inorder key, and 
  // then advances the internal state in anticipation of future
  // calls.  If a key is in fact returned (via the reference 
  // parameter), true is also returned.
  // False is returned when the internal state has reached null,
  // meaning no more keys are available.  This is the end of the
  // inorder traversal.

  // Space complexity: O(1)
  // Time complexity:  O(lgN) worst-case
  
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  bool next(KeyT& key)
  {
    if(newNode == nullptr) //If the node is null
		return false;      //Just return
	if (newNode->isThreaded || newNode->Right == nullptr)
	{   //If the node is treaded or if the node to the right is null
		key = newNode->Key; //Then the key becomes the nodes key
		newNode = newNode->Right; //and the node will be pointing at the right
	}
	
	else
	{ //If the node is not threaded
		key = newNode->Key; //Then we assign the key to the key of that curent node
		newNode = newNode->Right; //The node will be pointing to the right
			
		while(newNode->Left != nullptr) //If the node at the left is not null
		{
			newNode = newNode->Left; //Then mode to the left
		}
	}
    return true; //Returining true as a default
  }

/*************************************************************************************************/
  // dump
  // Dumps the contents of the tree to the output stream, using a
  // recursive inorder traversal.
 
  void dump(ostream& output) const
  {
    output << "**************************************************" << endl;
    output << "********************* AVLT ***********************" << endl;

    output << "** size: " << this->size() << endl;
    output << "** height: " << this->height() << endl;

    // inorder traversal, with one output per line: either 
    // (key,value,height) or (key,value,height,THREAD)
    // (key,value,height) if the node is not threaded OR thread==nullptr
    // (key,value,height,THREAD) if the node is threaded and THREAD denotes the next inorder key
	cout << "Root is: " << Root->Key << endl;
	dumpHelper(Root, output);
    output << "**************************************************" << endl;
  }
	
}; //closing the avlt class
