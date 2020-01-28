#pragma once

#include <string_view>
#include <algorithm>
#include <vector>
#include <sstream>

#ifndef CONCEPT
#if __INTELLISENSE__ || !defined(__cpp_concepts)
#define concept inline constexpr bool
#define requires(...)
#define CONCEPT(...) typename
#else
#include <concepts>
#define CONCEPT(...) __VA_ARGS__
#endif
#endif

namespace string_ops
{
	using std::basic_string_view;
	using std::string_view;

	/// ///////////////////////////// ///
	/// ASCII functions
	/// ///////////////////////////// ///

	/// Our own functions that do not block, are defined for values outside of uint8_t, and do not depend on locale
	inline constexpr bool isalpha(char32_t cp) noexcept { return (cp >= 'A' && cp <= 'Z') || (cp >= 'a' && cp <= 'z'); }
	inline constexpr bool isdigit(char32_t d)  noexcept { return d >= '0' && d <= '9'; }
	inline constexpr bool isxdigit(char32_t d) noexcept { return (d >= '0' && d <= '9') || (d >= 'A' && d <= 'F') || (d >= 'a' && d <= 'f'); }
	inline constexpr bool isalnum(char32_t cp) noexcept { return ::string_ops::isdigit(cp) || ::string_ops::isalpha(cp); }
	inline constexpr bool isident(char32_t cp) noexcept { return ::string_ops::isdigit(cp) || ::string_ops::isalpha(cp) || cp == '_'; }
	inline constexpr bool isspace(char32_t d)  noexcept { return d == ' ' || d == '\t' || d == '\n' || d == '\v' || d == '\r' || d == '\f'; }
	inline constexpr bool ispunct(char32_t d)  noexcept { return (d >= 33 && d <= 47) || (d >= 58 && d <= 64) || (d >= 91 && d <= 96) || (d >= 123 && d <= 126); }
	inline constexpr bool islower(char32_t cp) noexcept { return (cp >= 'a' && cp <= 'z'); }
	inline constexpr bool isupper(char32_t cp) noexcept { return (cp >= 'A' && cp <= 'Z'); }
	inline constexpr bool iscntrl(char32_t cp) noexcept { return cp == 0x7F || cp < 0x20; }
	inline constexpr bool isblank(char32_t cp) noexcept { return cp == ' ' || cp == '\t'; }
	inline constexpr bool isgraph(char32_t cp) noexcept { return ::string_ops::isalnum(cp) || ::string_ops::ispunct(cp); }
	inline constexpr bool isprint(char32_t cp) noexcept { return ::string_ops::isgraph(cp) || cp == ' '; }
	
	inline constexpr char32_t toupper(char32_t cp) noexcept { return (cp >= 'a' && cp <= 'z') ? (cp ^ 0b100000) : cp; }
	inline constexpr char32_t tolower(char32_t cp) noexcept { return (cp >= 'A' && cp <= 'Z') ? (cp | 0b100000) : cp; }
	
	inline constexpr char32_t todigit(int v) noexcept { return char32_t(v) + U'0'; }
	inline constexpr char32_t toxdigit(int v) noexcept { return (v > 9) ? (char32_t(v - 10) + U'A') : (char32_t(v) + U'0'); }

	template <typename T>
	constexpr basic_string_view<T> make_sv(const T* start, const T* end) noexcept { return basic_string_view<T>{ start, static_cast<size_t>(end - start) }; }
	template <CONCEPT(std::contiguous_iterator) IT, typename T = typename std::iterator_traits<IT>::value_type>
	constexpr basic_string_view<T> make_sv(IT start, IT end) noexcept { return basic_string_view<T>{ std::to_address(start), static_cast<size_t>(std::distance(std::to_address(start), std::to_address(end))) }; }

	template <typename T>
	basic_string_view<T> trim_whitespace_right(basic_string_view<T> str) noexcept { return make_sv(str.begin(), std::find_if_not(str.rbegin(), str.rend(), ::string_ops::isspace).base()); }
	template <typename T>
	basic_string_view<T> trim_whitespace_left(basic_string_view<T> str) noexcept { return make_sv(std::find_if_not(str.begin(), str.end(), ::string_ops::isspace), str.end()); }
	template <typename T>
	basic_string_view<T> trim_whitespace(basic_string_view<T> str) noexcept { return trim_whitespace_left(trim_whitespace_right(str)); }
	template <typename T>
	basic_string_view<T> trim_until(basic_string_view<T> str, T chr) noexcept { return make_sv(std::find(str.begin(), str.end(), chr), str.end()); }

	template <typename T, typename FUNC>
	string_view trim_while(basic_string_view<T> str, FUNC&& func) noexcept { return make_sv(std::find_if_not(str.begin(), str.end(), std::forward<FUNC>(func)), str.end()); }

	template <typename T>
	T consume(basic_string_view<T>& str)
	{
		if (str.empty())
			return {};
		const auto result = str[0];
		str.remove_prefix(1);
		return result;
	}

	template <typename T>
	bool consume(basic_string_view<T>& str, T val)
	{
		if (str.empty())
			return {};
		if (str[0] == val)
		{
			str.remove_prefix(1);
			return true;
		}
		return false;
	}

	template <typename T, typename FUNC>
	basic_string_view<T> consume_while(basic_string_view<T>& str, FUNC&& pred)
	{
		const auto start = str.begin();
		while (!str.empty() && pred(str[0]))
			str.remove_prefix(1);
		return make_sv(start, str.begin());
	}

	template <typename T, typename FUNC>
	void split(basic_string_view<T> source, basic_string_view<T> delim, FUNC&& func) noexcept
	{
		size_t start = 0, next = 0;
		while ((next = source.find_first_of(delim, start)) != std::string::npos)
		{
			func(make_sv(source.begin() + start, source.begin() + next), false);
			start = next + delim.size();
		}
		func(make_sv(source.begin() + start, source.end()), true);
	}

	template <typename T>
	inline std::vector<basic_string_view<T>> split(basic_string_view<T> source, basic_string_view<T> delim) noexcept
	{
		std::vector<basic_string_view<T>> result;
		::string_ops::split(source, delim, [&](basic_string_view<T> str, bool last) {
			result.push_back(str);
		});
		return result;
	}

	template <typename T, typename CHAR_T>
	std::basic_string<CHAR_T> join(T&& source, basic_string_view<CHAR_T> delim)
	{
		std::basic_stringstream<CHAR_T> strm;
		bool first = true;
		for (auto&& p : std::forward<T>(source))
		{
			if (!first) strm << delim;
			strm << p;
			first = false;
		}
		return strm.str();
	}

	template <typename T, typename FUNC, typename CHAR_T>
	std::basic_string<CHAR_T> join(T&& source, basic_string_view<CHAR_T> delim, FUNC&& transform_func)
	{
		std::basic_stringstream<CHAR_T> strm;
		bool first = true;
		for (auto&& p : source)
		{
			if (!first) strm << delim;
			strm << transform_func(p);
			first = false;
		}
		return strm.str();
	}

}