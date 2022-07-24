#include "TBIGNUM.h"

// CRIAÇÃO

TBIGNUM *TBIGNUM_cria(void) {
  TBIGNUM *novo = (TBIGNUM *)malloc(sizeof(TBIGNUM));
  novo->prim = NULL;
  novo->ultimo = NULL;
  novo->sinal = ' ';
  novo->tamanho = 0;
  return novo;
}

// IMPRESSÃO

void TBIGNUM_imprime(TBIGNUM *num) {
  if (!num->prim)
    return;
  TNO *p = num->prim;
  if (num->sinal == '-')
    printf("-");
  while (p) {
    printf("%d", p->info);
    p = p->prox;
  }
}

void TBIGNUM_imprime_arq(TBIGNUM *num, FILE *fp){
	if (!num->prim || !fp) return;
	TNO *p = num->prim;
	if (num->sinal == '-')
    fprintf(fp, "%c", num->sinal);
  while (p) {
    fprintf(fp, "%d", p->info);
    p = p->prox;
  }
}

// INSERÇÃO

void TBIGNUM_ins_ini(TBIGNUM *num, int digito) {
  TNO *x = (TNO *)malloc(sizeof(TNO));
  x->info = digito;
  if (!num->prim) {
    num->ultimo = x;
  } else
    num->prim->ant = x;
  x->prox = num->prim;
  x->ant = NULL;
  num->prim = x;
  num->tamanho++;
}

void TBIGNUM_ins_fim(TBIGNUM *num, int digito) {
  if (!num->prim) {
    TBIGNUM_ins_ini(num, digito);
    return;
  }
  TNO *p = num->ultimo;
  p->prox = (TNO *)malloc(sizeof(TNO));
  p->prox->info = digito;
  p->prox->prox = NULL;
  p->prox->ant = p;
  num->ultimo = p->prox;
  num->tamanho++;
}

// REMOÇÃO

void TBIGNUM_remove(TBIGNUM *num, int digito) {
  TNO *p = num->prim;
  while ((p) && (p->info != digito))
    p = p->prox;
  if (!p)
    return;
  if (!p->ant) {
    num->prim = p->prox;
    p->prox->ant = NULL;
  } else if (p == num->ultimo) {
    p->ant->prox = p->prox;
    num->ultimo = p->ant;
  } else {
    p->ant->prox = p->prox;
    p->prox->ant = p->ant;
  }
  num->tamanho--;
  free(p);
}

// LIBERAÇÃO

void TBIGNUM_libera(TBIGNUM *num) {
  TNO *p = num->prim, *q;
  while (p) {
    q = p;
    p = p->prox;
    free(q);
  }
  free(num);
}

// CONVERSAO STR PRA LISTA

TBIGNUM *TBIGNUM_converte(char *str) {
  int i = 0;
  TBIGNUM *num = TBIGNUM_cria();

  if (str[0] == '-')
    num->sinal = '-';
  else
    num->sinal = '+';

  while (str[i + 1] != '\0' && str[i] == '0')
    i++;

  // só vai começar a inserir a partir do primeiro digito diferente de zero para
  // evitar que 1 e 001, por exemplo, tenham tamanhos diferentes

  for (i = i; ((i < strlen(str)) && (str[i] != '\0')); i++) {
    int temp = str[i];
    if (str[i] >= 48 && str[i] <= 57) {
      TBIGNUM_ins_fim(num, (str[i] - 48));
    }
  }
  return num;
}

// SOMA

