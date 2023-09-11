#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include "Instructions.hpp"
#include "Expressions.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "CommentFilterLexer.hpp"
#include "Interpreter.hpp"
#include "ProgramTreePrinter.hpp"

// Lexer tests

TEST(LexerTest, Opening_parenth_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "(";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Opening_parenth);
}

TEST(LexerTest, Closing_parenth_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << ")";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Closing_parenth);
}

TEST(LexerTest, Opening_curly_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "{";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Opening_curly);
}

TEST(LexerTest, Closing_curly_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "}";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Closing_curly);
}

TEST(LexerTest, Colon_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << ":";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Colon);
}

TEST(LexerTest, Semicolon_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << ";";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Semicolon);
}

TEST(LexerTest, Comma_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << ",";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Comma);
}

TEST(LexerTest, Underscore_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "_";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Underscore);
}

TEST(LexerTest, Assign_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "=";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Assign);
}

TEST(LexerTest, Plus_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "+";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Plus);
}

TEST(LexerTest, Minus_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "-";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Minus);
}

TEST(LexerTest, Multiplication_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "*";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Multiplication);
}

TEST(LexerTest, Division_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "/";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Division);
}

TEST(LexerTest, Modulo_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "%";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Modulo);
}

TEST(LexerTest, String_concat_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "|";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::String_concat);
}

TEST(LexerTest, Gt_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << ">";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Gt);
}

TEST(LexerTest, Lt_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "<";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Lt);
}

TEST(LexerTest, Gte_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << ">=";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Gte);
}

TEST(LexerTest, Lte_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "<=";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Lte);
}

TEST(LexerTest, Equals_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "==";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Equals);
}

TEST(LexerTest, Not_equals_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "!=";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Not_equals);
}

TEST(LexerTest, Integer_type_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "int";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Integer_type);
}

TEST(LexerTest, Floating_type_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "float";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Floating_type);
}

TEST(LexerTest, String_type_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "str";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::String_type);
}

TEST(LexerTest, File_type_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "file";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::File_type);
}

TEST(LexerTest, Bool_type_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "bool";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Bool_type);
}

TEST(LexerTest, Void_type_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "void";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Void_type);
}

TEST(LexerTest, Return_keywd_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "return";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Return_keywd);
}

TEST(LexerTest, Function_keywd_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "fun";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Function_keywd);
}

TEST(LexerTest, If_keywd_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "if";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::If_keywd);
}

TEST(LexerTest, Else_keywd_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "else";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Else_keywd);
}

TEST(LexerTest, While_keywd_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "while";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::While_keywd);
}

TEST(LexerTest, Const_keywd_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "const";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Const_keywd);
}

TEST(LexerTest, Match_keywd_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "match";
  Lexer lex(s);
  Token token = lex.get_next_token();
  EXPECT_EQ(token.get_type(), TokenType::Match_keywd);
}


TEST(LexerTest, Integer_literal_test1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "0";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Integer_literal);
  int64_t value = std::get<int64_t>(token.get_value());
  EXPECT_EQ(value, 0);
}

TEST(LexerTest, Integer_literal_test2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "420";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Integer_literal);
  int64_t value = std::get<int64_t>(token.get_value());
  EXPECT_EQ(value, 420);
}

TEST(LexerTest, Illegal_char_sequence){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "\\";
  Lexer lex(s);
  ASSERT_THROW(Token token = lex.get_next_token(), TokenizationError<char>);
}
TEST(LexerTest, Integer_literal_test3){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "0233";
  Lexer lex(s);
  
  ASSERT_THROW({Token token = lex.get_next_token();}, TokenizationError<char>);
}

TEST(LexerTest, Integer_literal_test4){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "13786//fwo87fh";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Integer_literal);
  int64_t value = std::get<int64_t>(token.get_value());
  EXPECT_EQ(value, 13786);
}

TEST(LexerTest, Integer_literal_test5){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "  233";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Integer_literal);
  int64_t value = std::get<int64_t>(token.get_value());
  EXPECT_EQ(value, 233);
}

TEST(LexerTest, Integer_literal_test6){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "\n  18446744073709551615 7544";
  Lexer lex(s);
  ASSERT_THROW({Token token = lex.get_next_token();}, TokenizationError<char>);
}

TEST(LexerTest, Integer_literal_test7){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "1234567898245746872275453875687458725832587469875657";
  Lexer lex(s);
  ASSERT_THROW(Token token = lex.get_next_token(), TokenizationError<char>);
}

TEST(LexerTest, Floating_literal_test1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "1.23";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Floating_literal);
  double value = std::get<double>(token.get_value());
  EXPECT_DOUBLE_EQ(value, 1.23);
}

TEST(LexerTest, Floating_literal_test2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "1.0";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Floating_literal);
  double value = std::get<double>(token.get_value());
  EXPECT_DOUBLE_EQ(value, 1.0);
}

TEST(LexerTest, Floating_literal_test3){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "0.01";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Floating_literal);
  double value = std::get<double>(token.get_value());
  EXPECT_DOUBLE_EQ(value, 0.01);
}

TEST(LexerTest, Floating_literal_test4){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "123456789.97654345678";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Floating_literal);
  double value = std::get<double>(token.get_value());
  EXPECT_DOUBLE_EQ(value, 123456789.97654345678);
}

TEST(LexerTest, Floating_literal_test5){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "0123.3123.2321";  
  Lexer lex(s);
  ASSERT_THROW({Token token = lex.get_next_token();}, TokenizationError<char>);
  ASSERT_THROW({Token token = lex.get_next_token();}, TokenizationError<char>);
}

