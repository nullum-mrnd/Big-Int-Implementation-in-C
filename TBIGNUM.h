#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no{
  int info;
  struct no *prox;
  struct no *ant;
}TNO;

typedef struct bignum{
  TNO *prim;
  TNO *ultimo;
  char sinal;
  int tamanho;
}TBIGNUM;


TBIGNUM* TBIGNUM_soma(TBIGNUM *num1, TBIGNUM *num2);
TBIGNUM* TBIGNUM_sub(TBIGNUM *num1, TBIGNUM *num2);
TBIGNUM* TBIGNUM_mult(TBIGNUM *num1, TBIGNUM *num2);
TBIGNUM* TBIGNUM_div(TBIGNUM *num1, TBIGNUM *num2);
TBIGNUM* TBIGNUM_resto(TBIGNUM *num1, TBIGNUM *num2);
int compara_num(TBIGNUM *num1, TBIGNUM *num2);
TBIGNUM* TBIGNUM_converte(char *str);


TBIGNUM* TBIGNUM_cria(void);
void TBIGNUM_ins_ini(TBIGNUM *num, int digito);
void TBIGNUM_ins_fim(TBIGNUM *num, int digito);
void TBIGNUM_libera(TBIGNUM *num);
void TBIGNUM_imprime(TBIGNUM *num);
void TBIGNUM_imprime_rev(TBIGNUM *num);
void TBIGNUM_remove(TBIGNUM *num, int digito);
TBIGNUM* TBIGNUM_copia(TBIGNUM *num);