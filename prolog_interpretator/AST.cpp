#include "AST.h"
#include "Tokens.h"

// Определение относительно структуры факта
#define PROGRAMM_SPACE 0
#define PROLOG_SENTENCE_SPACE 1
#define FACT_SPACE 2
#define RULE_SPACE 2
#define HEADER_SPACE 3
#define IF_SPACE 3
#define RULE_BODY_SPACE 3
#define DOT_SPACE 3
#define STRUCT_SPACE 4
#define TARGET_SPACE 4
#define ATOM_SPACE 5
#define TERM_SPACE 5
#define COMMA_SPACE 5
#define OPEN_PARANTHESIS_SPACE 5
#define CLOSE_PARANTHESIS_SPACE 5
#define NUMBER_SPACE 6
#define LIST_SPACE 6



std::vector<Token> ASTtokens;
std::vector<ASTNode> ast;
std::vector<ASTNode> tmp;

Token currentASTToken, previousASTToken;
int globalSpace = 0;
int tokenCounter = 0;
int recursionCounter = 1;
int shift = 0;

//struct ASTNode
//{
//	std::string nodeName = "";
//	std::string tokenValue = "";
//	int spaceCount = 0;
//};

void ASTCreate(std::vector<Token> _tokens)
{
	ASTtokens = _tokens;

	ASTAddNode("Program", "", PROGRAMM_SPACE);

	ASTNextToken();
	while (currentASTToken.type != "EOF")
	{
		ASTAddNode("PROLOG-sentence", "", PROLOG_SENTENCE_SPACE);

		if (currentASTToken.type == "Atom")
		{
			tmp.push_back(ASTNode{ "Header_declare", "", HEADER_SPACE } );
			tmp.push_back(ASTNode{ "Structure_declare", "", STRUCT_SPACE } );
			tmp.push_back(ASTNode{ currentASTToken.type, currentASTToken.value, ATOM_SPACE });
			
			ASTNextToken();
			if (currentASTToken.type == "OpenParenthesis")
			{
				//tmp.push_back(ASTNode{ "", currentASTToken.value, OPEN_PARANTHESIS_SPACE });
				ASTCheckStructure();
				ASTCheckHeader();
			}
		}
	}

	
	ASTOut();
}

void ASTCheckHeader()
{
	if (currentASTToken.type == "Dot")
	{
		//tmp.push_back(ASTNode{ "", currentASTToken.value, DOT_SPACE});
		ASTAddNode("Fact", "", FACT_SPACE);
		TransferTmpToAST();

		ASTNextToken();
	}
	else if (currentASTToken.type == "If")
	{
		ASTCheckRule();
		ASTAddNode("Rule", "", FACT_SPACE);
		TransferTmpToAST();
	}
}

