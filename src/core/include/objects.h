#pragma once

#include <iostream>

class Object {
	friend std::ostream& operator<<(std::ostream& p_os, const Object& p_obj);
public:
	virtual ~Object() = default;

	bool is_valid() { return m_valid; }

protected:
	void invalidate() { m_valid = false; }

private:
	bool m_valid = true;
	Object* m_instance_id = this;
};