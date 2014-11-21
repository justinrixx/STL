/***********************************************************************
 * Header:
 *    List
 * Summary:
 *    This file has lots of good stuff! The main event here is the list
 *    class, a node-based container.
 *    Extra credit attempted: const iterators!
 *
 *    This will contain the class definition of:
 *        List              : A class that holds stuff
 *        ListIterator      : An interator through List
 *        ConstListIterator : A const iterator
 *        Node              : A doubly linked node
 * Author
 *    Justin Ricks
 ************************************************************************/

#ifndef LIST_H
#define LIST_H

#include <cassert>
#include <iostream>

// forward declaration for the iterators and node classes
template <class T>
class ListIterator;

template <class T>
class Node;

template <class T>
class ConstListIterator;

/************************************************
 * List
 * A linked list container
 ***********************************************/
template <class T>
class List
{
  public:
   // default constructor
   List() : pHead(NULL), pTail(NULL), numItems(0) {}

   // copy constructor
   List(const List & rhs) throw (const char *);

   // destructor
   ~List() { freeData(this->pHead); }

   // is the container empty?
   bool empty() const { return numItems == 0; }

   // clear the contents of the container
   void clear() { freeData(this->pHead); }

   // insert an item at the head of the container
   void push_front(const T & t) throw (const char *);

   // insert an item at the tail of the container
   void push_back(const T & t) throw (const char *);

   // assignment operator
   List <T> & operator = (const List & rhs) throw (const char *);

   // get item at the front of the list
   T & front() throw (const char *);

   // get item at the back of the list
   T & back() throw (const char *);

   // insert in the middle somewhere
   ListIterator <T> insert(ListIterator <T> it, const T & t) throw (const char *);

   // returns size of list
   int size() const { return numItems; }
   
   // remove from the middle of the list
   void remove(ListIterator <T> it) throw (const char *);

   /********************** Iterators **********************/
   
   // return an iterator to the beginning of the list
   ListIterator <T> begin() const { return ListIterator<T>(pHead); }

   // return an iterator to the end of the list
   ListIterator <T> end() const { return ListIterator<T>(NULL); }

   // return an reverse iterator to the end of the list
   ListIterator <T> rbegin() const { return ListIterator<T>(pTail); }

   // return an reverse iterator to the end of the list
   ListIterator <T> rend() const { return ListIterator<T>(NULL); }


   /******************* Const Iterators ********************/

   // return a const iterator to the beginning of the list
   ConstListIterator <T> cbegin() const { return ConstListIterator<T>(pHead); }

   // return an iterator to the end of the list
   ConstListIterator <T> cend() const { return ConstListIterator<T>(NULL); }

   // return an reverse iterator to the end of the list
   ConstListIterator <T> crbegin() const { return ConstListIterator<T>(pTail); }

   // return an reverse iterator to the end of the list
   ConstListIterator <T> crend() const { return ConstListIterator<T>(NULL); }

  private:
   Node <T> * pHead; // pointer to the list of data
   Node <T> * pTail; // the last item in the list

   int numItems;

   // free the data in this thing. This is used by both the clear
   // function, and the destructor, so it gets its own function
   void freeData(Node <T> * & pHead);

   // debug code;
   void displayTheList();
};

/**************************************************
 * LIST ITERATOR
 * An iterator through List
 *************************************************/
template <class T>
class ListIterator
{
  public:
   // default constructor
   ListIterator() : p(0x00000000) {}

   // initialize the pointer to something
   ListIterator(Node <T> * p) : p(p) {}

   // copy constructor
   ListIterator(const ListIterator & rhs) { *this = rhs; }

   // assignment operator
   ListIterator & operator = (const ListIterator & rhs)
   {
      this->p = rhs.p;
      return *this;
   }

   // not equals operator
   bool operator != (const ListIterator & rhs) const
   {
      return rhs.p != this->p;
   }

   // equals equals operator
   bool operator == (const ListIterator & rhs) const
   {
      return rhs.p == this->p;
   }

   // dereference operator
   T & operator * ()
   {
      return p->data;
   }
   
   // prefix increment
   ListIterator <T> & operator ++ ()
   {
      p = p->pNext;
      return *this;
   }

   // postfix increment
   ListIterator <T> operator ++ (int postfix)
   {
      ListIterator tmp(*this);

      p = p->pNext;
      return tmp;
   }

   // prefix decrement
   ListIterator <T> & operator -- ()
   {
      p = p->pPrev;
      return *this;
   }

   // postfix decrement
   ListIterator <T> operator -- (int postfix)
   {
      ListIterator tmp(*this);

      p = p->pPrev;
      return tmp;
   }
   

  private:
   Node <T> * p;

   // last of all, remember, the List class is your friend :)
   friend ListIterator <T> List <T> :: insert(ListIterator <T> it,
                                              const T & t)
                                             throw (const char *);

