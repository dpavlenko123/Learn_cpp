Реализация vector с гарантиями исключений.
Реализовать итератор и константный итератор.

Дополнительно:

- Реализовать reverse_iterator и const_reverse_iterator

- Реализовать step_range и следующие функции:

1. template<typename Range> //Range - это любой объект с парой итераторов  (begin, end) 
    step_range(Range, size_t step = 0)

2. step_range.begin()
3. step_range.end()
4. operator<<

Примеры:

```c++
std::vector vec = {1,2,3,4,5};

step_range srange(vec, 2);

std::cout << srange; //1,3,5

auto it = srange.begin() + 1; //*it == 3
auto it = srange.begin() + 2; //*it == 5
```
