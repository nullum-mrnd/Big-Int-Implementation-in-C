#include "TBIGNUM.h"

//CRIAÇÃO

TBIGNUM* TBIGNUM_cria(void){
  TBIGNUM *novo = (TBIGNUM *) malloc(sizeof(TBIGNUM));
  novo->prim = NULL;
  novo->ultimo = NULL;
  novo->sinal = ' ';
  novo->tamanho = 0;
  return novo;
}

//IMPRESSÃO

void TBIGNUM_imprime(TBIGNUM *num){
  if(!num->prim) return;
  TNO *p = num->prim;
  if(num->sinal == '-') printf("-");
  while(p){
    printf("%d", p->info);
    p=p->prox;
  }
}

// INSERÇÃO

void TBIGNUM_ins_ini(TBIGNUM *num, int digito){
  TNO *x = (TNO *)malloc(sizeof(TNO));
  x->info = digito;
  if(!num->prim){
    num->ultimo = x;
  }else num->prim->ant = x;
  x->prox = num->prim;
  x->ant = NULL;
  num->prim = x;
  num->tamanho++;
}

void TBIGNUM_ins_fim(TBIGNUM *num, int digito){
   if(!num->prim){
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

void TBIGNUM_remove(TBIGNUM *num, int digito){
  TNO *p = num->prim;
  while((p) && (p->info != digito)) p = p->prox;
  if(!p) return;
  if(!p->ant){
    num->prim = p->prox;
    p->prox->ant = NULL;
  }else if(p == num->ultimo){
    p->ant->prox = p->prox;
    num->ultimo = p->ant;
  }else{
    p->ant->prox = p->prox;
    p->prox->ant = p->ant;
  }
  num->tamanho--;
  free(p);
}

// LIBERAÇÃO

void TBIGNUM_libera(TBIGNUM *num){
  TNO *p = num->prim, *q;
  while(p){
    q = p;
    p = p->prox;
    free(q);
  }
  free(num);
}

// CONVERSAO STR PRA LISTA

TBIGNUM* TBIGNUM_converte(char *str){
  int i = 0;
  TBIGNUM *num = TBIGNUM_cria();
  
  if(str[0] == '-') num->sinal = '-';
  else num->sinal = '+';

  while(str[i+1] != '\0' && str[i] == '0')  i++;
  
  // só vai começar a inserir a partir do primeiro digito diferente de zero para evitar que 1 e 001, por exemplo, tenham tamanhos diferentes
  
  for(i=i; ((i<strlen(str)) && (str[i] != '\0')); i++){
    int temp = str[i];
    if(str[i] >= 48 && str[i]<= 57){
      TBIGNUM_ins_fim(num, (str[i]-48));
    }
  }
  return num;
}

// SOMA

TBIGNUM* TBIGNUM_soma(TBIGNUM *num1, TBIGNUM *num2){
  TNO *p = num1->ultimo, *q = num2->ultimo;
  TBIGNUM *resp = TBIGNUM_cria();

  
  if(num1->sinal == '-' && num2->sinal == '+'){ // se o primeiro numero for negativo
    num1->sinal = '+';
    resp = TBIGNUM_sub(num2, num1);
    num1->sinal = '-';
    return resp;
    
  }else if (num1->sinal == '+' && num2->sinal == '-'){ // se o segundo numero for negativo
    num2->sinal = '+';
    resp = TBIGNUM_sub(num1, num2);
    num2->sinal = '-';
    return resp;
    
  }else if (num1->sinal == '-' && num2->sinal == '-'){ // se ambos forem negativos
    resp->sinal = '-';
    
  }
  
  int temp, vai_um = 0, aux_p, aux_q;
  
  while(p || q){
    
    if(p) aux_p = p->info;
    else aux_p = 0;
    if(q) aux_q = q->info;
    else aux_q = 0;
    
    temp = aux_p + aux_q + vai_um;
    
    if(vai_um) vai_um = 0;
    if(temp>9){
      TBIGNUM_ins_ini(resp, (temp - ((temp/10) * 10)));
      vai_um = temp/10;
    }else{
      TBIGNUM_ins_ini(resp, temp);
    }

    if(p) p = p->ant;
    if(q) q = q->ant;
  }
  if(vai_um) TBIGNUM_ins_ini(resp,vai_um);
  
  return resp;
}

// SUBTRACAO

TBIGNUM* TBIGNUM_sub(TBIGNUM *num1, TBIGNUM *num2){
  TNO *p = num1->ultimo, *q = num2->ultimo;
  TBIGNUM *resp = TBIGNUM_cria();
  
  if(num1->sinal == '-' && num2->sinal == '+'){
    num1->sinal = '+';
    resp = TBIGNUM_soma(num1,num2);
    num1->sinal = '-';
    resp->sinal = '-';
    return resp;
    
  }else if(num1->sinal == '-' && num2->sinal == '-'){
    num1->sinal = '+';
    num2->sinal = '+';
    resp = TBIGNUM_sub(num2,num1);
    num1->sinal = '-';
    num2->sinal = '-';
    return resp;
    
  }else if(num1->sinal == '+' && num2->sinal == '-'){
    num2->sinal = '+';
    resp = TBIGNUM_soma(num1,num2);
    num2->sinal = '-';
    return resp;
  }
  
  int temp, maior = compara_num(num1,num2), empresta = 0, aux_p, aux_q, debug = 0;
  
  while(p || q){
    
    // OS AUX_P E AUX_Q SERVEM PARA QUANDO UM DOS NÚMEROS NÃO POSSUIR MAIS DIGITOS
    // ELE SER SUBSTITUÍDO POR ZERO, POIS O PONTEIRO PARA O ANTERIOR NÃO SERIA MAIS
    // VÁLIDO (SEGM. FAULT)
    
    if(p) aux_p = p->info;
    else aux_p = 0;
    if(q) aux_q = q->info;
    else aux_q = 0;
    
    if(maior >= 0){
      if(aux_p < aux_q){
        if(empresta) temp = (aux_p - 1) - aux_q + 10;
        else temp = aux_p - aux_q + 10;
        TBIGNUM_ins_ini(resp,temp);
        empresta = 1;
      }else{
        if(empresta) temp = (aux_p - 1) - aux_q;
        else temp = aux_p - aux_q;
        TBIGNUM_ins_ini(resp,temp);
        empresta = 0;
      }
    }else{
      if(aux_q < aux_p){
        if(empresta) temp = (aux_q - 1) - aux_p + 10;
        else temp = aux_q - aux_p + 10;
        TBIGNUM_ins_ini(resp,temp);
        empresta = 1;
      }else{
        if(empresta) temp = (aux_q - 1) - aux_p;
        else temp = aux_q - aux_p;
        TBIGNUM_ins_ini(resp,temp);
        empresta = 0;
      }
    }

    if(p) p = p->ant;
    if(q) q = q->ant;
  }
  
  while(resp->prim->info == 0 && resp->prim != resp->ultimo) TBIGNUM_remove(resp, 0);
  if(maior == -1){
    resp->sinal = '-';
  }

  
  return resp;
}


// MULTIPLICACAO

TBIGNUM* TBIGNUM_mult(TBIGNUM *num1, TBIGNUM *num2){
  TBIGNUM *resp = TBIGNUM_cria(), *contador = TBIGNUM_copia(num2),
  *aux = TBIGNUM_cria();
  
  TBIGNUM_ins_ini(resp, 0);
  TBIGNUM_ins_ini(aux, 1);  
  
  while((contador->prim != contador->ultimo) || (contador->prim->info != 0)){
    resp = TBIGNUM_soma(resp, num1);
    contador = TBIGNUM_sub(contador, aux);
  }
  
  while(resp->prim->info == 0 && resp->prim != resp->ultimo) TBIGNUM_remove(resp, 0);

  if((num1->sinal == '-' && num2->sinal == '+') || (num2->sinal == '-' && num1->sinal == '+')){
    resp->sinal = '-';
  }
  
  return resp;
}

// DIVISAO

TBIGNUM* TBIGNUM_div(TBIGNUM *num1, TBIGNUM *num2){
  TBIGNUM *dividendo = TBIGNUM_copia(num1), *divisoes = TBIGNUM_cria(),
  *aux = TBIGNUM_cria();

    
  TBIGNUM_ins_ini(divisoes, 0);
  TBIGNUM_ins_ini(aux, 1);  

  while(compara_num(dividendo, num2) != -1){
    dividendo = TBIGNUM_sub(dividendo, num2);
    divisoes = TBIGNUM_soma(divisoes, aux);
  }

  // se apenas um dos numeros for negativo o resultado e negativo
  if((num1->sinal == '-' && num2->sinal == '+') || (num2->sinal == '-' && num1->sinal == '+')){
    divisoes->sinal = '-';
  }
  
  return divisoes;
}

// RESTO

TBIGNUM* TBIGNUM_resto(TBIGNUM *num1, TBIGNUM *num2){
  TBIGNUM *dividendo = TBIGNUM_copia(num1), *divisoes = TBIGNUM_cria(),
  *aux = TBIGNUM_cria();
  
  // se apenas um dos numeros for negativo o resultado e negativo
  if((num1->sinal == '-' && num2->sinal == '+') || (num2->sinal == '-' && num1->sinal == '+')){
    dividendo->sinal = '-';
  }
  
  TBIGNUM_ins_ini(divisoes, 0);
  TBIGNUM_ins_ini(aux, 1);  

  while(compara_num(dividendo, num2) != -1){
    dividendo = TBIGNUM_sub(dividendo, num2);
    divisoes = TBIGNUM_soma(divisoes, aux);
  }
  return dividendo;
}

// FUNCOES AUXILIARES

int compara_num(TBIGNUM *num1, TBIGNUM *num2){
  TNO *p = num1->prim, *q = num2->prim;
  
  if(num1->tamanho > num2->tamanho) return 1;
  if(num1->tamanho < num2->tamanho) return -1;
  p = num1->prim, q = num2->prim;
  while(p && q){
    if(p->info > q->info) return 1;
    if(p->info < q->info) return -1;
    p = p->prox;
    q = q->prox;
  }
  return 0;
}

TBIGNUM* TBIGNUM_copia(TBIGNUM *num){
  TBIGNUM *copia = TBIGNUM_cria();
  TNO *p = num->prim;
  while(p){
    TBIGNUM_ins_fim(copia, p->info);
    p = p->prox;
  }
  return copia;
  }
