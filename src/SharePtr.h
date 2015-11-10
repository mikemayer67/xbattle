#ifndef _SHAREPOINTER_H_
#define _SHAREPOINTER_H_

////////////////////////////////////////////////////////////////////////////////
//  This package contains a single header file, which defines a pair of
//  (nearly identical) templated classes for implementing a simple, yet 
//  highly useful pointer wrapper which tracks number or references to a 
//  given memory allocation and deallocates when the final reference goes 
//  out of scope.
//
//  shr<T>        - smart pointer used in place of T*
//  const_shr<T>  - smart pointer used in place of const T*
//
// When it comes to reference tracking, it always comes down to 3 options:
//   1) keep a counter in the wrapper class
//   2) require that the referenced object hold counter
//   3) insert a wrapper around the object to hold the counter
//
//   Option 1 is problematic because you must take care when it comes
//     to "forking" or "chaining" the smart pointer: e.g.
//       shr<T> t1 = new T;    // t1.count=1
//       shr<T> t2 = t1;       // t1.count=2 t2.count=2
//       shr<T> t3 = t2;       // t1.count=2 t2.count=3 t3.count=3
//       shr<T> t4 = t1;       // t1.count=3 t2.count=3 t3.count=3 t4.count=3
//
//   Option 2 is limiting because it only works with classes defined to
//     work with your smart pointer.  It does not work with primitive 
//     types or classes you have no control over (or do not wish to perturb).
//
//   Option 3 solves the problems with options 1 and 2, but introduces
//     a new layer of overhead which could, in some circumstances, become
//     a performance hit.
//
// The smart pointer implemented in this package takes a slightly modified
//   approach to option 1.  But, rather than storing the reference count, 
//   it stores a reference (actually a pointer) to the reference count.
//   Thus whenever a new shr<T> is constructed from an existing one
//   (or assigned to from another shr<T>), the shared reference count
//   is updated and ALL shr<T> pointing to the same memory have access
//   to the new count.
//
// Note, it is still possible to "break" this smart pointer type if you 
//   don't follow a couple simple rules:
//
//   - do not assign a raw pointer directly to more than one shr<T>
//   - do not deallocate a raw pointer after assigning it to a shr<T>
//   - do not deallocate a raw pointer returned from the raw() method
//
//--------------------------------------------------------------------------------
//
// There is very little you need to know to use shr<T>.  (See below for
//   the differences between shr<T> and const_shr<T>.).
//
// Constructors:
//   A shr<T> can be constructed from either a raw pointer or from another shr<T>
//
//     shr<T> t1 = new T;
//     shr<T> t2 = t1;
//     shr<T> t3(new T);
//     shr<T> t4(t3);
//
//     t1 and t2 both point to the same T pointer and have a reference count of 2
//     t3 and t4 both point to antother T pointer and have a reference count of 2
//
//  Assignment:
//    A shr<T> have assigned to it anything that can be used for construction
//
//      t3 = t1;    // t1, t2, & t3 all point to same T and have reference count of 3
//                  // t4 now has a reference count of 1
//
//      t4 = new T; // old T object pointed to by t4 is deallocated
//                  // t4 now points to a new T with a reference count of 1
//
//  Dereferencing:
//    A shr<T> can be used with both the * and -> operators (as though the 
//      shr<T> was the underlying raw pointer).  In addition, shr<T> provide
//      the raw() method which returns the underlying raw pointer.
//
//      Note that the . operator applies to the shr<T> object itself, NOT
//      the underlying T pointer.
//
//      x = t1->getXValue();     // invokes the T::getXValue method
//      x = t1->x_value;         // accesses the T::x_value property
//
//      x = (*t1).getXValue();   // same as t1->getXValue()
//      x = (*t1).x_value;       // same as t1->x_value 
//
//      T& tr = *t1;             // tr is a reference to the T to which t1 points
//      T* tp = &(*t1);          // tp is raw pointer to the T to which t1 points
//                               //   CAUTION:: Do NOT deallocate tp or pass it
//                               //             to any function/method that will
//
//      T* tp = t1.raw();        // tp is raw pointer to the T to which t1 points
//                               //   CAUTION:: Do NOT deallocate tp or pass it
//                               //             to any function/method that will
//
//  Determining State:
//    The following methods can be used to test the state of a shr<T>:
//
//      bool isSet()             // returns true unless pointing to NULL
//      bool isNull()            // returns true   if   pointing to NULL
//      bool isNotNull()         // returns true unless pointing to NULL
//                               //   (yes, this is the same as isSet() )
//      unsigned long refCount() // returns number of shr<T> and const_shr<T>
//                               //   pointing to the current pointer
//                               //   return 0 if pointing to NULL
//
//      In addition, the == operator can be used to determine if two share
//        pointers (shr<T> or const_shr<T>) reference the same T*
//
//      Finally, the < operator can be used to determine the sorting
//        order of two pointers.  The only reason that this was included
//        is so that it can be used within a std::set.
//
//  Exceptions:
//    Any attempt to derefence a NULL pointer will result in a std::runtime_error
//      exception being thrown.
//
//------------------------------------------------------------------------------     
//
// The const_shr<T> class is an extension to shr<T> that protects the content
//   of T from being modified.  It is analogous to the relationship between
//   const T* and T*.
//
//   Note that const_shr<T> protects the content of T.  It does not prevent
//   you from reassigning the const_shr<T> to point to a different T.
//
//                           Modify T content    Modify which T is referenced
//                           ----------------    ----------------------------
//     shr<T>                       YES                      YES
//     const_shr<T>                  NO                      YES
//     const shr<T>                 YES                       NO
//     const const_shr<T>            NO                       NO
//
//   Here are some of the key differences:
//
//     - a const_shr<T> can be constructed from a shr<T>
//     - a shr<T> cannot be constructed from a const_shr<T>
//
//     - a const_shr<T> can be assigned a shr<T>
//     - a shr<T> cannot be assigned a const_shr<T>
//
//     - a const_shr<T> can invoke const methods on T
//     - a shr<T> cannot invoke const methods on T
//
//     - a const_shr<T> cannot modify any of T's attributes unless marked mutable
//     - a shr<T> can modify any of T's attributes not marked const
//
//     - the raw() method on a const_shr<T> returns a const T* pointer
//     - the raw() method on a shr<T> returns a T* pointer
//
////////////////////////////////////////////////////////////////////////////////