void ASTCheckStructure()
{
	if (recursionCounter == 1)
	{
		ASTNextToken();
		while (currentASTToken.type != "CloseParenthesis")
		{
			if (currentASTToken.type == "Atom")
			{
				ASTNextToken();

				if (currentASTToken.type == "OpenParenthesis")
				{
					tmp.push_back(ASTNode{ "Term", "", TERM_SPACE + shift} );

					++recursionCounter;
					ASTCheckStructure();
					--recursionCounter;
				}
				else
				{
					// ATOM + 1 тк в аргументамх структуры Терм, а за ним атом
					tmp.push_back(ASTNode{ "Term", "", TERM_SPACE + shift });
					tmp.push_back(ASTNode{ previousASTToken.type, previousASTToken.value, ATOM_SPACE + 1 + shift });
				}
				//else if (currentASTToken.type == "Comma")
				//{
				//	//tmp.push_back(ASTNode{ "", currentASTToken.value, COMMA_SPACE * recursionCounter });
				//}
				//else if (currentASTToken.type == "CloseParenthesis")
				//{
				//	//tmp.push_back(ASTNode{ "", currentASTToken.value, CLOSE_PARANTHESIS_SPACE * recursionCounter });
				//}

			}
			else if (currentASTToken.type == "OpenBracket")
			{
				tmp.push_back(ASTNode{ "Term", "", TERM_SPACE + shift });
				tmp.push_back(ASTNode{ "List_declare", "", TERM_SPACE + 1 + shift });

				ASTCheckList();
			}
			else if (currentASTToken.type == "Number")
			{
				tmp.push_back(ASTNode{ "Term", "", TERM_SPACE + shift });
				tmp.push_back(ASTNode{ currentASTToken.type, currentASTToken.value, NUMBER_SPACE + shift });

				ASTNextToken();
			}
			else if (currentASTToken.type == "Comma")
			{
				ASTNextToken();
			}
		}
		ASTNextToken();
	}
	else
	{
		// show recursion structure declare
		tmp.push_back(ASTNode{ "Struct_declare", "", (STRUCT_SPACE + recursionCounter + shift)});
		tmp.push_back(ASTNode{ previousASTToken.type, previousASTToken.value, (ATOM_SPACE + recursionCounter + shift) }); //7

		ASTNextToken();
		while (currentASTToken.type != "CloseParenthesis")
		{
			if (currentASTToken.type == "Atom")
			{
				ASTNextToken();
				if (currentASTToken.type == "OpenParenthesis")
				{
					tmp.push_back(ASTNode{ "Term", "", TERM_SPACE + recursionCounter + shift });

					++recursionCounter;
					++shift;
					ASTCheckStructure();
					--shift;
					--recursionCounter;
				}
				else
				{
					// ATOM + 1 тк в аргументамх структуры Терм, а за ним атом
					tmp.push_back(ASTNode{ "Term", "", TERM_SPACE + recursionCounter + shift });
					tmp.push_back(ASTNode{ previousASTToken.type, previousASTToken.value, ATOM_SPACE + recursionCounter + shift + 1 });
				}
				//else if (currentASTToken.type == "Comma")
				//{
				//	//tmp.push_back(ASTNode{ "", currentASTToken.value, COMMA_SPACE * recursionCounter });
				//}
				//else if (currentASTToken.type == "CloseParenthesis")
				//{
				//	//tmp.push_back(ASTNode{ "", currentASTToken.value, CLOSE_PARANTHESIS_SPACE * recursionCounter });
				//}

			}
			else if (currentASTToken.type == "Number")
			{
				tmp.push_back(ASTNode{ "Term", "", TERM_SPACE + recursionCounter + shift });
				tmp.push_back(ASTNode{ currentASTToken.type, currentASTToken.value, NUMBER_SPACE + recursionCounter + shift });

				ASTNextToken();
			}
			else if (currentASTToken.type == "OpenBracket")
			{
				/*tmp.push_back(ASTNode{ "Term", "", TERM_SPACE + recursionCounter + shift });
				tmp.push_back(ASTNode{ "List_declare", "", TERM_SPACE + 1 + recursionCounter + shift });*/

				++recursionCounter;
				shift -= 1;
				ASTCheckList();
				shift += 1;
				--recursionCounter;
			}
			else if (currentASTToken.type == "Comma")
			{
				ASTNextToken();
			}
		}
		ASTNextToken();
	}
}

