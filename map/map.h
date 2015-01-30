/***********************************************************************
 * Header:
 *    Map
 * Summary:
 *    
 *    This will contain the class definition of:
 *        Map          : An associative array ADT
 *        MapIterator  : An interator through the map
 * Author
 *    Justin Ricks, based on the BST written by Justin Ricks and Nick Recht
 ************************************************************************/

#ifndef MAP_H
#define MAP_H

#include "bst.h"
#include "pair.h"

// forward declaration for the iterator
template <class T1, class T2>
class MapIterator;

/************************************************
 * Map
 * A Map container. Pretty useful!
 ***********************************************/
template <class T1, class T2>
class Map
{
  public:
   // Default constructor -- don't do anything
   //  everything is handled in the BST's constructor
   Map() : numItems(0) {};

   Map(const Map <T1, T2> & rhs) throw (const char *)
      { *this = rhs; }

   // Destructor -- likewise
   ~Map() {};

   // Assignment Operator
   Map <T1, T2> & operator = (const Map <T1, T2> & rhs)
      throw (const char *)
      { this->bst = rhs.bst; this->numItems = rhs.numItems; }

   // clear the map's contents
   void clear() { bst.clear(); numItems = 0; }

   // how many items are in the map?
   int size() { return numItems; }
   
   // and is the map empty?
   bool empty() { return bst.empty(); };
   
   // find the location of an item
   MapIterator <T1, T2> find(const T1 & t)
   {
      T2 empty;

      Pair <T1, T2> temp (t, empty);
      return MapIterator <T1, T2> (bst.find(temp));
   }

   // square bracket operator
   T2 & operator [] (const T1 & key);

   // iterator methods
   MapIterator <T1, T2> begin()
      { return MapIterator <T1, T2> (bst.begin()); }

   MapIterator <T1, T2> end()
      { return MapIterator <T1, T2> (bst.end()); }

   MapIterator <T1, T2> rbegin()
      { return MapIterator <T1, T2> (bst.rbegin()); }

   MapIterator <T1, T2> rend()
      { return MapIterator <T1, T2> (bst.rend()); }

   // what's underneath? a BST!
  private:
   BST < Pair <T1, T2> > bst;

   int numItems;
};

/************************************************
 * MapIterator
 * An iterator for the Map class; uses a BSTIterator
 ***********************************************/
template <class T1, class T2>
class MapIterator
{
  public:
   //Constructors
   MapIterator() { };
   MapIterator(const MapIterator & rhs) { *this = rhs;    }
   MapIterator(const BSTIterator < Pair <T1, T2> > & rhs)
      { this->it = rhs; }

   //Operators
   MapIterator & operator = (const MapIterator & rhs)
   {
      this->it = rhs.it;
      return *this;
   }

   bool operator == (const MapIterator & rhs) const
      { return this->it == rhs.it; }
   
   bool operator != (const MapIterator & rhs) const
      { return !(*this == rhs); }

   // dereference -- just dereference the underlying iterator
   const T2 & operator * ()  { return (*it).second; }

   //Increment
   MapIterator & operator ++ ()
   {
      ++it;
      
      return (*this);
   }
   
   MapIterator & operator ++ (int postfix)
   {
      MapIterator tmp(*this);
      ++(*this);

      return tmp;
   }

   //Decrement
   MapIterator & operator -- ()
   {
      --it;
      
      return *this;
   }
   
   MapIterator & operator -- (int postfix)
   {
      MapIterator tmp(*this);
      --(*this);

      return tmp;
   }

  private:
   BSTIterator < Pair <T1, T2> > it;
};

/***********************************************
 * Square Bracket Operator
 * Gets a value based on the key. Also used to
 * add new values to the map
 **********************************************/
template <class T1, class T2>
T2 & Map <T1, T2> :: operator [] (const T1 & key)
{
   // we didn't get passed in a T2, so we have to make one up
   T2 empty;
   Pair <T1, T2> search(key, empty);
   
   // find out if it already exists
   BSTIterator < Pair <T1, T2> > iter = this->bst.find(search);
   
   // if it's in the tree, return its value
   if (iter != this->bst.end())
      return iter.p->data.second; 
   
   // if it's not in the tree, create a new pair
   else
   {
      Pair<T1, T2> * newPair = new Pair<T1, T2>(key, empty);

      // insert the new pair into the tree
      bst.insert(*newPair);

      ++numItems;

      // now find it and return its data
      iter = this->bst.find(*newPair); 
      return iter.p->data.second;
      
   }
}

#endif // MAP_H
