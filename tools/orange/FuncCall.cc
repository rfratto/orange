/*
** Copyright 2014-2015 Robert Fratto. See the LICENSE.txt file at the top-level 
** directory of this distribution.
**
** Licensed under the MIT license <http://opensource.org/licenses/MIT>. This file 
** may not be copied, modified, or distributed except according to those terms.
*/ 

#include <orange/FuncCall.h>
#include <orange/generator.h>

Value* FuncCall::Codegen() {
	SymTable *curTab = GE::runner()->symtab();
	
	FunctionStmt* function = (FunctionStmt*)curTab->find(m_name); 

	if (function == nullptr) {
		throw CompilerMessage(*this, "No function " + m_name + " found!");
	}

	Function* llvmFunction = (Function*) function->getValue();

	if (llvmFunction == nullptr) {
		throw CompilerMessage(*this, "No function " + m_name + " generated!");
	}

	if (llvmFunction->isVarArg() == false && llvmFunction->arg_size() != m_arguments.size()) {
		throw CompilerMessage(*this, "Invalid number of arguments");
	} else if (llvmFunction->isVarArg() && m_arguments.size() == 0) {
		throw CompilerMessage(*this, "Must have at least one argument in a variable argument call!");		
	}

	if (m_arguments.size()) {
		std::vector<Value*> Args;

		auto arg_it = llvmFunction->arg_begin();
		for (unsigned int i = 0; i < llvmFunction->arg_size(); i++, arg_it++) {
			Value *vArg = m_arguments[i]->Codegen();

			if (m_arguments[i]->returnsPtr()) {
				vArg = GE::builder()->CreateLoad(vArg);
			}

			AnyType* anyType = new AnyType(arg_it->getType());
			CastingEngine::CastValueToType(&vArg, anyType, m_arguments[i]->isSigned(), true);
			delete anyType;
			Args.push_back(vArg);
		}

	// If we're calling a variable argument function, add our other arguments.
		if (llvmFunction->isVarArg()) {
			for (unsigned int i = llvmFunction->arg_size(); i < m_arguments.size(); i++) {
				Value *vArg = m_arguments[i]->Codegen();

				if (m_arguments[i]->returnsPtr()) {
					vArg = GE::builder()->CreateLoad(vArg);
				}

				Args.push_back(vArg);			
			}
		}

		return GE::builder()->CreateCall(llvmFunction, Args);
	} else {
		return GE::builder()->CreateCall(llvmFunction);
	}

}

ASTNode* FuncCall::clone() {
	FuncCall* cloned = new FuncCall(m_name);

	for (auto arg : m_arguments) {
		cloned->m_arguments.push_back((Expression*)arg->clone());
	}

	return cloned; 
}

std::string FuncCall::string() {
	std::stringstream ss;
	ss << m_name << "(";

	for (int i = 0; i < m_arguments.size(); i++) {
		ss << " " << m_arguments[i]->string() << " ";
		if (i+1 < m_arguments.size()) {
			ss << ",";
		}
	}

	ss << ")";
	return ss.str();
}

AnyType* FuncCall::getType() {
	SymTable* curTab = GE::runner()->symtab();
  ASTNode* node = curTab->find(m_name);
  
  if (node == nullptr) {
      throw CompilerMessage(*this, m_name + " does not exist!");
  }
  
  if ((node->getClass() != "FunctionStmt" &&
       node->getClass() != "ExternFunction")) {
      throw CompilerMessage(*this, m_name + " is not a function!");
  }
    
	if (node->getClass() == "FunctionStmt" && ((FunctionStmt*)node)->isGeneric()) {
		// Try resolving...
		if (m_resolved == false) {
			resolve(); 
			return getType();
		}

		throw CompilerMessage(*this, "Getting the type of a generic function is NYI!");
	}

	return node->getType();
}

void FuncCall::resolve() {
	if (m_resolved) return;
	m_resolved = true;

	// Look for the function in the symbol table.
	SymTable *curTab = GE::runner()->symtab();
	ASTNode* function = curTab->find(m_name); 

	if (function == nullptr) {
		throw CompilerMessage(*this, "No function " + m_name + " found!");
	}

	if ((function->getClass() != "FunctionStmt" &&
		function->getClass() != "ExternFunction")) {
		throw CompilerMessage(*this, m_name + " is not a function!");
	}

	for (auto arg : m_arguments) arg->resolve();

	// TODO: create clone of function if needed (for generics).
	// Call resolve on the clone, since we're the one that creates it. 
	if (function->getClass() == "FunctionStmt") {
		FunctionStmt* fstmt = (FunctionStmt*)function;
		if (fstmt->isGeneric()) {
			FunctionStmt* clone = fstmt->createGenericClone(m_arguments);
			m_name = clone->name();
			clone->resolve();
		}
	}
}

bool FuncCall::isSigned() {
	// This will need to be dynamic at some point.
	return false; 
}

FuncCall::FuncCall(std::string name) : m_name(name) {}
FuncCall::FuncCall(std::string name, ArgList arguments) : m_name(name), m_arguments(arguments) {}