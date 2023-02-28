#include <string>

#include "Tokens.h"

Token CreateToken(std::string type, std::string value)
{
	Token tmp{ type, value };
	return tmp;
}



