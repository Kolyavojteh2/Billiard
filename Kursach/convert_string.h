#pragma once

#include <string>
#include <sstream>

// Реалізація перетворень чисел із/в строку шляхом потоків std
using std::ostringstream;
using std::istringstream;
using std::string;

namespace Vojtekh
{
	namespace convert_string
	{
#define string_to_int from_string<int>;
#define string_to_double from_string<double>;
#define int_to_string to_string<int>;
#define double_to_string to_string<double>;
#define string_to_short from_string<short>;
#define string_to_float from_string<float>;
#define short_to_string to_string<short>;
#define float_to_string to_string<float>;

		template<typename T> string to_string(T value)
		{
			ostringstream oss;
			oss << value;
			return oss.str();
		}

		template<typename T> T from_string(string str)
		{
			istringstream iss(str);
			T value;
			iss >> value;
			return value;
		}

	}
}
/*
	int_value = from_string<int>(str);
	str = to_string<int>(int_value);

	int_value = string_to_int(str);
*/