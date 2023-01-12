#include "SyntaxTreeNodeTypeRef.h"
#include "SyntaxTreeNodeRef.h"

using namespace WestCoastCode;
using namespace WestCoastCode::Compilation;

SyntaxTreeNodeTypeRef::SyntaxTreeNodeTypeRef(SourceCodeView sourceCode)
	: _parent(nullptr), _sourceCode(sourceCode)
{
}

SyntaxTreeNodeTypeRef::~SyntaxTreeNodeTypeRef()
{
	for (auto&& c : _children)
		delete c;
}

void SyntaxTreeNodeTypeRef::ToString(StringStream& s, int indent) const
{
	s << Indent(indent);
	s << "Ref(name=" << _fullName << ", definitions=[";
	for (int i = 0; i < _definitions.size(); ++i) {
		if (i != 0)
			s << ",";
		char tmp[32];
		sprintf(tmp, "%p", _definitions[i]);
		s << tmp;
	}
	s << "])" << std::endl;
	for (auto&& c : _children)
		c->ToString(s, indent + 1);
}

ISyntaxTree* SyntaxTreeNodeTypeRef::GetSyntaxTree() const
{
	return _parent->GetSyntaxTree();
}

ISyntaxTreeNode* SyntaxTreeNodeTypeRef::GetRootNode()
{
	if (_parent)
		return _parent->GetRootNode();
	return this;
}

void SyntaxTreeNodeTypeRef::SetParent(ISyntaxTreeNode* parent)
{
	_parent = parent;
}

bool SyntaxTreeNodeTypeRef::Visit(ISyntaxTreeNodeVisitor<const ISyntaxTreeNode>* visitor) const
{
	return false;
}

bool SyntaxTreeNodeTypeRef::Visit(ISyntaxTreeNodeVisitor<ISyntaxTreeNode>* visitor)
{
	return false;
}

bool SyntaxTreeNodeTypeRef::Query(ISyntaxTreeNodeVisitor<ISyntaxTreeNode>* visitor)
{
	return false;
}

void SyntaxTreeNodeTypeRef::AddNode(ISyntaxTreeNode* node)
{
	_children.push_back(node);
	node->SetParent(this);
}

SyntaxTreeNodeTypeRef* SyntaxTreeNodeTypeRef::Parse(ParserState* state)
{
	Token* const t = state->token;
	if (t->GetType() != TokenType::Identity)
		throw ParseErrorExpectedIdentity(state);

	auto typeref = new SyntaxTreeNodeTypeRef(SourceCodeView(state->sourceCode, t));
	auto mem = MemoryGuard(typeref);
	auto ref = SyntaxTreeNodeRef::Parse(state, DefinitionQueryType::Type, 
		(DefinitionQueryTypes)DefinitionQueryType::All);
	typeref->AddNode(ref);
	typeref->_fullName = ref->GetName();
	return mem.Done();
}
