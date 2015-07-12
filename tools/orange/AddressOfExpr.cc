/*
** Copyright 2014-2015 Robert Fratto. See the LICENSE.txt file at the top-level 
** directory of this distribution.
**
** Licensed under the MIT license <http://opensource.org/licenses/MIT>. This file 
** may not be copied, modified, or distributed except according to those terms.
*/ 

#include <orange/AddressOfExpr.h>
#include <orange/generator.h>

std::string AddressOfExpr::string() {
	return "&" + m_expr->string();
}

Value* AddressOfExpr::Codegen() {
	m_value = m_expr->Codegen();
	return m_value;
}

ASTNode* AddressOfExpr::clone() {
	auto clone = new AddressOfExpr((Expression *)m_expr->clone());
	clone->copyProperties(this);
	return clone;
}

void AddressOfExpr::resolve() { 
	ASTNode::resolve();

	if (m_resolved) return; 
	m_resolved = true;

	if (m_expr->returnsPtr() == false) {
		throw CompilerMessage(*m_expr, "can not get the address of this expression"); 
	}

	m_type = m_expr->getType()->getPointerTo();
}

AddressOfExpr::AddressOfExpr(Expression* expr) {
	if (expr == nullptr) {
		throw std::runtime_error("AddressOfExpr ctor: expr can not be nullptr");
	}

	m_expr = expr;

	addChild("m_expr", m_expr);
}