TBIGNUM *TBIGNUM_soma(TBIGNUM *num1, TBIGNUM *num2) {
  TNO *p = num1->ultimo, *q = num2->ultimo;
  TBIGNUM *resp = TBIGNUM_cria();

  if (num1->sinal == '-' && num2->sinal == '+') {
    num1->sinal = '+';
    resp = TBIGNUM_sub(num2, num1);
    num1->sinal = '-';
    return resp;

  } else if (num1->sinal == '+' && num2->sinal == '-') {
    num2->sinal = '+';
    resp = TBIGNUM_sub(num1, num2);
    num2->sinal = '-';
    return resp;

  } else if (num1->sinal == '-' &&
             num2->sinal == '-') {
    resp->sinal = '-';
  }

  int temp, vai_um = 0, aux_p, aux_q;

  while (p || q) {

    if (p)
      aux_p = p->info;
    else
      aux_p = 0;
    if (q)
      aux_q = q->info;
    else
      aux_q = 0;

    temp = aux_p + aux_q + vai_um;

    if (vai_um)
      vai_um = 0;
    if (temp > 9) {
      TBIGNUM_ins_ini(resp, (temp - ((temp / 10) * 10)));
      vai_um = temp / 10;
    } else {
      TBIGNUM_ins_ini(resp, temp);
    }

    if (p)
      p = p->ant;
    if (q)
      q = q->ant;
  }
  if (vai_um)
    TBIGNUM_ins_ini(resp, vai_um);

  return resp;
}

// SUBTRACAO

TBIGNUM *TBIGNUM_sub(TBIGNUM *num1, TBIGNUM *num2) {
  TNO *p = num1->ultimo, *q = num2->ultimo;
  TBIGNUM *resp = TBIGNUM_cria();

  if (num1->sinal == '-' && num2->sinal == '+') {
    num1->sinal = '+';
    resp = TBIGNUM_soma(num1, num2);
    num1->sinal = '-';
    resp->sinal = '-';
    return resp;

  } else if (num1->sinal == '-' && num2->sinal == '-') {
    num1->sinal = '+';
    num2->sinal = '+';
    resp = TBIGNUM_sub(num2, num1);
    num1->sinal = '-';
    num2->sinal = '-';
    return resp;

  } else if (num1->sinal == '+' && num2->sinal == '-') {
    num2->sinal = '+';
    resp = TBIGNUM_soma(num1, num2);
    num2->sinal = '-';
    return resp;
  }

  int temp, maior = compara_num(num1, num2), empresta = 0, aux_p, aux_q;

  while (p || q) {

    if (p)
      aux_p = p->info;
    else
      aux_p = 0;
    if (q)
      aux_q = q->info;
    else
      aux_q = 0;

    if (maior >= 0) {
      if (aux_p < aux_q || (empresta == 1 && aux_p - 1 < aux_q)) {
        if (empresta) {
          temp = (aux_p - 1) - aux_q + 10;
        } else
          temp = aux_p - aux_q + 10;
        TBIGNUM_ins_ini(resp, temp);
        empresta = 1;
      } else {
        if (empresta)
          temp = (aux_p - 1) - aux_q;
        else
          temp = aux_p - aux_q;
        TBIGNUM_ins_ini(resp, temp);
        empresta = 0;
      }
    } else {
      if (aux_q < aux_p || (empresta == 1 && aux_q - 1 < aux_p)) {
        if (empresta)
          temp = (aux_q - 1) - aux_p + 10;
        else
          temp = aux_q - aux_p + 10;
        TBIGNUM_ins_ini(resp, temp);
        empresta = 1;
      } else {
        if (empresta)
          temp = (aux_q - 1) - aux_p;
        else
          temp = aux_q - aux_p;
        TBIGNUM_ins_ini(resp, temp);
        empresta = 0;
      }
    }

    if (p)
      p = p->ant;
    if (q)
      q = q->ant;
  }

  while (resp->prim->info == 0 && resp->prim != resp->ultimo)
    TBIGNUM_remove(resp, 0);
  if (maior == -1) {
    resp->sinal = '-';
  }

  p = resp->prim;
  while (p)
    return resp;
}

// MULTIPLICACAO

