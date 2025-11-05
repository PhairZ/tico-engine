#include "data.h"

#include <iostream>
#include <string>

Vector2I::Vector2I(const Vector2& p_other) :
	x(p_other.x), y(p_other.y) {
}

bool Vector2I::operator==(const Vector2I& p_other) const {
    return (x == p_other.x && y == p_other.y);
}

Vector2I Vector2I::operator+(const Vector2I& p_other) const {
    return {x + p_other.x, y + p_other.y};
}

Vector2I Vector2I::operator-(const Vector2I& p_other) const {
	return { x - p_other.x, y - p_other.y };
}

Vector2I Vector2I::operator*(const Vector2I& p_other) const {
	return { x * p_other.x, y * p_other.y };
}

Vector2I Vector2I::operator/(const Vector2I& p_other) const {
	return {
		p_other.x == 0 ? x : x / p_other.x,
		p_other.y == 0 ? y : y / p_other.y
	};
}

Vector2I& Vector2I::operator+=(const Vector2I& p_other) {
    return *this = *this + p_other;
}

Vector2I& Vector2I::operator-=(const Vector2I& p_other) {
	return *this = *this - p_other;
}

Vector2I& Vector2I::operator*=(const Vector2I& p_other) {
	return *this = *this * p_other;
}

Vector2I& Vector2I::operator/=(const Vector2I& p_other) {
	return *this = *this / p_other;
}

bool Vector2::operator==(const Vector2& p_other) const {
	return (x == p_other.x && y == p_other.y);
}

double Vector2I::length() const {
	return std::hypot(x, y);
}

Vector2I Vector2I::normalized() const {
	double len = length();
	return (len > 0) ? Vector2I(x / len, y / len) : Vector2I(0);
}

std::string Vector2I::to_str() const {
	return '(' + std::to_string(x) + ", " + std::to_string(y) + ')';
}

Vector2::Vector2(const Vector2I& p_other) :
	x(p_other.x), y(p_other.y) {
}

Vector2 Vector2::operator+(const Vector2& p_other) const {
	return { x + p_other.x, y + p_other.y };
}

Vector2 Vector2::operator-(const Vector2& p_other) const {
	return { x - p_other.x, y - p_other.y };
}

Vector2 Vector2::operator*(const Vector2& p_other) const {
	return { x * p_other.x, y * p_other.y };
}

Vector2 Vector2::operator/(const Vector2& p_other) const {
	return {
		p_other.x == 0 ? x : x / p_other.x,
		p_other.y == 0 ? y : y / p_other.y
	};
}

Vector2& Vector2::operator+=(const Vector2& p_other) {
	return *this = *this + p_other;
}

Vector2& Vector2::operator-=(const Vector2& p_other) {
	return *this = *this - p_other;
}

Vector2& Vector2::operator*=(const Vector2& p_other) {
	return *this = *this * p_other;
}

Vector2& Vector2::operator/=(const Vector2& p_other) {
	return *this = *this / p_other;
}

double Vector2::length() const {
	return std::hypot(x, y);
}

Vector2 Vector2::normalized() const {
	double len = length();
	return (len > 0) ? Vector2(x / len, y / len) : Vector2(0.0);
}

std::string Vector2::to_str() const {
	return '(' + std::to_string(x) + ", " + std::to_string(y) + ')';
}
