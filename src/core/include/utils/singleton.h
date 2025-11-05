#pragma once

#include <memory>

#define SINGLETON(class_name) public:					\
	class_name(class_name&) = delete;					\
	class_name& operator=(class_name&) = delete;		\
	class_name(class_name&&) = delete;					\
	class_name& operator=(class_name&&) = delete;		\
														\
	static class_name& get_singleton() {				\
		static class_name* instance = new class_name();	\
		return *instance;								\
	}													\
private:												\
	class_name()
