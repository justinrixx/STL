/***********************************************************************
* Header:
*    BST
* Summary:
*    This file contains a BinarySearch Tree Abstract data type. In
*    order to use this ADT, the data must support the greater than (>)
*    operator. The tree maintains a sorted order, so when you insert
*    into it and then iterate through, you will find that it comes out
*    in sorted order. The tree supports self-balancing so the items can
*    always be inserted in any order and the finding will remain O(logn)
*
*    This will contain the class definition of:
*        BST          : A tree ADT
*        BSTIterator  : An interator through the tree
*        BinaryNode   : A node of the tree
* Author
*    Nick Recht, Justin Ricks
************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>
#include <iostream>


#define NODE_ERROR_ALLOCATION "ERROR: Unable to allocate a node"

// forward declaration for the iterator and node classes
template <class T>
class BSTIterator;

template <class T>
class BinaryNode;

template <class T1, class T2>
class Map;

/************************************************
* BST
* A Binary Search Tree container
***********************************************/
template <class T>
class BST
{
public:
	// default constructor
	BST() : pRoot(NULL) {}

	// copy constructor
	BST(const BST & rhs) throw (const char *);

	// destructor
	~BST() { clear(); }

	// is the container empty?
	bool empty() const { return pRoot == NULL; }

	// clear the contents of the BST
	void clear() { deleteBinaryTree(pRoot); pRoot = NULL; };

	// insert an item into the tree
	void insert(const T & t) throw (const char *);

	// find if a value is in the tree
	BSTIterator<T> find(const T & t);

	// assignment operator
	BST <T> & operator = (const BST & rhs) throw (const char *);

	// remove from the middle of the list
	void remove(BSTIterator <T> it) throw (const char *);

	//get the root
	BinaryNode<T> * getRoot() { return pRoot;  }

	//Iterators
	BSTIterator<T> begin();
	BSTIterator<T> end() { return BSTIterator<T>(NULL); }
	BSTIterator<T> rbegin();
	BSTIterator<T> rend(){ return BSTIterator<T>(NULL); }

private:
	BinaryNode <T> * pRoot; // pointer to the list of data

	// free the data in this thing. This is used by both the clear
	// function, and the destructor, so it gets its own function
	void deleteBinaryTree(BinaryNode <T> * node);

	// this function is called by the insert function. It uses recursion
	// to add an item to it's proper location in the BST
	void recursiveAdd(const T & t, BinaryNode <T> * pNode)
		throw (const char *);

	void copy(BinaryNode <T> * pThis, BinaryNode <T> * pRhs)
		throw (const char *);

	//A recursive function to possibly fix any color problems
	void setColor(BinaryNode <T> * pThis);

        //Functions for Rotation
	void rotateRight(BinaryNode <T> * pThis, BinaryNode <T> * pPar,
		BinaryNode <T> * pGrand, BinaryNode <T> * pSib,
		BinaryNode <T> * pAunt); 
	void rotateLeft(BinaryNode <T> * pThis, BinaryNode <T> * pPar,
		BinaryNode <T> * pGrand, BinaryNode <T> * pSib,
		BinaryNode <T> * pAunt);
	void rotateDoubleRight(BinaryNode <T> * pThis, BinaryNode <T> * pPar,
		BinaryNode <T> * pGrand, BinaryNode <T> * pSib,
		BinaryNode <T> * pAunt);
	void rotateDoubleLeft(BinaryNode <T> * pThis, BinaryNode <T> * pPar,
		BinaryNode <T> * pGrand, BinaryNode <T> * pSib,
		BinaryNode <T> * pAunt);
};

/************************************************
* BSTIterator
* An iterator for the BST class
***********************************************/
template<class T>
class BSTIterator
{
public:
   //Constructors
  BSTIterator() : p(NULL) { };
   BSTIterator(BinaryNode<T> * pNode) { p = pNode; }
  BSTIterator(const BSTIterator & rhs) : p(NULL) { *this = rhs; }
   
   //Operators
   BSTIterator & operator = (const BSTIterator & rhs)
      {
         p = rhs.p;
         return *this;
      }
   
