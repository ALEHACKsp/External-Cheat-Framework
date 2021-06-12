#ifndef _CustomString_H_
#define _CustomString_H_

#include <Windows.h>

class CString
{
private:
    char* _String;
public:
    CString();                                          // No-args constructor
    CString(const char*);                               // Overloaded constructor
    CString(const CString&);                      // Copy constructor
    CString(CString&&) noexcept;                  // Move constructor
    ~CString();                                         // Destructor

    CString& operator=(const CString&);           // Copy assignment
    CString& operator=(CString&&) noexcept;       // Move assignment

    CString operator-() const;
    bool operator==(const CString&) const;
    bool operator!=(const CString&) const;
    bool operator<(const CString&) const;
    bool operator>(const CString&) const;
    CString operator+(const CString&) const;
    CString& operator+=(const CString&);
    CString operator*(int) const;
    CString& operator*=(int);
    CString& operator++();
    CString operator++(int);
    CString& operator--();
    CString operator--(int);

    int Length() const;
    const char* String() const;
};

#endif // _CUSTOMSTRING_H_