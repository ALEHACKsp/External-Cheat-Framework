// Custom string class, because why not.

#ifndef _CustomString_Hpp_
#define _CustomString_Hpp_

#include <Windows.h>
#include <string>

class CString
{
private:
    char*       _String;
public:
    CString();                                          // No-args constructor
    CString(const char*);                               // Overloaded constructor
    CString(const CString&);                            // Copy constructor
    CString(CString&&) noexcept;                        // Move constructor
    ~CString();                                         // Destructor

    CString&    operator=(const CString&);              //
    CString&    operator=(CString&&) noexcept;          //
    CString     operator-() const;                      //
    bool        operator==(const CString&) const;       //
    bool        operator!=(const CString&) const;       //
    bool        operator<(const CString&) const;        //
    bool        operator>(const CString&) const;        //
    CString     operator+(const CString&) const;        // Operators
    CString&    operator+=(const CString&);             //
    CString     operator*(int) const;                   //
    CString&    operator*=(int);                        //
    CString&    operator++();                           //
    CString     operator++(int);                        //
    CString&    operator--();                           //
    CString     operator--(int);                        //

    int         Length() const;                         //
    const char* String() const;                         // Getters
    std::string CppString() const;                      //
};

#endif // _CUSTOMSTRING_Hpp_