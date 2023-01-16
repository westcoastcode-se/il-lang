#include "SyntaxTreeNode.h"

namespace WestCoastCode::Compilation
{
	class SyntaxTreeNodeConstant : public ISyntaxTreeNodeConstant
	{
	public:
		SyntaxTreeNodeConstant(ISyntaxTreeNodeFuncDef* func, SourceCodeView sourceCode, 
			const PrimitiveValue& value, ISyntaxTreeNodePrimitive* stackType)
			: _function(func), _parent(nullptr), _sourceCode(sourceCode), _value(value), _stackType(stackType) {
		}

		// Inherited via ISyntaxTreeNode
		const ID& GetID() const final { return _id; }
		virtual void ToString(StringStream& s, int indent) const override;
		virtual ISyntaxTree* GetSyntaxTree() const override;
		virtual ISyntaxTreeNode* GetRootNode() override;
		ISyntaxTreeNode* GetParent() const final { return _parent; }
		virtual void SetParent(ISyntaxTreeNode* parent) override;
		ReadOnlyArray<ISyntaxTreeNode*> GetChildren() const final {
			return ReadOnlyArray<ISyntaxTreeNode*>();
		}
		const SourceCodeView* GetSourceCode() const final { return &_sourceCode; }
		const PrimitiveValue& GetValue() const final { return _value; }
		void Compile(Builder::Linker* linker, Builder::Instructions& instructions) final;
		ISyntaxTreeNodeFuncDef* GetFunction() final { return _function; }
		ISyntaxTreeNodePackage* GetPackage() final { return _function->GetPackage(); }
		ISyntaxTreeNodeType* GetStackType() final { return _stackType; }
		Vector<ISyntaxTreeNodeOp*> OptimizeOp(ISyntaxTreeNodeOptimizer* optimizer) final { return Vector<ISyntaxTreeNodeOp*>(); }
	
	public:
		// Try to cast this constant into a new constant
		SyntaxTreeNodeConstant* Cast(ISyntaxTreeNodeType* newType);

		// Parse the supplied state and return a constant
		static SyntaxTreeNodeConstant* Parse(const ParserState* state);

	private:
		const ID _id;
		ISyntaxTreeNodeFuncDef* const _function;
		ISyntaxTreeNode* _parent;
		const SourceCodeView _sourceCode;
		const PrimitiveValue _value;
		ISyntaxTreeNodePrimitive* const _stackType;
	};
}
