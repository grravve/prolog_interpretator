#include "NewParser.h";
#include "Lexer.h"
#include "AST.h"

Token PreviousToken;
Token CurrentToken;

std::string InputString;

std::vector<Token> Tokens;

std::string ArithmeticOperators[]
{
	"Divide",
	"Plus",
	"Minus",
	"Div",
	"Multi",
	"Mod"
};

std::string LogicOperators[]
{
	"LessOrEqual",
	"Equal",
	"NotEqual",
	"Greater",
	"GreaterOrEqual",
	"Less",
	"LessOrEqual"
};


void StartAnalysis(std::string str)
{
	InputString = str;
	GetNextToken();

	Program();
}

void Program()
{
	bool IsProgramEnd = false;
	bool IsSentence = false;

	while (!IsProgramEnd)
	{
		Sentence();

		//Check EOF
		GetNextToken();

		if (CurrentToken.type == "EOF")
		{
			IsProgramEnd = true;
		}
	}
}

void Sentence()
{
	bool IsFactSign = false;
	bool IsRuleSign = false;

	Header();

	GetNextToken();

	IsFactSign = Dot();

	if (IsFactSign)
	{
		return;
	}

	IsRuleSign = RuleSign();

	if (!IsRuleSign)
	{
		Error("Parser recevied not fact's and not rule's sign");
	}

	GetNextToken();

	RuleBody();
}

void Term()
{
	bool IsNumber = false;
	bool IsAtom = false;
	bool IsOpenBracket = false;
	bool IsVariable = false;

	IsNumber = Number();

	if (IsNumber)
	{
		GetNextToken();

		return;
	}

	IsVariable = Variable();

	if (IsVariable)
	{
		GetNextToken();

		return;
	}

	IsAtom = Atom();

	if (IsAtom)
	{
		GetNextToken();

		bool IsOpenParenthesis = OpenParenthesis();

		if (!IsOpenParenthesis)
		{
			return; // It's Atom
		}

		Structure();

		return;
	}

	IsOpenBracket = OpenBracket();

	if (IsOpenBracket)
	{
		List();

		GetNextToken();

		return;
	}

	Error("Invalid Term token\nToken type: " + CurrentToken.type + " " + CurrentToken.value);
}

void Header()
{
	Structure();
}

void Structure()
{
	bool IsOpenParanthesis = false;
	bool IsCloseParanthesis = false;
	bool IsComma = false;
	bool IsTerm = false;

	GetNextToken();

	Term();

	bool IsTermsEnd = false;

	while (!IsTermsEnd)
	{
		IsCloseParanthesis = CloseParenthesis();

		if (IsCloseParanthesis)
		{
			break;
		}

		IsComma = Comma();

		if (!IsComma)
		{
			Error("Invalid comma token: \nIvalid token type:" + CurrentToken.type + "Value:" + CurrentToken.value);
		}

		GetNextToken();

		Term();
	}

	return;
}

void List()
{
	bool IsOpenBracket = false;
	bool IsCloseBracket = false;
	bool IsListHeader = false;
	bool IsVerticalBar = false;
	bool IsTerm = false;

	IsOpenBracket = OpenBracket();

	if (!IsOpenBracket)
	{
		Error("Invalid list's open bracket token: \nIvalid token type:" + CurrentToken.type + "Value:" + CurrentToken.value);
	}

	IsCloseBracket = CloseBracket();

	// Empty list

	if (IsCloseBracket)
	{
		return;
	}

	GetNextToken();

	ListHeader();

	GetNextToken();

	IsCloseBracket = CloseBracket();

	// Check simple list

	if (IsCloseBracket)
	{
		return;
	}

	// Check list's tail

	IsVerticalBar = VerticalBar();

	if (!IsVerticalBar)
	{
		Error("Invalid list's vertical bar token: \nIvalid token type:" + CurrentToken.type + "Value:" + CurrentToken.value);
	}

	GetNextToken();
	
	Term();

	IsCloseBracket = CloseBracket();

	if (!IsCloseBracket)
	{
		Error("Invalid list's close bracket token: \nIvalid token type:" + CurrentToken.type + "Value:" + CurrentToken.value);
	}

	return;
}

void ListHeader()
{
	bool IsComma = true;

	Term();

	while (true)
	{
		IsComma = Comma();
		
		if (!IsComma)
		{
			break;
		}

		GetNextToken();
		
		Term();
	}

	return;
}

void RuleBody()
{
	bool IsComma = false;
	bool IsDot = false;

	Target();

	//GetNextToken();

	while (true)
	{
		IsDot = Dot();

		if (IsDot)
		{
			break;
		}

		IsComma = Comma();

		if(!IsComma)
		{
			Error("Invalid comma token: \nIvalid token type:" + CurrentToken.type + "Value:" + CurrentToken.value);
		}

		GetNextToken();
		
		Target();

		IsDot = Dot();

		if (IsDot)
		{
			break;
		}

		GetNextToken();
	}

	return;
}