TEST(LexerTest, Floating_literal_test6){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "1O3";
  Lexer lex(s);
  ASSERT_THROW({Token token = lex.get_next_token();}, TokenizationError<char>);
}

TEST(LexerTest, Floating_literal_test7){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "123456789.";
  Lexer lex(s);
  ASSERT_THROW(Token token = lex.get_next_token(), TokenizationError<char>);
}

TEST(LexerTest, Identifier_test1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "Identyfikator_zmiennej";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  std::string value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, s.str());
}

TEST(LexerTest, Identifier_test2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "startswithsmallletterbutalsonumbers2132andunderscores______";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  std::string value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, s.str());
}

TEST(LexerTest, Identifier_test3){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "_startswithunderscore";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_NE(token.get_type(), TokenType::Identifier);
  EXPECT_EQ(token.get_type(), TokenType::Underscore);
}

TEST(LexerTest, Identifier_test4){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "288startswithnumber";
  Lexer lex(s);
  ASSERT_THROW({Token token = lex.get_next_token();}, TokenizationError<char>);
}

TEST(LexerTest, Comment_test1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "#svjniovjns\n";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Comment);
  std::string value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "svjniovjns");
}

TEST(LexerTest, Comment_test2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "# j dn9 7h9hw9o__(0/v.;vvnjsnvjdv ";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Comment);
  std::string value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, " j dn9 7h9hw9o__(0/v.;vvnjsnvjdv ");
}

TEST(LexerTest, Comment_test3){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "   # sovowivvvvvvovkkkkd\\dfsx\t\n";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Comment);
  std::string value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, " sovowivvvvvvovkkkkd\\dfsx\t");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::ETX_token);
}

TEST(LexerTest, String_literal_test1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "\"   # sovowivvvvvvovkkkkd\\nfsx\t\"\n";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::String_literal);
  std::string value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "   # sovowivvvvvvovkkkkd\nfsx\t");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::ETX_token);
}
//sfvibfiwbvihwbwbfiuwfiuhwifuhwiufhiuwhfiuhweiufhiuwhefiuhweiufhiuewhfiuhewiufhuwiehfiuhwiehfuhweiuhfiewhfiuwehfiwue
TEST(LexerTest, String_literal_test_emptystr){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "\"\"";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::String_literal);
  std::string value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::ETX_token);
}

TEST(LexerTest, String_literal_test_newlinebeforequot){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "\"?\n\"";
  Lexer lex(s);
  ASSERT_THROW({Token token = lex.get_next_token();}, TokenizationError<char>);
}

TEST(LexerTest, String_literal_test_ETXbeforeendquot){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "\"svvs";
  Lexer lex(s);
  ASSERT_THROW({Token token = lex.get_next_token();}, TokenizationError<char>);
}

TEST(LexerTest, String_literal_test_backslachandnothing){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "\"\\";
  Lexer lex(s);
  ASSERT_THROW({Token token = lex.get_next_token();}, TokenizationError<char>);
}

TEST(LexerTest, String_literal_test_bad_escaped){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "\\l";
  Lexer lex(s);
  ASSERT_THROW({Token token = lex.get_next_token();}, TokenizationError<char>);
}

TEST(LexerTest, Compound_test1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "match(x % 42, y % 22){\r\nisEven, 11: 12 + x,\r\n0, _: to_int(\"3\"),\r\n_, _: do_calcs(x)};";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Match_keywd);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Opening_parenth);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  std::string value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "x");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Modulo);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Integer_literal);
  int64_t value2 = std::get<int64_t>(token.get_value());
  EXPECT_EQ(value2, 42);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Comma);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "y");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Modulo);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Integer_literal);
  value2 = std::get<int64_t>(token.get_value());
  EXPECT_EQ(value2, 22);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Closing_parenth);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Opening_curly);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "isEven");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Comma);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Integer_literal);
  value2 = std::get<int64_t>(token.get_value());
  EXPECT_EQ(value2, 11);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Colon);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Integer_literal);
  value2 = std::get<int64_t>(token.get_value());
  EXPECT_EQ(value2, 12);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Plus);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "x");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Comma);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Integer_literal);
  value2 = std::get<int64_t>(token.get_value());
  EXPECT_EQ(value2, 0);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Comma);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Underscore);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Colon);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "to_int");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Opening_parenth);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::String_literal);
  value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "3");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Closing_parenth);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Comma);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Underscore);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Comma);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Underscore);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Colon);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "do_calcs");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Opening_parenth);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "x");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Closing_parenth);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Closing_curly);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Semicolon);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::ETX_token);
}

TEST(LexerTest, Compound_test2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "zmienna: float = 420.69;\nfun funkcja(arg1: float, arg2: int): void{\nzmienna = zmienna * arg1 + arg2;} #komentarz\n\n\t";
  Lexer lex(s);
  Token token = lex.get_next_token();
  
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  std::string value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "zmienna");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Colon);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Floating_type);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Assign);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Floating_literal);
  double value3 = std::get<double>(token.get_value());
  EXPECT_DOUBLE_EQ(value3, 420.69);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Semicolon);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Function_keywd);
  ASSERT_EQ(token.get_position().line, 2);
  ASSERT_EQ(token.get_position().column, 1);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "funkcja");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Opening_parenth);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "arg1");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Colon);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Floating_type);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Comma);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "arg2");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Colon);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Integer_type);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Closing_parenth);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Colon);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Void_type);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Opening_curly);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "zmienna");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Assign);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "zmienna");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Multiplication);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "arg1");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Plus);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Identifier);
  value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "arg2");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Semicolon);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Closing_curly);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Comment);
  value = std::get<std::string>(token.get_value());
  EXPECT_EQ(value, "komentarz");
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::ETX_token);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::ETX_token);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::ETX_token);
}