template <typename T>
class const_shr
{
  public:
    const_shr(const T *p=NULL)        : _ptr(NULL), _refCount(NULL) { set(p); }
    const_shr(const const_shr<T> &p)  : _ptr(NULL), _refCount(NULL) { set(p); }

    ~const_shr() { decr(); }

    const_shr<T> &operator=(const T*  p)           { set(p); return *this; }
    const_shr<T> &operator=(const const_shr<T> &p) { set(p); return *this; }

    void set(const T* p)
    {
      decr();
      _ptr = p;
      if(p!=NULL)
      {
        _refCount = new unsigned long;
        *_refCount = 1;
      }
    }

    void set(const const_shr<T> &p)
    {
      decr();
      _ptr = p._ptr;
      if( p.isSet() )
      {
        _refCount = p._refCount;
        *_refCount += 1;
      }
    }

    const T &operator*(void)  const { assert_set(); return *_ptr; }
    const T *operator->(void) const { assert_set(); return  _ptr; }
    const T *raw(void)        const {               return  _ptr; }

    bool isSet(void)     const { return _ptr!=NULL; }
    bool isNull(void)    const { return _ptr==NULL; }
    bool isNotNull(void) const { return _ptr!=NULL; }

    unsigned long refCount(void) const
    {
      return ( _refCount==NULL ? 0UL : *_refCount );
    }

    bool operator==(const const_shr<T> &p) const { return _ptr==p._ptr; }
    bool operator<(const const_shr<T> &p)  const { return _ptr<p._ptr; }

  protected:

    void decr(void)
    {
      if( _refCount == NULL ) return;

      *_refCount -= 1;
      if(*_refCount==0)
      {
        delete _ptr;
        delete _refCount;
        _ptr      = NULL;
        _refCount = NULL;
      }
    }

    virtual void assert_set(void) const
    {
      if(_ptr==NULL) throw std::runtime_error("Attempting to dereference NULL const_shr<T>");
    }

  protected:
    const       T *_ptr;
    unsigned long *_refCount;
};

template <typename T>
class shr : public const_shr<T>
{
  typedef const_shr<T> Parent_t;

  public:
  shr(T *p=NULL)        : Parent_t(p) {}
  shr(const shr<T> &p)  : Parent_t(p) {}

  shr<T> &operator=(T*  p)           { Parent_t::set(p); return *this; }
  shr<T> &operator=(const shr<T> &p) { Parent_t::set(p); return *this; }

  ////////////////////////////////////////////////////////////////////////
  // Note that although we are casting away the const-ness of _ptr,
  //   all is OK.  Because this is a shr<T> object, it must have been
  //   constructed with either a non-const T* or another shr<T>.  The
  //   only reason that _ptr is a const T* is that it is inherited from
  //   the parent const_shr<T> class, which requires it to be a const T*
  ////////////////////////////////////////////////////////////////////////
  T &operator*(void)  const { assert_set(); return *const_cast<T*>(this->_ptr); }
  T *operator->(void) const { assert_set(); return  const_cast<T*>(this->_ptr); }
  T *raw(void)        const {               return  const_cast<T*>(this->_ptr); }

  protected:
  virtual void assert_set(void) const
  {
    if(this->_ptr==NULL) throw std::runtime_error("Attempting to dereference NULL shr<T>");
  }

};

#endif  // _SHAREPOINTER_H_
