#include "mystring.hpp"
#include <cstring>
#include <cassert>
#include <iostream>
#include <stdexcept>
using namespace std;

void MyString::reallocate(size_t new_capacity)
{
    if (new_capacity <= capacity)
        return;

    char *new_data = new char[new_capacity];
    if (data != nullptr)
    {
        memcpy(new_data, data, length + 1);
        delete[] data;
    }
    data = new_data;
    capacity = new_capacity;
}

void MyString::copy_from(const char *str, size_t len)
{
    if (len + 1 > capacity)
    {
        reallocate(len + 1);
    }
    memcpy(data, str, len);
    data[len] = '\0';
    length = len;
}

MyString::MyString() : data(nullptr), length(0), capacity(0) {}

MyString::MyString(const char *str) : MyString()
{
    if (str != nullptr)
    {
        length = strlen(str);
        reallocate(length + 1);
        memcpy(data, str, length + 1);
    }
}

MyString::MyString(const MyString &other) : MyString()
{
    if (other.data != nullptr)
    {
        length = other.length;
        reallocate(other.capacity);
        memcpy(data, other.data, length + 1);
    }
}

MyString::MyString(MyString &&other) noexcept
    : data(other.data), length(other.length), capacity(other.capacity)
{
    other.data = nullptr;
    other.length = 0;
    other.capacity = 0;
}

MyString::~MyString()
{
    delete[] data;
    data = nullptr;
}

MyString &MyString::operator=(const MyString &other)
{
    if (this != &other)
    {
        if (other.length + 1 <= capacity)
        {
            memcpy(data, other.data, other.length + 1);
            length = other.length;
        }
        else
        {
            delete[] data;
            length = other.length;
            capacity = other.capacity;
            data = new char[capacity];
            memcpy(data, other.data, other.length + 1);
        }
    }
    return *this;
}

MyString &MyString::operator=(const char *str)
{
    set_new_string(str);
    return *this;
}

char MyString::get(int i) const
{
    if (i < 0 || i >= static_cast<int>(length))
    {
        cout << "Out of bounds" << endl;
        return '\0';
    }
    return data[i];
}

void MyString::set(int i, char c)
{
    if (i < 0 || i >= static_cast<int>(length))
    {
        return;
    }
    data[i] = c;
}

void MyString::set_new_string(const char *str)
{
    if (str == nullptr)
    {
        if (data != nullptr)
            data[0] = '\0';
        length = 0;
        return;
    }
    length = strlen(str);
    reallocate(length + 1);
    memcpy(data, str, length + 1);
}

void MyString::print() const
{
    cout << c_str() << endl;
}

void MyString::read_line()
{
    if (data == nullptr)
    {
        reallocate(16);
        data[0] = '\0';
    }
    const size_t INITIAL_CAPACITY = 16;
    const size_t CHUNK_SIZE = 256;

    if (capacity < INITIAL_CAPACITY)
    {
        delete[] data;
        capacity = INITIAL_CAPACITY;
        data = new char[capacity];
    }
    cin.clear();
    size_t pos = 0;
    while (true)
    {
        if (pos + 1 >= capacity)
        {
            capacity = (pos + CHUNK_SIZE) * 2;
            char *new_data = new char[capacity];
            if (pos > 0)
            {
                memcpy(new_data, data, pos);
            }
            delete[] data;
            data = new_data;
        }

        char ch;
        cin.get(ch);

        if (ch == '\n')
        {
            break;
        }

        data[pos++] = ch;
    }

    data[pos] = '\0';
    length = pos;
}

size_t MyString::get_length() const
{
    return length;
}

size_t MyString::get_capacity() const
{
    return capacity;
}

const char *MyString::c_str() const
{
    return data ? data : "";
}

MyString &MyString::append(const char *str, size_t str_len)
{
    if (!str || str_len == 0)
        return *this;

    size_t new_len = length + str_len;
    char *new_data = new char[new_len + 1];

    if (data)
    {
        // как раз вот здесь оптимизируем, потому что по известной длине строки мы можем сразу внести данные
        // также везде поменял strcpy на memcpy во имя оптимизации
        memcpy(new_data, data, length);
    }
    memcpy(new_data + length, str, str_len);
    new_data[new_len] = '\0';

    delete[] data;
    data = new_data;
    length = new_len;
    capacity = new_len + 1;

    return *this;
}

MyString &MyString::operator+=(const char *str)
{
    if (!str)
    {
        return *this;
    }

    return append(str, strlen(str));
}