   bool operator == (const BSTIterator & rhs) const { return this->p == rhs.p; }
   bool operator != (const BSTIterator & rhs) const { return !(*this == rhs); }
   
   const T & operator * ()  { return p->data; }
   //Increment
   BSTIterator & operator ++ ()
   {
      findSuc();
      return *this;
   }
   BSTIterator & operator ++ (int postfix)
   {
      BSTIterator tmp(*this);
      ++(*this);
      
      return tmp;
   }
   //Decrement
   BSTIterator & operator -- ()
   {
      findPre();
      return *this;
   }
   BSTIterator & operator -- (int postfix)
   {
      BSTIterator tmp(*this);
      --(*this);
      
      return tmp;
   }
	
  private:
   BinaryNode<T> * p;
   
   friend void BST <T> ::remove(BSTIterator <T> it) throw (const char *);
   
   template <class T1, class T2>
   friend class Map;
   
   void findSuc(); //Successor
   void findPre(); //Predeccesor
};

/************************************************************************
* The BinaryNode Class
* See description above
***********************************************************************/
template <class T>
class BinaryNode
{
public:
   //default constructor
  BinaryNode() : pLeft(NULL), pRight(NULL), pParent(NULL), isRed(true) { }
   //nondefault constructor
  BinaryNode(const T & data) : pLeft(NULL), pRight(NULL), pParent(NULL), isRed(true)
   {
      this->data = data;
   }
   
   //Add a new node to the left of this
   // Throws is the given node already points to another node
   // or a new node can't be allocated
   BinaryNode * addLeft(const T & newData) throw (const char*);
   BinaryNode * addLeft(BinaryNode * newNode);
   //Add a new node to the right of this
   // Throws is the given node already points to another node
   // or a new node can't be allocated
   BinaryNode * addRight(const T & newData) throw (const char*);
   BinaryNode * addRight(BinaryNode * newNode);
   
   //data items -- All public
   T data;
   BinaryNode * pLeft;
   BinaryNode * pRight;
   BinaryNode * pParent;
   bool isRed; 
};


/************************************************************************
* addLeft
* Takes a pointer to a node and sets the pLeft of this to the passed node
***********************************************************************/
template<class T>
BinaryNode<T> * BinaryNode<T>::addLeft(BinaryNode * newNode)
{
   assert(!pLeft);
   
   this->pLeft = newNode;
   newNode->pParent = this;
   
   return this;
}

/************************************************************************
* addLeft
* Takes a data item and creates a new node, then assigns the pLeft
* of this to the new node
***********************************************************************/
template<class T>
BinaryNode<T> * BinaryNode<T>::addLeft(const T & newData) throw (const char*)
{

   BinaryNode * pNew = new(std::nothrow) BinaryNode(newData);
   if (pNew != NULL)
      return addLeft(pNew);
   else
      throw NODE_ERROR_ALLOCATION;

}

/************************************************************************
* addRight
* Takes a pointer to a node and sets the pRight of this to the passed node
***********************************************************************/
template<class T>
BinaryNode<T> * BinaryNode<T>::addRight(BinaryNode * newNode)
{
	assert(!pRight);

	this->pRight = newNode;
	newNode->pParent = this;

	return this;
}

/************************************************************************
* addRight
* Takes a data item and creates a new node, then sets the pRight of this
* to the new node
***********************************************************************/
template<class T>
BinaryNode<T> * BinaryNode<T>::addRight(const T & newData) throw (const char*)
{

	BinaryNode * pNew = new(std::nothrow) BinaryNode(newData);
	if (pNew != NULL)
		return addRight(pNew);
	else
		throw NODE_ERROR_ALLOCATION;

}

/************************************************************************
* BST :: deleteBinaryTree
* recursively deletes a binary tree from existence
***********************************************************************/
template <class T>
void BST <T> ::deleteBinaryTree(BinaryNode<T> * node)
{
	if (node == NULL)
		return;

	if (node->pLeft)
		deleteBinaryTree(node->pLeft);
	if (node->pRight)
		deleteBinaryTree(node->pRight);

	delete node;
}

