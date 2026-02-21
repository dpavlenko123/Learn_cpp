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

    MyVector(size_t size, const T& val = {})
        : _size(size), _capacity(size) {
        if (_capacity > 0) {
            _vector = new T[_capacity];
            for (size_t i = 0; i < _size; ++i) {
                _vector[i] = val;
            }
        }
    }

    MyVector(const std::initializer_list<T>& list)
        : _size(list.size()), _capacity(list.size()) {
        if (_capacity > 0) {
            _vector = new T[_capacity];
            auto pos = _vector;
            for (auto el : list) {
                *pos = el;
                ++pos;
            }
        }
    }

    MyVector(const MyVector& other)
        : _size(other._size), _capacity(other._capacity) {
        if (_capacity > 0) {
            _vector = new T[_capacity];
            for (size_t i = 0; i < _size; ++i) {
                _vector[i] = other._vector[i];
            }
        }
    }

    MyVector(MyVector&& other) noexcept
        : _size(other._size), _capacity(other._capacity) {
        _vector = other._vector;
        other._size = 0;
        other._capacity = 0;
        other._vector = nullptr;
    }

    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            delete[] _vector;
            _size = other._size;
            _capacity = other._capacity;

            if (_capacity == 0) {
                _vector = nullptr;
            } else {
                _vector = new T[_capacity];
                for (size_t i = 0; i < _size; ++i) {
                    _vector[i] = other._vector[i];
                }
            }
        }
        return *this;
    }

    MyVector& operator=(MyVector&& other) noexcept {
        if (this != &other) {
            delete[] _vector;
            _size = other._size;
            _capacity = other._capacity;
            _vector = other._vector;

            other._size = 0;
            other._capacity = 0;
            other._vector = nullptr;
        }
        return *this;
    }

    void push_back(const T& elem) {
        if (_size == _capacity) {
            _capacity = (_capacity == 0) ? 1 : _capacity * 2;
            T* new_vector = new T[_capacity];

            for (size_t i = 0; i < _size; ++i) {
                new_vector[i] = std::move(_vector[i]);
            }

            delete[] _vector;
            _vector = new_vector;
        }

        _vector[_size] = elem;
        ++_size;
    }

    void push_back(T&& elem) {
        if (_size == _capacity) {
            _capacity = (_capacity == 0) ? 1 : _capacity * 2;
            T* new_vector = new T[_capacity];

            for (size_t i = 0; i < _size; ++i) {
                new_vector[i] = std::move(_vector[i]);
            }

            delete[] _vector;
            _vector = new_vector;
        }

        _vector[_size] = std::move(elem);
        ++_size;
    }

    template<typename... Args>
    requires std::constructible_from<T, Args...>
    void emplace_back(Args&&... args) {
        if (_size == _capacity) {
            _capacity = (_capacity == 0) ? 1 : _capacity * 2;
            T* new_vector = new T[_capacity];

            for (size_t i = 0; i < _size; ++i) {
                new_vector[i] = std::move(_vector[i]);
            }

            delete[] _vector;
            _vector = new_vector;
        }

        new (&_vector[_size]) T(std::forward<Args>(args)...);
        ++_size;
    }

    T& operator[](const size_t& idx) {
        if (idx >= _size)
            throw "Out of range";
        return _vector[idx];
    }

    const T& operator[](const size_t& idx) const {
        if (idx >= _size)
            throw "Out of range";
        return _vector[idx];
    }

    size_t size() const noexcept {
        return _size;
    }

    size_t capacity() const noexcept {
        return _capacity;
    }

    bool empty() const noexcept {
        return _vector == nullptr;
    }
    ~MyVector() {
            delete[] _vector;
        }
};