   friend void List <T> :: remove(ListIterator <T> it)
                       throw (const char *);
};

/**************************************************
 * CONST LIST ITERATOR
 * A const iterator through List
 *************************************************/
template <class T>
class ConstListIterator
{
  public:
   // default constructor
   ConstListIterator() : p(0x00000000) {}

   // initialize the pointer to something
   ConstListIterator(Node <T> * p) : p(p) {}

   // copy constructor
   ConstListIterator(const ConstListIterator & rhs) { *this = rhs; }

   // assignment operator
   ConstListIterator & operator = (const ConstListIterator & rhs)
   {
      this->p = rhs.p;
      return *this;
   }

   // not equals operator
   bool operator != (const ConstListIterator & rhs) const
   {
      return rhs.p != this->p;
   }

   // equals equals operator
   // equals equals operator
   bool operator == (const ConstListIterator & rhs) const
   {
      return rhs.p == this->p;
   }

   // dereference operator -- return a copy
   T operator * () const
   {
      return p->data;
   }

   // prefix increment
   ConstListIterator <T> & operator ++ ()
   {
      p = p->pNext;
      return *this;
   }

   // postfix increment
   ConstListIterator <T> operator ++ (int postfix)
   {
      ConstListIterator tmp(*this);

      p = p->pNext;
      return tmp;
   }

   // prefix decrement
   ConstListIterator <T> & operator -- ()
   {
      p = p->pPrev;
      return *this;
   }

   // postfix decrement
   ConstListIterator <T> operator -- (int postfix)
   {
      ConstListIterator tmp(*this);

      p = p->pPrev;
      return tmp;
   }


  private:
   Node <T> * p;
}; 

/******************************************************************
 * Node Class
 * This is a public class; no private anything, used in implementing
 * a linked list. This Node class is double linked (forward and back)
 ******************************************************************/
template <class T>
class Node
{
  // everything's public!
  public:
   Node() : pNext(NULL), pPrev(NULL) {}

   // non-default constructor
   Node(const T & t)
      : pNext(NULL), pPrev(NULL) { this->data = t; }

   // pointer to the next item
   Node * pNext;

   // pointer to the previous item
   Node * pPrev;

   // the data
   T data;
};


/*******************************************
 * LIST :: COPY CONSTRUCTOR
 *******************************************/
template <class T>
List <T> :: List(const List <T> & rhs) throw (const char *)
{
   this->pHead = this->pTail = NULL;
   this->numItems = 0;
   
   *this = rhs;
}

/***************************************************
 * LIST :: INSERT
 * Insert an item in the middle of the container.
 * Passed in the data to insert, and an iterator to the
 * node to insert before.
 **************************************************/
template <class T>
ListIterator <T> List <T> :: insert(ListIterator <T> it, const T & t) throw (const char *)
{
   // create a temporary pointer so there's something to return
   Node <T> * pTemp = it.p;
   
   // special case: if we're inserting at the head
   if (it.p == pHead)
   {
      try
      {
         Node <T> * pNew = new Node <T> (t);

         // point forward to head, and point head back to new
         pNew->pNext = pHead;
         pHead->pPrev = pNew;

         // update the member variable
         pHead = pNew;

         return ListIterator <T> (pTemp);
      }
      
      catch (std::bad_alloc)
      {
         throw "ERROR: Could not allocate a new node for list";
      }
   }
   
   // special case: inserting at the end
   else if (it.p == NULL)
   {
      try
      {
         Node <T> * pNew = new Node <T> (t);

         // point back to tail, point tail forward to new
         pNew->pPrev = pTail;
         pTail->pNext = pNew;

         // update the member variable
         pTail = pNew;
      }

      catch (std::bad_alloc)
      {
         throw "ERROR: Could not allocate a new node for list";
      }
   }
   
   else
   {
      try
      {
         Node <T> * pNew = new Node <T> (t);

         // set the new node's pointers
         pNew->pNext = it.p;
         pNew->pPrev = it.p->pPrev;

         // change the adjacent nodes' pointers
         pNew->pNext->pPrev = pNew;
         pNew->pPrev->pNext = pNew;

         return ListIterator <T> (pTemp);
      }

      catch (std::bad_alloc)
      {
         throw "ERROR: Could not allocate a new node for list";
      }
   }
}

/***************************************************
 * LIST :: FREE DATA
 * Deallocate a list
 **************************************************/
template <class T>
void List <T> :: freeData(Node <T> * & pHead)
{
   // if this is null, return out
   if (pHead == NULL)
      return;

   else
   {
      // call freeData on the next item
      freeData(pHead->pNext);

      // now delete the data
      delete pHead->pNext;
      pHead = NULL;
   }

   this->numItems = 0;
   this->pTail = NULL;
   
   return;
}

