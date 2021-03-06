package ast

type Node interface {
	isNode()
}

type Statement interface {
	isStatement()
}

type Expression interface {
	isExpression()
}

type Identifier interface {
	Expression
	isIdentifier()
}

//
// Statements
//

type AliasDecl struct {
	Name string
	Type Type
}

type ClassDecl struct {
	Name   string
	Supers []Identifier
	Body   []Node
}

type VarDecl struct {
	Name  string
	Type  *Type
	Value Expression
}

type GetterStmt struct {
	Body []Node
}

type SetterStmt struct {
	SetterVariable *VarDecl
	Body           []Node
}

type PropertyDecl struct {
	Name   string
	Type   *Type
	Getter *GetterStmt
	Setter *SetterStmt
}

type EnumDecl struct {
	Name    string
	Members []string
}

type ExtensionDecl struct {
	Name   string
	Supers []Identifier
	Body   []Node
}

type InterfaceDecl struct {
	Name   string
	Supers []Identifier
	Body   []Node
}

type PackageDecl struct {
	Name string
}

type ImportDecl struct {
	Name string
}

//
// Expressions
//

type FunctionExpr struct {
	Name       string
	Parameters []*VarDecl
	RetType    *Type
	Body       []Node
}

type ExternFuncExpr struct {
	Name       string
	Parameters []*VarDecl
	RetType    *Type
}

type ArrayExpr struct {
	Members []Expression
}

type LoopExpr struct {
	Initializer  Expression
	Condition    Expression
	Afterthought Expression

	CheckTime LoopConditionTime
	Body      []Node
}

type UnaryExpr struct {
	Operation string
	Operand   Expression
}

type BinaryExpr struct {
	Operation string
	LHS       Expression
	RHS       Expression
}

type CallExpr struct {
	Object    Expression
	Arguments []Expression
}

type ArrayAccessExpr struct {
	Object Expression
	Index  Expression
}

type MemberAccessExpr struct {
	Object Expression
	Name   string
}

//
// Identifiers
//

type NamedIDExpr struct {
	Name string
}

type IDAccessExpr struct {
	LHS Identifier
	RHS string
}

//
// Implement interfaces to enforce types
//

func (s AliasDecl) isNode()        {}
func (s ClassDecl) isNode()        {}
func (s VarDecl) isNode()          {}
func (s GetterStmt) isNode()       {}
func (s SetterStmt) isNode()       {}
func (s PropertyDecl) isNode()     {}
func (s EnumDecl) isNode()         {}
func (s ExtensionDecl) isNode()    {}
func (s InterfaceDecl) isNode()    {}
func (s PackageDecl) isNode()      {}
func (s ImportDecl) isNode()       {}
func (e FunctionExpr) isNode()     {}
func (e ExternFuncExpr) isNode()   {}
func (e ArrayExpr) isNode()        {}
func (e LoopExpr) isNode()         {}
func (e UnaryExpr) isNode()        {}
func (e BinaryExpr) isNode()       {}
func (e CallExpr) isNode()         {}
func (e ArrayAccessExpr) isNode()  {}
func (e MemberAccessExpr) isNode() {}
func (i NamedIDExpr) isNode()      {}
func (i IDAccessExpr) isNode()     {}

func (s AliasDecl) isStatement()     {}
func (s ClassDecl) isStatement()     {}
func (s VarDecl) isStatement()       {}
func (s GetterStmt) isStatement()    {}
func (s SetterStmt) isStatement()    {}
func (s PropertyDecl) isStatement()  {}
func (s EnumDecl) isStatement()      {}
func (s ExtensionDecl) isStatement() {}
func (s InterfaceDecl) isStatement() {}
func (s PackageDecl) isStatement()   {}
func (s ImportDecl) isStatement()    {}

func (e FunctionExpr) isExpression()     {}
func (e ExternFuncExpr) isExpression()   {}
func (e ArrayExpr) isExpression()        {}
func (e LoopExpr) isExpression()         {}
func (e UnaryExpr) isExpression()        {}
func (e BinaryExpr) isExpression()       {}
func (e CallExpr) isExpression()         {}
func (e ArrayAccessExpr) isExpression()  {}
func (e MemberAccessExpr) isExpression() {}
func (i NamedIDExpr) isExpression()      {}
func (i IDAccessExpr) isExpression()     {}

func (i NamedIDExpr) isIdentifier()  {}
func (i IDAccessExpr) isIdentifier() {}
