#include "TBIGNUM.h"

int main(void) {
  printf("\n##### CALCULADORA DE INTEIROS IMPLEMENTADOS POR LISTA #####\n");
  printf("\nOperações disponíveis:\n\n  soma (+)\n  subtração (-)\n  multiplicação (*)\n  divisão (/)\n  resto (%%)\n");
  printf("\nVocê quer inserir os valores e operadores manualmente ou por meio de um arquivo texto? Digite '1' para manual ou '2' para arquivo:\n");
  int tipo;
  scanf("%d", &tipo);

  if (tipo == 1) {
    while (1) {
      char str1[1000], str2[1000], op;
      printf("Digite o numero a operacao e o outro numero ... ");
      scanf("%s", str1);
      scanf(" %c", &op);
      scanf("%s", str2);
      if ((op != '+') && (op != '-') && (op != '*') && (op != '/') &&
          (op != '%')) {
        printf("ERRO: operador invalido!\n");
        printf("Quer continuar (digite 0 para sair)? \n");
        scanf(" %c", &op);
        if (op == '0')
          return 0;
        continue;
      }
      TBIGNUM *num1 = TBIGNUM_converte(str1), *num2 = TBIGNUM_converte(str2),
              *resp;
      if (op == '+')
        resp = TBIGNUM_soma(num1, num2);
      else if (op == '-')
        resp = TBIGNUM_sub(num1, num2);
      else if (op == '*')
        resp = TBIGNUM_mult(num1, num2);
      else if (op == '/' && !(num2->tamanho == 1 && num2->prim->info == 0))
        resp = TBIGNUM_div(num1, num2);
      else if (op == '%' && !(num2->tamanho == 1 && num2->prim->info == 0))
        resp = TBIGNUM_resto(num1, num2);
      else {
        printf("NAO E POSSIVEL DIVIDIR POR ZERO!\n");
        TBIGNUM_libera(num1);
        TBIGNUM_libera(num2);
        continue;
      }

      printf("\n");
      TBIGNUM_imprime(num1);
      printf(" %c ", op);
      TBIGNUM_imprime(num2);
      printf(" = ");
      TBIGNUM_imprime(resp);
      printf("\n");

      TBIGNUM_libera(num1);
      TBIGNUM_libera(num2);
      TBIGNUM_libera(resp);

      printf("Quer continuar (digite 0 para sair)? ");
      scanf(" %c", &op);
      if (op == '0')
        return 0;
    }
  }
  if (tipo == 2) {
    while (1) {
      char arquivo[11];
      printf("Insira o nome do arquivo:\n");
      scanf("%s", arquivo);
      FILE *fp = fopen(arquivo, "r"), *fpo = fopen("Resultados.txt", "a");
      if (!fp || !fpo)
        exit(1);
      char str1[1000], str2[1000], op;
      int r1;
      r1 = fscanf(fp, "%s %c %s", str1, &op, str2);
      fprintf(fpo, "%s", "RESULTADOS:\n");

      while (r1 == 3) {
        if ((op != '+') && (op != '-') && (op != '*') && (op != '/') &&
            (op != '%')) {
          fprintf(fpo, "%c", '\n');
          fprintf(fpo, "%s", "ERRO: operador invalido!\n");
          r1 = fscanf(fp, "%s %c %s", str1, &op, str2);
        }
        TBIGNUM *num1 = TBIGNUM_converte(str1), *num2 = TBIGNUM_converte(str2),
                *resp;
        if (op == '+')
          resp = TBIGNUM_soma(num1, num2);
        else if (op == '-')
          resp = TBIGNUM_sub(num1, num2);
        else if (op == '*')
          resp = TBIGNUM_mult(num1, num2);
        else if (op == '/' && !(num2->tamanho == 1 && num2->prim->info == 0))
          resp = TBIGNUM_div(num1, num2);
        else if (op == '%' && !(num2->tamanho == 1 && num2->prim->info == 0))
          resp = TBIGNUM_resto(num1, num2);
        else {
          printf("NAO E POSSIVEL DIVIDIR POR ZERO!\n");
          TBIGNUM_libera(num1);
          TBIGNUM_libera(num2);
          continue;
        }

        TBIGNUM_imprime_arq(num1, fpo);
        fprintf(fpo, " %c ", op);
        TBIGNUM_imprime_arq(num2, fpo);
        fprintf(fpo, " %c ", '=');
        TBIGNUM_imprime_arq(resp, fpo);
        fprintf(fpo, "%c", '\n');

        TBIGNUM_libera(num1);
        TBIGNUM_libera(num2);
        TBIGNUM_libera(resp);

        r1 = fscanf(fp, "%s %c %s", str1, &op, str2);
      }
      printf("\nUm arquivo de saída com os resultados foi criado ou atualizado!\n");
      fclose(fp);

      printf("\nDeseja inserir outro arquivo? Digite '1' para Sim ou '2' para Não:\n");
      scanf(" %c", &op);
      if (op == '2')
        return 0;
    }
  }
}