/*******************************************
* BST :: COPY CONSTRUCTOR
*******************************************/
template <class T>
BST <T> ::BST(const BST <T> & rhs) throw (const char *)
{
   this->pRoot = NULL;
   
   *this = rhs;
}

/*******************************************
* BST :: ASSIGNMENT OPERATOR
* Copies the contents of one tree into another
*******************************************/
template <class T>
BST <T> & BST <T> :: operator = (const BST & rhs)
throw (const char *)
{
   clear(); 
   // if this is empty, we're done
   if (rhs.empty())
      return *this;
   
   else
   {
      // copy the root
      this->pRoot = new BinaryNode <T>(rhs.pRoot->data);
      pRoot->isRed = false;
      
      // recurse down this baby
      copy(rhs.pRoot, this->pRoot);
   }
   
   return *this;
}

/*************************************************
*BST::COPY
* A recursive copy function.Goes through each node
* and copies it's children into a new tree.
**************************************************/
template <class T>
void BST <T> ::copy(BinaryNode<T> * pThis, BinaryNode<T> * pRhs)
throw (const char*)
{
   // base case -- no children
   if (pThis->pLeft == NULL && pThis->pRight == NULL)
      return;
   
   // recursive case -- copy the children
   else
   {
      if (pThis->pLeft)
      {
         // create a copy node in the new tree
         pRhs->addLeft(pThis->pLeft->data);
         pRhs->pLeft->isRed = pThis->pLeft->isRed; 
         
         // recurse down the tree
         copy(pThis->pLeft, pRhs->pLeft);
      }
      
      if (pThis->pRight)
      {
         // same as above, but for right
         pRhs->addRight(pThis->pRight->data);
         pRhs->pRight->isRed = pThis->pRight->isRed;
         
         copy(pThis->pRight, pRhs->pRight);
      }
   }
}

/*******************************************
* BST :: INSERT
* Inserts an item into the proper location in
* the binary search tree
*******************************************/
template <class T>
void BST <T> ::insert(const T & t) throw (const char *)
{
   // if empty, create a new node
   if (empty())
   {
      try
      {
         pRoot = new BinaryNode <T>(t);
         setColor(pRoot); 
      }
      catch (std::bad_alloc)
      {
         throw NODE_ERROR_ALLOCATION;
      }
	}
   
   else
      recursiveAdd(t, pRoot);
}

/*******************************************
* BST :: REMOVE
* Deletes an item from a BST
*******************************************/
template <class T>
void BST <T> ::remove(BSTIterator <T> it) throw (const char *)
{
   if (it.p == NULL)
      throw "ERROR: null iterator";
   
   // two children
   if (it.p->pLeft != NULL && it.p->pRight != NULL)
   {
      // copy the pointer for later use
      BinaryNode <T> * pTemp = it.p;
      
      // get the inorder successor
      ++it;
      
      // copy the data in
      pTemp->data = it.p->data;
      
      remove(it);
   }
   
   // barren or one heir
   else
   {
      // am I the left or right child?
      if (it.p->pParent->pLeft == it.p)
      {
         // is my child left or right?
         if (it.p->pLeft != NULL)
         {
            // point parent down to child
            it.p->pParent->pLeft = it.p->pLeft;
            
            // point child up to parent
            it.p->pLeft->pParent = it.p->pParent;
         }
         else
         {
            // point parent down
            it.p->pParent->pLeft = it.p->pRight;
            
            // point child up
            if (it.p->pLeft != NULL)
               it.p->pLeft->pParent = it.p->pParent;
         }
         
      }
      else
      {
         // then we must be the right child
         assert(it.p->pParent->pRight == it.p);
         
         if (it.p->pLeft != NULL)
         {
            it.p->pParent->pRight = it.p->pLeft;
            
            it.p->pLeft->pParent = it.p->pParent;
         }
         else
         {
            it.p->pParent->pRight = it.p->pRight;
            
            // don't de-reference if it's null
            if (it.p->pRight != NULL)
               it.p->pRight->pParent = it.p->pParent;
         }
      }
      
      // now kill it
      delete it.p;
      
   }
}

