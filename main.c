#include <stdio.h>
#include <stdlib.h>

#define SELECT_INTEL ".intel_syntax noprefix"
#define LABEL_MAIN ".globl main"
#define HEADER_MAIN "main:"

//#define MOV(a, b) "  mov" a b
#define RET       "  ret"

int main(int argc, char **argv){
  if(argc != 2){
    fprintf(stderr, "引数の個数が正しくありません\n");
    return 1;
  }
  
  printf("%s\n", SELECT_INTEL);
  printf("%s\n", LABEL_MAIN);
  printf("%s\n", HEADER_MAIN);
  printf("%s %d\n", "  mov rax, ", atoi(argv[1]));
  printf("%s\n", RET);
  
  return 0;
}
