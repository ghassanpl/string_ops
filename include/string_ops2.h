/// Copyright 2017-2020 Ghassan.pl
/// Usage of the works is permitted provided that this instrument is retained with
/// the works, so that any entity that uses the works is notified of this instrument.
/// DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
#pragma once

#include <string_view>
#include <algorithm>
#include <vector>
#include <sstream>
#include <charconv>

namespace ghassanpl::string_ops
{
	using std::string_view;

	/// ///////////////////////////// ///
	/// ASCII functions
	/// ///////////////////////////// ///

	namespace ascii
	{

		/// Our own functions that do not block, are defined for values outside of uint8_t, and do not depend on locale
		[[nodiscard]] inline constexpr bool isalpha(char32_t cp) noexcept { return (cp >= 'A' && cp <= 'Z') || (cp >= 'a' && cp <= 'z'); }
		[[nodiscard]] inline constexpr bool isdigit(char32_t d)  noexcept { return d >= '0' && d <= '9'; }
		[[nodiscard]] inline constexpr bool isxdigit(char32_t d) noexcept { return (d >= '0' && d <= '9') || (d >= 'A' && d <= 'F') || (d >= 'a' && d <= 'f'); }
		[[nodiscard]] inline constexpr bool isalnum(char32_t cp) noexcept { return ::ghassanpl::string_ops::ascii::isdigit(cp) || ::ghassanpl::string_ops::ascii::isalpha(cp); }
		[[nodiscard]] inline constexpr bool isident(char32_t cp) noexcept { return ::ghassanpl::string_ops::ascii::isdigit(cp) || ::ghassanpl::string_ops::ascii::isalpha(cp) || cp == '_'; }
		[[nodiscard]] inline constexpr bool isspace(char32_t d)  noexcept { return d == ' ' || d == '\t' || d == '\n' || d == '\v' || d == '\r' || d == '\f'; }
		[[nodiscard]] inline constexpr bool ispunct(char32_t d)  noexcept { return (d >= 33 && d <= 47) || (d >= 58 && d <= 64) || (d >= 91 && d <= 96) || (d >= 123 && d <= 126); }
		[[nodiscard]] inline constexpr bool islower(char32_t cp) noexcept { return (cp >= 'a' && cp <= 'z'); }
		[[nodiscard]] inline constexpr bool isupper(char32_t cp) noexcept { return (cp >= 'A' && cp <= 'Z'); }
		[[nodiscard]] inline constexpr bool iscntrl(char32_t cp) noexcept { return cp == 0x7F || cp < 0x20; }
		[[nodiscard]] inline constexpr bool isblank(char32_t cp) noexcept { return cp == ' ' || cp == '\t'; }
		[[nodiscard]] inline constexpr bool isgraph(char32_t cp) noexcept { return ::ghassanpl::string_ops::ascii::isalnum(cp) || ::ghassanpl::string_ops::ascii::ispunct(cp); }
		[[nodiscard]] inline constexpr bool isprint(char32_t cp) noexcept { return ::ghassanpl::string_ops::ascii::isgraph(cp) || cp == ' '; }

		[[nodiscard]] inline constexpr char32_t toupper(char32_t cp) noexcept { return (cp >= 'a' && cp <= 'z') ? (cp ^ 0b100000) : cp; }
		[[nodiscard]] inline constexpr char32_t tolower(char32_t cp) noexcept { return (cp >= 'A' && cp <= 'Z') ? (cp | 0b100000) : cp; }

		[[nodiscard]] inline std::string tolower(std::string&& str) noexcept { std::for_each(str.begin(), str.end(), [](char& cp) { cp = (char)tolower(cp); }); return str; }
		[[nodiscard]] inline std::string tolower(std::string_view str) noexcept { return tolower(std::string{str}); }

		[[nodiscard]] inline std::string toupper(std::string&& str) noexcept { std::for_each(str.begin(), str.end(), [](char& cp) { cp = (char)toupper(cp); }); return str; }
		[[nodiscard]] inline std::string toupper(string_view str) noexcept { return toupper(std::string{str}); }

		[[nodiscard]] inline constexpr char32_t todigit(int v) noexcept { return char32_t(v) + U'0'; }
		[[nodiscard]] inline constexpr char32_t toxdigit(int v) noexcept { return (v > 9) ? (char32_t(v - 10) + U'A') : (char32_t(v) + U'0'); }

