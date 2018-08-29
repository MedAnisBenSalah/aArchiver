#ifndef BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A
#define BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A

#include <string>

class CEncoder
{
	public:
		static inline bool CEncoder::IsEncoded(unsigned char c) {
			return (isalnum(c) || (c == '+') || (c == '/'));
		}

		static std::string Encode(unsigned char const*, unsigned int len);
		static std::string Decode(std::string const& s);
};

#endif /* BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A */