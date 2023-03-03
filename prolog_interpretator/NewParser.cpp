#include "NewParser.h";
#include "Lexer.h"
#include "NewAST.h"

Token PreviousToken;
Token CurrentToken;

AST Tree;

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
	Tree = AST();

	InputString = str;
	GetNextToken();

	Tree.AddRoot(new Root("Programm"));

	Program();

	Tree.PrintTree();
}

void Program()
{
	bool IsProgramEnd = false;
	bool IsSentence = false;

	while (!IsProgramEnd)
	{
		Tree.AddChildren(new Node("PrologSentence", Tree.CurrentNode));

		Sentence();

		Tree.ReturnToParent();

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

	Tree.AddChildren(new Node("Header", Tree.CurrentNode));

	Header();
	
	Tree.ReturnToParent();

	GetNextToken();

	IsFactSign = Dot();

	if (IsFactSign)
	{
		Tree.AddChildren(new Leaf(CurrentToken.type, CurrentToken.value, Tree.CurrentNode));
		Tree.ReturnToParent();

		return;
	}

	IsRuleSign = RuleSign();

	if (!IsRuleSign)
	{
		Error("Parser recevied not fact's and not rule's sign");
	}

	Tree.AddChildren(new Leaf(CurrentToken.type, CurrentToken.value, Tree.CurrentNode));
	Tree.ReturnToParent();

	GetNextToken();

	Tree.AddChildren(new Node("RuleBody", Tree.CurrentNode));

	RuleBody();

	Tree.ReturnToParent();
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
		Tree.AddChildren(new Leaf(CurrentToken.type, CurrentToken.value, Tree.CurrentNode));
		Tree.ReturnToParent();

		GetNextToken();

		return;
	}

	IsVariable = Variable();

	if (IsVariable)
	{
		Tree.AddChildren(new Leaf(CurrentToken.type, CurrentToken.value, Tree.CurrentNode));
		Tree.ReturnToParent();

		GetNextToken();

		return;
	}

	IsAtom = Atom();

	if (IsAtom)
	{
		Token atomToken = CurrentToken;

		GetNextToken();

		bool IsOpenParenthesis = OpenParenthesis();

		if (!IsOpenParenthesis)
		{
			Tree.AddChildren(new Leaf(atomToken.type, atomToken.value, Tree.CurrentNode));
			Tree.ReturnToParent();

			return; // It's Atom
		}

		Tree.AddChildren(new Node("Structure", Tree.CurrentNode));
		Tree.AddChildren(new Leaf(atomToken.type, atomToken.value, Tree.CurrentNode));
		Tree.ReturnToParent();
		Tree.AddChildren(new Leaf(CurrentToken.type, CurrentToken.value, Tree.CurrentNode));

		bool callFromHeader = false; // It's structure

		Structure(false);

		Tree.ReturnToParent();

		GetNextToken();

		return;
	}

	IsOpenBracket = OpenBracket();

	if (IsOpenBracket)
	{
		Tree.AddChildren(new Node("List", Tree.CurrentNode));
		Tree.AddChildren(new Leaf(CurrentToken.type, CurrentToken.value, Tree.CurrentNode));
		Tree.ReturnToParent();

		List();

		Tree.ReturnToParent();

		GetNextToken();

		return;
	}

	Error("Invalid Term token\nToken type: " + CurrentToken.type + " " + CurrentToken.value);
}

void Header()
{
	bool callFromHeader = true;

	Tree.AddChildren(new Node("Structure", Tree.CurrentNode));

	Structure(callFromHeader);
}

