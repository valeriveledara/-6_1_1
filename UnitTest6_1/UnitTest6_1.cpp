﻿#include "pch.h"
#include "CppUnitTest.h"
#include "..\ЛабораторнаяРаботаПаттерны\ЛабораторнаяРаботаПаттерны.cpp"
#include <string> 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1lab6
{
	TEST_CLASS(UnitTest1lab6)
	{
	public:

		TEST_METHOD(TestConstuctorNumber)
		{
			Number* n32 = new Number(32.0);
			Assert::IsTrue(n32->value() == 32.0);
		}
		TEST_METHOD(TestEvaluateNumber)
		{
			Number* n32 = new Number(32.0);
			Assert::IsTrue(n32->evaluate() == 32.0);
		}
		TEST_METHOD(TestPrintNumber)
		{
			Number* n32 = new Number(32.0);
			Assert::IsTrue(n32->print() == to_string(32.0));
		}
		TEST_METHOD(TestConstuctorBinOperation)
		{
			Number* n32 = new Number(32.0);
			Number* n16 = new Number(16.0);
			BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
			Assert::IsTrue(minus->left() == n32 && minus->operation() == BinaryOperation::MINUS && minus->right() == n16);

		}
		TEST_METHOD(TestBinOperMinus)
		{
			Number* n32 = new Number(32.0);
			Number* n16 = new Number(16.0);
			BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
			Assert::IsTrue(minus->evaluate() == 16.0);
		}
		TEST_METHOD(TestBinOperPlus) {
			Number* n = new Number(-16.0);
			Number* n16 = new Number(16.0);
			BinaryOperation* plus = new BinaryOperation(n, BinaryOperation::PLUS, n16);
			Assert::IsTrue(plus->evaluate() == 0.0);
		}
		TEST_METHOD(TestBinOperDiv) {
			Number* n32 = new Number(32.0);
			Number* n16 = new Number(16.0);
			BinaryOperation* div = new BinaryOperation(n32, BinaryOperation::DIV, n16);
			Assert::IsTrue(div->evaluate() == 2.0);
		}
		TEST_METHOD(TestBinOperDivByZero) {
			Number* n32 = new Number(32.0);
			Number* n0 = new Number(0.0);
			BinaryOperation* div_z = new BinaryOperation(n32, BinaryOperation::DIV, n0);
			Assert::ExpectException<std::invalid_argument>([div_z]() {div_z->evaluate(); });

		}
		TEST_METHOD(TestBinOperMul) {
			Number* n32 = new Number(32.0);
			Number* n16 = new Number(16.0);
			BinaryOperation* mul = new BinaryOperation(n32, BinaryOperation::MUL, n16);
			Assert::IsTrue(mul->evaluate() == 512.0);
		}
		TEST_METHOD(TestBinOperPrint) {
			Number* n32 = new Number(32.0);
			Number* n16 = new Number(16.0);
			BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
			Assert::IsTrue(minus->print() == to_string(32.0) + string(1, BinaryOperation::MINUS) + to_string(16.0));
		}
		TEST_METHOD(TestConstructorFuncCall) {
			Number* n16 = new Number(16.0);
			FunctionCall* callSqrt = new FunctionCall("sqrt", n16);
			Assert::IsTrue(callSqrt->arg() == n16 && callSqrt->name() == "sqrt");

		}
		TEST_METHOD(TestEvaluteFuncCallSQRT) {
			Number* n16 = new Number(16.0);
			FunctionCall* callSqrt = new FunctionCall("sqrt", n16);
			Assert::IsTrue(callSqrt->evaluate() == 4.0);

		}
		TEST_METHOD(TestEvaluteNegativeSQRTFuncCall) {
			Number* n16 = new Number(-16.0);
			FunctionCall* callSqrt = new FunctionCall("sqrt", n16);
			Assert::ExpectException<std::invalid_argument>([callSqrt]() {callSqrt->evaluate(); });

		}
		TEST_METHOD(TestEvaluteFuncCallABS) {
			Number* n16 = new Number(-16.0);
			FunctionCall* callSqrt = new FunctionCall("abs", n16);
			Assert::IsTrue(callSqrt->evaluate() == 16.0);

		}
		TEST_METHOD(TestConstructorValue) {
			Variable* var = new Variable("var");
			Assert::IsTrue(var->name() == "var");
		}
		TEST_METHOD(TestPrintValue) {
			Variable* var = new Variable("var");
			Assert::IsTrue(var->print() == "var");
		}
		TEST_METHOD(TestEvaluateValue) {
			Variable* var = new Variable("var");
			Assert::IsTrue(var->evaluate() == 0.0);
		}
		TEST_METHOD(TestCST) {
			Number* n32 = new Number(32.0);
			Number* n16 = new Number(16.0);
			BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
			FunctionCall* callSqrt = new FunctionCall("sqrt", minus);
			Variable* var = new Variable("var");
			BinaryOperation* mult = new BinaryOperation(var, BinaryOperation::MUL, callSqrt);
			FunctionCall* callAbs = new FunctionCall("abs", mult);
			CopySyntaxTree CST;
			Expression* newExpr = callAbs->transform(&CST);
			Assert::IsTrue(newExpr->print() == callAbs->print());
		}
		TEST_METHOD(CopyNumber)
		{
			CopySyntaxTree tree_copy;
			Number num(42.0);
			Expression* exp = num.transform(&tree_copy);
			Number* result = dynamic_cast<Number*>(exp);
			Assert::IsNotNull(result);
			Assert::AreEqual(42.0, result->value());
		}
		TEST_METHOD(CopyVariable)
		{
			CopySyntaxTree tree_copy;
			Variable var("x");
			Expression* exp = var.transform(&tree_copy);
			Variable* result = dynamic_cast<Variable*>(exp);
			Assert::IsNotNull(result);
			Assert::AreEqual(std::string("x"), result->name());
		}
		TEST_METHOD(TestTransformNumber)
		{
			FoldConstants fc;
			Number num(10.0);
			Expression* result = fc.transformNumber(&num);
			Assert::IsTrue(dynamic_cast<Number*>(result) != nullptr);
			Assert::AreEqual(10.0, dynamic_cast<Number*>(result)->value());
		}
		TEST_METHOD(TestTransformFunctionCall)
		{
			FoldConstants transformer;
			Expression* argumentExpression = new Number(9.0);
			Expression* expression = new FunctionCall("sqrt", argumentExpression);
			Expression* transformedExpression = transformer.transformFunctionCall(dynamic_cast<FunctionCall*>(expression));
			Assert::IsNotNull(transformedExpression);
			Assert::AreEqual(3.0, dynamic_cast<Number*>(transformedExpression)->value());
		}
		TEST_METHOD(TestTransformBinaryOperation)
		{
			FoldConstants transformer;
			Expression* leftExpression = new Number(2.0);
			Expression* rightExpression = new Number(3.0);
			Expression* expression = new BinaryOperation(leftExpression, '+', rightExpression);
			Expression* transformedExpression = transformer.transformBinaryOperation(dynamic_cast<BinaryOperation*>(expression));

			Assert::IsNotNull(transformedExpression);
			Assert::AreEqual(5.0, dynamic_cast<Number*>(transformedExpression)->value());

		}
	};
}
