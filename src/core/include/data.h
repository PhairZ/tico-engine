#pragma once

#include <string>

struct Vector2;

struct Vector2I {
  int x, y;

  constexpr Vector2I() : x(0), y(0) {}

  constexpr Vector2I(int n) : x(n), y(n) {}

  constexpr Vector2I(int x, int y) : x(x), y(y) {}

  constexpr Vector2I(const Vector2I &p_other) : x(p_other.x), y(p_other.y) {}

  Vector2I(const Vector2 &p_other);

  double length() const;
  Vector2I normalized() const;
  std::string to_str() const;

  bool operator==(const Vector2I &other) const;

  Vector2I operator+(const Vector2I &p_other) const;
  Vector2I operator-(const Vector2I &p_other) const;
  Vector2I operator*(const Vector2I &p_other) const;
  Vector2I operator/(const Vector2I &p_other) const;

  Vector2I &operator+=(const Vector2I &p_other);
  Vector2I &operator-=(const Vector2I &p_other);
  Vector2I &operator*=(const Vector2I &p_other);
  Vector2I &operator/=(const Vector2I &p_other);
};

struct Vector2 {
  double x, y;

  constexpr Vector2() : x(0.0), y(0.0) {}

  constexpr Vector2(double n) : x(n), y(n) {}

  constexpr Vector2(double x, double y) : x(x), y(y) {}

  constexpr Vector2(const Vector2 &p_other) : x(p_other.x), y(p_other.y) {}

  Vector2(const Vector2I &p_other);

  double length() const;
  Vector2 normalized() const;
  std::string to_str() const;

  bool operator==(const Vector2 &p_other) const;

  Vector2 operator+(const Vector2 &p_other) const;
  Vector2 operator-(const Vector2 &p_other) const;
  Vector2 operator*(const Vector2 &p_other) const;
  Vector2 operator/(const Vector2 &p_other) const;

  Vector2 &operator+=(const Vector2 &p_other);
  Vector2 &operator-=(const Vector2 &p_other);
  Vector2 &operator*=(const Vector2 &p_other);
  Vector2 &operator/=(const Vector2 &p_other);
};
