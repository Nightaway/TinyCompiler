#ifndef __TOKEN_H__
#define __TOKEN_H__

namespace tiny {
namespace internal { 
		
// TOKEN_LIST takes a list of 3 macros M, all of which satisfy the
// same signature M(name, string, precedence), where name is the
// symbolic token name, string is the corresponding syntactic symbol
// (or NULL, for literals), and precedence is the precedence (or 0).
// The parameters are invoked for token categories as follows:
//
//   T: Non-keyword tokens
//   K: Keyword tokens

// IGNORE_TOKEN is a convenience macro that can be supplied as
// an argument (at any position) for a TOKEN_LIST call. It does
// nothing with tokens belonging to the respective category.

#define IGNORE_TOKEN(name, string, precedence)

#define TOKEN_LIST(T, K)                                                \
  /* End of source indicator. */                                        \
  T(EOS, "EOS", 0)                                                      \
																																				\
  T(LPAREN, "(", 0)                                                     \
  T(RPAREN, ")", 0)                                                     \
  T(LBRACK, "[", 0)                                                     \
  T(RBRACK, "]", 0)                                                     \
  T(LBRACE, "{", 0)                                                     \
  T(RBRACE, "}", 0)                                                     \
  T(COLON, ":", 0)                                                      \
  T(SEMICOLON, ";", 0)                                                  \
  T(PERIOD, ".", 0)                                                     \
  T(CONDITIONAL, "?", 3)                                                \
  T(INC, "++", 0)                                                       \
  T(DEC, "--", 0)                                                       \
																																				\
  T(ASSIGN, "=", 2)                                                     \
  T(ASSIGN_BIT_OR, "|=", 2)                                             \
  T(ASSIGN_BIT_XOR, "^=", 2)                                            \
  T(ASSIGN_BIT_AND, "&=", 2)                                            \
  T(ASSIGN_SHL, "<<=", 2)                                               \
  T(ASSIGN_SAR, ">>=", 2)                                               \
  T(ASSIGN_ADD, "+=", 2)                                                \
  T(ASSIGN_SUB, "-=", 2)                                                \
  T(ASSIGN_MUL, "*=", 2)                                                \
  T(ASSIGN_DIV, "/=", 2)                                                \
  T(ASSIGN_MOD, "%=", 2)                                                \
																																				\
  /* Binary operators sorted by precedence. */                          \
  /* IsBinaryOp() relies on this block of enum values */                \
  /* being contiguous and sorted in the same order! */                  \
  T(COMMA, ",", 1)                                                      \
  T(OR, "||", 4)                                                        \
  T(AND, "&&", 5)                                                       \
  T(BIT_OR, "|", 6)                                                     \
  T(BIT_XOR, "^", 7)                                                    \
  T(BIT_AND, "&", 8)                                                    \
  T(SHL, "<<", 11)                                                      \
  T(SAR, ">>", 11)                                                      \
  T(ADD, "+", 12)                                                       \
  T(SUB, "-", 12)                                                       \
  T(MUL, "*", 13)                                                       \
  T(DIV, "/", 13)                                                       \
  T(MOD, "%", 13)                                                       \
																																				\
  /* Compare operators sorted by precedence. */                         \
  /* IsCompareOp() relies on this block of enum values */               \
  /* being contiguous and sorted in the same order! */                  \
  T(EQ, "==", 9)                                                        \
  T(NE, "!=", 9)                                                        \
  T(LT, "<", 10)                                                        \
  T(GT, ">", 10)                                                        \
  T(LTE, "<=", 10)                                                      \
  T(GTE, ">=", 10)                                                      \
																																				\
  /* Unary operators. */                                               	\
  /* IsUnaryOp() relies on this block of enum values */                 \
  /* being contiguous and sorted in the same order! */                  \
  T(NOT, "!", 0)                                                        \
  T(BIT_NOT, "~", 0)                                                    \
																																				\
  /* Keywords  */                    																		\
  K(VAR, "var", 0)      																								\
																																				\
	/* ID */                                            	  							\
	T(ID, "id", 0)																												\
																																				\
	/* Number */																													\
	T(NUMBER, "num", 0)        																						\
                                                                				\
  /* Illegal token - not able to scan. */                               \
  T(ILLEGAL, "ILLEGAL", 0)                                              \
																																				\
  /* Scanner-internal use only. */                                      \
  T(WHITESPACE, NULL, 0)


class Token {
public:
	#define T(name, string, precedence) name,
		enum Value {
			TOKEN_LIST(T, T)
			NUM_TOKENS
		};
	#undef T

  // Returns a string corresponding to the C++ token name
  // (e.g. "LT" for the token LT).
  static const char* Name(Value tok) {
    return name_[tok];
  }

  // Predicates
  static bool IsKeyword(Value tok) {
    return token_type[tok] == 'K';
  }

 	static bool IsAssignmentOp(Value tok) {
    return ASSIGN <= tok && tok <= ASSIGN_MOD;
  }

 	static bool IsBinaryOp(Value op) {
    return COMMA <= op && op <= MOD;
  }

 	static bool IsCompareOp(Value op) {
    return EQ <= op && op <= GTE;
  }

 	static bool IsOrderedRelationalCompareOp(Value op) {
    return op == LT || op == LTE || op == GT || op == GTE;
  }

  static bool IsEqualityOp(Value op) {
    return op == EQ;
  }

  static bool IsInequalityOp(Value op) {
    return op == NE;
  }

 	static bool IsArithmeticCompareOp(Value op) {
    return IsOrderedRelationalCompareOp(op) ||
        IsEqualityOp(op) || IsInequalityOp(op);
  }

  static Value NegateCompareOp(Value op) {
    switch (op) {
      case EQ: return NE;
      case NE: return EQ;
      case LT: return GTE;
      case GT: return LTE;
      case LTE: return GT;
      case GTE: return LT;
      default:
        UNREACHABLE();
        return op;
    }
  }

  static Value ReverseCompareOp(Value op) {
    switch (op) {
      case EQ: return EQ;// Whether there is a line terminator whitespace character after
  			// the current token, and  before the next. Does not count newlines
  			// inside multiline comments.
      case NE: return NE;
      case LT: return GT;
      case GT: return LT;
      case LTE: return GTE;
      case GTE: return LTE;
      default:
        UNREACHABLE();
        return op;
    }
  }

  static bool IsBitOp(Value op) {
    return (BIT_OR <= op && op <= SAR) || op == BIT_NOT;
  }

  static bool IsUnaryOp(Value op) {
    return (NOT <= op && op <= BIT_NOT) || op == ADD || op == SUB;
  }

  static bool IsCountOp(Value op) {
    return op == INC || op == DEC;
  }

  static bool IsShiftOp(Value op) {
    return (SHL <= op) && (op <= SAR);
  }

  // Returns a string corresponding to the JS token string
  // (.e., "<" for the token LT) or NULL if the token doesn't
  // have a (unique) string (e.g. an IDENTIFIER).
  static const char* String(Value tok) {
    return string_[tok];
  }

  // Returns the precedence > 0 for binary and compare
  // operators; returns 0 otherwise.
  static int Precedence(Value tok) {
    return precedence_[tok];
  }

 private:
  static const char* const name_[NUM_TOKENS];
  static const char* const string_[NUM_TOKENS];
  static const int8_t precedence_[NUM_TOKENS];
  static const char token_type[NUM_TOKENS];
};

}}

#endif
