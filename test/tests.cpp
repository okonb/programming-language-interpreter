#include <gtest/gtest.h>
#include <sstream>
#include "Lexer.hpp"

// Lexer tests

TEST(LexerTest, Opening_parenth_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "(";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Opening_parenth);
}

TEST(LexerTest, Closing_parenth_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << ")";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Closing_parenth);
}

TEST(LexerTest, Opening_curly_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "{";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Opening_curly);
}

TEST(LexerTest, Closing_curly_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "}";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Closing_curly);
}

TEST(LexerTest, Colon_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << ":";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Colon);
}

TEST(LexerTest, Semicolon_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << ";";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Semicolon);
}

TEST(LexerTest, Comma_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << ",";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Comma);
}

TEST(LexerTest, Underscore_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "_";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Underscore);
}

TEST(LexerTest, Assign_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "=";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Assign);
}

TEST(LexerTest, Plus_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "+";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Plus);
}

TEST(LexerTest, Minus_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "-";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Minus);
}

TEST(LexerTest, Multiplication_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "*";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Multiplication);
}

TEST(LexerTest, Division_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "/";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Division);
}

TEST(LexerTest, Modulo_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "%";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Modulo);
}

TEST(LexerTest, String_concat_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "|";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::String_concat);
}

TEST(LexerTest, Gt_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << ">";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Gt);
}

TEST(LexerTest, Lt_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "<";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Lt);
}

TEST(LexerTest, Gte_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << ">=";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Gte);
}

TEST(LexerTest, Lte_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "<=";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Lte);
}

TEST(LexerTest, Equals_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "==";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Equals);
}

TEST(LexerTest, Not_equals_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "!=";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Not_equals);
}

TEST(LexerTest, Integer_type_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "int";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Integer_type);
}

TEST(LexerTest, Floating_type_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "float";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Floating_type);
}

TEST(LexerTest, String_type_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "str";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::String_type);
}

TEST(LexerTest, File_type_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "file";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::File_type);
}

TEST(LexerTest, Bool_type_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "bool";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Bool_type);
}

TEST(LexerTest, Void_type_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "void";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Void_type);
}

TEST(LexerTest, Return_keywd_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "return";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Return_keywd);
}

TEST(LexerTest, Function_keywd_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "fun";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Function_keywd);
}

TEST(LexerTest, If_keywd_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "if";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::If_keywd);
}

TEST(LexerTest, Else_keywd_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "else";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Else_keywd);
}

TEST(LexerTest, While_keywd_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "while";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::While_keywd);
}

TEST(LexerTest, Const_keywd_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "const";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Const_keywd);
}

TEST(LexerTest, Match_keywd_test){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "match";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Match_keywd);
}


TEST(LexerTest, Integer_literal_test1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "0";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_literal);
  int64_t value = std::get<int64_t>(lex.get_current_token().value().value);
  EXPECT_EQ(value, 0);
}

TEST(LexerTest, Integer_literal_test2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "420";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_literal);
  int64_t value = std::get<int64_t>(lex.get_current_token().value().value);
  EXPECT_EQ(value, 420);
}

TEST(LexerTest, Integer_literal_test3){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "0233";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_literal);
  int64_t value = std::get<int64_t>(lex.get_current_token().value().value);
  EXPECT_EQ(value, 0);
}

TEST(LexerTest, Integer_literal_test4){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "13786//fwo87fh";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_literal);
  int64_t value = std::get<int64_t>(lex.get_current_token().value().value);
  EXPECT_EQ(value, 13786);
}

TEST(LexerTest, Integer_literal_test5){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "  233";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_literal);
  int64_t value = std::get<int64_t>(lex.get_current_token().value().value);
  EXPECT_EQ(value, 233);
}

TEST(LexerTest, Integer_literal_test6){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "\n  18446744073709551615 7544";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Error_token);
}

TEST(LexerTest, Floating_literal_test1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "1.23";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Floating_literal);
  double value = std::get<double>(lex.get_current_token().value().value);
  EXPECT_DOUBLE_EQ(value, 1.23);
}

