#ifndef MYSTRING_HPP
#define MYSTRING_HPP

#include <iostream>

class MyString
{
private:
    char *data;
    size_t length;
    size_t capacity;

    void reallocate(size_t new_capacity);
    void copy_from(const char *str, size_t len);

    MyString &append(const char *str, size_t str_len);
    static MyString concat(const char *s1, size_t len1, const char *s2, size_t len2);

public:
    MyString();
    MyString(const char *str);
    MyString(const MyString &other);
    MyString(MyString &&other) noexcept;
    ~MyString();

    MyString &operator=(const MyString &other);
    MyString &operator=(const char *str);

    char get(int i) const;
    void set(int i, char c);
    void set_new_string(const char *str);

    void print() const;
    void read_line();

    size_t get_length() const;
    size_t get_capacity() const;

    const char *c_str() const;

    MyString &operator+=(const MyString &other);
    MyString &operator+=(const char *str);

    MyString operator+(const MyString &other) const;
    MyString operator+(const char *str) const;

    // случай аналогичный матрицам. нужна дружественная функция для const char* + MyString
    friend MyString operator+(const char *str, const MyString &s);

    MyString &operator=(MyString &&other) noexcept;

    bool operator==(const MyString &other) const;
    bool operator==(const char *str) const;
    friend bool operator==(const char *str, const MyString &s);

    bool operator!=(const MyString &other) const;
    bool operator!=(const char *str) const;
    friend bool operator!=(const char *str, const MyString &s);

    bool operator<(const MyString &other) const;
    bool operator<(const char *str) const;
    friend bool operator<(const char *str, const MyString &s);

    bool operator<=(const MyString &other) const;
    bool operator<=(const char *str) const;
    friend bool operator<=(const char *str, const MyString &s);

    char &operator[](size_t i);
    const char &operator[](size_t i) const; // для константного объекта MyString

    friend std::ostream &operator<<(std::ostream &os, const MyString &s);
    friend std::istream &operator>>(std::istream &is, MyString &s);
};

#endif