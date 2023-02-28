#include <iostream>
#include <string>
#include <fstream>

#include "Lexer.h"
#include "Tokens.h"
#include "NewParser.h"

int main()
{
	std::string str;
	Token tmp;
	std::ifstream codeFile;
	codeFile.open("test4.txt", std::ios::in);
	
	std::string tmpStr;
	while (std::getline(codeFile, tmpStr))
	{
		str += tmpStr;
	}
	str += '\0';
	codeFile.close();
	//std::cout << str;

	//std::getline(std::cin, str);
	StartAnalysis(str);

	return 0;
}