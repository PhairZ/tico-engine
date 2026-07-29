#pragma once

#include "data.h"
#include "objects.h"
#include "renderer.h"

#include <vector>

class Block : Object {
public:
  Block() : color(Renderer::TRANSPARENT), m_position(Vector2I()) {}
  Block(const Vector2I &p_pos, Renderer::Color p_color)
      : color(p_color), m_position(p_pos) {}

  const Vector2I &get_position() const { return m_position; }

  void draw();
  int move(const Vector2I &p_offset);
  int rotate(bool p_ccw = false);

  const Renderer::Color color;

  static constexpr int VARIATIONS = 7;

  virtual const std::vector<Vector2I> get_shape() const { return {Vector2I()}; }

private:
  void clear();

  Vector2I m_position;
  int m_rot_offset = 0;
};

class OTetroid : public Block {
public:
  using Block::Block;

  const std::vector<Vector2I> get_shape() const override {
    return {Vector2I(0, 0), Vector2I(0, 1), Vector2I(1, 0), Vector2I(1, 1)};
  }
};

class TTetroid : public Block {
public:
  using Block::Block;

  const std::vector<Vector2I> get_shape() const override {
    return {Vector2I(0, 0), Vector2I(0, 1),  Vector2I(1, 0),  Vector2I(-1, 0),

            Vector2I(0, 0), Vector2I(-1, 0), Vector2I(0, -1), Vector2I(0, 1),

            Vector2I(0, 0), Vector2I(0, -1), Vector2I(1, 0),  Vector2I(-1, 0),

            Vector2I(0, 0), Vector2I(1, 0),  Vector2I(0, -1), Vector2I(0, 1)};
  }
};

class LTetroid : public Block {
public:
  using Block::Block;

  const std::vector<Vector2I> get_shape() const override {
    return {Vector2I(0, 0), Vector2I(-1, 0), Vector2I(1, 0),  Vector2I(1, 1),

            Vector2I(0, 0), Vector2I(0, -1), Vector2I(0, 1),  Vector2I(-1, 1),

            Vector2I(0, 0), Vector2I(1, 0),  Vector2I(-1, 0), Vector2I(-1, -1),

            Vector2I(0, 0), Vector2I(0, 1),  Vector2I(0, -1), Vector2I(1, -1)};
  }
};

class JTetroid : public Block {
public:
  using Block::Block;

  const std::vector<Vector2I> get_shape() const override {
    return {Vector2I(0, 0), Vector2I(1, 0),  Vector2I(-1, 0), Vector2I(-1, 1),

            Vector2I(0, 0), Vector2I(0, 1),  Vector2I(0, -1), Vector2I(-1, -1),

            Vector2I(0, 0), Vector2I(-1, 0), Vector2I(1, 0),  Vector2I(1, -1),

            Vector2I(0, 0), Vector2I(0, -1), Vector2I(0, 1),  Vector2I(1, 1)};
  }
};

class STetroid : public Block {
public:
  using Block::Block;

  const std::vector<Vector2I> get_shape() const override {
    return {Vector2I(0, 0), Vector2I(1, 0), Vector2I(0, 1),  Vector2I(-1, 1),

            Vector2I(0, 0), Vector2I(0, 1), Vector2I(-1, 0), Vector2I(-1, -1)};
  }
};

class ZTetroid : public Block {
public:
  using Block::Block;

  const std::vector<Vector2I> get_shape() const override {
    return {Vector2I(0, 0), Vector2I(-1, 0), Vector2I(0, 1),  Vector2I(1, 1),

            Vector2I(0, 0), Vector2I(0, -1), Vector2I(-1, 0), Vector2I(-1, 1)};
  }
};

class ITetroid : public Block {
public:
  using Block::Block;

  const std::vector<Vector2I> get_shape() const override {
    return {Vector2I(0, 0), Vector2I(-1, 0), Vector2I(1, 0), Vector2I(2, 0),

            Vector2I(0, 0), Vector2I(0, -1), Vector2I(0, 1), Vector2I(0, 2)};
  }
};