/***************************************************
 * LIST :: ASSIGNMENT OPERATOR
 * Copy a list and all its contents
 **************************************************/
template <class T>
List <T> & List <T> :: operator = (const List & rhs) throw (const char *)
{
   try
   {
      // first off, clear the contents of this
      this->freeData(this->pHead);

      assert(rhs.numItems >= 0);
      
      if (rhs.numItems == 0)
         return *this;

      this->numItems = rhs.numItems;
      
      // create a new head node * to return
      this->pHead = new Node <T>;

      // working copies, so we don't wreck where the head is
      Node <T> * thisHeadCopy = this->pHead;
      Node <T> * rhsHeadCopy = rhs.pHead;

      // copy the data in head
      thisHeadCopy->data = rhsHeadCopy->data;

      this->pTail = thisHeadCopy;

      // loop till the next item is the end of the list
      while(rhsHeadCopy->pNext)
      {
         // create a new node to hold what's in next
         thisHeadCopy->pNext = new Node <T> (rhsHeadCopy->pNext->data);

         // set the prev pointer to this
         thisHeadCopy->pNext->pPrev = thisHeadCopy;

         // advance the lists
         thisHeadCopy = thisHeadCopy->pNext;
         rhsHeadCopy  = rhsHeadCopy->pNext;

         // and set the tail
         this->pTail = thisHeadCopy;
      }

      // return this
      return *this;
   }

   catch (std::bad_alloc)
   {
      throw "ERROR: unable to allocate a new node for a list";
   }

   return *this;
}

/***************************************************
 * LIST :: PUSH_FRONT
 * Adds an item to the front of the list
 **************************************************/
template <class T>
void List <T> :: push_front(const T & t) throw (const char *)
{
   try
   {
      // create a new node
      Node <T> * pNew = new Node <T> (t);

      // point the new node to the first item in the list
      pNew->pNext = pHead;

      // since this is the front of the list, its pPrev is NULL
      pNew->pPrev = NULL;

      // change the old head's pPrev to point to the new head
      if (pHead)
         pHead->pPrev = pNew;

      // change the pHead to point to the new head
      pHead = pNew;

      // make sure pTail is pointing to something
      if (!pTail)
         pTail = pNew;

      ++numItems;
   }

   catch (std::bad_alloc)
   {
      throw "ERROR: Could not allocate a new node for list";
   }
   
}

/***************************************************
 * LIST :: PUSH_BACK
 * Adds an item to the back of the list
 **************************************************/
template <class T>
void List <T> :: push_back(const T & t) throw (const char *)
{
   try
   {
      // create a new node
      Node <T> * pNew = new Node <T> (t);

      // set it's pPrev to pTail
      pNew->pPrev = this->pTail;

      // set pTail's next to the new tail
      if (this->pTail)
         this->pTail->pNext = pNew;

      // update the pTail variable
      pTail = pNew;

      // make sure pHead is pointing to something
      if (!pHead)
         pHead = pNew;

      ++numItems;

   }

   catch (std::bad_alloc)
   {
      throw "ERROR: Could not allocate a new node for list";
   }
}

/***************************************************
 * LIST :: FRONT
 * Returns the item at the front of the list
 **************************************************/
template <class T>
T & List <T> :: front() throw (const char *)
{
   if (pHead == NULL)
      throw "ERROR: Attempting to access from empty list";

   return pHead->data;
}

/***************************************************
 * LIST :: BACK
 * Returns the item at the back of the list
 **************************************************/
template <class T>
T & List <T> :: back() throw (const char *)
{
   if (pTail == NULL)
      throw "ERROR: Attempting to access from empty list";

   return pTail->data;
}

/***************************************************
 * LIST :: REMOVE
 * Remove an item in the middle of the list. Is passed
 * an iterator to the item to remove
 * Returns an iterator to the item after the one removed
 **************************************************/
template <class T>
void List <T> :: remove(ListIterator <T> it) throw (const char *)
{
   if (this->empty() || it == this->end())
      throw "ERROR: unable to remove from an empty list";

   // handle the two special cases of removing the head and tail items
   if (it.p == pHead)
   {
      it.p->pNext->pPrev = NULL;

      pHead = it.p->pNext;

      delete it.p;
   }

   else if (it.p == pTail)
   {
      it.p->pPrev->pNext = NULL;

      pTail = it.p->pPrev;

      delete it.p;
   }
   
   else
   {
      // pointer dance with the privates!
      
      it.p->pPrev->pNext = it.p->pNext;

      it.p->pNext->pPrev = it.p->pPrev;
      
      delete it.p;
   }

   --numItems;

   if (numItems == 0)
   {
      this->pHead = NULL;
      this->pTail = NULL;
   }

   return;
}

#endif // CONTAINER_H