TBIGNUM *TBIGNUM_mult(TBIGNUM *num1, TBIGNUM *num2) {
  TBIGNUM *resp = TBIGNUM_cria(), *aux, *aux2 = TBIGNUM_cria(), *temp;
  TNO *p, *q;
  TBIGNUM ***tabuada = Cria_tabuada();
  int contador_digito = 0, i, j;
  int contador_linha = 0;

  q = num2->ultimo;

  while (q) {
    p = num1->ultimo;
    contador_linha = 0;
    while (p) {
      if (contador_linha) {
        aux = TBIGNUM_copia(tabuada[p->info][q->info]);
        for (i = 0; i < contador_linha; i++) {
          TBIGNUM_ins_fim(aux, 0);
        }
      } else
        aux = TBIGNUM_copia(tabuada[p->info][q->info]);

      temp = aux2;
      aux2 = TBIGNUM_soma(aux2, aux);
      TBIGNUM_libera(temp);
      p = p->ant;
      contador_linha++;
      TBIGNUM_libera(aux);
    }

    if (contador_digito) {
      for (i = 0; i < contador_digito; i++)
        TBIGNUM_ins_fim(aux2, 0);
    }
    temp = resp;
    resp = TBIGNUM_soma(resp, aux2);
    TBIGNUM_libera(temp);

    temp = aux2;
    aux2 = TBIGNUM_sub(aux2, aux2);
    TBIGNUM_libera(temp);
    q = q->ant;
    contador_digito++;
  }

  if ((num1->sinal == '-' && num2->sinal == '+') ||
      (num2->sinal == '-' && num1->sinal == '+')) {
    resp->sinal = '-';
  }

  while (resp->prim != resp->ultimo && resp->prim->info == 0) {
    TBIGNUM_remove(resp, 0);
  }

  TBIGNUM_libera(aux2);
  libera_tabuada(tabuada);
  return resp;
}

// DIVISAO

TBIGNUM *TBIGNUM_div(TBIGNUM *num1, TBIGNUM *num2) {
  TBIGNUM *resp = TBIGNUM_cria(), *dividendo = TBIGNUM_cria(), *aux;

  if (compara_num(num1, num2) == -1) {
    TBIGNUM_ins_ini(resp, 0);
    TBIGNUM_libera(dividendo);
    return resp;
  }

  TBIGNUM **vet = Cria_mult_vet(num2);
  TNO *p = num1->prim;
  int i;

  TBIGNUM_ins_fim(dividendo, p->info);

  while (compara_num(dividendo, num2) == -1 && p->prox) {
    p = p->prox;
    TBIGNUM_ins_fim(dividendo, p->info);
  }

  while (p) {

    if (compara_num(dividendo, num2) == -1)
      TBIGNUM_ins_fim(dividendo, p->info);

    for (i = 0; i < 11 && (compara_num(vet[i], dividendo) != 1); i++)
      ;

    if (i == 11) { // nenhum numero em vet é maior ou igual ao dividendo(vai pra prox iteração)
      TBIGNUM_ins_fim(dividendo, p->info);
    } else if (compara_num(vet[i], dividendo) == 1) { // vet[i] MAIOR QUE dividendo
      aux = dividendo;
      dividendo = TBIGNUM_sub(dividendo, vet[i - 1]);
      TBIGNUM_ins_fim(resp, i - 1);
      TBIGNUM_libera(aux);
    } else if (compara_num(vet[i], dividendo) == 0) { // vet[i] IGUAl AO dividendo
      aux = dividendo;
      dividendo = TBIGNUM_sub(dividendo, vet[i]);
      TBIGNUM_ins_fim(resp, i);
      TBIGNUM_libera(aux);
    }
    p = p->prox;
  }

  TBIGNUM_libera(dividendo);
  for (i = 0; i < 11; i++) {
    TBIGNUM_libera(vet[i]);
  }
  free(vet);

  if ((num1->sinal == '-' && num2->sinal == '+') || (num2->sinal == '-' && num1->sinal == '+')) {
    resp->sinal = '-';
  }

  return resp;
}

// RESTO

