#include "TBIGNUM.h"

int main(void){
   while(1){
    char str1[1000], str2[1000], op;
    printf("Digite o numero a operacao e o outro numero ... ");
    scanf("%s", str1);
    scanf(" %c", &op);
    scanf("%s", str2);
    if((op != '+') && (op != '-') && (op != '*') && (op != '/') && (op != '%')){
      printf("ERRO: operador invalido!\n");
      printf("Quer continuar (digite 0 para sair)? \n"); 
      scanf(" %c", &op);
      if(op == '0') return 0;
      continue;
    }
    TBIGNUM *num1 = TBIGNUM_converte(str1), *num2 = TBIGNUM_converte(str2), *resp;
    if(op == '+') resp = TBIGNUM_soma(num1, num2);
    else if(op == '-') resp = TBIGNUM_sub(num1, num2);
    else if(op == '*') resp = TBIGNUM_mult(num1, num2);
    else if(op == '/' && !(num2->tamanho == 1 && num2->prim->info == 0))  resp = TBIGNUM_div(num1, num2);
    else if(op == '%' && !(num2->tamanho == 1 && num2->prim->info == 0))  resp = TBIGNUM_resto(num1, num2);
    else {
      printf("NAO E POSSIVEL DIVIDIR POR ZERO!\n");
      TBIGNUM_libera(num1);
      TBIGNUM_libera(num2);
      continue;
    }

    printf("%s %c %s = ", str1, op, str2);
    TBIGNUM_imprime(resp);
    printf("\n");
    
    TBIGNUM_libera(num1);
    TBIGNUM_libera(num2);
    TBIGNUM_libera(resp);
    
    
    printf("Quer continuar (digite 0 para sair)? "); 
    scanf(" %c", &op);
    if(op == '0') return 0;
  }
}