void ASTCheckList()
{
 	if (recursionCounter == 1)
	{
		ASTNextToken();
		while (currentASTToken.type != "CloseBracket")
		{
			if (currentASTToken.type == "Atom")
			{
				ASTNextToken();
				if (currentASTToken.type == "OpenParenthesis")
				{
					tmp.push_back(ASTNode{ "Term", "", LIST_SPACE + 1 + shift });

					++recursionCounter;
					shift += 2;
					ASTCheckStructure();
					shift -= 2;
					--recursionCounter;
				}
				else if (currentASTToken.type == "Comma" || currentASTToken.type == "CloseBracket" || currentASTToken.type == "VerticalBar")
				{
					tmp.push_back(ASTNode{ "Term", "", LIST_SPACE + 1 + shift });
					tmp.push_back(ASTNode{ previousASTToken.type, previousASTToken.value, LIST_SPACE + 2 + shift });
				}
			}
			else if (currentASTToken.type == "Number")
			{
				tmp.push_back(ASTNode{ "Term", "", LIST_SPACE + 1 + shift });
				tmp.push_back(ASTNode{ currentASTToken.type, currentASTToken.value, LIST_SPACE + 2 + shift });
				ASTNextToken();
			}
			else if (currentASTToken.type == "OpenBracket")
			{
				++recursionCounter;
				ASTCheckList();
				--recursionCounter;
				//NextToken(str);
			}
			else if (currentASTToken.type == "Comma")
			{
				ASTNextToken();
			}
			else if (currentASTToken.type == "VerticalBar")
			{
				tmp.push_back(ASTNode{ "List_continue_declare", "",  LIST_SPACE + 1 + shift });
				ASTNextToken();
				if (currentASTToken.type == "Atom")
				{
					ASTNextToken();
					if (currentASTToken.type == "OpenParenthesis")
					{
						tmp.push_back(ASTNode{ "Term", "", LIST_SPACE + 2 + shift });

						++recursionCounter;
						shift += 3;
						ASTCheckStructure();
						shift -= 3;
						--recursionCounter;
						// При возврате рекурсии необходимо перейти к следующему токену, ибо текущий будет равен закрывающей скобке, что приведет к выходу из всех рекурсий (если они имеются).
						//NextToken(str);
					}
					else if (currentASTToken.type == "CloseBracket")
					{
						tmp.push_back(ASTNode{ "Term", "", LIST_SPACE +2 + shift });
						tmp.push_back(ASTNode{ previousASTToken.type, previousASTToken.value, LIST_SPACE + 3 + shift });
					}
				}
				else if (currentASTToken.type == "Number")
				{
					tmp.push_back(ASTNode{ "Term", "", LIST_SPACE + 2 + shift });
					tmp.push_back(ASTNode{ currentASTToken.type, currentASTToken.value, LIST_SPACE + 3 + shift });
					ASTNextToken();
				}
				else if (currentASTToken.type == "OpenBracket")
				{
					++recursionCounter;
					shift += 1;
					ASTCheckList();
					shift -= 1;
					--recursionCounter;
				}
			}
		}
	}
	else
	{
		tmp.push_back(ASTNode{ "Term", "", (TERM_SPACE + recursionCounter + shift) });
		tmp.push_back(ASTNode{ "List_declare", "", (LIST_SPACE + recursionCounter + shift) });
		//tmp.push_back(ASTNode{ previousASTToken.type, previousASTToken.value, (LIST_SPACE + 1 + recursionCounter + shift) }); //list_space потмоу что элемент за объявлением листа

		ASTNextToken();
		while (currentASTToken.type != "CloseBracket")
		{
			if (currentASTToken.type == "Atom")
			{
				ASTNextToken();
				if (currentASTToken.type == "OpenParenthesis")
				{
					tmp.push_back(ASTNode{ "Term", "", LIST_SPACE + 1 });

					++recursionCounter;
					ASTCheckStructure();
					--recursionCounter;
				}
				else if (currentASTToken.type == "Comma" || currentASTToken.type == "CloseBracket" || currentASTToken.type == "VerticalBar")
				{
					tmp.push_back(ASTNode{ "Term", "", LIST_SPACE + 1 + recursionCounter + shift });
					tmp.push_back(ASTNode{ previousASTToken.type, previousASTToken.value, LIST_SPACE + 2 + recursionCounter + shift });
				}
			}
			else if (currentASTToken.type == "Number")
			{
				tmp.push_back(ASTNode{ "Term", "", LIST_SPACE + 1 + recursionCounter + shift });
				tmp.push_back(ASTNode{ currentASTToken.type, currentASTToken.value, LIST_SPACE + 2 + recursionCounter + shift });
				ASTNextToken();
			}
			else if (currentASTToken.type == "OpenBracket")
			{
				++recursionCounter;
				shift += 1;
				ASTCheckList();
				shift -= 1;
				--recursionCounter;
				//NextToken(str);
			}
			else if (currentASTToken.type == "Comma")
			{
				ASTNextToken();
			}
			else if (currentASTToken.type == "VerticalBar")
			{
				tmp.push_back(ASTNode{ "List_continue_declare", "", LIST_SPACE + 1 + recursionCounter + shift });
				ASTNextToken();
				if (currentASTToken.type == "Atom")
				{
					ASTNextToken();
					if (currentASTToken.type == "OpenParenthesis")
					{
						tmp.push_back(ASTNode{ "Term", "", LIST_SPACE + 2 + recursionCounter + shift });

						++recursionCounter;
						shift += 3;
						ASTCheckStructure();
						shift -= 3;
						--recursionCounter;
						// При возврате рекурсии необходимо перейти к следующему токену, ибо текущий будет равен закрывающей скобке, что приведет к выходу из всех рекурсий (если они имеются).
						//NextToken(str);
					}
					else if (currentASTToken.type == "CloseBracket")
					{
						tmp.push_back(ASTNode{ "Term", "", LIST_SPACE + 2 + recursionCounter + shift });
						tmp.push_back(ASTNode{ previousASTToken.type, previousASTToken.value, LIST_SPACE + 3 + recursionCounter + shift });
						continue;
					}
				}
				else if (currentASTToken.type == "Number")
				{
					tmp.push_back(ASTNode{ "Term", "", LIST_SPACE + 2 + recursionCounter + shift });
					tmp.push_back(ASTNode{ currentASTToken.type, currentASTToken.value, LIST_SPACE + 3 + recursionCounter + shift });
					ASTNextToken();
				}
				else if (currentASTToken.type == "OpenBracket")
				{
					++recursionCounter;
					shift += 2;
					ASTCheckList();
					shift -= 2;
					--recursionCounter;
				}
			}
		}
	}
	ASTNextToken();
}

