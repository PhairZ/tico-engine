#pragma once

#include <string>

class Object {
public:
	virtual ~Object() = default;

	bool is_valid() { return m_valid; }
	std::string to_str() const;

protected:
	void invalidate() { m_valid = false; }

private:
	bool m_valid = true;
	Object* const m_instance_id = this;
};