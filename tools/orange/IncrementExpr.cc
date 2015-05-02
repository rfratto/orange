/*
** Copyright 2014-2015 Robert Fratto. See the LICENSE.txt file at the top-level 
** directory of this distribution.
**
** Licensed under the MIT license <http://opensource.org/licenses/MIT>. This file 
** may not be copied, modified, or distributed except according to those terms.
*/ 

#include <orange/IncrementExpr.h>
#include <orange/generator.h>

Value* IncrementExpr::Codegen() {
	Value* origValue = m_expr->Codegen();
	Value* loadedVal = GE::builder()->CreateLoad(origValue);

	Value* newValue = nullptr;

	bool isFP = loadedVal->getType()->isFloatingPointTy();

	uint64_t delta = (m_op == "++") ? 1 : -1;  

	if (isFP) {
		newValue = GE::builder()->CreateFAdd(loadedVal, ConstantFP::get(loadedVal->getType(), delta));
	} else {
		newValue = GE::builder()->CreateAdd(loadedVal, ConstantInt::get(loadedVal->getType(), delta, true));	
	}

	GE::builder()->CreateStore(newValue, origValue);

	if (m_preincrement) {
		return GE::builder()->CreateLoad(origValue);
	} else {
		return loadedVal;
	}
}

ASTNode* IncrementExpr::clone() {
	return new IncrementExpr((Expression *)m_expr->clone(), m_op, m_preincrement);
}

AnyType* IncrementExpr::getType() {
	return m_expr->getType();
}

void IncrementExpr::resolve() {
	if (m_resolved) return;
	m_resolved = true;

	m_expr->resolve();

	if (m_expr->returnsPtr() == false) {
		throw CompilerMessage(*m_expr, "expression must be a variable or a memory location!");
	}
}


bool IncrementExpr::isSigned() {
	return m_expr->isSigned();
}

IncrementExpr::IncrementExpr(Expression* expression, StrElement op, bool preincrement) : m_op(op) {
	if (expression == nullptr) {
		throw std::runtime_error("IncrementExpr requires expression to not be null!");
	}

	m_expr = expression; 
	m_preincrement = preincrement; 
}