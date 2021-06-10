#include "CustomString.h"

CCustomString::CCustomString()
    : _String{ nullptr }
{
    _String = new char[1];
    *_String = '\0';
}

CCustomString::CCustomString(const char* s)
    : _String{ nullptr }
{
    if (s == nullptr)
    {
        _String = new char[1];
        *_String = '\0';
    }
    else
    {
        _String = new char[strlen(s) + 1];
        strcpy(_String, s);
    }
}

// Copy constructor
CCustomString::CCustomString(const CCustomString& source)
    : _String{ nullptr }
{
    _String = new char[strlen(source._String) + 1];
    strcpy(_String, source._String);
}

// Move constructor
CCustomString::CCustomString(CCustomString&& source) noexcept
    :_String(source._String)
{
    source._String = nullptr;
}

// Destructor
CCustomString::~CCustomString()
{
    delete[] _String;
}

// Copy assignment
CCustomString& CCustomString::operator=(const CCustomString& rhs)
{
    if (this == &rhs)
        return *this;

    delete[] _String;

    _String = new char[strlen(rhs._String) + 1];
    strcpy(_String, rhs._String);

    return *this;
}

// Move assignment
CCustomString& CCustomString::operator=(CCustomString&& rhs) noexcept
{
    if (this == &rhs)
        return *this;

    delete[] _String;

    _String = rhs._String;
    rhs._String = nullptr;

    return *this;
}


CCustomString CCustomString::operator-() const
{
    char* buff = new char[strlen(_String) + 1];
    strcpy(buff, _String);

    for (size_t i = 0; i < strlen(_String); i++)
        buff[i] = tolower(buff[i]);

    CCustomString out(buff);

    delete[] buff;

    return out;
}

bool CCustomString::operator==(const CCustomString& rhs) const
{
    return (strcmp(this->_String, rhs._String) == 0);
}

bool CCustomString::operator!=(const CCustomString& rhs) const
{
    return (strcmp(this->_String, rhs._String) != 0);
}

bool CCustomString::operator<(const CCustomString& rhs) const
{
    return this->_String < rhs._String;
}

bool CCustomString::operator>(const CCustomString& rhs) const
{
    return this->_String > rhs._String;
}

CCustomString CCustomString::operator+(const CCustomString& rhs) const
{
    char* buff = new char[strlen(_String) + strlen(rhs._String) + 1];

    strcpy(buff, _String);
    strcat(buff, rhs._String);

    CCustomString out(buff);

    delete[] buff;

    return out;
}

CCustomString& CCustomString::operator+=(const CCustomString& rhs)
{
    *this = *this + rhs;

    return *this;
}

CCustomString CCustomString::operator*(int rhs) const
{
    CCustomString out(_String);

    for (size_t i = 1; i < rhs; i++)
        out += *this;

    return out;
}

CCustomString& CCustomString::operator*=(int rhs)
{
    *this = *this * rhs;

    return *this;
}

CCustomString& CCustomString::operator++()
{
    for (size_t i = 0; i < strlen(_String); i++)
        _String[i] = toupper(_String[i]);

    return *this;
}

CCustomString CCustomString::operator++(int)
{
    CCustomString out(*this);
    operator++();
    return out;
}

CCustomString& CCustomString::operator--()
{
    for (size_t i = 0; i < strlen(_String); i++)
        _String[i] = tolower(_String[i]);

    return *this;
}

CCustomString CCustomString::operator--(int)
{
    CCustomString out(*this);
    operator--();
    return out;
}

int CCustomString::Length() const
{
    return strlen(_String);
}

const char* CCustomString::String() const
{
    return _String;
}