
#ifndef FPP_FORMATTER
#define FPP_FORMATTER

#include <concepts>
#include <string>

namespace FPP {
	template<typename St>
	concept StringLike = requires(St string) {
		std::string() + string;
	};

	template<typename ...Ne>
	concept NotEmpty = requires(Ne ...notEmpty) { sizeof...(Ne) > 0; };
	
	class Formatter {
	private:
		static std::string internalString;
		
		template<StringLike First>
		auto static internalFormat(First first) -> const char* {
			internalString += first;
			return internalString.c_str();
		}
		
		template<StringLike First, NotEmpty ...Rest>
		auto static internalFormat(First first, Rest ...rest) -> const char* {
			internalString += first;
			return internalFormat(rest...);
		}

	public:
		template<StringLike First, NotEmpty ...Rest>
		auto static format(First first, Rest ...rest) -> const char* {
			internalString = "";
			return internalFormat(first, rest...);
		}
	};
}

#endif