TEST(LexerTest, Repetition_tests){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "intt floatt floaty voidd !-nott";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_NE(token.get_type(), TokenType::Integer_type);
  token = lex.get_next_token();
  ASSERT_NE(token.get_type(), TokenType::Floating_type);
  token = lex.get_next_token();
  ASSERT_NE(token.get_type(), TokenType::Floating_type);
  token = lex.get_next_token();
  ASSERT_NE(token.get_type(), TokenType::Void_type);
  ASSERT_THROW({token = lex.get_next_token();}, TokenizationError<char>);
  token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Minus);
  token = lex.get_next_token();
  ASSERT_NE(token.get_type(), TokenType::Not);
}

TEST(LexerTest, Lte_and_assign_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "<= 4";
  Lexer lex(s);
  Token token = lex.get_next_token();
  ASSERT_EQ(token.get_type(), TokenType::Lte);
  token = lex.get_next_token();
  ASSERT_NE(token.get_type(), TokenType::Assign);
  token = lex.get_next_token();
}

TEST(ParserTest, Function_test1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "fun testowa(param1:const int, bruh : float) : int {}";
  Lexer lex(s);
  Parser parser(lex);
  auto fun_vect = parser.parse();
  auto program = Program<char>(std::move(fun_vect));
  ASSERT_EQ(program.get_function_definitions()->at("testowa")->get_name(), "testowa");
  ASSERT_EQ(program.get_function_definitions()->at("testowa")->get_type()->get_is_const(), false);
  ASSERT_EQ(program.get_function_definitions()->at("testowa")->get_type()->get_type(), Type::Integer);
  auto& param1 = program.get_function_definitions()->at("testowa")->get_parameters()->at(0);
  ASSERT_EQ(param1->get_name(), "param1");
  ASSERT_EQ(param1->get_type()->get_is_const(), true);
  ASSERT_EQ(param1->get_type()->get_type(), Type::Integer);
  auto &param2 = program.get_function_definitions()->at("testowa")->get_parameters()->at(1);
  ASSERT_EQ(param2->get_name(), "bruh");
  ASSERT_EQ(param2->get_type()->get_is_const(), false);
  ASSERT_EQ(param2->get_type()->get_type(), Type::Floating);
  ASSERT_EQ(program.get_function_definitions()->at("testowa")->get_block()->size(), 0);
  ASSERT_NO_THROW(ProgramTreePrinter<char>{std::cout}.print_program(program));
}

TEST(ParserTest, check_and_advance){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "fun testowa(param1:const int, bruh : float) : int {}";
  Lexer lex(s);
  ParserBase parser(lex);
  ASSERT_EQ(parser.check_and_advance(TokenType::Function_keywd), true);
  ASSERT_EQ(parser.check_and_advance(TokenType::Function_keywd), false);
}

TEST(ParserTest, is_current_token_relation_operator){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "<= :";
  Lexer lex(s);
  ParserBase parser(lex);
  ASSERT_EQ(parser.is_current_token_relation_operator(), true);
  parser.advance_token();
  ASSERT_EQ(parser.is_current_token_relation_operator(), false);
}

TEST(ParserTest, is_current_token_a_type){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "file :";
  Lexer lex(s);
  ParserBase parser(lex);
  ASSERT_EQ(parser.is_current_token_a_type(), true);
  parser.advance_token();
  ASSERT_EQ(parser.is_current_token_a_type(), false);
}

TEST(ParserTest, try_parse_literal){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "\"dziala\" 123 122.21";
  Lexer lex(s);
  ParserBase parser(lex);
  auto str_literal = std::get<std::basic_string<char>>(dynamic_cast<LiteralExpression<char>*>(parser.try_parse_literal().release())->get_value());
  ASSERT_EQ(str_literal, "dziala");
  
  auto int_literal = std::get<int64_t>(dynamic_cast<LiteralExpression<char>*>(parser.try_parse_literal().release())->get_value());
  ASSERT_EQ(int_literal, 123);
  
  auto float_literal = std::get<double>(dynamic_cast<LiteralExpression<char>*>(parser.try_parse_literal().release())->get_value());
  ASSERT_DOUBLE_EQ(float_literal, 122.21);
  ASSERT_EQ(parser.is_current_token_a_type(), false);
}

TEST(ParserTest, try_parse_expression1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "a + b";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = dynamic_cast<TwoArgExpression<char>*>(parser.try_parse_expression().release());
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_string_repr(), "PlusExpression");
  ASSERT_EQ(dynamic_cast<IdentifierExpression<char>*>(expr->get_left_expression().get())->get_value(), "a");
  ASSERT_EQ(dynamic_cast<IdentifierExpression<char>*>(expr->get_right_expression().get())->get_value(), "b");
}

TEST(ParserTest, try_parse_expression2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "3 % c";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = dynamic_cast<TwoArgExpression<char>*>(parser.try_parse_expression().release());
  auto &left = expr->get_left_expression();
  auto &right = expr->get_right_expression();
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_string_repr(), "ModuloExpression");
  ASSERT_EQ(std::get<int64_t>(dynamic_cast<LiteralExpression<char>*>(left.get())->get_value()), 3);
  ASSERT_EQ(dynamic_cast<IdentifierExpression<char>*>(right.get())->get_value(), "c");
}

