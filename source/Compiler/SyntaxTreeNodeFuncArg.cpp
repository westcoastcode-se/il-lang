#include "SyntaxTreeNodeFuncArg.h"
#include "SyntaxTreeNodeTypeRef.h"

using namespace WestCoastCode;
using namespace WestCoastCode::Compilation;

SyntaxTreeNodeFuncArg::SyntaxTreeNodeFuncArg(SourceCodeView sourceCode, ReadOnlyString name)
	: _parent(nullptr), _sourceCode(sourceCode), _name(name), _variableType(nullptr)
{
}

SyntaxTreeNodeFuncArg::~SyntaxTreeNodeFuncArg()
{
	for (auto&& c : _children)
		delete c;
}

void SyntaxTreeNodeFuncArg::ToString(StringStream& s, int indent) const
{
	s << Indent(indent);
	s << "FuncArg(name=" << _name << ", type=";
	if (_variableType) {
		char tmp[32];
		sprintf(tmp, "%p", _variableType);
		s << tmp;
	}
	s << ")" << std::endl;
	for (auto&& c : _children)
		c->ToString(s, indent + 1);
}

ISyntaxTree* SyntaxTreeNodeFuncArg::GetSyntaxTree() const
{
	return _parent->GetSyntaxTree();
}

void SyntaxTreeNodeFuncArg::SetParent(ISyntaxTreeNode* parent)
{
	_parent = parent;
}

bool SyntaxTreeNodeFuncArg::Visit(ISyntaxTreeNodeVisitor<const ISyntaxTreeNode>* visitor) const
{
	return false;
}

bool SyntaxTreeNodeFuncArg::Visit(ISyntaxTreeNodeVisitor<ISyntaxTreeNode>* visitor)
{
	return false;
}

bool SyntaxTreeNodeFuncArg::Query(ISyntaxTreeNodeVisitor<ISyntaxTreeNode>* visitor)
{
	return false;
}

void SyntaxTreeNodeFuncArg::SetVariableType(ISyntaxTreeNodeTypeRef* type)
{
	assert(_variableType == nullptr &&
		"Expected a type to not bet set twice");
	AddNode(type);
	_variableType = type;
}

void SyntaxTreeNodeFuncArg::AddNode(ISyntaxTreeNode* node)
{
	_children.push_back(node);
	node->SetParent(this);
}

SyntaxTreeNodeFuncArg* SyntaxTreeNodeFuncArg::Parse(ParserState* state)
{
	Token* const t = state->token;
	if (t->GetType() != TokenType::Identity)
		throw ParseErrorExpectedIdentity(state);

	auto arg = new SyntaxTreeNodeFuncArg(SourceCodeView(state->sourceCode, t), t->GetString());
	auto guard = MemoryGuard(arg);
	t->Next();	
	arg->SetVariableType(SyntaxTreeNodeTypeRef::Parse(state));
	return guard.Done();
}
