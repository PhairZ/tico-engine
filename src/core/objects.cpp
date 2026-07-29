#include "objects.h"

#include <typeinfo>

std::string Object::to_str() const {
  return "<" + std::string(typeid(m_instance_id).name()) + ":" +
         std::to_string((size_t)m_instance_id) + ">";
}