TEST(LexerTest, Floating_literal_test2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "1.0";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Floating_literal);
  double value = std::get<double>(lex.get_current_token().value().value);
  EXPECT_DOUBLE_EQ(value, 1.0);
}

TEST(LexerTest, Floating_literal_test3){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "0.01";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Floating_literal);
  double value = std::get<double>(lex.get_current_token().value().value);
  EXPECT_DOUBLE_EQ(value, 0.01);
}

TEST(LexerTest, Floating_literal_test4){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "123456789.97654345678";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Floating_literal);
  double value = std::get<double>(lex.get_current_token().value().value);
  EXPECT_DOUBLE_EQ(value, 123456789.97654345678);
}

TEST(LexerTest, Floating_literal_test5){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "0123.3123.2321";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_literal);
  int64_t value = std::get<int64_t>(lex.get_current_token().value().value);
  EXPECT_DOUBLE_EQ(value, 0);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Floating_literal);
  double value2 = std::get<double>(lex.get_current_token().value().value);
  EXPECT_DOUBLE_EQ(value2, 123.3123);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Error_token);
}

TEST(LexerTest, Identifier_test1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "Identyfikator_zmiennej";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  std::string value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, s.str());
}

TEST(LexerTest, Identifier_test2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "startswithsmallletterbutalsonumbers2132andunderscores______";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  std::string value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, s.str());
}

TEST(LexerTest, Identifier_test3){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "_startswithunderscore";
  Lexer lex(s);
  ASSERT_NE(lex.get_next_token().type, TokenType::Identifier);
  EXPECT_EQ(lex.get_current_token()->type, TokenType::Underscore);
}

TEST(LexerTest, Identifier_test4){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "288startswithnumber";
  Lexer lex(s);
  ASSERT_NE(lex.get_next_token().type, TokenType::Identifier);
  EXPECT_EQ(lex.get_current_token()->type, TokenType::Integer_literal);
}

TEST(LexerTest, Comment_test1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "#svjniovjns\n";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Comment);
  std::string value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "svjniovjns");
}

TEST(LexerTest, Comment_test2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "# j dn9 7h9hw9o__(0/v.;vvnjsnvjdv ";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Comment);
  std::string value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, " j dn9 7h9hw9o__(0/v.;vvnjsnvjdv ");
}

TEST(LexerTest, Comment_test3){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "   # sovowivvvvvvovkkkkd\\dfsx\t\n";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Comment);
  std::string value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, " sovowivvvvvvovkkkkd\\dfsx\t");
  ASSERT_EQ(lex.get_next_token().type, TokenType::ETX_token);
}

TEST(LexerTest, String_literal_test1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "\"   # sovowivvvvvvovkkkkd\\nfsx\t\"\n";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::String_literal);
  std::string value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "   # sovowivvvvvvovkkkkd\nfsx\t");
  ASSERT_EQ(lex.get_next_token().type, TokenType::ETX_token);
}
//sfvibfiwbvihwbwbfiuwfiuhwifuhwiufhiuwhfiuhweiufhiuwhefiuhweiufhiuewhfiuhewiufhuwiehfiuhwiehfuhweiuhfiewhfiuwehfiwue
TEST(LexerTest, String_literal_test_emptystr){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "\"\"";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::String_literal);
  std::string value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "");
  ASSERT_EQ(lex.get_next_token().type, TokenType::ETX_token);
}

TEST(LexerTest, String_literal_test_newlinebeforequot){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "\"?\n\"";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Error_token);
}

TEST(LexerTest, String_literal_test_ETXbeforeendquot){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "\"svvs";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Error_token);
  ASSERT_EQ(lex.get_next_token().type, TokenType::ETX_token);
}