TBIGNUM *TBIGNUM_resto(TBIGNUM *num1, TBIGNUM *num2) {
  TBIGNUM *dividendo, *aux;

  if (compara_num(num1, num2) == -1) {
    dividendo = TBIGNUM_copia(num1);
    return dividendo;
  }

  dividendo = TBIGNUM_cria();

  TBIGNUM **vet = Cria_mult_vet(num2);
  TNO *p = num1->prim;
  int i;

  TBIGNUM_ins_fim(dividendo, p->info);

  while (compara_num(dividendo, num2) == -1 && p->prox) {
    p = p->prox;
    TBIGNUM_ins_fim(dividendo, p->info);
  }

  while (p) {

    if (compara_num(dividendo, num2) == -1)
      TBIGNUM_ins_fim(dividendo, p->info);

    for (i = 0; i < 11 && (compara_num(vet[i], dividendo) != 1); i++)
      ;

    if (i == 11) { // nenhum numero em vet é maior ou igual ao dividendo(vai pra
                   // prox iteração)
      TBIGNUM_ins_fim(dividendo, p->info);
    } else if (compara_num(vet[i], dividendo) ==
               1) { // vet[i] MAIOR QUE dividendo
      aux = dividendo;
      dividendo = TBIGNUM_sub(dividendo, vet[i - 1]);
      TBIGNUM_libera(aux);
    } else if (compara_num(vet[i], dividendo) ==
               0) { // vet[i] IGUAl AO dividendo
      aux = dividendo;
      dividendo = TBIGNUM_sub(dividendo, vet[i]);
      TBIGNUM_libera(aux);
    }
    p = p->prox;
  }

  for (i = 0; i < 11; i++) {
    TBIGNUM_libera(vet[i]);
  }
  free(vet);

  return dividendo;
}

// FUNCOES AUXILIARES

//Compara dois números e devolve 1 (se num1 > num2), -1 (se num2 > num1) ou 0 (se ambos forem iguais)
int compara_num(TBIGNUM *num1, TBIGNUM *num2) {
  TNO *p = num1->prim, *q = num2->prim;

  if (num1->tamanho > num2->tamanho)
    return 1;
  if (num1->tamanho < num2->tamanho)
    return -1;
  p = num1->prim, q = num2->prim;
  while (p && q) {
    if (p->info > q->info)
      return 1;
    if (p->info < q->info)
      return -1;
    p = p->prox;
    q = q->prox;
  }
  return 0;
}

TBIGNUM *TBIGNUM_copia(TBIGNUM *num) {
  TBIGNUM *copia = TBIGNUM_cria();
  TNO *p = num->prim;
  while (p) {
    TBIGNUM_ins_fim(copia, p->info);
    p = p->prox;
  }
  return copia;
}

// converte inteiro pra TBIGNUM
TBIGNUM *IntToBignum(int x) {
  TBIGNUM *resp = TBIGNUM_cria();
  int aux;
  if (x == 0)
    TBIGNUM_ins_ini(resp, x);
  while (x) {
    aux = x;
    TBIGNUM_ins_ini(resp, (x - (aux / 10) * 10));
    x /= 10;
  }
  return resp;
}

TBIGNUM ***Cria_tabuada(void) {
  int i, j;
  TBIGNUM ***tabuada = (TBIGNUM ***)malloc(10 * sizeof(TBIGNUM **));
  for (i = 0; i < 10; i++) {
    tabuada[i] = (TBIGNUM **)malloc(10 * sizeof(TBIGNUM *));
  }
  for (i = 0; i < 10; i++) {
    for (j = 0; j < 10; j++) {
      tabuada[i][j] = IntToBignum(i * j);
    }
  }
  return tabuada;
}

void libera_tabuada(TBIGNUM ***tabuada) {
  int i, j;
  for (i = 0; i < 10; i++) {
    for (j = 0; j < 10; j++) {
      TBIGNUM_libera(tabuada[i][j]);
    }
    free(tabuada[i]);
  }
  free(tabuada);
}

//Cria um vetor que vai de num * 0 a num *10 para auxiliar na divisão longa
TBIGNUM **Cria_mult_vet(TBIGNUM *num) {
  TBIGNUM **vet = (TBIGNUM **)malloc(11 * sizeof(TBIGNUM *)), *temp;
  int i;

  for (i = 0; i < 11; i++) {
    temp = IntToBignum(i);
    vet[i] = TBIGNUM_mult(num, temp);
    TBIGNUM_libera(temp);
  }
  return vet;
}

void libera_mult_vet(TBIGNUM **vet) {
  int i;
  for (i = 0; i < 11; i++) {
    TBIGNUM_libera(vet[i]);
  }
  free(vet);
}