void ASTCheckRule()
{
	if (currentASTToken.type == "If")
	{
		tmp.push_back(ASTNode{ "Body", "", RULE_BODY_SPACE });
		ASTCheckBody();
	}
}

void ASTCheckBody()
{
	std::vector<ASTNode> structure;
	while (currentASTToken.type != "Dot")
	{
		// for every term Atom, Number, Structure, List
		ASTNextToken();
		if (currentASTToken.type == "Atom")
		{
			// Target = structure | exp
			tmp.push_back(ASTNode{ "Target", "", TARGET_SPACE });
			ASTNextToken();
			if (currentASTToken.type == "OpenParenthesis") 
			{
 				ASTNode header = { previousASTToken.type, previousASTToken.value, TARGET_SPACE + 4 };
				tmp.push_back(ASTNode{ "Expression_declare", "", TARGET_SPACE + 1 });
				tmp.push_back(ASTNode{ "Term", "", TARGET_SPACE + 2});
				tmp.push_back(ASTNode{ "Structure_declare", "", TARGET_SPACE + 3});

				tmp.push_back(header);
				int tmpSize = tmp.size();

				shift += 3;
				ASTCheckStructure();
				//

				if (currentASTToken.type == "Unification" || currentASTToken.type == "Is" || currentASTToken.type == "Less" || currentASTToken.type == "LessOrEqual"
					|| currentASTToken.type == "Greater" || currentASTToken.type == "GreaterOrEqual" || currentASTToken.type == "CutOperator" || currentASTToken.type == "Equal" || currentASTToken.type == "NotEqual")
				{
					shift -= 3;
					
					tmp.push_back(ASTNode{ "", currentASTToken.value, TARGET_SPACE + 2 });
					
					//mb shift
					//shift += 3;
					ASTCheckExpression();
					//shift -= 3;
				}
				else if (currentASTToken.type == "Comma" || currentASTToken.type == "Dot")
				{
					//Данные манипуляции используются чтобы удалить объявление выражения и заменить его объявлением структуры (ибо сам факт оюъявление структуры определяется позже)
					header.spaceCount -= 2;
					structure.push_back(header);
					for (int i = tmp.size() - (tmp.size() - tmpSize), j = 1; i < tmp.size(); i++, j++)
					{
						structure.push_back(tmp[i]);
						//изменение отступов
						structure[j].spaceCount -= shift - 1;
					}
					shift -= 3;
					tmp.erase(tmp.begin() + tmpSize - 4, tmp.end());
					


					tmp.push_back(ASTNode{ "Structure_declare", "", TARGET_SPACE + 1 });

					for (int i = 0; i < structure.size(); i++)
					{
						tmp.push_back(structure[i]);
					}
					structure.clear();
					
					continue;
				}
			}
			else if (currentASTToken.type == "Unification" || currentASTToken.type == "Is" || currentASTToken.type == "Less" || currentASTToken.type == "LessOrEqual"
				|| currentASTToken.type == "Greater" || currentASTToken.type == "GreaterOrEqual" || currentASTToken.type == "CutOperator" || currentASTToken.type == "Equal" || currentASTToken.type == "NotEqual")
			{
				tmp.push_back(ASTNode{ "Expression_declare", "", TARGET_SPACE + 1 });
				tmp.push_back(ASTNode{ "Term", "", TARGET_SPACE + 2});
				tmp.push_back(ASTNode{ previousASTToken.type, previousASTToken.value, TARGET_SPACE + 3 });
				tmp.push_back(ASTNode{ "", currentASTToken.value, TARGET_SPACE + 2 });

				ASTCheckExpression();
			}
		}
		else if (currentASTToken.type == "Number")
		{
			ASTNextToken();
			tmp.push_back(ASTNode{ "Target", "", TARGET_SPACE });
			if (currentASTToken.type == "Unification" || currentASTToken.type == "Is" || currentASTToken.type == "Less" || currentASTToken.type == "LessOrEqual"
				|| currentASTToken.type == "Greater" || currentASTToken.type == "GreaterOrEqual" || currentASTToken.type == "CutOperator" || currentASTToken.type == "Equal" || currentASTToken.type == "NotEqual")
			{
				tmp.push_back(ASTNode{ "Expression_declare", "", TARGET_SPACE + 1 });
				tmp.push_back(ASTNode{ "Term", "", TARGET_SPACE + 2 });
				tmp.push_back(ASTNode{ previousASTToken.type, previousASTToken.value, TARGET_SPACE + 3 });
				tmp.push_back(ASTNode{ "", currentASTToken.value, TARGET_SPACE + 2 });
				ASTCheckExpression();
			}
		}
		else if (currentASTToken.type == "OpenBracket")
		{
			tmp.push_back(ASTNode{ "Target", "", TARGET_SPACE });
			tmp.push_back(ASTNode{ "Expression_declare", "", TARGET_SPACE + 1 });
			tmp.push_back(ASTNode{ "Term", "", TARGET_SPACE + 2});
			tmp.push_back(ASTNode{ "List_declare", "", TARGET_SPACE + 3 });

			shift += 1;
			ASTCheckList();
			shift -= 1;
			// Target = expression
			if (currentASTToken.type == "Unification" || currentASTToken.type == "Is" || currentASTToken.type == "Less" || currentASTToken.type == "LessOrEqual"
				|| currentASTToken.type == "Greater" || currentASTToken.type == "GreaterOrEqual" || currentASTToken.type == "CutOperator" || currentASTToken.type == "Equal" || currentASTToken.type == "NotEqual")
			{
				tmp.push_back(ASTNode{ "", currentASTToken.value, TARGET_SPACE + 3 });
				
				shift += 1;
				ASTCheckExpression();
				shift -= 1;
			}
		}
		else if (currentASTToken.type == "Comma")
		{
			ASTNextToken();
		}
		//Случай когда в теле один оператор отсечения
		else if (currentASTToken.type == "CutOperator")
		{
			tmp.push_back(ASTNode{ "", currentASTToken.value, TARGET_SPACE + 2 });
			ASTNextToken();
		}
	}
	ASTNextToken();
}

