#ifndef __INCREMENT_EXPR_H__
#define __INCREMENT_EXPR_H__
#include "AST.h"

class IncrementExpr : public Expression {
public:
	virtual std::string getClass() { return "IncrementExpr"; }

	virtual bool isConstant() { return false; }

	virtual std::string string();
	virtual Type* getType();
	virtual void resolve();
	virtual Value* Codegen();

	Expression *expr; 
	std::string op; 
	bool prefixIncrement = false; 

	IncrementExpr(Expression *expr, std::string op, bool prefixIncrement);
};

#endif 