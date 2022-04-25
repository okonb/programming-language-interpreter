#include <gtest/gtest.h>
#include <sstream>
#include "Lexer.hpp"

/*
class LexerTest : public ::testing::Test {
 protected:
  void SetUp() override {

  }
  Lexer lexer;
  public:
  LexerTest() : lexer(str) {};
};
*/

// Lexer tests
TEST(LexerTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}
/*
TEST(LexerTest, EqualsTest){
  std::stringstream s;
  s << "==";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Equals);
}
*/

TEST(LexerTest, Opening_parenth_test){
  std::stringstream s;
  s << "(";
  Lexer lex(s);
  EXPECT_EQ(lex.get_next_token().type, TokenType::Opening_parenth);
}

