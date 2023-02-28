#pragma once
#include <string>

#ifndef TOKENS
#define TOKENS

struct Token
{
	std::string type;
	std::string value;
};

Token CreateToken(std::string type, std::string value);

#endif TOKENS