void Structure(bool callFromHeader)
{
	bool IsOpenParanthesis = false;
	bool IsCloseParanthesis = false;
	bool IsComma = false;
	bool IsTerm = false;

	if (callFromHeader)
	{
		bool IsAtom = Atom();

		if (!IsAtom)
		{
			Error("Invalid atom token in the structure: \nIvalid token type:" + CurrentToken.type + "Value:" + CurrentToken.value);
		}

		Tree.AddChildren(new Leaf(CurrentToken.type, CurrentToken.value, Tree.CurrentNode));

		GetNextToken();

		IsOpenParanthesis = OpenParenthesis();

		if (!IsOpenParanthesis)
		{
			Error("Invalid parenthesis token in the structure: \nIvalid token type:" + CurrentToken.type + "Value:" + CurrentToken.value);
		}

		Tree.ReturnToParent();

		Tree.AddChildren(new Leaf(CurrentToken.type, CurrentToken.value, Tree.CurrentNode));

		Tree.ReturnToParent();
	}

	GetNextToken();

	Tree.AddChildren(new Node("Term", Tree.CurrentNode));

	Term();

	Tree.ReturnToParent();

	bool IsTermsEnd = false;

	while (!IsTermsEnd)
	{
		IsCloseParanthesis = CloseParenthesis();

		if (IsCloseParanthesis)
		{
			Tree.AddChildren(new Leaf(CurrentToken.type, CurrentToken.value, Tree.CurrentNode));
			Tree.ReturnToParent();

			break;
		}

		IsComma = Comma();

		if (!IsComma)
		{
			Error("Invalid comma token: \nIvalid token type:" + CurrentToken.type + "Value:" + CurrentToken.value);
		}

		Tree.AddChildren(new Leaf(CurrentToken.type, CurrentToken.value, Tree.CurrentNode));
		Tree.ReturnToParent();

		GetNextToken();

		Tree.AddChildren(new Node("Term", Tree.CurrentNode));

		Term();

		Tree.ReturnToParent();
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
	bool IsComma = false;

	Term();

	while (!IsComma)
	{
		IsComma = Comma();
		
		if (!IsComma)
		{
			break;
		}

		GetNextToken();
		
		Term();

		IsComma = Comma();
	}

	return;
}

void RuleBody()
{
	bool IsComma = false;
	bool IsDot = false;

	Target();

	IsComma = Comma();
	IsDot = Dot();

	if (!IsComma && !IsDot)
	{
		GetNextToken();
	}


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
		IsComma = Comma();

		if (IsDot)
		{
			break;
		}

		if (!IsComma)
		{
			GetNextToken();
		}
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

		bool callFromHeader = false;

 		Structure(callFromHeader);
		
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
	bool isOperator = false;

	CalculationArithmeticExpression(isOperator);
	
	if (!isOperator)
	{
		LogicExpression(isOperator);

		return;
	}

	bool IsDot = Dot();
	bool IsComma = Comma();

	if (!IsDot && !IsComma)
	{
		LogicExpression(!isOperator);
	}

	return;
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
			return; // ��� �����
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

void CalculationArithmeticExpression(bool& isOperator)
{
	bool IsVariable = Variable();
	bool IsNumber = Number();

	if (IsVariable || IsNumber)
	{
		GetNextToken();

		bool IsCalculationOperator = IsOperator();

		if (!IsCalculationOperator)
		{
			isOperator = false;
			return;
		}

		GetNextToken();
		
		ArithmeticExpression();

		bool IsDot = Dot();
		bool IsComma = Comma();

		isOperator = true;

		if (!IsDot && !IsComma)
		{
			GetNextToken();
		}

		return;
	}

	Error("Invalid token in calculation arithmetic expression \nIvalid token type:" + CurrentToken.type + "Value:" + CurrentToken.value);
}

void LogicExpression(bool isOperator)
{
	if (!isOperator) // � ���������� �.�. ��� is
	{
		if (PreviousToken.type == "Variable" || PreviousToken.type == "Number")
		{
			bool IsArithemeticOperator = ArithmeticOperator();

			if (IsArithemeticOperator)
			{
				GetNextToken();

				bool IsNumber = Number();
				bool IsVariable = Variable();

				if (!IsNumber && !IsVariable)
				{
					Error("Invalid token in logic expression: \nIvalid token type:" + CurrentToken.type + "Value:" + CurrentToken.value);
				}

				ArithmeticExpression();
			}
		}
	}
	else
	{
		ArithmeticExpression();
		GetNextToken();
	}

	bool IsLogicOperator = LogicOperator();

	if (!IsLogicOperator)
	{
		Error("Invalid token in logic operator \nIvalid token type:" + CurrentToken.type + "Value:" + CurrentToken.value);
	}

	GetNextToken();

	ArithmeticExpression();
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