TEST(ParserTest, try_parse_expression3){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "2 + 3 * 8";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = dynamic_cast<TwoArgExpression<char>*>(parser.try_parse_expression().release());
  ASSERT_NE(expr, nullptr);
  auto &left = expr->get_left_expression();
  auto &right = expr->get_right_expression();
  ASSERT_EQ(expr->get_string_repr(), "PlusExpression");
  ASSERT_EQ(std::get<int64_t>(dynamic_cast<LiteralExpression<char>*>(left.get())->get_value()), 2);
  auto right_mult = dynamic_cast<TwoArgExpression<char>*>(right.get());
  ASSERT_EQ(right_mult->get_string_repr(), "MultiplicationExpression");
  
  auto &left2 = right_mult->get_left_expression();
  auto &right2 = right_mult->get_right_expression();
  
  ASSERT_EQ(std::get<int64_t>(dynamic_cast<LiteralExpression<char>*>(left2.get())->get_value()), 3);
  ASSERT_EQ(std::get<int64_t>(dynamic_cast<LiteralExpression<char>*>(right2.get())->get_value()), 8);
}

TEST(ParserTest, try_parse_expression4){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "(2 + 3) * 8";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = dynamic_cast<TwoArgExpression<char>*>(parser.try_parse_expression().release());
  ASSERT_NE(expr, nullptr);
  auto &left = expr->get_left_expression();
  auto &right = expr->get_right_expression();
  ASSERT_EQ(expr->get_string_repr(), "MultiplicationExpression");
  ASSERT_EQ(std::get<int64_t>(dynamic_cast<LiteralExpression<char>*>(right.get())->get_value()), 8);
  auto left_add = dynamic_cast<TwoArgExpression<char>*>(left.get());
  ASSERT_EQ(left_add->get_string_repr(), "PlusExpression");
  
  auto &left2 = left_add->get_left_expression();
  auto &right2 = left_add->get_right_expression();
  
  ASSERT_EQ(std::get<int64_t>(dynamic_cast<LiteralExpression<char>*>(left2.get())->get_value()), 2);
  ASSERT_EQ(std::get<int64_t>(dynamic_cast<LiteralExpression<char>*>(right2.get())->get_value()), 3);
}


TEST(ParserTest, try_parse_expression5){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "2 * function(123)";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = dynamic_cast<TwoArgExpression<char>*>(parser.try_parse_expression().release());
  ASSERT_NE(expr, nullptr);
  auto &left = expr->get_left_expression();
  auto &right = expr->get_right_expression();
  ASSERT_EQ(expr->get_string_repr(), "MultiplicationExpression");
  ASSERT_EQ(std::get<int64_t>(dynamic_cast<LiteralExpression<char>*>(left.get())->get_value()), 2);
  auto right_call = dynamic_cast<FunctionCall<char>*>(right.get());
  ASSERT_EQ(right_call->get_string_repr(), "FunctionCallExpression");
  
  ASSERT_EQ(right_call->get_name(), "function");
  ASSERT_EQ(right_call->get_arguments()->size(), 1);
}

TEST(ParserTest, try_parse_expression6){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "a + b or 3 < ident and 9 * 222";
  Lexer lex(s);
  ParserBase parser(lex);
  parser.try_parse_expression();
}

TEST(ParserTest, try_parse_expression_fail1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "2 * fun(123)";
  Lexer lex(s);
  ParserBase parser(lex);

  ASSERT_THROW(parser.try_parse_expression();, SyntaxErrorException<char>);
}


TEST(ParserTest, try_parse_expression_fail2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "4 * * ident";
  Lexer lex(s);
  ParserBase parser(lex);

  ASSERT_THROW(parser.try_parse_expression();, SyntaxErrorException<char>);
}

TEST(ParserTest, try_parse_expression_fail3){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "8 + ((9 - 17)";
  Lexer lex(s);
  ParserBase parser(lex);

  ASSERT_THROW(parser.try_parse_expression();, SyntaxErrorException<char>);
}

TEST(ParserTest, try_parse_return_statement){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "return ekspresja * 12;";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = dynamic_cast<ReturnInstruction<char>*>(parser.try_parse_return_statement().release());
  ASSERT_NE(expr, nullptr);
  const auto &inner = dynamic_cast<TwoArgExpression<char>*>(expr->get_expression().get());
  const auto &left = dynamic_cast<IdentifierExpression<char>*>(inner->get_left_expression().get());
  const auto &right = dynamic_cast<LiteralExpression<char>*>(inner->get_right_expression().get());
  ASSERT_EQ(left->get_value(), "ekspresja");
  ASSERT_EQ(std::get<int64_t>(right->get_value()), 12);
}


TEST(ParserTest, try_parse_var_def_assign_or_funcall1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "var : const int = 14 * cos(3)";
  Lexer lex(s);
  ParserBase parser(lex);
  VarDefinitionInstruction<char>* expr;
  ASSERT_NO_THROW( expr = dynamic_cast<VarDefinitionInstruction<char>*>(parser.try_parse_var_def_assign_or_funcall().release()));
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_name(), "var");
  ASSERT_EQ(expr->get_type()->get_type(), Type::Integer);
  ASSERT_EQ(expr->get_type()->get_is_const(), true);
  ASSERT_NO_THROW([[maybe_unused]] const auto* _ = dynamic_cast<TwoArgExpression<char>*>(expr->get_expression().get()));
}

