#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SELECT_INTEL  printf(".intel_syntax noprefix\n")
#define LABEL_MAIN    printf(".globl main\n")
#define HEADER_MAIN   printf("main:\n")

#define ERR(txt)      fprintf(stderr, txt)
#define ERR_CHAR(txt, c) fprintf(stderr, txt, c)

#define _MOV(a, b)    printf("  mov %s, %ld\n", a, b)
#define _ADD(a, b)    printf("  add %s, %ld\n", a, b)
#define _SUB(a, b)    printf("  sub %s, %ld\n", a, b)

#define MOV(a)        _MOV("rax", a)
#define ADD(a)        _ADD("rax", a)
#define SUB(a)        _SUB("rax", a)

#define RET          printf("  ret\n")

int main(int argc, char **argv){
  if(argc != 2){
    ERR("引数の個数が正しくありません\n");
    return 1;
  }

  char *p = argv[1];
  
  SELECT_INTEL;
  LABEL_MAIN;
  HEADER_MAIN;

  MOV(strtol(p, &p, 10));

  while(*p){
    if(*p == '+'){
      ADD(strtol((++p), &p, 10));
      continue;
    }

    if(*p == '-'){
      SUB(strtol((++p), &p, 10));
      continue;
    }
    
    ERR_CHAR("予期しない文字です: '%c'\n", *p);
  }
  
  RET;
  
  return 0;
}
