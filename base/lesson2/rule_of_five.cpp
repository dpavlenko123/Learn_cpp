
#include <iostream>


struct Foo {
    Foo(){std::cout << "Foo()" << std::endl;};

    Foo(const Foo&) {std::cout << "const Foo&" << std::endl;}

    Foo(Foo&&) {std::cout << "Foo&&" << std::endl;}

    Foo& operator=(const Foo&) {std::cout << " op const Foo&" << std::endl; return *this;}

    Foo& operator=(Foo&&) {std::cout << " op Foo&&" << std::endl; return *this;}

    ~Foo() { std::cout << "~Foo" << std::endl; }
};

template<typename T>
class Buffer {
public:
    Buffer(T* data) : data_(data) {}

    Buffer(const Buffer& other)
        : data_(other.data_) {}

    Buffer(Buffer&& other) noexcept
        : data_(other.data_) {
        other.data_ = nullptr;
    }

    Buffer& operator=(const Buffer& other) {
        if (this == &other) return *this;

        data_ = other.data_;

        return *this;
    }

    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            data_ = other.data_;
            other.data_ = nullptr;
        }
        return *this;
    }

    ~Buffer() = default;

private:
    T* data_ = nullptr;
};


std::vector<Buffer> vec;

vec1 = std::move(vec);

Foo f() {
    Foo f;
    return std::move(f);
}

int main() {
    auto foo = f();
}