TEST(ParserTest, try_parse_var_def_assign_or_funcall2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "var = 14";
  Lexer lex(s);
  ParserBase parser(lex);
  AssignmentInstruction<char>* expr;
  ASSERT_NO_THROW( expr = dynamic_cast<AssignmentInstruction<char>*>(parser.try_parse_var_def_assign_or_funcall().release()));
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_name(), "var");
  ASSERT_NO_THROW([[maybe_unused]] const auto* _ = dynamic_cast<LiteralExpression<char>*>(expr->get_expression().get()));
}


TEST(ParserTest, try_parse_var_def_assign_or_funcall3){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "var()";
  Lexer lex(s);
  ParserBase parser(lex);
  FunctionCall<char>* expr;
  ASSERT_NO_THROW( expr = dynamic_cast<FunctionCall<char>*>(parser.try_parse_var_def_assign_or_funcall().release()));
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_name(), "var");
  ASSERT_EQ(expr->get_arguments()->size(), 0);
}

TEST(ParserTest, try_parse_while_block1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "while(condition){funcall();}";
  Lexer lex(s);
  ParserBase parser(lex);
  WhileInstruction<char>* expr;
  ASSERT_NO_THROW( expr = dynamic_cast<WhileInstruction<char>*>(parser.try_parse_while_block().release()));
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_code_block()->size(), 1);
  ASSERT_EQ(dynamic_cast<IdentifierExpression<char>*>(expr->get_condition().get())->get_value(), "condition");
}

TEST(ParserTest, try_parse_while_block_fail){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "while(condition){funcall();";
  Lexer lex(s);
  ParserBase parser(lex);
  ASSERT_THROW([[maybe_unused]] const auto* _ = dynamic_cast<WhileInstruction<char>*>(parser.try_parse_while_block().release()), UnexpectedTokenException<char>);
}

TEST(ParserTest, try_parse_if_block1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "if(condition){funcall();}";
  Lexer lex(s);
  ParserBase parser(lex);
  IfInstruction<char>* expr;
  ASSERT_NO_THROW( expr = dynamic_cast<IfInstruction<char>*>(parser.try_parse_if_block().release()));
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_code_block()->size(), 1);
  ASSERT_EQ(dynamic_cast<IdentifierExpression<char>*>(expr->get_condition().get())->get_value(), "condition");
}


TEST(ParserTest, try_parse_if_block2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "if(condition){funcall();}else{statm();}";
  Lexer lex(s);
  ParserBase parser(lex);
  IfInstruction<char>* expr;
  ASSERT_NO_THROW( expr = dynamic_cast<IfInstruction<char>*>(parser.try_parse_if_block().release()));
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_code_block()->size(), 1);
  ASSERT_EQ(dynamic_cast<IdentifierExpression<char>*>(expr->get_condition().get())->get_value(), "condition");
  ASSERT_NE(expr->get_else_block().get(), nullptr);
}

TEST(ParserTest, try_parse_if_block3){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "if(condition){funcall();}else if(cond2){statm();}else{statm3();}";
  Lexer lex(s);
  ParserBase parser(lex);
  IfInstruction<char>* expr;
  ASSERT_NO_THROW( expr = dynamic_cast<IfInstruction<char>*>(parser.try_parse_if_block().release()));
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_code_block()->size(), 1);
  ASSERT_EQ(dynamic_cast<IdentifierExpression<char>*>(expr->get_condition().get())->get_value(), "condition");
  ASSERT_NE(expr->get_else_block().get(), nullptr);
  auto if2 = dynamic_cast<IfInstruction<char>*>(expr->get_else_block().get());
  ASSERT_NE(if2, nullptr);
  ASSERT_EQ(dynamic_cast<IfInstruction<char>*>(if2->get_else_block().get())->get_code_block()->size(), 1);
}

TEST(ParserTest, try_parse_else_block1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "else if(cond2){statm();}";
  Lexer lex(s);
  ParserBase parser(lex);
  IfInstruction<char>* expr;
  ASSERT_NO_THROW( expr = dynamic_cast<IfInstruction<char>*>(parser.try_parse_else_block().release()));
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_code_block()->size(), 1);
  ASSERT_EQ(dynamic_cast<IdentifierExpression<char>*>(expr->get_condition().get())->get_value(), "cond2");
  ASSERT_EQ(expr->get_else_block().get(), nullptr);
}

TEST(ParserTest, try_parse_else_block2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "else {statm();}";
  Lexer lex(s);
  ParserBase parser(lex);
  IfInstruction<char>* expr;
  ASSERT_NO_THROW( expr = dynamic_cast<IfInstruction<char>*>(parser.try_parse_else_block().release()));
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_code_block()->size(), 1);
  ASSERT_EQ(expr->get_condition().get(), nullptr);
  ASSERT_EQ(expr->get_else_block().get(), nullptr);
}



TEST(ParserTest, try_parse_parenths_and_args1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "()";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = parser.try_parse_parenths_and_args().release();
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->size(), 0);
}

TEST(ParserTest, try_parse_parenths_and_args2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "(pierwszy, drugi() - 2)";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = parser.try_parse_parenths_and_args().release();
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->size(), 2);
}


TEST(ParserTest, try_parse_statement_or_control_block1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "if(condition){funcall();}else if(cond2){statm();}else{statm3();}";
  Lexer lex(s);
  ParserBase parser(lex);
  IfInstruction<char>* expr;
  ASSERT_NO_THROW( expr = dynamic_cast<IfInstruction<char>*>(parser.try_parse_statement_or_control_block().release()));
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_code_block()->size(), 1);
  ASSERT_EQ(dynamic_cast<IdentifierExpression<char>*>(expr->get_condition().get())->get_value(), "condition");
  ASSERT_NE(expr->get_else_block().get(), nullptr);
  auto if2 = dynamic_cast<IfInstruction<char>*>(expr->get_else_block().get());
  ASSERT_NE(if2, nullptr);
  ASSERT_EQ(dynamic_cast<IfInstruction<char>*>(if2->get_else_block().get())->get_code_block()->size(), 1);
}