/*******************************************
* BST :: RECURSIVEADD
* Inserts an item into its proper location
* in a BST
*******************************************/
template <class T>
void BST <T> ::recursiveAdd(const T & t, BinaryNode <T> * pNode) throw (const char *)
{
   // go left
   if (pNode->data > t)
   {
      // anchor left
      if (pNode->pLeft == NULL)
      {
         pNode->addLeft(t);
         setColor(pNode->pLeft);
      }
      else
         recursiveAdd(t, pNode->pLeft);
   }

   // go right
   else
   {
      // anchor right
      if (pNode->pRight == NULL)
      {
         pNode->addRight(t);
         setColor(pNode->pRight);
      }
      else
         recursiveAdd(t, pNode->pRight);
   }
}

/*******************************************
* BST :: FIND
* Finds a given value if it is in the tree.
* Returns end() otherwise
*******************************************/
template<class T>
BSTIterator<T> BST <T> ::find(const T & t)
{
   BinaryNode<T> * tmp = pRoot;
   
   while (tmp) //we haven't arrived at a leaf
   {
      if (tmp->data == t)
         return BSTIterator<T>(tmp);
      else if (tmp->data > t)
         tmp = tmp->pLeft;
      else //tmp->data < t
         tmp = tmp->pRight;
   }
   
   return end();
}

/************************************************************************
* setColor
* checks all the different possible cases for rb tree balancing and calls
* itself recursively if another node needs fixing
***********************************************************************/
template<class T>
void BST <T> :: setColor(BinaryNode <T> * pThis)
{
   //Set all of our family relations
   BinaryNode<T> * pPar = pThis->pParent; 
   BinaryNode<T> * pGrand;
   BinaryNode<T> * pSib;
   BinaryNode<T> * pAunt; 
   //If you have a parent
   
   if (pPar)
   {
      //CASE 2
      if (!(pPar->isRed) && pThis->isRed)
         return; //nothing else needed
      else
         pGrand = pPar->pParent;
	}
   //CASE 1
   //pThis must be the root so it should be black
   else
   {
      pThis->isRed = false; 
      return; //no balancing will be needed
   }
   if (pPar->pLeft == pThis) //&& pPar->pRight)
      pSib = pPar->pRight;
   else if (pPar->pRight == pThis) //&& pPar->pLeft)
      pSib = pPar->pLeft;
   else
      return; //no balancing will be needed
   if (pGrand)
   {
      if (pGrand->pLeft == pPar) //&& pGrand->pRight)
         pAunt = pGrand->pRight;
      else if (pGrand->pRight == pPar) //&& pGrand->pLeft)
         pAunt = pGrand->pLeft;
      else
         return; //no balancing will be needed
   }
   else
		return; //no balancing will be needed
   
   //Now all of the other cases
   
   //CASE 3
   //if (pPar->isRed && !(pGrand->isRed) && (!pAunt || (pAunt && pAunt->isRed)))
   if (pPar->isRed && !(pGrand->isRed) && pAunt && pAunt->isRed)
   {
      pGrand->isRed = true;
      pPar->isRed = false;
      if (pAunt)
         pAunt->isRed = false; 
      //recursive call to grandparent
      setColor(pGrand); 
      return; 
   }
   
   //CASE 4
   else
   {
      //Left child of Grand
      if (pGrand->pLeft == pPar)
      {
         //a, left child of parent
         if (pPar->pLeft == pThis)
            rotateLeft(pThis, pPar, pGrand, pSib, pAunt); 
         //b, right child of parent
         else
            rotateDoubleLeft(pThis, pPar, pGrand, pSib, pAunt); 
      }
      //Right child of Grand
      else
      {
         //c, right child of parent
         if (pPar->pRight == pThis)
            rotateRight(pThis, pPar, pGrand, pSib, pAunt);
         //d, left child of parent
         else
            rotateDoubleRight(pThis, pPar, pGrand, pSib, pAunt);
      }      
   }  
}

