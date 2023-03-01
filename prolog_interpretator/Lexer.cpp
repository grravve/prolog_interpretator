#include <iostream>
#include <stdlib.h>
#include <string>

#include "Tokens.h"
#include "Lexer.h"

int currentChar = 0;

std::string ReadLetter(std::string string)
{
	//Чтение символов
	std::string tmp;
	if(string[currentChar] == 'm') //Проверки на mod, is
	{
		IncreaseCurrentChar();
		if (string[currentChar] == 'o')
		{
			IncreaseCurrentChar();
			if (string[currentChar] == 'd')
			{
				IncreaseCurrentChar();
				if (!isalpha(string[currentChar]))
				{
					return "mod";
				}
				else
				{
					return "mod" + ReadAtom(string, 0);
				}
			}
			else
			{
				//IncreaseCurrentChar();
				tmp += "mo" + ReadAtom(string, 0);
				//IncreaseCurrentChar();
				return tmp;
			}
		}
		else
		{
			//IncreaseCurrentChar();
			tmp += "m" + ReadAtom(string, 0);
			//IncreaseCurrentChar();
			return tmp;
		}
		
	}
	// перед i пробел и после s пробел
	else if (string[currentChar] == 'i')
	{
		IncreaseCurrentChar();
		if (string[currentChar] == 's')
		{
			IncreaseCurrentChar();
			if (!isalpha(string[currentChar]))
			{
				return "is";
			}
			else
			{
				return "is" + ReadAtom(string, 0);
			}
		}
		else
		{
			tmp += "i" + ReadAtom(string, 0);
			IncreaseCurrentChar();
			return tmp;
		}
	}
	else if (isupper(string[currentChar]) || string[currentChar] == '_')
	{
		return ReadVariable(string);
	}
	else
	{
		return ReadAtom(string, 0);
	}
}

std::string ReadVariable(std::string string)
{
	std::string result;

	result += string[currentChar++];

	while (isdigit(string[currentChar]) || (isalpha(string[currentChar]) && islower(string[currentChar])))
	{
		result += string[currentChar];
		currentChar++;
	}

	return result;
}

std::string ReadAtom(std::string string, int mode)
{
	std::string tmp;
	switch (mode)
	{
		case 0:
		{
			//1 случай: вызов из числа/буквы
			while (isdigit(string[currentChar]) || isalpha(string[currentChar]) || string[currentChar] == '_' && currentChar < string.size())
			{
				tmp += string[currentChar];
				currentChar++;
			}
			break;
		}

		case 1:
		{ 
			//2 случай: вызов из спец символа (комбинации специальных символов + - * = < > :)
			while (string[currentChar] == '+' || string[currentChar] == '-' || string[currentChar] == '*' 
				|| string[currentChar] == '=' || string[currentChar] == '<' || string[currentChar] == '>' || string[currentChar] == ':')
			{
				tmp += string[currentChar];
				currentChar++;
			}
			break;
		}
		
		case 2:
		{
			//3 случай: вызов из одинарных кавычек
			do
			{
				if (currentChar >= string.size())
				{
					return tmp;
				}
				tmp += string[currentChar];
				currentChar++;
			} while (string[currentChar] != '\'');

			tmp += '\'';
			break;
		}
	default:
		tmp = "";
		break;
	}
	return tmp;
}

std::string ReadNumber(std::string string, bool &isAtom, bool isMinus)
{
	std::string word;
	bool isNumber = true;
	bool isDot = false;

	if (isMinus)
	{
		word += '-';
	}
	//Пока встречаются цифры, то это число
	//Если встреться что-то из НЕцифры, то это атом

	while (isNumber && isdigit(string[currentChar]) || (string[currentChar] == '.' && isdigit(string[currentChar + 1])))
	{
		if (string[currentChar] == '.')
		{
			if (isDot == false)
			{
				isDot = true;
			}
			else if (isDot == true)
			{
				return "-illigal-";
			}
		}

		word += string[currentChar];
		currentChar++;
		if (isalpha(string[currentChar]) || string[currentChar] == '_')
		{
			isNumber = false;
		}
		
	}

	if (isNumber == false)
	{
		word += ReadAtom(string, 0);
		isAtom = true;
	}
	else
	{
		isAtom = false;
	}
	return word;
}