TEST(LexerTest, Compound_test1){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "match(x % 42, y % 22){\r\nisEven, 11: 12 + x,\r\n0, _: to_int(\"3\"),\r\n_, _: do_calcs(x)};";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Match_keywd);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Opening_parenth);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  std::string value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "x");
  ASSERT_EQ(lex.get_next_token().type, TokenType::Modulo);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_literal);
  int64_t value2 = std::get<int64_t>(lex.get_current_token().value().value);
  EXPECT_EQ(value2, 42);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Comma);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "y");
  ASSERT_EQ(lex.get_next_token().type, TokenType::Modulo);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_literal);
  value2 = std::get<int64_t>(lex.get_current_token().value().value);
  EXPECT_EQ(value2, 22);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Closing_parenth);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Opening_curly);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "isEven");
  ASSERT_EQ(lex.get_next_token().type, TokenType::Comma);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_literal);
  value2 = std::get<int64_t>(lex.get_current_token().value().value);
  EXPECT_EQ(value2, 11);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Colon);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_literal);
  value2 = std::get<int64_t>(lex.get_current_token().value().value);
  EXPECT_EQ(value2, 12);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Plus);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "x");
  ASSERT_EQ(lex.get_next_token().type, TokenType::Comma);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_literal);
  value2 = std::get<int64_t>(lex.get_current_token().value().value);
  EXPECT_EQ(value2, 0);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Comma);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Underscore);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Colon);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "to_int");
  ASSERT_EQ(lex.get_next_token().type, TokenType::Opening_parenth);
  ASSERT_EQ(lex.get_next_token().type, TokenType::String_literal);
  value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "3");
  ASSERT_EQ(lex.get_next_token().type, TokenType::Closing_parenth);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Comma);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Underscore);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Comma);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Underscore);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Colon);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "do_calcs");
  ASSERT_EQ(lex.get_next_token().type, TokenType::Opening_parenth);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "x");
  ASSERT_EQ(lex.get_next_token().type, TokenType::Closing_parenth);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Closing_curly);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Semicolon);

  ASSERT_EQ(lex.get_next_token().type, TokenType::ETX_token);
}

TEST(LexerTest, Compound_test2){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "zmienna: float = 420.69;\nfun funkcja(arg1: float, arg2: int): void{\nzmienna = zmienna * arg1 + arg2;} #komentarz\n\n\t";
  Lexer lex(s);
  
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  std::string value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "zmienna");
  ASSERT_EQ(lex.get_next_token().type, TokenType::Colon);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Floating_type);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Assign);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Floating_literal);
  double value3 = std::get<double>(lex.get_current_token().value().value);
  EXPECT_DOUBLE_EQ(value3, 420.69);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Semicolon);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Function_keywd);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "funkcja");
  ASSERT_EQ(lex.get_next_token().type, TokenType::Opening_parenth);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "arg1");
  ASSERT_EQ(lex.get_next_token().type, TokenType::Colon);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Floating_type);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Comma);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "arg2");
  ASSERT_EQ(lex.get_next_token().type, TokenType::Colon);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_type);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Closing_parenth);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Colon);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Void_type);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Opening_curly);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "zmienna");
  ASSERT_EQ(lex.get_next_token().type, TokenType::Assign);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "zmienna");
  ASSERT_EQ(lex.get_next_token().type, TokenType::Multiplication);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "arg1");
  ASSERT_EQ(lex.get_next_token().type, TokenType::Plus);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Identifier);
  value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "arg2");
  ASSERT_EQ(lex.get_next_token().type, TokenType::Semicolon);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Closing_curly);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Comment);
  value = std::get<std::string>(lex.get_current_token().value().value);
  EXPECT_EQ(value, "komentarz");
  ASSERT_EQ(lex.get_next_token().type, TokenType::ETX_token);
  ASSERT_EQ(lex.get_next_token().type, TokenType::ETX_token);
  ASSERT_EQ(lex.get_next_token().type, TokenType::ETX_token);
}

TEST(LexerTest, Repetition_tests){
  std::stringstream s;
  s.unsetf(std::ios::skipws);
  s << "intt floatt floaty voidd !-nott";
  Lexer lex(s);
  
  ASSERT_NE(lex.get_next_token().type, TokenType::Integer_type);
  ASSERT_NE(lex.get_next_token().type, TokenType::Floating_type);
  ASSERT_NE(lex.get_next_token().type, TokenType::Floating_type);
  ASSERT_NE(lex.get_next_token().type, TokenType::Void_type);
  ASSERT_NE(lex.get_next_token().type, TokenType::Not_equals);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Minus);
  ASSERT_NE(lex.get_next_token().type, TokenType::Not);
}
