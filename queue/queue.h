/***********************************************************************
 * Header:
 *    Queue
 * Summary:
 *    This class contains the notion of a container: a bucket to hold
 *    data for the user. This is just a starting-point for more advanced
 *    constainers such as the vector, set, stack, queue, deque, and map
 *    which we will build later this semester.
 *
 *    This will contain the class definition of:
 *        Container         : A class that holds stuff
 *        ContainerIterator : An interator through Container
 * Author
 *    Justin Ricks
 ************************************************************************/

#ifndef QUEUE_H
#define QUEUE_H

#include <cassert>
 
/************************************************
 * CONTAINER
 * A class that holds stuff
 ***********************************************/
template <class T>
class Queue
{
  public:
   // default constructor
   Queue() : numPush(0), numPop(0), maxSize(0), data(0x00000000) {}

   // copy constructor
   Queue(const Queue & rhs) throw (const char *)
      : numPush(0), numPop(0), maxSize(0), data(NULL) { *this = rhs; }

   // non-default constructor : allocate to a specific size
   Queue(int maxSize) throw (const char *);

   // destructor
   ~Queue() { if (maxSize) delete [] data; }

   // assignment operator
   Queue <T> & operator = (const Queue & rhs);

   // is the container empty?
   bool empty() const
   { return numPush - numPop == 0; }

   // clear the contents of the container
   void clear() { numPush = numPop = 0; }

   // how many items are in the container?
   int size() const { return numPush - numPop; }

   // what is the capacity?
   int capacity() const { return maxSize; }

   // insert an item to the container
   void push(const T & t) throw (const char *);

   // pop an item out of the queue
   void pop()
   {
      if (empty())
         throw "ERROR: attempting to pop from an empty queue";
      numPop++;
   }

   // get the item at the front of the queue
   T & front()
   {
      if (empty())
         throw "ERROR: attempting to access an item in an empty queue";
      assert(iHead() <= maxSize && iHead() >= 0);
      return data[iHead()];
   }

   // get the item of the end of the queue
   T & back()
   {
      if (empty())
         throw "ERROR: attempting to access an item in an empty queue";
      assert(iTail() <= maxSize && iTail() >= 0);
      return data[iTail()];
   }

  private:
   // return the index of the head
   int iHead() const
   {
      if (numPush - numPop == 1)
         return iTail();
      return (numPop % maxSize);
   }

   // return the index of the tail
   int iTail() const { return (numPush - 1) % maxSize; }

   // doulble the buffer
   void reallocate() throw (const char *);

   //void displayTheQueue();
   
   T * data;     // array of data
   unsigned int numPush;  // how many times have we pushed?
   unsigned int numPop;   // how many times have we popped?
   int maxSize;  // capacity
};

/*******************************************
 * QUEUE :: ASSIGNMENT OPERATOR
 *
 * Does what assignment operators do: copy stuff
 *******************************************/
template <class T>
Queue <T> & Queue <T> :: operator= (const Queue & rhs)
{
   try
   {
      // allocate a new buffer
      T * newBuffer = new T[rhs.maxSize];

      // copy the array
      for (int i = 0; i < rhs.maxSize; i++)
         newBuffer[i] = rhs.data[i];

      // set the member variables
      this->numPush = rhs.numPush;
      this->numPop  = rhs.numPop;
      this->maxSize = rhs.maxSize;

      // free the old data
      delete [] this->data;

      // claim the new data
      this->data = newBuffer;

      return *this;
   }
   
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate a new buffer for queue";
   }
      
}

/**********************************************
 * CONTAINER : NON-DEFAULT CONSTRUCTOR
 * Preallocate the container to "maxSize"
 **********************************************/
template <class T>
Queue <T> :: Queue(int maxSize) throw (const char *)
{
   this->maxSize = 0;
   this->numPush = this->numPop = 0;
   this->data = NULL;
   
   assert(maxSize >= 0);

   // if the allocation size is 0, don't allocate anything
   if (maxSize == 0)
   {
      this->maxSize = this->numPush = numPop = 0;
      this->data = 0x00000000;
      return;
   }

   // try to allocate some memory
   try
   {
      data = new T[maxSize];
   }
   
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate a new buffer for queue";
   }

   // set the member variables
   this->maxSize  = maxSize;
   this->numPush = this->numPop = 0;
}

/***************************************************
 * QUEUE :: PUSH
 * Insert an item at the end of the queue
 **************************************************/
template <class T>
void Queue <T> :: push(const T & t) throw (const char *)
{
   // do we have to reallocate?
   if (maxSize == 0 || maxSize == (numPush - numPop))
      reallocate();

   // special case: if there are zero items, 0 will be
   // the correct place to insert
   if (numPush % maxSize == 0)
      data[0] = t;

   else
   {
      // ensure we don't overstep the array
      assert(iTail() + 1 < maxSize);

      // add the item to the array
      data[iTail() + 1] = t;
   }

   numPush++;

}

/***************************************************
 * QUEUE :: REALLOCATE
 * Double the buffer size for the queue
 **************************************************/
template <class T>
void Queue <T> :: reallocate() throw (const char *)
{
   // if we're at zero, just make 1
   if (maxSize == 0)
   {
      try
      {
         maxSize = 1;
         data = new T[maxSize];
         return;
      }
      catch (std::bad_alloc)
      {
         throw "ERROR: Unable to allocate a new buffer for queue";
      }
   }

   try
   {
      // allocate a new buffer
      T * newBuffer = new T[maxSize * 2];

      // now copy the crud over
      for (int i = 0; i < maxSize; i++)
      {
         assert(i < maxSize * 2 && (iHead() + 1) % maxSize < maxSize);
         newBuffer[i] = data[(iHead() + i) % maxSize];
      }

      // free the old buffer!
      delete [] data;

      data = newBuffer;

      // we have as many items as our old size was
      numPush = maxSize;
      numPop = 0;
      maxSize *= 2;
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate a new buffer for queue";
   }
}

/***************************************************
 * QUEUE :: DISPLAYTHEQUEUE
 * This is for debug purposes. Really came in handy!
 **************************************************/
/*
template <class T>
void Queue <T> :: displayTheQueue()
{
   std::cerr << "Maxsize is " << maxSize << std::endl
             << "numPush is " << numPush << std::endl
             << "numPop is " << numPop << std::endl
             << "iHead() is " << iHead() << std::endl
             << "iTail() is " << iTail() << std::endl;
   
   for (int i = 0; i < maxSize; i++)
   {
      std::cerr << data[i] << ' ';
   }

   std::cerr << std::endl;
}*/

#endif // QUEUE_H