		[[nodiscard]] constexpr bool strings_equal_ignore_case(string_view a, string_view b)
		{
			return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b) { return toupper(a) == toupper(b); });
		}

		[[nodiscard]] constexpr bool lexicographical_compare_ignore_case(string_view a, string_view b)
		{
			return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b) { return toupper(a) == toupper(b); });
		}

	}

	/// ///////////////////////////// ///
	/// Makes
	/// ///////////////////////////// ///

	[[nodiscard]] inline constexpr string_view make_sv(const char* start, const char* end) noexcept { return string_view{ start, static_cast<size_t>(end - start) }; }
	template <std::contiguous_iterator IT, typename T = typename std::iterator_traits<IT>::value_type>
	[[nodiscard]] inline constexpr string_view make_sv(IT start, IT end) noexcept { return string_view{ std::to_address(start), static_cast<size_t>(std::distance(std::to_address(start), std::to_address(end))) }; }

	[[nodiscard]] inline std::string make_string(const char* start, const char* end) noexcept { return std::string{ start, static_cast<size_t>(end - start) }; }
	template <std::contiguous_iterator IT, typename T = typename std::iterator_traits<IT>::value_type>
	[[nodiscard]] inline std::string make_string(IT start, IT end) noexcept { return std::string{ std::to_address(start), static_cast<size_t>(std::distance(std::to_address(start), std::to_address(end))) }; }

	/// ///////////////////////////// ///
	/// Trims
	/// ///////////////////////////// ///

	[[nodiscard]] inline string_view trimmed_whitespace_right(string_view str) noexcept { return make_sv(str.begin(), std::find_if_not(str.rbegin(), str.rend(), ::ghassanpl::string_ops::ascii::isspace).base()); }
	[[nodiscard]] inline string_view trimmed_whitespace_left(string_view str) noexcept { return make_sv(std::find_if_not(str.begin(), str.end(), ::ghassanpl::string_ops::ascii::isspace), str.end()); }
	[[nodiscard]] inline string_view trimmed_whitespace(string_view str) noexcept { return trimmed_whitespace_left(trimmed_whitespace_right(str)); }
	[[nodiscard]] inline string_view trimmed_until(string_view str, char chr) noexcept { return make_sv(std::find(str.begin(), str.end(), chr), str.end()); }

	template <typename FUNC>
	[[nodiscard]] inline string_view trimmed_while(string_view str, FUNC&& func) noexcept { return make_sv(std::find_if_not(str.begin(), str.end(), std::forward<FUNC>(func)), str.end()); }

	/// ///////////////////////////// ///
	/// Consume
	/// ///////////////////////////// ///
	
	inline char consume(string_view& str)
	{
		if (str.empty())
			return {};
		const auto result = str[0];
		str.remove_prefix(1);
		return result;
	}

	inline bool consume(string_view& str, char val)
	{
		if (str.starts_with(val))
		{
			str.remove_prefix(1);
			return true;
		}
		return false;
	}

	inline bool consume(string_view& str, string_view val)
	{
		if (str.starts_with(val))
		{
			str.remove_prefix(val.size());
			return true;
		}
		return false;
	}

	/*
	template <size_t N>
	bool consume(string_view& str, char(&val)[N])
	{
		return consume(str, make_sv(val, val + N));
	}
	*/
	
	inline bool consume_at_end(string_view& str, char val)
	{
		if (str.ends_with(val))
		{
			str.remove_prefix(1);
			return true;
		}
		return false;
	}

	inline bool consume_at_end(string_view& str, string_view val)
	{
		if (str.ends_with(val))
		{
			str.remove_suffix(val.size());
			return true;
		}
		return false;
	}

	template <typename FUNC>
	requires std::invocable<FUNC, char>
	inline string_view consume_while(string_view& str, FUNC&& pred)
	{
		const auto start = str.begin();
		while (!str.empty() && pred(str[0]))
			str.remove_prefix(1);
		return make_sv(start, str.begin());
	}

	template <typename FUNC>
	inline string_view consume_while(string_view& str, char c)
	{
		const auto start = str.begin();
		while (str.starts_with(c))
			str.remove_prefix(1);
		return make_sv(start, str.begin());
	}

	inline string_view consume_until(string_view& str, char c)
	{
		const auto start = str.begin();
		while (!str.empty() && str[0] != c)
			str.remove_prefix(1);
		return make_sv(start, str.begin());
	}

	inline string_view consume_n(string_view& str, size_t n)
	{
		n = std::min(str.size(), n);
		auto result = str.substr(0, n);
		str.remove_prefix(n);
		return result;
	}

	inline string_view consume_c_identifier(string_view& str)
	{
		if (str.empty() || !(ascii::isalpha(str[0]) || str[0] == '_'))
			return {};

		const auto start = str.begin();
		str.remove_prefix(1);
		consume_while(str, ascii::isident);
		return make_sv(start, str.begin());
	}

	inline pair<string_view, double> consume_c_float(string_view& str)
	{
		if (str.empty() || !(ascii::isdigit(str[0]) || str[0] == '-'))
			return {};

		pair<string_view, double> result;

		auto from_chars_result = std::from_chars(str.data(), str.data() + str.size(), result.second);
		if (from_chars_result.ec != std::errc{})
			return { {}, std::numeric_limits<double>::quiet_NaN() };

		result.first = make_sv(str.data(), from_chars_result.ptr);
		str.remove_prefix(result.first.size());
		return result;
	}

	inline pair<string_view, int64_t> consume_c_integer(string_view& str, int base = 10)
	{
		if (str.empty() || !(ascii::isdigit(str[0]) || str[0] == '-'))
			return {};

		pair<string_view, int64_t> result;

		auto from_chars_result = std::from_chars(str.data(), str.data() + str.size(), result.second, base);
		if (from_chars_result.ec != std::errc{})
			return { {}, 0 };

		result.first = make_sv(str.data(), from_chars_result.ptr);
		str.remove_prefix(result.first.size());
		return result;
	}

	size_t append_utf8(std::string& buffer, char32_t cp);

	inline pair<string_view, std::string> consume_c_string(string_view& strv)
	{
		if (strv.empty() || strv[0] != '"')
			return {};

		pair<string_view, std::string> result;

		auto view = strv;
		auto start = view.begin();
		view.remove_prefix(1);
		while (view[0] != '"')
		{
			auto cp = consume(view);
			if (cp == '\\')
			{
				auto escape_start = view.begin();

				cp = consume(view);
				if (view.empty())
					return {}; /// Unterminated string literal

				switch (cp)
				{
				case 'n': result.second += '\n'; break;
				case '"': result.second += '"'; break;
				case '\'': result.second += '\''; break;
				case '\\': result.second += '\\'; break;
				case 'b': result.second += '\b'; break;
				case 'r': result.second += '\r'; break;
				case 'f': result.second += '\f'; break;
				case 't': result.second += '\t'; break;
				case '0': result.second += '\0'; break;
				case 'o':
				{
					auto num = consume_n(view, 3);
					if (num.size() < 3 || view.empty()) return {}; /// malformed

					auto parsed = consume_c_integer(num, 8);
					if (parsed.first.empty() || !num.empty()) return {}; /// malformed

					if (parsed.second > 255) return {}; /// invalid octal
					result.second.push_back((char)parsed.second);
					break;
				}
				case 'x':
				{
					auto num = consume_n(view, 2);
					if (num.size() < 2 || view.empty()) return {}; /// malformed

					auto parsed = consume_c_integer(num, 16);
					if (parsed.first.empty() || !num.empty()) return {}; /// malformed

					append_utf8(result.second, (char32_t)parsed.second);
					break;
				}
				case 'u':
				{
					auto num = consume_n(view, 4);
					if (num.size() < 4 || view.empty()) return {}; /// malformed

					auto parsed = consume_c_integer(num, 16);
					if (parsed.first.empty() || !num.empty()) return {}; /// malformed

					append_utf8(result.second, (char32_t)parsed.second);
					break;
				}
				case 'U':
				{
					auto num = consume_n(view, 8);
					if (num.size() < 8 || view.empty()) return {}; /// malformed

					auto parsed = consume_c_integer(num, 16);
					if (parsed.first.empty() || !num.empty()) return {}; /// malformed

					append_utf8(result.second, (char32_t)parsed.second);
					break;
				}
				default:
					return {}; /// unknown escape character
				}
			}
			else
			{
				result.second += cp;
			}

			if (view.empty())
				return {}; /// unterminated
		}

		if (!consume(view, '"'))
			return {}; /// unterminated

		result.first = make_sv(start, view.begin());
		strv = view;
		return result;
	}

	/// ///////////////////////////// ///
	/// Basic UTF-8 stuff
	/// ///////////////////////////// ///
	
	/// Assuming str is valid UTF-8
	inline char32_t consume_utf8(string_view& str)
	{
		if (str.empty()) return 0;
		auto it = (uint8_t*)std::to_address(str.begin());
		char32_t cp = *it;

		int length = 0;
		if (cp < 0x80) length = 1;
		else if ((cp >> 5) == 0x6)  length = 2;
		else if ((cp >> 4) == 0xe)  length = 3;
		else if ((cp >> 3) == 0x1e) length = 4;
		else return 0;

		switch (length) {
		case 2:
			++it; cp = ((cp << 6) & 0x7ff) + ((*it) & 0x3f);
			break;
		case 3:
			++it; cp = ((cp << 12) & 0xffff) + ((*it << 6) & 0xfff);
			++it; cp += (*it) & 0x3f;
			break;
		case 4:
			++it; cp = ((cp << 18) & 0x1fffff) + (((*it) << 12) & 0x3ffff);
			++it; cp += ((*it) << 6) & 0xfff;
			++it; cp += (*it) & 0x3f;
			break;
		}
		str.remove_prefix(length);
		return cp;
	}

	/// Assuming codepoint is valid
	inline size_t append_utf8(std::string& buffer, char32_t cp)
	{
		if (cp < 0x80)
		{
			buffer += static_cast<char>(cp);
			return 1;
		}
		else if (cp < 0x800)
		{
			buffer += static_cast<char>((cp >> 6) | 0xc0);
			buffer += static_cast<char>((cp & 0x3f) | 0x80);
			return 2;
		}
		else if (cp < 0x10000)
		{
			buffer += static_cast<char>((cp >> 12) | 0xe0);
			buffer += static_cast<char>(((cp >> 6) & 0x3f) | 0x80);
			buffer += static_cast<char>((cp & 0x3f) | 0x80);
			return 3;
		}
		else
		{
			buffer += static_cast<char>((cp >> 18) | 0xf0);
			buffer += static_cast<char>(((cp >> 12) & 0x3f) | 0x80);
			buffer += static_cast<char>(((cp >> 6) & 0x3f) | 0x80);
			buffer += static_cast<char>((cp & 0x3f) | 0x80);
			return 4;
		}
	}

	/// ///////////////////////////// ///
	/// Other
	/// ///////////////////////////// ///

	template <typename DELIM, typename FUNC>
	inline void split(string_view source, DELIM&& delim, FUNC&& func) noexcept
	{
		size_t next = 0;
		while ((next = source.find_first_of(delim)) != std::string::npos)
		{
			func(source.substr(0, next), false);
			source.remove_prefix(next + 1);
		}
		func(source, true);
	}

	template <typename DELIM, typename FUNC>
	inline void natural_split(string_view source, DELIM&& delim, FUNC&& func) noexcept
	{
		size_t next = 0;
		while ((next = source.find_first_of(delim)) != std::string::npos)
		{
			func(source.substr(0, next), false);
			source.remove_prefix(next + 1);

			if ((next = source.find_first_not_of(delim)) == std::string::npos)
				return;

			source.remove_prefix(next + 1);
		}

		if (!source.empty())
			func(source, true);
	}

	template <typename DELIM>
	[[nodiscard]] inline std::vector<string_view> split(string_view source, DELIM&& delim) noexcept
	{
		std::vector<string_view> result;
		::ghassanpl::string_ops::split(source, forward<DELIM>(delim), [&result](string_view str, bool last) {
			result.push_back(str);
		});
		return result;
	}

	template <typename DELIM>
	[[nodiscard]] inline std::vector<string_view> natural_split(string_view source, DELIM&& delim) noexcept
	{
		std::vector<string_view> result;
		::ghassanpl::string_ops::natural_split(source, forward<DELIM>(delim), [&result](string_view str, bool last) {
			result.push_back(str);
		});
		return result;
	}

	template <std::ranges::range T, typename DELIM>
	[[nodiscard]] inline auto join(T&& source, DELIM&& delim)
	{
		std::stringstream strm;
		bool first = true;
		for (auto&& p : std::forward<T>(source))
		{
			if (!first) strm << forward<DELIM>(delim);
			strm << p;
			first = false;
		}
		return strm.str();
	}

	template <std::ranges::range T, typename FUNC, typename DELIM>
	[[nodiscard]] inline auto join(T&& source, DELIM&& delim, FUNC&& transform_func)
	{
		std::stringstream strm;
		bool first = true;
		for (auto&& p : source)
		{
			if (!first) strm << forward<DELIM>(delim);
			strm << transform_func(p);
			first = false;
		}
		return strm.str();
	}

	template <typename NEEDLE, typename REPLACE>
	inline void replace(std::string& subject, NEEDLE&& search, REPLACE&& replace)
	{
		using std::empty;
		using std::size;

		if (std::string_view{ search }.empty())
			return;

		const auto search_size = std::string_view{ search }.size();
		const auto replace_size = std::string_view{ replace }.size();

		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos)
		{
			subject.replace(pos, search_size, replace);
			pos += replace_size;
		}
	}


	template <typename T>
	inline auto from_chars(std::string_view str, T& value, const int base = 10) noexcept {
		return std::from_chars(str.data(), str.data() + str.size(), value, base);
	}

}