/************************************************************************
* rotateRight
* Performs a right rotation
***********************************************************************/
template<class T>
void BST<T>::rotateRight(BinaryNode <T> * pThis, BinaryNode <T> * pPar,
	BinaryNode <T> * pGrand, BinaryNode <T> * pSib,
	BinaryNode <T> * pAunt)
{
	
	if (pGrand->pParent)
	{
		if (pGrand->pParent->pLeft == pGrand)
			pGrand->pParent->pLeft = pPar;
		else
			pGrand->pParent->pRight = pPar;
	}
	else
		pRoot = pPar;
	pPar->pParent = pGrand->pParent;
	pPar->pLeft = pGrand;
	pGrand->pParent = pPar;
	if (pSib)
		pSib->pParent = pGrand;
	pGrand->pRight = pSib;

	pPar->isRed = false;
	pGrand->isRed = true;
}

/************************************************************************
* rotateLeft
* Performs a left rotation
***********************************************************************/
template<class T>
void BST<T>::rotateLeft(BinaryNode <T> * pThis, BinaryNode <T> * pPar,
	BinaryNode <T> * pGrand, BinaryNode <T> * pSib,
	BinaryNode <T> * pAunt)
{
	if (pGrand->pParent)
	{
		if (pGrand->pParent->pLeft == pGrand)
			pGrand->pParent->pLeft = pPar;
		else
			pGrand->pParent->pRight = pPar;
	}
	else
		pRoot = pPar;
	pPar->pParent = pGrand->pParent;
	pPar->pRight = pGrand;
	pGrand->pParent = pPar;
	if (pSib)
		pSib->pParent = pGrand;
	pGrand->pLeft = pSib;

	pPar->isRed = false;
	pGrand->isRed = true;
}

/************************************************************************
* rotateDoubleLeft
* Performs a double left rotation
***********************************************************************/
template<class T>
void BST<T>::rotateDoubleLeft(BinaryNode <T> * pThis, BinaryNode <T> * pPar,
	BinaryNode <T> * pGrand, BinaryNode <T> * pSib,
	BinaryNode <T> * pAunt)
{
	if (pGrand->pParent)
	{
		if (pGrand->pParent->pLeft == pGrand)
			pGrand->pParent->pLeft = pThis;
		else
			pGrand->pParent->pRight = pThis;
	}
	else
		pRoot = pThis;

	pThis->pParent = pGrand->pParent;
	pGrand->pParent = pThis;
	pPar->pParent = pThis;
	//hook up the children
	if (pThis->pLeft)
		pThis->pLeft->pParent = pPar;
	if (pThis->pRight)
		pThis->pRight->pParent = pGrand;
	pPar->pRight = pThis->pLeft;
	pGrand->pLeft = pThis->pRight;
	pThis->pLeft = pPar;
	pThis->pRight = pGrand;

	pThis->isRed = false;
	pGrand->isRed = true;
}

/************************************************************************
* rotateDoubleRight
* Performs a double right rotation
***********************************************************************/
template<class T>
void BST<T>::rotateDoubleRight(BinaryNode <T> * pThis, BinaryNode <T> * pPar,
	BinaryNode <T> * pGrand, BinaryNode <T> * pSib,
	BinaryNode <T> * pAunt)
{
	if (pGrand->pParent)
	{
		if (pGrand->pParent->pLeft == pGrand)
			pGrand->pParent->pLeft = pThis;
		else
			pGrand->pParent->pRight = pThis;
	}
	else
		pRoot = pThis;

	pThis->pParent = pGrand->pParent;
	pGrand->pParent = pThis;
	pPar->pParent = pThis;
	//hook up the children
	if (pThis->pLeft)
		pThis->pLeft->pParent = pGrand;
	if (pThis->pRight)
		pThis->pRight->pParent = pPar;
	pPar->pLeft = pThis->pRight;
	pGrand->pRight = pThis->pLeft;

	pThis->pRight = pPar;
	pThis->pLeft = pGrand;

	pThis->isRed = false;
	pGrand->isRed = true;
}


