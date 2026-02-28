#include <iostream>
#include <iterator>
#include <type_traits>
#include <concepts>

template<typename T, bool IsConst>
class MyIterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = std::conditional_t<IsConst, const T*, T*>;
    using reference = std::conditional_t<IsConst, const T&, T&>;

private:
    pointer ptr = nullptr;

public:
    MyIterator(pointer ptr_) : ptr(ptr_) {}

    reference operator*() const { return *ptr; }
    pointer operator->() const { return ptr; }

    MyIterator& operator++() { ++ptr; return *this; }
    MyIterator operator++(int) { MyIterator tmp = *this; ++ptr; return tmp; }

    MyIterator& operator--() { --ptr; return *this; }
    MyIterator operator--(int) { MyIterator tmp = *this; --ptr; return tmp; }

    MyIterator& operator+=(difference_type n) { ptr += n; return *this; }
    MyIterator& operator-=(difference_type n) { ptr -= n; return *this; }

    MyIterator operator+(difference_type n) const { return MyIterator(ptr + n); }
    MyIterator operator-(difference_type n) const { return MyIterator(ptr - n); }

    difference_type operator-(const MyIterator& other) const { return ptr - other.ptr; }
    reference operator[](difference_type n) const { return ptr[n]; }

    bool operator==(const MyIterator& other) const { return ptr == other.ptr; }
    bool operator!=(const MyIterator& other) const { return ptr != other.ptr; }
    bool operator<(const MyIterator& other) const { return ptr < other.ptr; }
    bool operator>(const MyIterator& other) const { return ptr > other.ptr; }
};


template<typename Iterator>
class ReverseIterator {
private:
    Iterator iter;

public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    using pointer = typename std::iterator_traits<Iterator>::pointer;
    using reference = typename std::iterator_traits<Iterator>::reference;

    explicit ReverseIterator(Iterator it) : iter(it) {}

    reference operator*() const { 
        Iterator tmp = iter;
        return *--tmp; 
    }

    pointer operator->() const { return &(operator*()); }

    reference operator[](difference_type n) const { return iter[-n - 1]; }

    ReverseIterator& operator++() { --iter; return *this; }
    ReverseIterator operator++(int) { ReverseIterator tmp = *this; --iter; return tmp; }

    ReverseIterator& operator--() { ++iter; return *this; }
    ReverseIterator operator--(int) { ReverseIterator tmp = *this; ++iter; return tmp; }

    ReverseIterator& operator+=(difference_type n) { iter -= n; return *this; }
    ReverseIterator& operator-=(difference_type n) { iter += n; return *this; }

    ReverseIterator operator+(difference_type n) const { return ReverseIterator(iter - n); }
    ReverseIterator operator-(difference_type n) const { return ReverseIterator(iter + n); }

    difference_type operator-(const ReverseIterator& other) const { return iter - other.iter; }

    bool operator==(const ReverseIterator& other) const { return iter == other.iter; }
    bool operator!=(const ReverseIterator& other) const { return iter != other.iter; }
    bool operator<(const ReverseIterator& other) const { return iter > other.iter; }
    bool operator>(const ReverseIterator& other) const { return iter < other.iter; }
};