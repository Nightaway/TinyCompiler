#include <tiny.h>
#include <cstring>

#include <gtest/gtest.h>

TEST(TokenizationTest, EmptySourceTest1) {
	char source[] = "";
	unsigned index = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(index, 0);
	EXPECT_EQ(tok.type, tiny::kTokenType_Empty);
	EXPECT_EQ(tok.wordtype, tiny::kTokenType_Undefined);
	EXPECT_EQ(tok.kwtype, tiny::kWord_Undefined);
	ASSERT_STREQ(tok.strv, kStrUndefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	EXPECT_EQ(tok.optype, tiny::kOperation_Undefined);
}

TEST(TokenizationTest, EmptySourceTest2) {
	char source[] = "		";
	unsigned index = 0;
	unsigned source_len = strlen(source);
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, source_len, index);
	EXPECT_EQ(index, source_len);
	EXPECT_EQ(tok.type, tiny::kTokenType_Empty);
	EXPECT_EQ(tok.wordtype, tiny::kTokenType_Undefined);
	EXPECT_EQ(tok.kwtype, tiny::kWord_Undefined);
	ASSERT_STREQ(tok.strv, kStrUndefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	EXPECT_EQ(tok.optype, tiny::kOperation_Undefined);
}

TEST(TokenizationTest, EmptySourceTest3) {
	char source[] = "	";
	unsigned index = 0;
	unsigned source_len = strlen(source);
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(index, source_len);
	EXPECT_EQ(tok.type, tiny::kTokenType_Empty);
	EXPECT_EQ(tok.wordtype, tiny::kTokenType_Undefined);
	EXPECT_EQ(tok.kwtype, tiny::kWord_Undefined);
	ASSERT_STREQ(tok.strv, kStrUndefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	EXPECT_EQ(tok.optype, tiny::kOperation_Undefined);
}

TEST(TokenizationTest, NumberTokenTest) {
	char source[] = " 1231231	";
	unsigned source_len = strlen(source);
	unsigned index = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(index, source_len - 1);
	EXPECT_EQ(tok.type, tiny::kTokenType_Word);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Num);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.optype, tiny::kOperation_Undefined);
	EXPECT_EQ(tok.intv, 1231231);
	ASSERT_STREQ(tok.strv, "1231231");
}

TEST(TokenizationTest, KeywordIfTokenTest) {
	char source[] = "	if	";
	unsigned source_len = strlen(source);
	unsigned index = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(index, source_len - 1);
	EXPECT_EQ(tok.type, tiny::kTokenType_Word);
	EXPECT_EQ(tok.wordtype, tiny::kWord_KeyWord);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_if);
	EXPECT_EQ(tok.optype, tiny::kOperation_Undefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "if");
}

TEST(TokenizationTest, KeywordThenTokenTest) {
	char source[] = "	then	";
	unsigned index = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Word);
	EXPECT_EQ(tok.wordtype, tiny::kWord_KeyWord);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_then);
	EXPECT_EQ(tok.optype, tiny::kOperation_Undefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "then");
}

TEST(TokenizationTest, KeywordElseTokenTest) {
	char source[] = "	else	";
	unsigned index = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Word);
	EXPECT_EQ(tok.wordtype, tiny::kWord_KeyWord);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_else);
	EXPECT_EQ(tok.optype, tiny::kOperation_Undefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "else");
}

TEST(TokenizationTest, OpAssignTokenTest) {
	char source[] = "	=	";
	unsigned index = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Op);
	EXPECT_EQ(tok.optype, tiny::kOperation_assign);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Undefined);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "=");
}

TEST(TokenizationTest, OpAddTokenTest) {
	char source[] = "+	";
	unsigned index = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Op);
	EXPECT_EQ(tok.optype, tiny::kOperation_add);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Undefined);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "+");
}

TEST(TokenizationTest, OpSubTokenTest) {
	char source[] = " -	";
	unsigned index = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Op);
	EXPECT_EQ(tok.optype, tiny::kOperation_sub);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Undefined);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "-");
}

TEST(TokenizationTest, OpMulTokenTest) {
	char source[] = "	*	";
	unsigned index = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Op);
	EXPECT_EQ(tok.optype, tiny::kOperation_mul);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Undefined);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "*");
}

