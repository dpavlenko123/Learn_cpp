#include <iostream>
#include <type_traits>
#include <concepts>
#include <iterator>

template<typename Range>
requires requires(Range cont) { std::begin(cont); std::end(cont); } &&
         std::random_access_iterator<typename Range::iterator>
class step_range {
private:
    Range range;
    size_t step = 1;

public:
    class step_iterator {
    public:
        using base_iterator = typename Range::iterator;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = typename std::iterator_traits<base_iterator>::value_type;
        using pointer = typename std::iterator_traits<base_iterator>::pointer;
        using reference = typename std::iterator_traits<base_iterator>::reference;

    private:
        base_iterator ptr;
        size_t step = 1;

    public:
        step_iterator() = default;
        step_iterator(base_iterator p, size_t s) : ptr(p), step(s) {}

        reference operator*() const { return *ptr; }
        pointer operator->() const { return ptr; }

        step_iterator& operator++() { 
            ptr += step; 
            return *this; 
        }

        step_iterator operator++(int) { 
            step_iterator tmp = *this; 
            ptr += step; 
            return tmp; 
        }

        step_iterator& operator--() { 
            ptr -= step; 
            return *this; 
        }

        step_iterator operator--(int) { 
            step_iterator tmp = *this; 
            ptr -= step; 
            return tmp; 
        }

        step_iterator& operator+=(difference_type n) { 
            ptr += n * step; 
            return *this; 
        }

        step_iterator& operator-=(difference_type n) { 
            ptr -= n * step; 
            return *this; 
        }

        step_iterator operator+(difference_type n) const { 
            step_iterator tmp = *this; 
            tmp += n; 
            return tmp; 
        }

        step_iterator operator-(difference_type n) const { 
            step_iterator tmp = *this; 
            tmp -= n; 
            return tmp; 
        }

        difference_type operator-(const step_iterator& other) const { 
            return (ptr - other.ptr) / step; 
        }

        bool operator==(const step_iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const step_iterator& other) const { return ptr != other.ptr; }
        bool operator<(const step_iterator& other) const { return ptr < other.ptr; }
        bool operator>(const step_iterator& other) const { return ptr > other.ptr; }
    };

    step_range(const Range& r, size_t st) : range(r), step(st) {}

    step_iterator begin() const { 
        return step_iterator(range.begin(), step); 
    }

    step_iterator end() const { 
        auto count = (range.size() + step - 1) / step; 
        return step_iterator(range.begin() + count, step); 
    }

    friend std::ostream& operator<<(std::ostream& out, const step_range& range) {
        for (auto el : range) {
            out << el << " ";
        }
        return out;
    }
};