#include "NewAST.h";

void AST::AddRoot(Root root)
{
	TreeRoot = root;
	CurrentNode = TreeRoot;
	PreviousNode = TreeRoot;
}

void AST::ReturnToParent()
{
	ASTNode Parent = CurrentNode.GetParent();
	PreviousNode = CurrentNode;
	CurrentNode = Parent;
}

void AST::AddChildren(ASTNode node)
{
	CurrentNode.AddChildren(node);
	PreviousNode = CurrentNode;
	CurrentNode = node;
};

void AST::AddParent(ASTNode node)
{
	CurrentNode.AddParent(node);
}

void AST::PrintTree()
{

}