void ASTCheckExpression()
{
	ASTNextToken();
	if (currentASTToken.type == "Atom")
	{
		ASTNextToken();
		if (currentASTToken.type == "Plus" || currentASTToken.type == "Minus" || currentASTToken.type == "Modulo" || currentASTToken.type == "Multi"
			|| currentASTToken.type == "Div" || currentASTToken.type == "Divide")
		{
			tmp.push_back(ASTNode{ "Term", "", TARGET_SPACE + 3 + shift});
			tmp.push_back(ASTNode{ previousASTToken.type, previousASTToken.value, TARGET_SPACE + 4 + shift });
			tmp.push_back(ASTNode{ "", currentASTToken.value, TARGET_SPACE + 2 + shift });

			//shift += 1;
			ASTCheckExpression();
			//shift -= 1;
		}
		else if (currentASTToken.type == "OpenParenthesis")
		{
			tmp.push_back(ASTNode{ "Term", "", TARGET_SPACE + 3 + shift });
			tmp.push_back(ASTNode{ "Structure_declare", "", TARGET_SPACE + 4 + shift });
			tmp.push_back(ASTNode{ "Term", "", TARGET_SPACE + 5 + shift });
			tmp.push_back(ASTNode{ previousASTToken.type, previousASTToken.value, TARGET_SPACE + 6 + shift });
			shift += 4;
			ASTCheckStructure();
			shift -= 4;
			if (currentASTToken.type == "Plus" || currentASTToken.type == "Minus" || currentASTToken.type == "Modulo" || currentASTToken.type == "Multi"
				|| currentASTToken.type == "Div" || currentASTToken.type == "Divide")
			{
				tmp.push_back(ASTNode{ "", currentASTToken.value, TARGET_SPACE + 2 + shift });
				ASTCheckExpression();
			}
		}
		else if (currentASTToken.type == "Comma" || currentASTToken.type == "Dot")
		{
			tmp.push_back(ASTNode{ previousASTToken.type, previousASTToken.value, TARGET_SPACE + 3 + shift });
			return;
		}
	}
	else if (currentASTToken.type == "Number")
	{

		ASTNextToken();
		if (currentASTToken.type == "Plus" || currentASTToken.type == "Minus" || currentASTToken.type == "Modulo" || currentASTToken.type == "Multi"
			|| currentASTToken.type == "Div" || currentASTToken.type == "Divide")
		{

			tmp.push_back(ASTNode{ "Term", "", TARGET_SPACE + 3 + shift });
			tmp.push_back(ASTNode{ previousASTToken.type, previousASTToken.value, TARGET_SPACE + 4 + shift });
			tmp.push_back(ASTNode{ "", currentASTToken.value, TARGET_SPACE + 2 + shift });
			

			//shift += 3;
			ASTCheckExpression();
			//shift -= 3;
		}
		else if (currentASTToken.type == "Comma" || currentASTToken.type == "Dot")
		{
			tmp.push_back(ASTNode{ "Term", "", TARGET_SPACE + 3 + shift });
			tmp.push_back(ASTNode{ previousASTToken.type, previousASTToken.value, TARGET_SPACE + 4 + shift });
			return;
		}
	}
	else if (currentASTToken.type == "OpenBracket")
	{
		//tmp.push_back(ASTNode{ "", previousASTToken.value, TARGET_SPACE + 2 + shift });
		tmp.push_back(ASTNode{ "Term", "", TARGET_SPACE + 3 + shift });
		tmp.push_back(ASTNode{ "List_declare", "", TARGET_SPACE + 4 + shift });
		
		shift += 2;
		ASTCheckList();
		shift -= 2;
		if (currentASTToken.type == "Plus" || currentASTToken.type == "Minus" || currentASTToken.type == "Modulo" || currentASTToken.type == "Multi"
			|| currentASTToken.type == "Div" || currentASTToken.type == "Divide")
		{
			tmp.push_back(ASTNode{ "", currentASTToken.value, TARGET_SPACE + 2 + shift });

			ASTCheckExpression();
		}
		else if (currentASTToken.type == "Comma" || currentASTToken.type == "Dot")
		{
			return;
		}
	}
}