void Target() 
{
	bool IsCutUperator = CutOperator();

	if (IsCutUperator)
	{
		GetNextToken();

		return;
	}

	bool IsAtom = Atom();

	if (IsAtom)
	{
		GetNextToken();

		bool IsOpenParenthesis = OpenParenthesis();
		
		if (!IsOpenParenthesis)
		{
			Error("Invalid target's structure token: \nIvalid token type:" + CurrentToken.type + "Value:" + CurrentToken.value);
		}

		Structure();
		
		return;
	}

	bool IsVariable = Variable();
	bool IsNumber = Number();

	if (!IsVariable && !IsNumber)
	{
		Error("Invalid target it is not Cut, Struct or Expression: \nIvalid token type:" + CurrentToken.type + "Value:" + CurrentToken.value);
	}

	Expression();

	// mb get token
}

void Expression()
{
	CalculationArithmeticExpression();
	
	// Сравнение

	// 1 проходит, а во 2 вылетают ошибки
}

void ArithmeticExpression()
{
	bool IsVariable = Variable();

	if (IsVariable)
	{
		GetNextToken();

		bool IsArithmeticOperator = ArithmeticOperator();

		if (!IsArithmeticOperator)
		{
			Error("Invalid token at arithmetic operator\nIvalid token type:" + CurrentToken.type + "Value:" + CurrentToken.value);
		}

		GetNextToken();

		ArithmeticExpression();

		return;
	}

	bool IsNumber = Number();

	if (IsNumber)
	{
		GetNextToken();

		bool IsArithmeticOperator = ArithmeticOperator();

		if (!IsArithmeticOperator)
		{
			return; // Это число
		}

		GetNextToken();

		IsVariable = Variable();

		if (!IsVariable)
		{
			ArithmeticExpression();

			return;
		}

		return;
	}

	Error("Invalid token in arithmetic expression \nIvalid token type:" + CurrentToken.type + "Value:" + CurrentToken.value);
}

void CalculationArithmeticExpression()
{
	bool IsVariable = Variable();
	bool IsNumber = Number();

	if (IsVariable || IsNumber)
	{
		GetNextToken();

		bool IsCalculationOperator = IsOperator();

		if (!IsCalculationOperator)
		{
			return;
		}
	}

	Error("Invalid token in calculation arithmetic expression \nIvalid token type:" + CurrentToken.type + "Value:" + CurrentToken.value);
}

void LogicalExpression()
{

}



bool Dot()
{
	if (CurrentToken.type != "Dot")
	{
		return false;
	}

	return true;
}

bool Comma()
{
	if (CurrentToken.type != "Comma")
	{
		return false;
	}

	return true;
}

bool OpenBracket()
{
	if (CurrentToken.type != "OpenBracket")
	{
		return false;
	}

	return true;
}

bool CloseBracket()
{
	if (CurrentToken.type != "CloseBracket")
	{
		return false;
	}

	return true;
}

bool OpenParenthesis()
{
	if (CurrentToken.type != "OpenParenthesis")
	{
		return false;
	}

	return true;
}

bool CloseParenthesis()
{
	if (CurrentToken.type != "CloseParenthesis")
	{
		return false;
	}

	return true;
}

bool VerticalBar()
{
	if (CurrentToken.type != "VerticalBar")
	{
		return false;
	}

	return true;
}

bool Atom()
{
	if (CurrentToken.type != "Atom")
	{
		return false;
	}

	return true;
}

bool Variable()
{
	if (CurrentToken.type != "Variable")
	{
		return false;
	}

	return true;
}

bool Number()
{
	if (CurrentToken.type != "Number")
	{
		return false;
	}

	return true;
}

bool CutOperator()
{
	if (CurrentToken.type != "CutOperator")
	{
		return false;
	}

	return true;
}

bool ArithmeticOperator()
{
	for (int i = 0; i < sizeof(ArithmeticOperators) / sizeof(ArithmeticOperators[0]); i++)
	{
		if (CurrentToken.type == ArithmeticOperators[i])
		{
			return true;
		}
	}

	return false;
}

bool LogicOperator()
{
	for (int i = 0; i < sizeof(LogicOperators) / sizeof(LogicOperators[0]); i++)
	{
		if (CurrentToken.type == LogicOperators[i])
		{
			return true;
		}
	}

	return false;
}

bool RuleSign()
{
	if (CurrentToken.type != "If")
	{
		return false;
	}

	return true;
}

bool IsOperator()
{
	if (CurrentToken.type != "Is")
	{
		return false;
	}

	return true;
}



void GetNextToken()
{
	PreviousToken = CurrentToken;
	CurrentToken = GetToken(InputString);
	Tokens.push_back(CurrentToken);
}

void Error(std::string message)
{
	std::cout << "ERROR: " << message << '\n';
	quick_exit(101);
}