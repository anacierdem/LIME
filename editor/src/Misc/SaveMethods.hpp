#pragma once

#include <memory>

// Functions to save various data structures. If a type that is attempted to be
// saved has a save method, it will get invoked. Generic types are handled here.
namespace Serialization {

	//TODO: saving and loading strings

	//VS sizeof fix
	template<std::size_t I, typename T>
	struct comparator
	{
		static const bool value = (I < sizeof(T));
	};

	template <typename T>
	class has_save
	{
		typedef char one;
		typedef struct { char a[2]; }  two;

		template <typename C> static one test( char[sizeof(&C::save)] ) ;
		template <typename C> static two test(...);

	public:
		enum {
			YES = comparator<2, T>::value,
			NO = !comparator<2, T>::value
		};
	};

	//http://stackoverflow.com/questions/19985160/distinguishing-integer-from-floating-point-types-in-a-template
	template<typename T>
	void save(std::ofstream& file,
		const T* const input, 
			typename std::enable_if<has_save<T>::YES>::type* = 0) {
		if (file.is_open()) {
			input->save(file);
		}
	}

	template<typename T>
	void save(std::ofstream& file,
		const T* const input,
		typename std::enable_if<has_save<T>::NO>::type* = 0) {
		if (file.is_open()) {
			file.write(reinterpret_cast<const char*>(input), sizeof(T));
		}
	}

	template<typename T>
	void save(std::ofstream& file, const T* const input, int size) {
		if (file.is_open()) {
			file.write(reinterpret_cast<const char*>(input), size);
		}
	}

	inline void save(std::ofstream& file,
		const std::string* const input) {
		if (file.is_open()) {
			const char* tmp = input->c_str();
			int size = input->size();
			file.write(reinterpret_cast<const char*>(&size), sizeof(size));
			file.write(reinterpret_cast<const char*>(tmp), input->size());
		}
	}

	template <typename T>
	class has_load
	{
		typedef char one;
		typedef struct { char a[2]; }  two;

		template <typename C> static one test( char[sizeof(&C::load)] ) ;
		template <typename C> static two test(...);

	public:
		enum {
			YES = comparator<2, T>::value,
			NO = !comparator<2, T>::value
		};
	};

	template<typename T>
	void load(std::ifstream& file,
		T* input,
		typename std::enable_if<has_load<T>::YES, T*>::type* = 0) {
		if (file.is_open()) {
			input->load(file);
		}
	}

	template<typename T>
	void load(std::ifstream& file,
              T* input,
			  typename std::enable_if<has_load<T>::NO, T*>::type* = 0) {
		if (file.is_open()) {
			file.read(reinterpret_cast<char*>(input), sizeof(T));
		}
	}

	template<typename T>
	void load(std::ifstream& file, T* input, int size) {
		if (file.is_open()) {
			file.read(reinterpret_cast<char*>(input), size);
		}
	}

	inline void load(std::ifstream& file,
		std::string* input) {
		if (file.is_open()) {
			int size;
			file.read(reinterpret_cast<char*>(&size), sizeof(int));
			std::unique_ptr<char[]> result(new char[size]);
			file.read(reinterpret_cast<char*>(result.get()), size);
			*input = std::string(result.get(), size);
		}
	}
}