TEST(ParserTest, try_parse_statement_or_control_block2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "a = 123;";
  Lexer lex(s);
  ParserBase parser(lex);
  AssignmentInstruction<char>* expr;
  ASSERT_NO_THROW( expr = dynamic_cast<AssignmentInstruction<char>*>(parser.try_parse_statement_or_control_block().release()));
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_name(), "a");
  ASSERT_EQ(std::get<int64_t>(dynamic_cast<LiteralExpression<char>*>(expr->get_expression().get())->get_value()), 123);
}

TEST(ParserTest, try_parse_code_block){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "{cds(); c=11;}";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = parser.try_parse_code_block().release();
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->size(), 2);
}

TEST(ParserTest, try_parse_type_identifier1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "int";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = parser.try_parse_type_identifier().release();
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_is_const(), false);
  ASSERT_EQ(expr->get_type(), Type::Integer);
}

TEST(ParserTest, try_parse_type_identifier2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "const file";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = parser.try_parse_type_identifier().release();
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_is_const(), true);
  ASSERT_EQ(expr->get_type(), Type::File);
}


TEST(ParserTest, try_parse_type_identifier_fail){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "constf ile";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = parser.try_parse_type_identifier().release();
  ASSERT_EQ(expr, nullptr);
}

TEST(ParserTest, try_parse_parameter_list_definition){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "jeden: const int, dwa: file";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = parser.try_parse_parameter_list_definition().release();
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->size(), 2);
  ASSERT_EQ(expr->at(0)->get_name(), "jeden");
  ASSERT_EQ(expr->at(1)->get_name(), "dwa");
  ASSERT_EQ(expr->at(0)->get_type()->get_is_const(), true);
  ASSERT_EQ(expr->at(1)->get_type()->get_is_const(), false);
  ASSERT_EQ(expr->at(0)->get_type()->get_type(), Type::Integer);
  ASSERT_EQ(expr->at(1)->get_type()->get_type(), Type::File);
}



TEST(ParserTest, try_parse_parameter_list_definition2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = parser.try_parse_parameter_list_definition().release();
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->size(), 0);
}

TEST(ParserTest, try_parse_match_expression){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "< 3 and > 1";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = dynamic_cast<TwoArgExpression<char>*>(parser.try_parse_match_expression().release());
  ASSERT_NE(expr, nullptr);
  auto &left = expr->get_left_expression();
  auto &right = expr->get_right_expression();
  ASSERT_EQ(expr->get_string_repr(), "MatchAndExpression");
  ASSERT_EQ(dynamic_cast<SingleArgExpression<char>*>(left.get())->get_expression_type(), ExpressionType::MatchLtExpression);
  ASSERT_EQ(dynamic_cast<SingleArgExpression<char>*>(right.get())->get_expression_type(), ExpressionType::MatchGtExpression);
}

TEST(ParserTest, try_parse_pattern1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = parser.try_parse_pattern().release();
  ASSERT_EQ(expr, nullptr);
}

TEST(ParserTest, try_parse_pattern2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "11, 2, _";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = parser.try_parse_pattern().release();
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->size(), 3);
  ASSERT_EQ(expr->at(0)->get_expression_type(), ExpressionType::IntegerLiteralExpression);
  ASSERT_EQ(expr->at(1)->get_expression_type(), ExpressionType::IntegerLiteralExpression);
  ASSERT_EQ(expr->at(2)->get_expression_type(), ExpressionType::UnderscoreExpression);
}

TEST(ParserTest, try_parse_match_line){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "11, 2, _ : expr";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = parser.try_parse_match_line().release();
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_pattern()->size(), 3);
  ASSERT_EQ(expr->get_pattern()->at(0)->get_expression_type(), ExpressionType::IntegerLiteralExpression);
  ASSERT_EQ(expr->get_pattern()->at(1)->get_expression_type(), ExpressionType::IntegerLiteralExpression);
  ASSERT_EQ(expr->get_pattern()->at(2)->get_expression_type(), ExpressionType::UnderscoreExpression);
  ASSERT_EQ(expr->get_expression()->get_expression_type(), ExpressionType::IdentifierExpression);
}


TEST(ParserTest, try_parse_match_block1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "{}";
  Lexer lex(s);
  ParserBase parser(lex);
  ASSERT_THROW(parser.try_parse_match_block().release(), SyntaxErrorException<char>);
}

TEST(ParserTest, try_parse_match_block2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "{11, 2, _ : expr, 32, 9, _ : expr2}";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = parser.try_parse_match_block().release();
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->at(0)->get_pattern()->size(), 3);
  ASSERT_EQ(expr->at(0)->get_pattern()->at(0)->get_expression_type(), ExpressionType::IntegerLiteralExpression);
  ASSERT_EQ(expr->at(0)->get_pattern()->at(1)->get_expression_type(), ExpressionType::IntegerLiteralExpression);
  ASSERT_EQ(expr->at(0)->get_pattern()->at(2)->get_expression_type(), ExpressionType::UnderscoreExpression);
  ASSERT_EQ(expr->at(0)->get_expression()->get_expression_type(), ExpressionType::IdentifierExpression);
  ASSERT_EQ(expr->at(1)->get_pattern()->size(), 3);
  ASSERT_EQ(expr->at(1)->get_pattern()->at(0)->get_expression_type(), ExpressionType::IntegerLiteralExpression);
  ASSERT_EQ(expr->at(1)->get_pattern()->at(1)->get_expression_type(), ExpressionType::IntegerLiteralExpression);
  ASSERT_EQ(expr->at(1)->get_pattern()->at(2)->get_expression_type(), ExpressionType::UnderscoreExpression);
  ASSERT_EQ(expr->at(1)->get_expression()->get_expression_type(), ExpressionType::IdentifierExpression);
}