MyString &MyString::operator+=(const MyString &other)
{
    return append(other.c_str(), other.length);
}

MyString MyString::concat(const char* s1, size_t len1, const char* s2, size_t len2)
{
    MyString result;
    result.length = len1 + len2;
    result.capacity = result.length + 1;
    result.data = new char[result.capacity];
    
    if (s1 && len1 > 0) 
        memcpy(result.data, s1, len1);
        
    if (s2 && len2 > 0) 
        memcpy(result.data + len1, s2, len2);
        
    // до этого выделялась память при операторе копирования и при +=, а сейчас один раз, вынес ещё в отдельный метод
    result.data[result.length] = '\0';
    
    return result;
}

MyString MyString::operator+(const MyString &other) const
{
    return concat(data, length, other.data, other.length);
}

MyString MyString::operator+(const char *str) const
{
    return concat(data, length, str, str ? strlen(str) : 0);
}

MyString operator+(const char *str, const MyString &s)
{
    return MyString::concat(str, str ? strlen(str) : 0, s.data, s.length);
}

MyString &MyString::operator=(MyString &&other) noexcept
{
    if (this != &other)
    {
        delete[] data;

        data = other.data;
        length = other.length;
        capacity = other.capacity;

        other.data = nullptr;
        other.length = 0;
        other.capacity = 0;
    }
    return *this;
}

bool MyString::operator==(const MyString &other) const
{
    return strcmp(c_str(), other.c_str()) == 0;
}

bool MyString::operator==(const char *str) const
{
    if (!str)
        return length == 0;
    return strcmp(c_str(), str) == 0;
}

bool operator==(const char *str, const MyString &s)
{
    if (!str)
        return s.get_length() == 0;
    return strcmp(str, s.c_str()) == 0;
}

bool MyString::operator!=(const MyString &other) const
{
    return !(*this == other);
}

bool MyString::operator!=(const char *str) const
{
    return !(*this == str);
}

bool operator!=(const char *str, const MyString &s)
{
    return !(str == s);
}

bool MyString::operator<(const MyString &other) const
{
    return strcmp(c_str(), other.c_str()) < 0;
}

bool MyString::operator<(const char *str) const
{
    if (!str)
        return false;
    return strcmp(c_str(), str) < 0;
}

bool operator<(const char *str, const MyString &s)
{
    if (!str)
        return s.get_length() > 0;
    return strcmp(str, s.c_str()) < 0;
}

bool MyString::operator<=(const MyString &other) const
{
    return strcmp(c_str(), other.c_str()) <= 0;
}

bool MyString::operator<=(const char *str) const
{
    if (!str)
        return length == 0;
    return strcmp(c_str(), str) <= 0;
}

bool operator<=(const char *str, const MyString &s)
{
    if (!str)
        return true;
    return strcmp(str, s.c_str()) <= 0;
}

char &MyString::operator[](size_t i)
{
    if (i >= length)
    {
        throw std::out_of_range("Перебор с циферками, дружок");
    }
    return data[i];
}

const char &MyString::operator[](size_t i) const
{
    if (i >= length)
    {
        throw std::out_of_range("Перебор с циферками, дружок");
    }
    return data[i];
}

std::ostream &operator<<(std::ostream &os, const MyString &s)
{
    os << s.c_str();
    return os;
}

std::istream &operator>>(std::istream &is, MyString &s)
{
    if (s.data == nullptr)
    {
        s.reallocate(16);
        s.data[0] = '\0';
    }
    const size_t INITIAL_CAPACITY = 16;
    const size_t CHUNK_SIZE = 256;

    if (s.capacity < INITIAL_CAPACITY)
    {
        delete[] s.data;
        s.capacity = INITIAL_CAPACITY;
        s.data = new char[s.capacity];
    }

    is.clear();
    size_t pos = 0;

    while (true)
    {
        if (pos + 1 >= s.capacity)
        {
            s.capacity = (pos + CHUNK_SIZE) * 2;
            char *new_data = new char[s.capacity];
            if (pos > 0)
            {
                memcpy(new_data, s.data, pos);
            }
            delete[] s.data;
            s.data = new_data;
        }

        char ch;

        if (!is.get(ch))
        {
            break;
        }

        if (ch == '\n')
        {
            break;
        }

        s.data[pos++] = ch;
    }

    s.data[pos] = '\0';
    s.length = pos;

    return is;
}