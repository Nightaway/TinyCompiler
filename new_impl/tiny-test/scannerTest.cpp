#include <gtest/gtest.h>

#include <Scannner.h>

TEST(ScannnerTest, test1) {
	EXPECT_EQ(tiny::internal::Token::NUM_TOKENS, 15);
}