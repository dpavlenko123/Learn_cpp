#include <iostream>
#include <initializer_list>

template<typename T>
class MyVector {
private:
    T* _vector = nullptr;
    size_t _size = 0;
    size_t _capacity = 0;

public:
    MyVector() = default;

    MyVector(size_t size, const T& val = {}) {
        reserve(size);
        for (size_t i = 0; i < size; ++i) {
            new (_vector + _size) T(val);
        }
    }

    MyVector(std::initializer_list<T> list) {

        reserve(list.size());

        for (const auto& x : list) {
            new (_vector + _size) T(x);
            ++_size;
        }

    }

    MyVector(const MyVector& other) {
        reserve(other.size());

        for (const auto& x : other) {
            new (_vector + _size) T(x);
            ++_size;
        }
    }

    MyVector(MyVector&& other) noexcept
    : _vector(other._vector),
        _size(other._size),
        _capacity(other._capacity)
    {
        other._vector = nullptr;
        other._size = 0;
        other._capacity = 0;
    }

    MyVector& operator=(const MyVector& other) { //copy and swap
        if(this == &other) return;

        MyVector tmp(other);
        swap(this, tmp);
        return *this;
    }

    MyVector& operator=(MyVector&& other) noexcept {
        if(this == &other) return;

        MyVector tmp(std::move(other));
        swap(this, tmp);
        return *this;
    }

    void push_back(const T& elem) {
        if(_capacity == _size) {
            size_t new_cap = (_capacity == 0) ? 1 : _capacity * 2;
            reserve(new_cap);
        }
        new (_vector + _size) T(elem);
        ++_size;
    }

    void push_back(T&& elem) {
        if(_capacity == _size) {
            size_t new_cap = (_capacity == 0) ? 1 : _capacity * 2;
            reserve(new_cap);
        }
        new (_vector + _size) T(std::move(elem));
        ++_size;
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        if(_capacity == _size) {
            size_t new_cap = (_capacity == 0) ? 1 : _capacity * 2;
            reserve(new_cap);
        }
        new (_vector + _size) T(std::forward<Args>(args)...);
        ++_size;
    }

    T& operator[](size_t idx) noexcept {
        return _vector[idx];
    }

    const T& operator[](size_t idx) const noexcept {
        return _vector[idx];
    }

    size_t size() const noexcept {
        return _size;
    }

    size_t capacity() const noexcept {
        return _capacity;
    }

    bool empty() const noexcept {
        return _size == 0;
    }
    ~MyVector() {
        ::operator delete(_vector);
    }

    void reserve(size_t new_cap) {
        if(new_cap <= _capacity) return;

        T* new_data = static_cast<T*>(::operator new(new_cap * sizeof(T)));

        for(size_t i = 0; i < _size; ++i) {
            new(new_data + i) T(std::move(_vector[i]));
        }

        for(size_t i = 0; i < _size; ++i) {
            (_vector + i)->~T();
        } 

        ::operator delete(_vector);

        _vector = new_data;
        _capacity = new_cap;
    }

};

new //allocate memory + ctor //std::bad_alloc
new[3]
::operator new //allocate memory return void* //std::bad_alloc
operator new nothrow //malloc

T* = new[3]
delete[]