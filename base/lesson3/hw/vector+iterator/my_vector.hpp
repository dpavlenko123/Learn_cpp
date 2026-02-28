#include <iostream>
#include <initializer_list>
#include "my_iterator.hpp"

template<typename T>
class MyVectorBuff {
private:
    T* _vector = nullptr;
    size_t _size = 0;
    size_t _capacity = 0;

public:
    MyVectorBuff(const MyVectorBuff& other) = delete;
    MyVectorBuff& operator=(const MyVectorBuff& other) = delete;

    MyVectorBuff(MyVectorBuff&& other) noexcept
        : _vector(other._vector), _size(other._size), _capacity(other._capacity) {
        other._vector = nullptr;
        other._size = 0;
        other._capacity = 0;
    }

    MyVectorBuff& operator=(MyVectorBuff&& other) noexcept {
        if (this == &other) return *this;

        MyVectorBuff tmp(std::move(other));
        std::swap(*this, tmp);
        return *this;
    }

    MyVectorBuff(size_t cap = 0) {
        _vector = static_cast<T*>(::operator new(sizeof(T) * cap));
        _capacity = cap;
    }

    ~MyVectorBuff() {
        T* start = _vector;
        T* end = _vector + _size;
        while (start != end) {
            start->~T();
            ++start;
        }
        ::operator delete(_vector);
    }
};


template<typename T>
class MyVector : private MyVectorBuff<T> {
private:
    using MyVectorBuff<T>::_vector;
    using MyVectorBuff<T>::_size;
    using MyVectorBuff<T>::_capacity;

public:
    using iterator = MyIterator<T, false>;
    using const_iterator = MyIterator<T, true>;
    using reverse_iterator = ReverseIterator<iterator>;
    using const_reverse_iterator = ReverseIterator<const_iterator>;
    using value_type = T;

    MyVector() = default;

    explicit MyVector(size_t size, const T& val = {})
        : MyVectorBuff<T>(size) {
        for (size_t i = 0; i < size; ++i) {
            new (_vector + i) T(val);
            ++_size;
        }
    }

    MyVector(std::initializer_list<T> list)
        : MyVectorBuff<T>(list.size()) {
        for (auto& elem : list) {
            new (_vector + _size) T(elem);
            ++_size;
        }
    }

    MyVector(const MyVector& other)
        : MyVectorBuff<T>(other.size()) {
        for (size_t i = 0; i < other.size(); ++i) {
            new (_vector + i) T(other[i]);
            ++_size;
        }
    }

    MyVector(MyVector&& other) noexcept
        : MyVectorBuff<T>(std::move(other)) { }

    MyVector& operator=(const MyVector& other) {
        if (this == &other) return *this;

        MyVector tmp(other);
        std::swap(*this, tmp);
        return *this;
    }

    MyVector& operator=(MyVector&& other) noexcept {
        if (this == &other) return *this;

        MyVector tmp(std::move(other));
        std::swap(*this, tmp);
        return *this;
    }

    void push_back(const T& elem) {
        if (_size == _capacity) {
            size_t new_cap = (_capacity == 0) ? 1 : _capacity * 2;
            reserve(new_cap);
        }
        new (_vector + _size) T(elem);
        ++_size;
    }

    void push_back(T&& elem) {
        if (_size == _capacity) {
            size_t new_cap = (_capacity == 0) ? 1 : _capacity * 2;
            reserve(new_cap);
        }
        new (_vector + _size) T(std::move(elem));
        ++_size;
    }

    template<typename... Args>
    requires std::constructible_from<T, Args...>
    void emplace_back(Args&&... args) {
        if (_size == _capacity) {
            size_t new_cap = (_capacity == 0) ? 1 : _capacity * 2;
            reserve(new_cap);
        }
        new (_vector + _size) T(std::forward<Args>(args)...);
        ++_size;
    }

    void reserve(size_t new_cap) {
        if (new_cap <= _capacity) return;

        T* new_vec = static_cast<T*>(::operator new(new_cap * sizeof(T)));

        for (size_t i = 0; i < _size; ++i) {
            new (new_vec + i) T(std::move(_vector[i]));
        }

        for (size_t i = 0; i < _size; ++i) {
            (_vector + i)->~T();
        }

        ::operator delete(_vector);
        _vector = new_vec;
        _capacity = new_cap;
    }

    T& operator[](const size_t& idx) { return _vector[idx]; }
    const T& operator[](const size_t& idx) const { return _vector[idx]; }

    size_t size() const noexcept { return _size; }
    size_t capacity() const noexcept { return _capacity; }
    bool empty() const noexcept { return _size == 0; }

    iterator begin() { return iterator(_vector); }
    iterator end() { return iterator(_vector + _size); }
    const_iterator cbegin() const { return const_iterator(_vector); }
    const_iterator cend() const { return const_iterator(_vector + _size); }

    reverse_iterator rbegin() { return reverse_iterator(iterator(_vector + _size)); }
    reverse_iterator rend() { return reverse_iterator(iterator(_vector)); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(const_iterator(_vector + _size)); }
    const_reverse_iterator crend() const { return const_reverse_iterator(const_iterator(_vector)); }
};