/************************************************************************
* findSuc
* A private function for the iterator class. Finds the in-order successor
* of the current p pointer or returns NULL if there isn't one
***********************************************************************/
template<class T>
void BSTIterator<T>::findSuc()
{
	//"Recursive Case" (we already point to NULL)
	if (!p)
		return;
	//If we have a right child
	else if (p->pRight)
	{
		BinaryNode<T> * tmp = p->pRight;
		//Go as far left as possible
		while (tmp->pLeft)
			tmp = tmp->pLeft;

		p = tmp;
		return;
	}
	else if (p->pParent) //We aren't already at the root Node
	{
		//if this is the left child
		if (p->pParent->pLeft == p)
		{
			p = p->pParent;

			return;
		}

		//if we are the right child
		else if (p->pParent->pRight == p)
		{
			BinaryNode<T> * tmp = p->pParent;
			//as long as we remain the right child
			while (tmp->pParent && tmp->pParent->pRight == tmp)
				tmp = tmp->pParent;

			if (!tmp->pParent) //we've arrived at the root node
			{
				p = NULL;
				return;
			}

			//Make it the parent
			tmp = tmp->pParent;

			p = tmp;
			return;
		}
	}
	else //we're already at the root and we have no more right children
	{
		p = NULL;
		return;
	}
}

/************************************************************************
* findPre
* A private function for the iterator class. Finds the in-order predeccessor
* of the current p pointer or returns NULL if there isn't one
***********************************************************************/
template<class T>
void BSTIterator<T>::findPre()
{
	//"Recursive Case" (we already point to NULL)
	if (!p)
		return;
	//If we have a left child
	else if (p->pLeft)
	{
		BinaryNode<T> * tmp = p->pLeft;
		//Go as far right as possible
		while (tmp->pRight)
			tmp = tmp->pRight;

		p = tmp;
		return;
	}
	else if (p->pParent) //We aren't already at the root Node
	{
		//if this is the right child
		if (p->pParent->pRight == p)
		{
			p = p->pParent;

			return;
		}

		//if we are the left child
		else if (p->pParent->pLeft == p)
		{
			BinaryNode<T> * tmp = p->pParent;
			//as long as we remain the left child
			while (tmp->pParent && tmp->pParent->pLeft == tmp)
				tmp = tmp->pParent;

			if (!tmp->pParent) //we've arrived at the root node
			{
				p = NULL;
				return;
			}

			//Make it the parent
			tmp = tmp->pParent;

			p = tmp;
			return;
		}
	}
	else //we're already at the root and we have no more right children
	{
		p = NULL;
		return;
	}
}

/************************************************************************
* begin
* Returns an iterator to the first item in the BST
***********************************************************************/
template<class T>
BSTIterator<T> BST<T>::begin()
{
	if (pRoot)
	{
		BinaryNode<T> * tmp = pRoot;

		while (tmp->pLeft)
			tmp = tmp->pLeft;
		return BSTIterator<T>(tmp);
	}
	else
		return BSTIterator<T>(NULL);
}

/************************************************************************
* rbegin
* Returns an iterator to the last item in the BST
***********************************************************************/
template<class T>
BSTIterator<T> BST<T>::rbegin()
{
	if (pRoot)
	{
		BinaryNode<T> * tmp = pRoot;

		while (tmp->pRight)
			tmp = tmp->pRight;

		return BSTIterator<T>(tmp);
	}
	else
		return BSTIterator<T>(NULL);
}

/************************************************************************
* testDisplay
* A recursive display function for testing the BST. Displays the current
* value in each node along with its level in the tree and current isRed
***********************************************************************/
template<class T>
void testDisplay(BinaryNode <T> * pRoot, int level)
{
	if (pRoot)
	{
		testDisplay(pRoot->pLeft, level + 1);
		std::cout << pRoot->data << "," << level << "," << pRoot->isRed << std::endl;
		testDisplay(pRoot->pRight, level + 1); 
	}
	else
		return; 
}

#endif // BST_H