void ASTOut()
{
	for (int i = 0; i < ast.size(); i++)
	{
		if (ast[i].nodeName != "" && ast[i].tokenValue != "")
		{
			std::cout << std::setw(ast[i].spaceCount + ast[i].nodeName.length()) << ast[i].nodeName << '\n'
				<< std::setw(ast[i].spaceCount + ast[i].tokenValue.length() + 1) << ast[i].tokenValue << '\n';
		}
		else if (ast[i].nodeName == "" && ast[i].tokenValue != "")
		{
			std::cout << std::setw(ast[i].spaceCount + ast[i].tokenValue.length()) << ast[i].tokenValue << '\n';
		}
		else
		{
			std::cout << std::setw(ast[i].spaceCount + ast[i].nodeName.length()) << ast[i].nodeName << '\n';
		}
	}
}

void ASTAddNode(std::string nodeName, std::string tokenValue, int spaceCount)
{
	ast.push_back(ASTNode{ nodeName, tokenValue, spaceCount });
}

void ASTNextToken()
{
	previousASTToken = currentASTToken;
	currentASTToken = ASTtokens[tokenCounter];
	if (tokenCounter <= ASTtokens.size())
	{
		tokenCounter++;
	}
}

void TransferTmpToAST()
{
	for (int i = 0; i < tmp.size(); i++)
	{
		ast.push_back(tmp[i]);
	}
	tmp.clear();
}