TEST(TokenizationTest, OpDivTokenTest) {
	char source[] = "  /	";
	unsigned index = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Op);
	EXPECT_EQ(tok.optype, tiny::kOperation_div);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Undefined);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "/");
}

TEST(TokenizationTest, OpEqualTokenTest) {
	char source[] = "  ==	";
	unsigned index = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Op);
	EXPECT_EQ(tok.optype, tiny::kOperation_equal);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Undefined);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "==");
}

TEST(TokenizationTest, OpGreatTokenTest) {
	char source[] = "  >	";
	unsigned index = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Op);
	EXPECT_EQ(tok.optype, tiny::kOperation_greate);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Undefined);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, ">");
}

TEST(TokenizationTest, OpGreatThanTokenTest) {
	char source[] = "  >=	";
	unsigned index = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Op);
	EXPECT_EQ(tok.optype, tiny::kOperation_greatethan);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Undefined);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, ">=");
}

TEST(TokenizationTest, OpLessTokenTest) {
	char source[] = "  <	";
	unsigned index = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Op);
	EXPECT_EQ(tok.optype, tiny::kOperation_less);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Undefined);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "<");
}

TEST(TokenizationTest, OpLessThanTokenTest) {
	char source[] = "  <=	";
	unsigned index = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Op);
	EXPECT_EQ(tok.optype, tiny::kOperation_lessthan);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Undefined);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "<=");
}

TEST(TokenizationTest, KeywordIfThenEndTokenTest) {
	char source[] = 
		"if	 1 then "
		"  i = 5  "
		"end	";
	unsigned index = 0;
	unsigned loc     = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Word);
	EXPECT_EQ(tok.wordtype, tiny::kWord_KeyWord);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_if);

	loc += index;
	tok = t.GetToken(source + loc, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Word);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Num);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.intv, 1);
	ASSERT_STREQ(tok.strv, "1");

	loc += index;
	tok = t.GetToken(source + loc, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Word);
	EXPECT_EQ(tok.wordtype, tiny::kWord_KeyWord);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_then);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "then");

	loc += index;
	tok = t.GetToken(source + loc, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Word);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Id);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "i");

	loc += index;
	tok = t.GetToken(source + loc, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Op);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Undefined);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.optype, tiny::kOperation_assign);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "=");

	loc += index;
	tok = t.GetToken(source + loc, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Word);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Num);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.optype, tiny::kOperation_Undefined);
	EXPECT_EQ(tok.intv, 5);
	ASSERT_STREQ(tok.strv, "5");

	loc += index;
	tok = t.GetToken(source + loc, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Word);
	EXPECT_EQ(tok.wordtype, tiny::kWord_KeyWord);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_end);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "end");
}

TEST(TokenizationTest, Expression1TokenTest) {
	char source[] = 
		"i = 1 + 2";
	unsigned index = 0;
	unsigned loc     = 0;
	tiny::Tokenizer t;
	tiny::Token tok = t.GetToken(source, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Word);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Id);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.optype, tiny::kOperation_Undefined);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "i");

	loc += index;
	tok = t.GetToken(source + loc, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Op);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Undefined);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.optype, tiny::kOperation_assign);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "=");

	loc += index;
	tok = t.GetToken(source + loc, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Word);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Num);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.optype, tiny::kOperation_Undefined);
	EXPECT_EQ(tok.intv, 1);
	ASSERT_STREQ(tok.strv, "1");

	loc += index;
	tok = t.GetToken(source + loc, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Op);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Undefined);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.optype, tiny::kOperation_add);
	EXPECT_EQ(tok.intv, tiny::kNumUndefined);
	ASSERT_STREQ(tok.strv, "+");

	loc += index;
	tok = t.GetToken(source + loc, strlen(source), index);
	EXPECT_EQ(tok.type, tiny::kTokenType_Word);
	EXPECT_EQ(tok.wordtype, tiny::kWord_Num);
	EXPECT_EQ(tok.kwtype, tiny::kKeyWord_Undefined);
	EXPECT_EQ(tok.optype, tiny::kOperation_Undefined);
	EXPECT_EQ(tok.intv, 2);
	ASSERT_STREQ(tok.strv, "2");
}