#include "NewParser.h";
#include "Lexer.h"
#include "AST.h"

Token PreviousToken;
Token CurrentToken;

std::string InputString;

std::vector<Token> Tokens;

std::string Operators[]
{
	"Unification",
	"CutOperator",
};

std::string ArithmeticOperators[]
{
	"Divide",
	"Plus",
	"Minus",
	"Div",
	"Multi"
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
	bool IsRuleBody = false;

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

	IsRuleBody = RuleBody();

	if (!IsRuleBody)
	{
		Error("Parser recevied error token in the rule's body");
	}
}

void Header()
{
	bool IsStructure = false;

	IsStructure = Structure();

	if (IsStructure)
	{
		return;
	}

	Error("Parser recevied error sign in a structure");
}

bool Structure()
{
	bool IsAtom = false;
	bool IsOpenParanthesis = false;
	bool IsCloseParanthesis = false;
	bool IsComma = false;
	bool IsTerm = false;

	IsAtom = Atom();

	if (!IsAtom)
	{
		return false;
	}

	GetNextToken();

	IsOpenParanthesis = OpenParenthesis();

	if (!IsOpenParanthesis)
	{
		return false;
	}

	GetNextToken();

	IsTerm = Term();

	if (!IsTerm)
	{
		return false;
	}

	bool IsTermsEnd = false;

	while (!IsTermsEnd)
	{
		IsCloseParanthesis = CloseParenthesis();

		if (IsCloseParanthesis)
		{
			IsTermsEnd = true;
			break;
		}

		IsComma = Comma();

		if (!IsComma)
		{
			return false;
		}

		GetNextToken();

		IsTerm = Term();

		if (!IsTerm)
		{
			return false;
		}
	}

	return true;
}

bool RuleBody()
{
	bool IsTarget = false;
	bool IsComma = false;
	bool IsDot = false;

	IsTarget = Target();

	if (!IsTarget)
	{
		return false;
	}

	GetNextToken();

	while (!IsDot)
	{
		IsDot = Dot();

		if (IsDot)
		{
			break;
		}

		IsComma = Comma();

		if(!IsComma)
		{
			return false;
		}

		GetNextToken();
		
		IsTarget = Target();

		if (IsTarget)
		{
			GetNextToken();

			continue;
		}

		return false;
	}

	return true;
}

bool Term()
{
	bool IsNumber = false;
	bool IsList = false;
	bool IsAtom = false;
	bool IsStructure = false;
	bool IsOpenBracket = false;

	IsNumber = Number();

	if (IsNumber)
	{
		GetNextToken();

		return true;
	}

	IsOpenBracket = OpenBracket();

	if (IsOpenBracket)
	{
		IsList = List();

		if (!IsList)
		{
			return false;
		}

		return true;
	}

	IsAtom = Atom();

	if (IsAtom)
	{
		GetNextToken();

		bool IsOpenParethesis = OpenParenthesis();

		if (IsOpenParethesis)
		{
			IsStructure = Structure();

			if (!IsStructure)
			{
				return false;
			}

			return true;
		}

		return true; // It's atom
	}

	return false;
}

bool List()
{
	bool IsOpenBracket = false;
	bool IsCloseBracket = false;
	bool IsListHeader = false;
	bool IsVerticalBar = false;
	bool IsTerm = false;

	IsOpenBracket = OpenBracket();

	if (!IsOpenBracket)
	{
		return false;
	}

	IsCloseBracket = CloseBracket();

	// Empty list

	if (IsCloseBracket)
	{
		return true;
	}

	GetNextToken();

	IsListHeader = ListHeader();

	if (!IsListHeader)
	{
		return false;
	}

	GetNextToken();

	IsCloseBracket = CloseBracket();

	// Check simple list

	if (IsCloseBracket)
	{
		return true;
	}

	// Check list's tail

	IsVerticalBar = VerticalBar();

	if (!IsVerticalBar)
	{
		return false;
	}

	GetNextToken();

	IsTerm = Term();

	if (!IsTerm)
	{
		return false;
	}

	GetNextToken();

	IsCloseBracket = CloseBracket();

	if (!IsCloseBracket)
	{
		return false;
	}

	return true;
}

bool ListHeader()
{
	bool IsTerm = true;
	bool IsComma = true;

	IsTerm = Term();

	if (!IsTerm)
	{
		return false;
	}

	while (true)
	{
		IsComma = Comma();
		
		if (!IsComma)
		{
			return false;
		}

		GetNextToken();
		
		IsTerm = Term();

		if (IsTerm)
		{
			GetNextToken();

			continue;
		}
		
		return false;
	}

	return true;
}

bool Expression()
{
	bool IsAtom = false;
	bool IsNumber = false;
	bool IsArithmetic = false;
	

	IsAtom = Atom();

	if (IsAtom)
	{

	}

	IsNumber = Number();

	if (IsNumber)
	{

	}

	return false;
}

bool ArithmeticExpression()
{

}

bool Target()
{
	bool IsAtom = false;
	bool IsStructure = false;
	bool IsExpression = false;

	IsAtom = Atom();

	if (IsAtom)
	{
		IsStructure = Structure();

		if (IsStructure)
		{
			return true;
		}
	}

	
	return true;
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

bool Number()
{
	if (CurrentToken.type != "Number")
	{
		return false;
	}

	return true;
}

bool ArithmeticOperator()
{
	for (int i = 0; i < sizeof(ArithmeticOperators); i++)
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
	for (int i = 0; i < sizeof(LogicOperators); i++)
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