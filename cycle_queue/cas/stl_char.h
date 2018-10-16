/*************************************************************************
	> File Name: stl_char.h
	> Author: liulianglu
	> Description:
   最近在的某个项目中，大量使用了数组——为了使用栈内存。
   但是有时候需要动态分配内存，结合C++的容器使用。
   既然是优化了内存，就不用string，而用该数组告诉分配器每次分配多大的。 
   再则，使用一些C++的习惯方便编程。
	> Created Time: Wed 06 Sep 2017 04:42:34 AM EDT
 ************************************************************************/

#ifndef _STL_CHAR_H
#define _STL_CHAR_H

#include<string.h>

template<class T, unsigned int N>
class stl_char
{
public:
    T   elements[N];

public:
    typedef T           value_type;
    typedef T*          iterator;
    typedef const T*    const_iterator;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef int         size_type;
    //typedef std::ptrdiff_t  diffrence_type;

    iterator    begin()     {   return elements; }
    iterator    end()       {   return elements + N; }

    int         size()      {   return N;   }
    int         memSize()   {   return sizeof(T) * N;  }
    
    stl_char()      {   memset(begin(), 0, memSize());  }
    stl_char(const_iterator  src) { strncpy(begin(), src, size());  }

    //拷贝有可能是两个不同的数组。
    template<typename T2>
    stl_char<T,N>& operator=(const stl_char<T2,N>& ths)
    {
        memcp(begin(), ths.begin(), ths.memSize());
        return *this;
    }

    template<typename T1>
    bool operator==(const stl_char<T1,N>& x)
    {
        for(iterator it1 = begin(), iterator it2 = x.begin();
                it1 != end() && it2 != x.end(); it1++, it2++)
        {
            if(it1 != it2)
            {
                return false;
            }
        }
        return true;
    }

    template<typename T1>
    bool operator!=(const stl_char<T1,N>& x)
    {
        return !*this == x;
    }
};

template<class T, int N>
bool operator==(const stl_char<T,N>& x, const stl_char<T,N>& y)
{
    return !strcasecmp(x,y);
}
template<class T, int N>
bool operator!=( const stl_char<T,N>& x, const stl_char<T,N>& y)
{
    return !(x==y);
}
typedef stl_char<char, 32> stl_char32;


#endif