void IncreaseCurrentChar()
{
	currentChar++;
}

bool CheckNextChar(std::string string, char nextChar)
{
	int currentNumber = currentChar;
	if (currentNumber >= string.size())
	{
		return false;
	}
	else
	{
		return string[currentNumber + 1] == nextChar;
	}
}

void SkipSpaces(std::string string)
{
	while (string[currentChar] == ' ' || string[currentChar] == '\t' || string[currentChar] == '\n')
	{
		currentChar++;
	}
}

Token CreateNumberOrAtom(std::string string, bool isMinus)
{
	bool isAtom = false;
	std::string word = ReadNumber(string, isAtom, isMinus);

	if (word == "-illigal-")
	{
		return CreateToken("Illigal", "Error in number");
	}

	if (isAtom)
	{
		return CreateToken("Atom", word);
	}
	else
	{
		return CreateToken("Number", word);
	}
}

Token CreateWord(std::string string)
{
	std::string word = ReadLetter(string);
	if (word == "is")
	{
		return CreateToken("Is", "is");
	}
	else if (word == "mod")
	{
		return CreateToken("Mod", "mod");
	}
	else
	{
		return CreateToken("Atom", word);
	}
}

Token GetToken(std::string string)
{
	// Терм - константы(атом или число), переменные, структуры(сложные термы) 
	// Атом есть любая последовательность символов, заключенная в одинарные кавычки. 
	// Кавычки опускаются, если и без них атом можно отличить от символов, используемых для обозначения переменных.
	// Кроме того, любой набор допустимых символов, заключенный в апострофы, также является атомом. 
	// Наконец, комбинации специальных символов + - * = < > :   DONE
	// 
	// Числа - Integer и Float  DONE
	// 
	// Списки - последовательность термов, заключенная в [ ]
	// 
	// Структуры - сложные термы. Атом(терм, терм)
	//
	// Факты - это утверждение о том, что соблюдается некоторое конкретное отношение. имя_предиката(аргументы), аргументы = термы
	//
	// Правила - 
	//
	// Операторы арифметики -  + - * / mod // 
	//
	// Операторы сравнения - = < =< > >= \==
	//
	// Отсечение '!' - 
	//
	// Is - 

	
	while (currentChar < string.size())
	{
		//Удалить пробелы
		SkipSpaces(string);

		//Проверка на символ


		if (isdigit(string[currentChar]))							// цифра
		{
			return CreateNumberOrAtom(string, false);
		}
		else if (isalpha(string[currentChar]))						// буква
		{
			if (string[currentChar] == '_')
			{
				std::string variable = ReadLetter(string);
				
				return CreateToken("Variable", variable);
			}

			if (isupper(string[currentChar]) == false)
			{
				return CreateWord(string);
			}
		}


		//Проверка на спец символ

		switch (string[currentChar])								//спец символ
		{
			case '!':
			{
				IncreaseCurrentChar();
				return CreateToken("CutOperator", "!");
			}

			case '=':
			{
				
				// =
				if (isdigit(string[currentChar + 1]) || isalpha(string[currentChar + 1]) || CheckNextChar(string, ' ') 
					|| CheckNextChar(string, '\0'))
				{
					IncreaseCurrentChar();
					return CreateToken("Unification", "=");
				}
				// =<
				if (CheckNextChar(string, '<'))
				{
					IncreaseCurrentChar();
					IncreaseCurrentChar();
					return CreateToken("LessOrEqual", "=<");
				}
				//// ==
				//if (CheckNextChar(string, '='))
				//{
				//	IncreaseCurrentChar();
				//	if (CheckNextChar(string, '=') == false)
				//	{
				//		IncreaseCurrentChar();
				//		return CreateToken("Equal", "==");
				//	}
				//	else
				//	{
				//		//Atom
				//		std::string tmp = "=";

				//		tmp += ReadAtom(string, 1);
				//		return CreateToken("Atom", tmp);
				//	}
				//}

				//=:=
				if (CheckNextChar(string, ':'))
				{
					IncreaseCurrentChar();

					if (CheckNextChar(string, '='))
					{
						IncreaseCurrentChar();
						return CreateToken("Equal", "=:=");
					}

					std::string tmp = "=:";

					tmp += ReadAtom(string, 1);
					return CreateToken("Atom", tmp);
				}
				
			}

			case '.':
			{
				IncreaseCurrentChar();
				return CreateToken("Dot", ".");
			}

			case ':':
			{
				// :-
				if (CheckNextChar(string, '-'))
				{
					IncreaseCurrentChar();
					IncreaseCurrentChar();
					return CreateToken("If", ":-");
				}
				//atom
				else
				{
					std::string tmp;

					tmp += ReadAtom(string, 1);
					return CreateToken("Atom", tmp);
				}
			}

			case '[':
			{
				IncreaseCurrentChar();
				return CreateToken("OpenBracket", "[");
			}
				
			case ']':
			{
				IncreaseCurrentChar();
				return CreateToken("CloseBracket", "]");
			}

			case '\\':
			{
				// \==  УТОЧНИТЬ
				if (CheckNextChar(string, '='))
				{
					IncreaseCurrentChar();
					if (CheckNextChar(string, '='))
					{
						IncreaseCurrentChar();
						IncreaseCurrentChar();
						return CreateToken("NotEqual", "\\==");
					}
					else
					{
						std::string error = "Error char is ";
						error += string[currentChar];
						IncreaseCurrentChar();
						return CreateToken("Illigal", error);
					}
				}
				else
				{
					std::string error = "Error char is ";
					error += string[currentChar];
					IncreaseCurrentChar();
					return CreateToken("Illigal", error);
				}
			}

			case '>':
			{
				//>
				if (isdigit(string[currentChar + 1]) || isalpha(string[currentChar + 1]) || CheckNextChar(string, ' ')
					|| CheckNextChar(string, '\0') || CheckNextChar(string, '.') || CheckNextChar(string, ','))
				{
					IncreaseCurrentChar();
					return CreateToken("Greater", ">");
				}
				// >=
				else if (CheckNextChar(string, '='))
				{
					IncreaseCurrentChar();
					IncreaseCurrentChar();
					return CreateToken("GreaterOrEqual", ">=");
				}
				else
				{
					// atom
					std::string tmp;

					tmp += ReadAtom(string, 1);
					return CreateToken("Atom", tmp);
				}
			}

			case '<':
			{
				// <
				if (isdigit(string[currentChar + 1]) || isalpha(string[currentChar + 1]) 
					|| CheckNextChar(string, ' ')
					|| CheckNextChar(string, '\0') 
					|| CheckNextChar(string, ',')
					|| CheckNextChar(string, '.'))
				{
					IncreaseCurrentChar();
					return CreateToken("Less", "<");
				}
				else if (CheckNextChar(string, '='))
				{
					IncreaseCurrentChar();
					IncreaseCurrentChar();
					return CreateToken("LessOrEqual", "<=");
				}
				else
				{
					std::string tmp;

					tmp += ReadAtom(string, 1);
					return CreateToken("Atom", tmp);
				}
				//atom
			}

			case ',':
			{
				IncreaseCurrentChar();
				return CreateToken("Comma", ",");
			}

			case '|':
			{
				IncreaseCurrentChar();
				return CreateToken("VerticalBar", "|");
			}

			case '\'':
			{
				std::string tmp = ReadAtom(string, 2);
				IncreaseCurrentChar();
				return CreateToken("Atom", tmp);
			}

			case '+':
			{
				if (CheckNextChar(string, ' ') || isdigit(string[currentChar + 1]) || CheckNextChar(string, '\0')  
					|| CheckNextChar(string, '.') || CheckNextChar(string, ','))
				{
					IncreaseCurrentChar();
					return CreateToken("Plus", "+");
				}
				else
				{
					//atom
					std::string tmp;

					tmp += ReadAtom(string, 1);
					return CreateToken("Atom", tmp);
				}
			}

			case '-':
			{
				//Если перед минусом и за минусом есть число (если перед минусом пробел и за минусом пробел), то это разность(обычный минус)
				//Если перед минусом нет числа,но после есть число, то это отрицание
				//Если за минусом есть минус, то это атом
				if (currentChar == 0)
				{
					if (isdigit(string[currentChar + 1]))
					{
						IncreaseCurrentChar();
						return CreateNumberOrAtom(string, true);
					}
					else 
					{
						IncreaseCurrentChar();
						return CreateToken("Minus", "-");
					}
				}
				else
				{
					if ((isdigit(string[currentChar - 1]) && isdigit(string[currentChar + 1]))
						|| (isalpha(string[currentChar - 1]) && isdigit(string[currentChar + 1]))
						|| (string[currentChar - 1] == ' ' && string[currentChar + 1] == ' ')
						|| ((string[currentChar - 1] == ')' || string[currentChar - 1] == ']') && isdigit(string[currentChar + 1]))
						|| CheckNextChar(string, '\0')
						|| CheckNextChar(string, '.')
						|| CheckNextChar(string, ','))
					{
						IncreaseCurrentChar();
						return CreateToken("Minus", "-");
					}
					else if (!isdigit(string[currentChar - 1]) && isdigit(string[currentChar + 1]))
					{
						IncreaseCurrentChar();
						return CreateNumberOrAtom(string, true);
					}
					else
					{
						std::string tmp;

						tmp += ReadAtom(string, 1);
						return CreateToken("Atom", tmp);
					}
				}

				



				//if (CheckNextChar(string, ' ') || isdigit(string[currentChar + 1]) || CheckNextChar(string, '\0')
				//	|| CheckNextChar(string, '.'))
				//{
				//	IncreaseCurrentChar();
				//	return CreateToken("Minus", "-");
				//}
				//else
				//{
				//	//atom
				//	std::string tmp;

				//	tmp += ReadAtom(string, 1);
				//	return CreateToken("Atom", tmp);
				//}
			}

			case '/':
			{
				//деление
				if (CheckNextChar(string, '/'))
				{
					IncreaseCurrentChar();
					IncreaseCurrentChar();
					return CreateToken("Div", "//");
				}
				else if (isdigit(string[currentChar + 1]) || string[currentChar] == ' ' || string[currentChar] == '\0' || CheckNextChar(string, ','))
				{
					IncreaseCurrentChar();
					return CreateToken("Divide", "/");
				}
				break;
			}

			case '*':
			{
				if (!CheckNextChar(string, '*'))
				{
					IncreaseCurrentChar();
					return CreateToken("Multi", "*");
				}
				else
				{
					std::string tmp;

					tmp += ReadAtom(string, 1);
					return CreateToken("Atom", tmp);
				}
				break;
			}

			case '(':
			{
				IncreaseCurrentChar();
				return CreateToken("OpenParenthesis", "(");
			}

			case ')':
			{
				IncreaseCurrentChar();
				return CreateToken("CloseParenthesis", ")");
			}

			//case '_':
			//{
			//	// начало переменной
			//	// атом с символом _ после 1 символа
			//	return CreateWord(string);
			//	
			//	// аргумент анонимной переменной
			//}

			case '\0':
			{
				return CreateToken("EOF", "\0");
			}

			default:
			{
				std::string error = "Error char is ";
				error += string[currentChar];
				IncreaseCurrentChar();
				return CreateToken("Illigal", error);
			}
		}
	}
	
}



//ОБРАБОТАТЬ СТРОКУ
//ВЕРНУТЬ ТОКЕН