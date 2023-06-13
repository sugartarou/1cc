#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SELECT_INTEL  printf(".intel_syntax noprefix\n")
#define LABEL_MAIN    printf(".globl main\n")
#define HEADER_MAIN   printf("main:\n")

#define _MOV(a, b)    printf("  mov %s, %ld\n", a, b)
#define _ADD(a, b)    printf("  add %s, %ld\n", a, b)
#define _SUB(a, b)    printf("  sub %s, %ld\n", a, b)

#define MOV(a)        _MOV("rax", a)
#define ADD(a)        _ADD("rax", a)
#define SUB(a)        _SUB("rax", a)

#define RET          printf("  ret\n")

// トークンの種類
typedef enum {
  TK_RECEIVED,  // 記号
  TK_NUM,       // 整数トークン
  TK_EOF,       // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;

// トークン型
struct Token {
  TokenKind kind; // トークンの型
  Token *next;    // 次の入力トークン
  int val;        // kindがTK_NUMの場合、その数値
  char *str;      // トークン文字列
};

// 現在着目しているトークン
Token *token;

// エラーを報告するための関数
// printfと同じ引数を取る
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進めて
// trueを返す。それ以外の場合にはfalseを返す。
bool consume(char op) {
  if (token->kind != TK_RECEIVED || token->str[0] != op)
    return false;
  token = token->next;
  return true;
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進める。
// それ以外の場合にはエラーを報告する。
void expect(char op) {
  if (token->kind != TK_RECEIVED || token->str[0] != op)
    error("'%c'ではありません", op);
  token = token->next;
}

// 次のトークンが数値の場合、トークンを1つ読み進めてその数値を返す。
// それ以外の場合にはエラーを報告する。
int expect_number() {
  if (token->kind != TK_NUM)
    error("数値ではありません");
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

// 新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

// 入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    // 空白文字をスキップ
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      cur = new_token(TK_RECEIVED, cur, p++);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error("トークナイズできません");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

int main(int argc, char **argv){
  if(argc != 2){
    error("引数の個数が正しくありません\n");
    return 1;
  }

  // トークナイズする
  token = tokenize(argv[1]);

  // アセンブリの前半部分を出力
  SELECT_INTEL;
  LABEL_MAIN;
  HEADER_MAIN;

  // 式の最初は数値でなければならないので、それをチェックして
  // 最初のmov命令を出力
  MOV(expect_number());

  // 「+ <数値>」あるいは「- <数値>」というトークンの並びを消費しつつ
  // アセンブリを出力
  while (!at_eof()) {
    if (consume('+')) {
      ADD(expect_number());
      continue;
    }

    expect('-');
    SUB(expect_number());    
  }
  
  RET;
  return 0;
}
