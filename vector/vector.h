/***********************************************************************
* Header:
*    Vector
* Summary:
*    This class contains a mimic of the Standard Template Library vector.
*
*    This file contains the class definitions of:
*        Vector              : The abstract data type
*        VectorIterator      : An interator through Vector
*        VectorConstIterator : A constant iterator throught the vector
* Author:
*    Justin Ricks
************************************************************************/

#ifndef VECTOR_H
#define VECTOR_H

#include <cassert>

// forward declaration for the iterator classes
template <class T>
class VectorIterator;

template <class T>
class VectorConstIterator;

/************************************************
 * VECTOR
 * A custom vector class
 ***********************************************/
template <class T>
class Vector
{
public:
   // Assignment Operator
   Vector & operator = (const Vector & rhs);
   
   // default constructor
   Vector() : numItems(0), maxSize(0), data(0x00000000) {}

   // copy constructor
   Vector(const Vector & rhs) throw (const char *)
      : numItems(0), maxSize(0), data(0x00000000) { *this = rhs; }
   
   // non-default constructor : pre-allocate
   Vector(int size) throw (const char *);
   
   // destructor : free everything
   ~Vector() { if (maxSize) delete [] data; }

   //                  Operators                        //
   // Square Bracket Operator
   T & operator [] (int index) throw (const char *);

   // Parentheses Operator (functor)
   T operator () (int index) const throw (const char *);
   
   // is the container currently empty
   bool empty() const   { return numItems == 0;         }

   // remove all the items from the vector
   void clear()         { numItems = 0;                 }

   // how many items are currently in the vector?
   int size() const     { return numItems;              }

   // what is the capacity of the vector?
   int capacity() const { return this->maxSize;         }

   // remove the last item
   void pop_back() { --numItems; }

   // push back an item
   void push_back(const T &t) throw (const char *);
   
   // return an iterator to the beginning of the list
   VectorIterator <T> begin() { return VectorIterator<T>(data); }

   // return an iterator to the end of the list
   VectorIterator <T> end() { return VectorIterator<T>(data + numItems); }

   // return a reverse iterator to the beginning of the list
   VectorIterator <T> rbegin()
   {
      return VectorIterator<T>(data + numItems - 1);
   }

   // return a reverse iterator to the end of the list
   VectorIterator <T> rend() { return VectorIterator<T>(data - 1); }

   // return a const iterator to the beginning of the list
   VectorConstIterator <T> cbegin() const
   {
      return VectorConstIterator<T>(data);
   }

   // return a const iterator to the end of the list
   VectorConstIterator <T> cend() const
   {
      return VectorConstIterator<T>(data + numItems);
   }

   // return a constant reverse iterator to the beginning
   VectorConstIterator <T> crbegin() const
   {
      return VectorConstIterator<T>(data + numItems - 1);
   }

   // return a constant reverse iterator to the end
   VectorConstIterator <T> crend() const
   {
      return VectorConstIterator<T>(data - 1);
   }
   
private:
   T * data;          // dynamically allocated array of T
   int numItems;      // how many items are currently in the Vector?
   int maxSize;       // how many items can I put on the Vector before full?

   // resize the array
   void reallocate () throw (const char *);
};

/**************************************************
 * VECTOR ITERATOR
 * An iterator through Container
 *************************************************/
template <class T>
class VectorIterator
{
  public:
   // default constructor
   VectorIterator() : p(0x00000000) {}

   // initialize to direct p to some item
   VectorIterator(T * p) : p(p) {}

   // copy constructor
   VectorIterator(const VectorIterator & rhs) { *this = rhs; }

   // assignment operator
   VectorIterator & operator = (const VectorIterator & rhs)
   {
      this->p = rhs.p;
      return *this;
   }

   // not equals operator
   bool operator != (const VectorIterator & rhs) const
   {
      return rhs.p != this->p;
   }

   // dereference operator
   T & operator * ()
   {
      return *p;
   }

   // prefix increment
   VectorIterator <T> & operator ++ ()
   {
      p++;
      return *this;
   }

   // postfix increment
   VectorIterator <T> operator++(int postfix)
   {
      VectorIterator tmp(*this);
      p++;
      return tmp;
   }

   // prefix decrement
   VectorIterator <T> & operator -- ()
   {
      p--;
      return *this;
   }

   // postfix decrement
   VectorIterator <T> operator -- (int postfix)
   {
      VectorIterator tmp(*this);
      p--;
      return tmp;
   }
   

   // only member variable is a pointer to a template
  private:
   T * p;
};

/**************************************************
 * VECTOR CONST ITERATOR
 * A constant iterator through Container
 *************************************************/
template <class T>
class VectorConstIterator
{
  public:
   // default constructor
   VectorConstIterator() : p(0x00000000) {}

   // initialize to direct p to some item
   VectorConstIterator(T * p) : p(p) {}

