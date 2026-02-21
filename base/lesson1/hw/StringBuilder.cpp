#include "StringBuilder.hpp"

StringBuilder::StringBuilder() : m_string("") {}

std::ostream& operator<<(std::ostream& out, const StringBuilder& obj) {
	out << obj.m_string;
	return out;
}

StringBuilder StringBuilder::operator+(const StringBuilder& other) const {
	StringBuilder tmp(*this);
	tmp.m_string += other.m_string;
	return tmp;
}

size_t StringBuilder::size() const {
	return m_string.size();
}

std::string StringBuilder::str() const {
	return m_string;
}