TEST(ParserTest, try_parse_match_operation){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "match(x, y, z){11, 2, _ : expr, \n 32, 9, _ : expr2}";
  Lexer lex(s);
  ParserBase parser(lex);
  auto expr = dynamic_cast<MatchOperation<char>*>(parser.try_parse_match_operation().release());
  ASSERT_NE(expr, nullptr);
  ASSERT_EQ(expr->get_block()->at(0)->get_pattern()->size(), 3);
  ASSERT_EQ(expr->get_block()->at(0)->get_pattern()->at(0)->get_expression_type(), ExpressionType::IntegerLiteralExpression);
  ASSERT_EQ(expr->get_block()->at(0)->get_pattern()->at(1)->get_expression_type(), ExpressionType::IntegerLiteralExpression);
  ASSERT_EQ(expr->get_block()->at(0)->get_pattern()->at(2)->get_expression_type(), ExpressionType::UnderscoreExpression);
  ASSERT_EQ(expr->get_block()->at(0)->get_expression()->get_expression_type(), ExpressionType::IdentifierExpression);
  ASSERT_EQ(expr->get_block()->at(1)->get_pattern()->size(), 3);
  ASSERT_EQ(expr->get_block()->at(1)->get_pattern()->at(0)->get_expression_type(), ExpressionType::IntegerLiteralExpression);
  ASSERT_EQ(expr->get_block()->at(1)->get_pattern()->at(1)->get_expression_type(), ExpressionType::IntegerLiteralExpression);
  ASSERT_EQ(expr->get_block()->at(1)->get_pattern()->at(2)->get_expression_type(), ExpressionType::UnderscoreExpression);
  ASSERT_EQ(expr->get_block()->at(1)->get_expression()->get_expression_type(), ExpressionType::IdentifierExpression);
  ASSERT_EQ(expr->get_arguments()->size(), 3);

}

TEST(ParserTest, big_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << 
  R"(
fun fibonacci(n: const int): int {
    return match(n){
        0:  0,
        1:  1,
        _:  fibonacci(n - 1) + fibonacci(n - 2)
    };
}

fun fizzbuzz(n: const int): void {
    temp: int = 1;
    while(temp <= n){
        match(temp % 3, temp % 5){
            0, 0:   print("fizzbuzz"),
            0, _:   print("fizz"),
            _, 0:   print("buzz"),
            _, _:   print(to_string_int(temp))
        }
        temp = temp + 1;
    }
}

fun progressive_tax(salary: const float): float {   # not accurate
    tax_rate: float = match(salary){
        < 1000.0:   0.1,
        < 2500.0:   0.3,
        _:          0.5
    };
    return salary * tax_rate;
}

fun is_even(n: const int): bool {
    return match(n % 2){
        0:  true,
        1:  false
    };
}

fun is_even2(n: const int): bool {
    return match(n){
        is_even:    true,
        _:          false
    };
}

fun while_return_test(): void{
    number: int = 4;
    while(number > 0){
        if(number == 1){
            return;
        }
        print(to_str_int(number) | "\n");
        number = number - 1;
    }
}

fun print_file(filename: const str): void {
    handle: file = open_file(filename);
    if(bad_file(handle)){
        return;
    }
    else{
        line: str = read_line(handle);
        while(line != EOF_MARKER){
            print(line | "\n");
            line = read_line(handle);
        }
        close_file(handle);
    }
}

fun add_one(number: int): void {
    number = number + 1;
}

fun multiple_matches(): int{
    return match(1, 2, 3){
        is_even2 and is_even, is_even2, is_even2: 0,
        _, is_even2, _              : 1,
        _, _, _                     : 2
    };
}

fun main(): int{
    if(arguments_number() >= 1){
        print("Opening and printing file " | argument(0) | "\n");
        print_file(argument(0));
        print("Done!\n");
    } else {
        print("No filename provided as argument, skipping...\n");
    }

    

    # bad_variable: int = 14.33;
    # print(to_str_int(bad_variable));

    number: int = 6;
    print("Fibonacci[" | to_str_int(number) | "] number is " | to_str_int(fibonacci(number)) | ".\n");

    print("Checking if passing by reference works - printed below should be five (4 after incrementing)\n");
    do_dodania: int = 4;
    add_one(do_dodania);
    print(to_str_int(do_dodania) | "\n");
    #add_one(do_dodania + 1);                   #generuje error - dodanie do stalej (r-wartosci)
    #print(to_str_int(do_dodania) | "\n");


    print("2 + 3 * 2 = " | to_str_int(2+3*2) | " == 8?\n");

    if(number < 12 and is_even(number) or not is_even(number)){
        print("Logical precedence works!\n");
    }

    print("Tax for 1500zl = " | to_str_float(progressive_tax(1500.0)) | "\n");

    print("If multiple pattern elements and many levels of match calls work,\nthis should be a 1: " | to_str_int(multiple_matches()) | "\n");

    while_return_test(); 

    if(not is_even2(3)){
        print("ok\n");
    }

    return 0;
  }


    )";

  std::stringstream null_stream{};
  null_stream.setstate(std::ios_base::badbit);
  Lexer lex(s);
  CommentFilterLexer<char> filtered_lex{lex};
  Parser parser(filtered_lex);
  auto fun_vect = parser.parse();
  auto program = Program<char>(std::move(fun_vect));
  ASSERT_NO_THROW(ProgramTreePrinter<char>{null_stream}.print_program(program));
}

