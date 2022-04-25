#include <gtest/gtest.h>
#include <sstream>
#include "Lexer.hpp"

// Lexer tests

TEST(LexerTest, Opening_parenth_test){
  std::stringstream s;
  s << "(";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Opening_parenth);
}

TEST(LexerTest, Closing_parenth_test){
  std::stringstream s;
  s << ")";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Closing_parenth);
}

TEST(LexerTest, Opening_curly_test){
  std::stringstream s;
  s << "{";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Opening_curly);
}

TEST(LexerTest, Closing_curly_test){
  std::stringstream s;
  s << "}";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Closing_curly);
}

TEST(LexerTest, Colon_test){
  std::stringstream s;
  s << ":";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Colon);
}

TEST(LexerTest, Semicolon_test){
  std::stringstream s;
  s << ";";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Semicolon);
}

TEST(LexerTest, Comma_test){
  std::stringstream s;
  s << ",";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Comma);
}

TEST(LexerTest, Underscore_test){
  std::stringstream s;
  s << "_";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Underscore);
}

TEST(LexerTest, Assign_test){
  std::stringstream s;
  s << "=";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Assign);
}

TEST(LexerTest, Plus_test){
  std::stringstream s;
  s << "+";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Plus);
}

TEST(LexerTest, Minus_test){
  std::stringstream s;
  s << "-";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Minus);
}

TEST(LexerTest, Multiplication_test){
  std::stringstream s;
  s << "*";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Multiplication);
}

TEST(LexerTest, Division_test){
  std::stringstream s;
  s << "/";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Division);
}

TEST(LexerTest, Modulo_test){
  std::stringstream s;
  s << "%";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Modulo);
}

TEST(LexerTest, String_concat_test){
  std::stringstream s;
  s << "|";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::String_concat);
}

TEST(LexerTest, Gt_test){
  std::stringstream s;
  s << ">";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Gt);
}

TEST(LexerTest, Lt_test){
  std::stringstream s;
  s << "<";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Lt);
}

TEST(LexerTest, Gte_test){
  std::stringstream s;
  s << ">=";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Gte);
}

TEST(LexerTest, Lte_test){
  std::stringstream s;
  s << "<=";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Lte);
}

TEST(LexerTest, Equals_test){
  std::stringstream s;
  s << "==";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Equals);
}

TEST(LexerTest, Not_equals_test){
  std::stringstream s;
  s << "!=";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Not_equals);
}


TEST(LexerTest, Integer_literal_test1){
  std::stringstream s;
  s << "0";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_literal);
  int64_t value = std::get<int64_t>(lex.get_current_token().value().value);
  EXPECT_EQ(value, 0);
}

TEST(LexerTest, Integer_literal_test2){
  std::stringstream s;
  s << "420";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_literal);
  int64_t value = std::get<int64_t>(lex.get_current_token().value().value);
  EXPECT_EQ(value, 420);
}

TEST(LexerTest, Integer_literal_test3){
  std::stringstream s;
  s << "0233";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_literal);
  int64_t value = std::get<int64_t>(lex.get_current_token().value().value);
  EXPECT_EQ(value, 0);
}

TEST(LexerTest, Integer_literal_test4){
  std::stringstream s;
  s << "13786//fwo87fh";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_literal);
  int64_t value = std::get<int64_t>(lex.get_current_token().value().value);
  EXPECT_EQ(value, 13786);
}

TEST(LexerTest, Integer_literal_test5){
  std::stringstream s;
  s << "  233";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Integer_literal);
  int64_t value = std::get<int64_t>(lex.get_current_token().value().value);
  EXPECT_EQ(value, 233);
}

TEST(LexerTest, Integer_literal_test6){
  std::stringstream s;
  s << "\n  18446744073709551615 7544";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Error_token);
}

TEST(LexerTest, Floating_literal_test1){
  std::stringstream s;
  s << "1.23";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Floating_literal);
  double value = std::get<double>(lex.get_current_token().value().value);
  EXPECT_DOUBLE_EQ(value, 1.23);
}

TEST(LexerTest, Floating_literal_test2){
  std::stringstream s;
  s << "1.0";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Floating_literal);
  double value = std::get<double>(lex.get_current_token().value().value);
  EXPECT_DOUBLE_EQ(value, 1.0);
}

TEST(LexerTest, Floating_literal_test3){
  std::stringstream s;
  s << "0.01";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Floating_literal);
  double value = std::get<double>(lex.get_current_token().value().value);
  EXPECT_DOUBLE_EQ(value, 0.01);
}

TEST(LexerTest, Floating_literal_test4){
  std::stringstream s;
  s << "123456789.97654345678";
  Lexer lex(s);
  ASSERT_EQ(lex.get_next_token().type, TokenType::Floating_literal);
  double value = std::get<double>(lex.get_current_token().value().value);
  EXPECT_DOUBLE_EQ(value, 123456789.97654345678);
}
/*
TEST(LexerTest, _test){
  std::stringstream s;
  s << "";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::);
}

TEST(LexerTest, _test){
  std::stringstream s;
  s << "";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::);
}

TEST(LexerTest, _test){
  std::stringstream s;
  s << "";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::);
}

TEST(LexerTest, _test){
  std::stringstream s;
  s << "";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::);
}


*/