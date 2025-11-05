#include "objects.h"

#include <typeinfo>

std::ostream& operator<<(std::ostream& p_os, const Object& p_obj) {
    return p_os << '<' << typeid(p_obj.m_instance_id).name() << ':' << p_obj.m_instance_id << '>';
}