TEST(InterpreterTest, negation_sticking_test1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << 
  R"(
fun main(): int{
  if(not true == false){
    return 0;
  }
  return 1;
}
    )";

  std::stringstream null_stream{};
  null_stream.setstate(std::ios_base::badbit);
  Lexer lex(s);
  CommentFilterLexer<char> filtered_lex{lex};
  Parser parser(filtered_lex);
  auto program = std::make_unique<Program<char>>(parser.parse());
  Interpreter<char> inter{std::move(program), null_stream, {}};
  ASSERT_EQ(inter.run(), 0L);
}


TEST(InterpreterTest, negation_sticking_test2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << 
  R"(
fun main(): int{
  if(not 3 > 2){
    return 0;
  }
  return 1;
}
    )";

  std::stringstream null_stream{};
  null_stream.setstate(std::ios_base::badbit);
  Lexer lex(s);
  CommentFilterLexer<char> filtered_lex{lex};
  Parser parser(filtered_lex);
  auto program = std::make_unique<Program<char>>(parser.parse());
  Interpreter<char> inter{std::move(program), null_stream, {}};
  ASSERT_THROW(inter.run(), OperatorArgumentMismatchException<char>);
}


TEST(InterpreterTest, big_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << 
  R"(
fun fibonacci(n: const int): int {
    return match(n){
        0:  0,
        1:  1,
        _:  fibonacci(n - 1) + fibonacci(n - 2)
    };
}

fun fizzbuzz(n: const int): void {
    temp: int = 1;
    while(temp <= n){
        match(temp % 3, temp % 5){
            0, 0:   print("fizzbuzz"),
            0, _:   print("fizz"),
            _, 0:   print("buzz"),
            _, _:   print(to_string_int(temp))
        }
        temp = temp + 1;
    }
}

fun progressive_tax(salary: const float): float {   # not accurate
    tax_rate: float = match(salary){
        < 1000.0:   0.1,
        < 2500.0:   0.3,
        _:          0.5
    };
    return salary * tax_rate;
}

fun is_even(n: const int): bool {
    return match(n % 2){
        0:  true,
        1:  false
    };
}

fun is_even2(n: const int): bool {
    return match(n){
        is_even:    true,
        _:          false
    };
}

fun while_return_test(): void{
    number: int = 4;
    while(number > 0){
        if(number == 1){
            return;
        }
        print(to_str_int(number) | "\n");
        number = number - 1;
    }
}

fun print_file(filename: const str): void {
    handle: file = open_file(filename);
    if(bad_file(handle)){
        return;
    }
    else{
        line: str = read_line(handle);
        while(line != EOF_MARKER){
            print(line | "\n");
            line = read_line(handle);
        }
        close_file(handle);
    }
}

fun add_one(number: int): void {
    number = number + 1;
}

fun multiple_matches(): int{
    return match(1, 2, 3){
        is_even2 and is_even, is_even2, is_even2: 0,
        _, is_even2, _              : 1,
        _, _, _                     : 2
    };
}

fun main(): int{
    if(arguments_number() >= 1){
        print("Opening and printing file " | argument(0) | "\n");
        print_file(argument(0));
        print("Done!\n");
    } else {
        print("No filename provided as argument, skipping...\n");
    }

    

    # bad_variable: int = 14.33;
    # print(to_str_int(bad_variable));

    number: int = 6;
    print("Fibonacci[" | to_str_int(number) | "] number is " | to_str_int(fibonacci(number)) | ".\n");

    print("Checking if passing by reference works - printed below should be five (4 after incrementing)\n");
    do_dodania: int = 4;
    add_one(do_dodania);
    print(to_str_int(do_dodania) | "\n");
    #add_one(do_dodania + 1);                   #generuje error - dodanie do stalej (r-wartosci)
    #print(to_str_int(do_dodania) | "\n");


    print("2 + 3 * 2 = " | to_str_int(2+3*2) | " == 8?\n");

    if(number < 12 and is_even(number) or not is_even(number)){
        print("Logical precedence works!\n");
    }

    print("Tax for 1500zl = " | to_str_float(progressive_tax(1500.0)) | "\n");

    print("If multiple pattern elements and many levels of match calls work,\nthis should be a 1: " | to_str_int(multiple_matches()) | "\n");

    while_return_test(); 

    if(not is_even2(3)){
        print("ok\n");
    }

    if(3 == 4){
      print("");
    }
    if(3 != 4){
      print("");
    }
    
    if(-3 / 12 <= -20 or 3 == 3){
      print("");
    }

    if("" != EOF_MARKER){
      if(to_str_bool(true) == "true"){
        if(3 == to_int_float(to_float_int(3))){
          print("ok\n");
        }
      }
    }

    return 0;
  }
    )";

  std::stringstream null_stream{};
  null_stream.setstate(std::ios_base::badbit);
  Lexer lex(s);
  CommentFilterLexer<char> filtered_lex{lex};
  Parser parser(filtered_lex);
  auto program = std::make_unique<Program<char>>(parser.parse());
  Interpreter<char> inter{std::move(program), null_stream, {"./Makefile"}};
  ASSERT_NO_THROW(inter.run());

}