   // copy constructor
   VectorConstIterator(const VectorConstIterator & rhs) { *this = rhs; }

   // assignment operator
   VectorConstIterator & operator = (const VectorConstIterator & rhs)
   {
      this->p = rhs.p;
      return *this;
   }

   // not equals operator
   bool operator != (const VectorConstIterator & rhs) const
   {
      return rhs.p != this->p;
   }

   // dereference operator - return by value
   T operator * () const
   {
      return *p;
   }

   // prefix increment
   VectorConstIterator <T> & operator ++ ()
   {
      p++;
      return *this;
   }

   // postfix increment
   VectorConstIterator <T> operator++(int postfix)
   {
      VectorConstIterator tmp(*this);
      p++;
      return tmp;
   }

   // prefix decrement
   VectorConstIterator <T> & operator -- ()
   {
      p--;
      return *this;
   }

   // postfix decrement
   VectorConstIterator <T> operator -- (int postfix)
   {
      VectorConstIterator tmp(*this);
      p--;
      return tmp;
   }
   

   // only member variable is a pointer to a template
  private:
   T * p;
};

/**********************************************
 * NON-DEFAULT CONSTRUCTOR
 * Preallocate the vector to "capacity"
 **********************************************/
template <class T>
Vector <T> :: Vector(int capacity) throw (const char *)
{
   assert(capacity >= 0);
   
   // do nothing if there is nothing to do
   if (capacity == 0)
   {
      this->maxSize = this->numItems = 0;
      this->data = 0x00000000;
      return;
   }

   // attempt to allocate
   try
   {
      data = new T[capacity];
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate buffer";
   }

   // set the capacity and initialize the number of items
   this->maxSize = capacity;
   this->numItems = 0;
}

/***************************************************
 * REALLOCATE
 * Attempt to double the size of the buffer
 ***************************************************/
template <class T>
void Vector <T> ::  reallocate () throw (const char *)
{
   assert(maxSize >= 0);

   // if we're starting at zero, increase to 1
   if (maxSize == 0)
   {
      try
      {
         // allocate 1 space
         T * newBuffer = new T[1];
         maxSize = 1;

         // free the old stuff
         delete [] data;

         // reset the pointer to valid data
         data = newBuffer;

         return;
      }
      catch (std::bad_alloc)
      {
         throw "ERROR: Unable to allocate a new buffer for Vector";
      }
   }

   // otherwise double the capacity
   try
   {
      // allocate a new buffer of double size
      T * newBuffer = new T[maxSize * 2];
      
      // copy the crud
      for (int i = 0; i < maxSize; i++)
      {
         newBuffer[i] = data[i];
      }

      // reset the maxSize
      maxSize *= 2;
      
      // free the resources
      delete [] data;

      // change the pointer
      data = newBuffer;

      return;
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate a new buffer for Vector";
   }
}

/***************************************************
 * PUSH_BACK
 * Add an item to the end of the vector
 ***************************************************/
template <class T>
void Vector <T> :: push_back(const T &t) throw (const char *)
{
   // is there space in the vector?
   if (maxSize == 0 || maxSize == numItems)
   {      
      reallocate();
   }

   // put the item at the end, increment the number of items
   data[numItems++] = t;
}

/***************************************************
 * OPERATOR []
 * Access an item by reference at a given index
 ***************************************************/
template <class T>
T & Vector <T> :: operator [] (int index) throw (const char *)
{
   if (index > this->maxSize)
      throw "ERROR: Invalid Index";
   
   return data[index];
}

/***************************************************
 * OPERATOR () (FUNCTOR)
 * Access an item by value at a given index. This
 * function may not change any member variables!
 ***************************************************/
template <class T>
T Vector <T> :: operator () (int index) const throw (const char *)
{
   if (index > this->maxSize)
      throw "ERROR: Invalid Index";
   
   return data[index];
}

/***************************************************
 * ASSIGNMENT OPERATOR
 * Copy the contents of another vector into this one
 ***************************************************/
template <class T>
Vector <T> & Vector <T> :: operator = (const Vector & rhs)
{
   assert(rhs.maxSize >= 0);

   // do nothing if there is nothing to do
   if (rhs.maxSize == 0)
   {
      maxSize = numItems = 0;

      delete data;

      data = 0x00000000;
      return *this;
   }

   // free the old data
   if (data != NULL)
      delete data;

   // attempt to allocate
   try
   {
      data = new T[rhs.maxSize];
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate a new buffer for Vector";
   }

   // copy over the stuff
   assert(rhs.numItems >= 0 && rhs.numItems <= rhs.maxSize);
   maxSize = rhs.maxSize;
   numItems = rhs.numItems;
   for (int i = 0; i < numItems; i++)
      data[i] = rhs.data[i];

   return *this;   
}

#endif // CONTAINER_H

