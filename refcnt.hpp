//
// A little attempt to maintain a reference-counting generic class based
// on templates.  The purpose is to allow the STL to contain inherited
// objects via pointers to the base class.
//
// Released to the public domain by the author, Darin McBride
// The author retains copyright.  Please send bugs (and, hopefully
// bugfixes as well) to the author: darin.mcbride@tower.alive.ampr.ab.ca
// The author can be reached in C_PLUSPLUS on the Fidonet backbone
// (often to the chagrin of other participants <G>)
//
// Usage Notes:
//       - Always use RefCount<base_class>, not RefCount<derived_class>
//         or RefCount<base_class*>.  The template parameter must be the
//         base class, not a pointer to it!
//       - this class expects that pointers given to it are then
//         OWNED by this class.
//       - Giving actual objects to the class will result in a
//         COPY of that object being owned by the class.  This requires
//         that a copy constructor exists for that object.
//       - Do not *ever* manually delete a pointer stored in this class;
//         assign a NULL pointer to it instead if the object must be deleted
//         before the pointer.

#ifndef __REF_COUNT_HPP
#define __REF_COUNT_HPP

#ifndef NULL
#define NULL ((void*)0)
#endif

template <class T>
class RefCount
{
    friend ostream& operator<<(ostream&, const RefCount&);
    public:
        // typedefs for later use
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef T const* pointer_to_const;
        typedef T* const const_pointer;

    private:
        pointer value;
        int*    refcnt;

        // utility functions:
        void InitRefCnt();
        void ReleaseRefCnt();
        void IncRefCnt();

        void ReleaseThis();

    public:
        RefCount();
        RefCount(const_pointer p);
        RefCount(const RefCount& rc);

        ~RefCount();

        RefCount& operator=(const RefCount& rc);
        RefCount& operator=(const_pointer val);

        reference operator*() const;
        const_pointer operator->() const;
        operator const_pointer() const;

        bool operator==(RefCount&) const;
};

template <class T>
inline ostream& operator<<(ostream& os, const RefCount<T>& rc)
{
    return os << *rc.value;
}

template <class T>
inline void RefCount::InitRefCnt()
{
    Assert(refcnt == NULL);
#ifdef PROVE_IT_WORKS_DAMMIT
    cout << "Initializing refcnt" << endl;
#endif
    refcnt = new int(1);
}

template <class T>
inline void RefCount::ReleaseRefCnt()
{
    Assert(refcnt != NULL);
    Assert(*refcnt == 0);
#ifdef PROVE_IT_WORKS_DAMMIT
    cout << "Releasing refcnt" << endl;
#endif
    delete refcnt;
}

template <class T>
inline void RefCount::IncRefCnt()
{
    Assert(refcnt != NULL);
    Assert(*refcnt > 0);
    ++(*refcnt);
#ifdef PROVE_IT_WORKS_DAMMIT
    cout << "refcnt incremented to " << *refcnt << endl;
#endif
}

template <class T>
inline void RefCount::ReleaseThis()
{
    Assert(refcnt != NULL);
    Assert(*refcnt > 0);
#ifdef PROVE_IT_WORKS_DAMMIT
    cout << "refcnt being decremented from " << *refcnt << endl;
#endif
    if (--(*refcnt) == 0) {
        ReleaseRefCnt();
        delete value;
    }
}

template <class T>
inline RefCount::RefCount() : value(NULL), refcnt(0)
{
    InitRefCnt();
}

template <class T>
inline RefCount::RefCount(const_pointer p) : value(p), refcnt(0)
{
    InitRefCnt();
}

template <class T>
inline RefCount::RefCount(const RefCount& rc) : value(rc.value), refcnt(rc.refcnt)
{
    IncRefCnt();
}

template <class T>
inline RefCount::~RefCount()
{
    ReleaseThis();
}

template <class T>
RefCount& RefCount::operator=(const RefCount& rc)
{
    if (this != &rc)
    {
        ReleaseThis();
        value  = rc.value;
        refcnt = rc.refcnt;
        IncRefCnt();
    }

    return *this;
}

template <class T>
RefCount& RefCount::operator=(const_pointer val)
{
    ReleaseThis();
    refcnt = NULL; // we don't want to refer to the old reference count
    InitRefCnt();
    value = val;

    return *this;
}

template <class T>
RefCount::reference RefCount::operator*() const
{
    return *value;
}

template <class T>
RefCount::const_pointer RefCount::operator->() const
{
    return value;
}

template <class T>
RefCount::operator RefCount::const_pointer() const
{
    return value;
}

template <class T>
bool RefCount::operator==(const RefCount& rc) const
{
    if (&rc == this || rc.refcnt == refcnt)
        return true;
    return *value == *rc.value;
}

#endif // __REF_COUNT_HPP
