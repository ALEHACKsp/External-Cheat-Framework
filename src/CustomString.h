#ifndef _CustomString_H_
#define _CustomString_H_

#include <Windows.h>

class CCustomString
{
private:
    char* _String;
public:
    CCustomString();                                          // No-args constructor
    CCustomString(const char*);                               // Overloaded constructor
    CCustomString(const CCustomString&);                      // Copy constructor
    CCustomString(CCustomString&&) noexcept;                  // Move constructor
    ~CCustomString();                                         // Destructor

    CCustomString& operator=(const CCustomString&);           // Copy assignment
    CCustomString& operator=(CCustomString&&) noexcept;       // Move assignment

    CCustomString operator-() const;
    bool operator==(const CCustomString&) const;
    bool operator!=(const CCustomString&) const;
    bool operator<(const CCustomString&) const;
    bool operator>(const CCustomString&) const;
    CCustomString operator+(const CCustomString&) const;
    CCustomString& operator+=(const CCustomString&);
    CCustomString operator*(int) const;
    CCustomString& operator*=(int);
    CCustomString& operator++();
    CCustomString operator++(int);
    CCustomString& operator--();
    CCustomString operator--(int);

    int Length() const;
    const char* String() const;
};

#endif // _CUSTOMSTRING_H_