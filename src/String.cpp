#include "String.hpp"

// No-args Constructor
CString::CString()
    : _String{ nullptr }
{
    _String = new char[1];
    *_String = '\0';
}

// Overloaded Constructor
CString::CString(const char* s)
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
CString::CString(const CString& source)
    : _String{ nullptr }
{
    _String = new char[strlen(source._String) + 1];
    strcpy(_String, source._String);
}

// Move constructor
CString::CString(CString&& source) noexcept
    :_String(source._String)
{
    source._String = nullptr;
}

// Destructor
CString::~CString()
{
    delete[] _String;
}

// Copy assignment
CString& CString::operator=(const CString& rhs)
{
    if (this == &rhs)
        return *this;

    delete[] _String;

    _String = new char[strlen(rhs._String) + 1];
    strcpy(_String, rhs._String);

    return *this;
}

// Move assignment
CString& CString::operator=(CString&& rhs) noexcept
{
    if (this == &rhs)
        return *this;

    delete[] _String;

    _String = rhs._String;
    rhs._String = nullptr;

    return *this;
}

// Negate operator
CString CString::operator-() const
{
    char* buff = new char[strlen(_String) + 1];
    strcpy(buff, _String);

    for (size_t i = 0; i < strlen(_String); i++)
        buff[i] = tolower(buff[i]);

    CString out(buff);

    delete[] buff;

    return out;
}

// Equal to operator
bool CString::operator==(const CString& rhs) const
{
    return (strcmp(this->_String, rhs._String) == 0);
}

// Not equal to operator
bool CString::operator!=(const CString& rhs) const
{
    return (strcmp(this->_String, rhs._String) != 0);
}

// Lexicographically smaller than operator
bool CString::operator<(const CString& rhs) const
{
    return this->_String < rhs._String;
}

// Lexicographically bigger than operator
bool CString::operator>(const CString& rhs) const
{
    return this->_String > rhs._String;
}

// Addition operator
CString CString::operator+(const CString& rhs) const
{
    char* buff = new char[strlen(_String) + strlen(rhs._String) + 1];

    strcpy(buff, _String);
    strcat(buff, rhs._String);

    CString out(buff);

    delete[] buff;

    return out;
}

// Addition assignment operator
CString& CString::operator+=(const CString& rhs)
{
    *this = *this + rhs;

    return *this;
}

// Multiply by operator
CString CString::operator*(int rhs) const
{
    CString out(_String);

    for (size_t i = 1; i < rhs; i++)
        out += *this;

    return out;
}

// Multiply by assignment operator
CString& CString::operator*=(int rhs)
{
    *this = *this * rhs;

    return *this;
}

// Uppercase conversion operator
CString& CString::operator++()
{
    for (size_t i = 0; i < strlen(_String); i++)
        _String[i] = toupper(_String[i]);

    return *this;
}

// Uppercase conversion operator
CString CString::operator++(int)
{
    CString out(*this);

    operator++();

    return out;
}

// Lowercase conversion operator
CString& CString::operator--()
{
    for (size_t i = 0; i < strlen(_String); i++)
        _String[i] = tolower(_String[i]);

    return *this;
}

// Lowercase conversion operator
CString CString::operator--(int)
{
    CString out(*this);
    operator--();
    return out;
}

// String length getter
int CString::Length() const
{
    return strlen(_String);
}

// C-style string getter
const char* CString::String() const
{
    return _String;
}

// C++-style string getter
std::string CString::CppString() const
{
    return _String;
}