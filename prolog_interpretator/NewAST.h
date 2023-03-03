#pragma once
#ifndef NEWAST
#define NEWAST

#include <vector>
#include <iostream>
#include <string>

class ASTNode
{
public:
	std::string Name;

	ASTNode() {};
	ASTNode(std::string name)
	{
		Name = name;
	}

	virtual void Print() {};
	virtual void AddChildren(ASTNode) {};
	virtual void AddParent(ASTNode) {};
	virtual ASTNode GetParent() { return ASTNode(); };
};

class Root : public ASTNode
{
public:
	std::vector<ASTNode> Childrens;

	Root(std::string name) : ASTNode(name) {};

	void AddChildren(ASTNode children)
	{
		Childrens.push_back(children);
	}

	void Print()
	{
		std::cout << Name << '\n';
	}
};

class Node : public ASTNode
{
public:
	ASTNode Parent;
	std::vector<ASTNode> Childrens;

	Node(std::string name) : ASTNode(name) {};

	Node(std::string name, ASTNode parent) : ASTNode(name)
	{
		Parent = parent;
	}


	void AddParent(ASTNode parent)
	{
		Parent = parent;
	}

	void AddChildren(ASTNode children)
	{
		Childrens.push_back(children);
	}

	ASTNode GetParent()
	{
		return Parent;
	}

	void Print()
	{
		std::cout << Name << '\n';
	}
};

class Leaf : public ASTNode
{
public:
	std::string Value;
	ASTNode Parent;

	void AddParent(ASTNode parent)
	{
		Parent = parent;
	}

	ASTNode GetParent()
	{
		return Parent;
	}

	void Print()
	{
		std::cout << Value << '\n';
	}
};


class AST
{
	public:
		Root TreeRoot;

		ASTNode CurrentNode;
		ASTNode PreviousNode;

		void AddRoot(Root);
		void AddChildren(ASTNode);
		void AddParent(ASTNode);
		void ReturnToParent();
		void PrintTree();
};

#endif NEWAST