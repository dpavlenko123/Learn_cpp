#include "StringBuilder.hpp"
#include <vector>
#include <iostream>
#include <set>

int main(){
    StringBuilder sb("Hello, world!");
    std::cout << sb << std::endl; ///оператор вывода, вывод в консоли: Hello, world!

    StringBuilder sb1(5);
    std::cout << sb1 << std::endl; //оператор вывода, вывод в консоли: 5

    std::vector<int> vec = {1,2,3,4,5};
    StringBuilder sb2(vec, "|");

    std::cout << sb2.size() << std::endl; // размер строки

    std::cout << sb2 << std::endl; //вывод всех элементов контейнера без разделителей, вывод в консоли: 12345

    std::cout << sb + sb2 << std::endl; //конкатинация строк,  оператор должен вернуть новый StringBulder.

    std::string str = sb2.str(); //возврат строки типа std::string
    std::vector<int> vec1 = {1,2,3,4,5};
    StringBuilder sb3 = StringBuilder("Values: ").append(vec1).reverse();

    std::cout << sb3; //54321 :seulaV
}