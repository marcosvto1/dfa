#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FINAL 1
#define INICIAL 0
#define LENDO_0 0
#define LENDO_1 1
#define NFINAL 2
#define DUAL_ESTADO 3

struct Auto{       
       char *estado;
       int tipo_estado;      
       struct Auto *ponteiro_estado;
}typedef Automato;

//global vars
Automato __dfa_inicio;
Automato __dfa;
int __contador_estado_final=0;
int __estado_dual=0;
int __tam_col=0;

int __get_qtd_linhas(FILE *fp){
   char c;
   int __contador_linha=0;
  while((c = getc(fp) ) != EOF){         
       if(c == '\n'){          
          __contador_linha++;
       }          
  } 
   return __contador_linha; 
}


int* buscarEstadoFinal(){
    int *p = (int*) malloc(2*sizeof(int));
    int s[4];
   return p;
}

void listVectorEstados(Automato estado[],int qtd_linhas){
      int linha;
      printf("entrou aqui\n");
     for(linha = 0 ; linha < qtd_linhas ; linha++){
 
           printf(" estaddo ->%s\n",estado[linha].estado);  

          // printf(" Filho 0 \t%s\n ",estado[linha].ponteiro_estado[LENDO_0].estado); 
         //  printf("Filho 1 \t%s\n ",estado[linha].ponteiro_estado[LENDO_1].estado); 
           printf("tipo estado  \t%d\n ",estado[linha].tipo_estado);
     }
}

Automato* getEstados(char mat_estado[][__tam_col],int qtd_linhas){
   Automato *estado;
   estado = (Automato*)malloc(qtd_linhas*sizeof(Automato));
  int linha;
  int col;
  int contador=0;

    for(linha = 0 ; linha < qtd_linhas ; linha++){
           
           printf("%c%c\n",mat_estado[linha][0], mat_estado[linha][1]);   
           estado[contador].estado = (char*)malloc(2*sizeof(char));
           estado[contador].estado[LENDO_0] = mat_estado[linha][0];
           estado[contador].estado[LENDO_1] = mat_estado[linha][1];
           contador++;
    } 
   
    return estado;
}

int compara_estado(Automato estado[],int qtd_linhas, char *validation){
  int i;
    printf("validation ->%s\n",validation );

     for(i=0; i< qtd_linhas; i++){
         
           if(strcasecmp(validation, estado[i].estado) == 0){
                 printf("entrou aqui na compara_estado %d\n", i);
                 return i;
                 break;
           }

     }
  return -1;
}

int setEstadoInicial(FILE *fp , Automato estado[] ,int qtd_linhas ){
      int linha=0;
int col=0;
int aux= 0;
int contador=0;
char *vector = (char*)malloc(1*sizeof(char));
int counter_realloc = 1;
char c;
 rewind(fp);
    while((c = getc(fp) ) != EOF){ 

        if(aux > __tam_col+2){
          if(c != '\t' && c!='\n' && c!= ' '){
            vector[contador]= c;
            counter_realloc++;
            vector = (char*)realloc(vector, counter_realloc*sizeof(char));
            contador++;
          }
        }
        aux++;     
    }

int indici;
char validation[2];
  for(linha =0 ; linha < contador; linha++){
        printf("%c\n", vector[linha] );

       if(vector[linha]  == '>' && vector[linha+1] == '*'){
            __estado_dual = 1;
            //__contador_estado_final++;
             printf("encontrou estado inicial e final mesmo tempo\n" );
             validation[0] = vector[linha+2];
             validation[1] = vector[linha+3];
              indici = compara_estado(estado,qtd_linhas,validation);
             if(indici != -1 ){
                 
                 estado[indici].tipo_estado = DUAL_ESTADO;
                 printf("DUAL_ESTADO -> %s\n",estado[indici].estado);
 
             }

             __dfa_inicio = estado[indici];
              printf("Estado INICIAL ->%s\n",__dfa_inicio.estado);
             return 1;
             break;
             

       }else if(vector[linha]  == '>' && vector[linha+1] != '*'){
               printf("encontrou estado inicial\n" );  
             validation[1] = vector[linha+2];
             validation[0] = vector[linha+3];
             indici = compara_estado(estado,qtd_linhas,validation);
             printf("indic %d\n",indici );
             if(indici != -1 ){
                 
                 estado[indici].tipo_estado = INICIAL;
                  __dfa_inicio = estado[indici];
                 printf("opa ->%s\n",__dfa_inicio.estado);

             }
             return 1;
             break;
             
       }
      

  }
 
  return 0;    
    
}

int setEstadoFinal(FILE *fp , Automato estado[] ,int qtd_linhas ){
  printf("Encontrando Estado Final \n");
int linha=0;
int col=0;
int aux= 0;
int contador=0;
char *vector = (char*)malloc(1*sizeof(char));
int counter_realloc = 1;
char c;
 rewind(fp);
    while((c = getc(fp) ) != EOF){
        if(aux > __tam_col+2){
          if(c != '\t' && c!='\n'){
            vector[contador]= c;
            counter_realloc++;
            vector = (char*)realloc(vector,counter_realloc*sizeof(char));            
            contador++;
          }
        }
        aux++;     
    }
  int p = 0 ;
   
 printf("tamnaho vector =: %d\n", strlen(vector));
int indici=0;
  for(linha =0 ; linha < contador; linha++){
        printf("%c\n", vector[linha] );        
       if(vector[linha] == '*'){
             printf("Encoutrou estado final " );            
             if(vector[linha+1] == 'q'){
               char *vali;
                vali = (char*)malloc(2*sizeof(char));                
                vali[0] = vector[linha+1];
                vali[1] = vector[linha+2];                
                 indici = compara_estado(estado,qtd_linhas,vali);
             }      
              printf(" position stats final-> %d\n", indici );
             if(indici != -1 ){                 
                 if(estado[indici].tipo_estado == DUAL_ESTADO){
                   printf("Encontrou estado Dual =:> %s",estado[indici].estado);continue;
                 }else{
                   estado[indici].tipo_estado = FINAL;
                 }
 
             }
             __contador_estado_final++;
        }  
  }

  if(__contador_estado_final >= 1){
    printf("QUANTIDADE DE ESTADO FINAL ->%d\n",__contador_estado_final );
    return 1;
  }
 
  return 0;    
    
}

void setFilhos(char mat_estado[][__tam_col], Automato automato[] ,int qtd_linhas, int tam_leitura){  
  
  int linha;
  int col;
  int contador=0;
  int indici;
  char *val;
  int cont_tam_leitura=0;
   int j;
   int k;
    for(linha = 0 ; linha < qtd_linhas ; linha++){
       automato[contador].ponteiro_estado= (Automato*)malloc(tam_leitura*sizeof(Automato));
       printf("Automato Pai -> %s\n", automato[contador].estado);
        for(j=2 ; j< __tam_col; j+=2){
          printf("Automato -> %s | Filho -> %d \n", automato[contador].estado, cont_tam_leitura);
            if(cont_tam_leitura != tam_leitura){
             
               for(k = 0 ;  k < 1; k++){    
                val = (char*)malloc(2*sizeof(char));  
                val[0] = mat_estado[linha][j];
                val[1] = mat_estado[linha][j+1];                 
                if(!strcasecmp(val, "--") == 0){                 
                 indici = compara_estado(automato,qtd_linhas,val);
                 automato[contador].ponteiro_estado[cont_tam_leitura] = automato[indici]; 
                  printf("Automato Filho =>%s \n", automato[contador].ponteiro_estado[cont_tam_leitura].estado );
                }           
              
                break;
               }
             cont_tam_leitura++;
            }   
           
       }
       contador++;
       cont_tam_leitura= 0;
    }   
      
}

int __getIndiciLeitura(char c ,char *vector_leitura, int tam_leitura){
   int indici = -1;  int i = 0;
  for(i; i < tam_leitura; i++){
      if(c == vector_leitura[i]){
        indici =  i;
        break;
      }
  }
  return indici;
}
int validationAutomato(char c , Automato estado[] , char *vector_leitura  , int qtd_linhas , int tam_leitura){

  printf("Validando char teste => %c | %d \n",c , __dfa.tipo_estado  );
  int indici;
  int indici_leitura;
  if(c == '2'){
      printf("AUTOMATO DE VALIDAÇÃO =: %s | %d \n", __dfa.estado , __dfa.tipo_estado);
      if(__dfa.tipo_estado == FINAL || __dfa.tipo_estado == DUAL_ESTADO){

         return 1;
      }else{
         return 2;
      }
      
  }
    
   indici_leitura = __getIndiciLeitura(c, vector_leitura , tam_leitura);
   if(indici_leitura == -1){
      printf("ARQUIVO DE TESTE INVALIDO , REVEJA SEU ARQUIVO E TENTE NOVAMENTE\n");
     exit(1);
   }

    printf("Automato ->%s\n",__dfa.estado);
    if(c == vector_leitura[indici_leitura] ){     
       indici = compara_estado(estado, qtd_linhas,__dfa.estado);
       if(indici == -1){
         printf("erro\n");
       }
       printf("indici e estado ->%d , %s\n", indici , estado[indici].ponteiro_estado[indici_leitura].estado);
       __dfa = estado[indici].ponteiro_estado[indici_leitura];
        printf("saiu \n" );
    }else{
        printf("ARQUIVO DE TESTE INVALIDO , REVEJA SEU ARQUIVO E TENTE NOVAMENTE\n");
        exit(1);
    }


  return 0;
}


char * __getLeituraEstados(FILE *fp){
    char *str; char c;int contador=0;int counter_realloc =1;
    str = (char*)malloc(1*sizeof(char)); 
    if(!str){printf("Error\n"); exit(1);}
    while((c = getc(fp) ) != EOF){ 
          if(c == '\n'){           
            break;
          }
          else if(c != '\t'){  
            printf("%c\n",c );           
            counter_realloc++;
            str = (char*)realloc(str,counter_realloc*sizeof(char));
            if(!str){printf("Error\n"); exit(1);} 
            str[contador] = c;   
            contador++;    
          }
    }
    
    printf("contador -:%d\n",contador );  
    int i = 0;    
    //printf("%s\n", str);   
    
    return str;
}
main(){

FILE *fp;
FILE *fp1;
char c;
char vector[90];



int cont=0;
int contador=0;
fp = fopen("arquivo.txt","r");   /* Arquivo ASCII, para leitura */
if(!fp)
{
    printf( "Erro na abertura do arquivo");
    exit(0);
}
char Linha[100];
int aux = 0;
int qtd_linhas = __get_qtd_linhas(fp);
rewind(fp);
char *vector_leitura = __getLeituraEstados(fp);
printf("%s\n",vector_leitura);
Automato *automato;
int tam_leitura = strlen(vector_leitura);
int tam_vector = (strlen(vector_leitura)*2); 
__tam_col = tam_vector+2;
char mat_estado[qtd_linhas][__tam_col];
printf("tamanho coluna %d\n", __tam_col);
rewind(fp);
//passar dados para um vector
int linha=0;
int col=0;

for(linha = 0 ; linha < qtd_linhas ; linha++){
    for(col = 0; col < __tam_col; col++){
       while((c = getc(fp) ) != EOF){ 
        if(aux > __tam_col+2){
          if(c != '\t' && c!='\n' && c != '>' && c!= '*'){
            mat_estado[linha][col] = c;
            contador++;
            break;        
          }
        }
        aux++;     
       }

    }//fin coluna

}//fim linha

for(linha = 0 ; linha < qtd_linhas ; linha++){
    for(col = 2; col < __tam_col ; col++){  

            printf("%c",mat_estado[linha][col]);
         
          }
          printf("\n");
}

 
 automato = getEstados(mat_estado,qtd_linhas);
 listVectorEstados(automato,qtd_linhas);      
 int validationInicial = setEstadoInicial(fp ,automato , qtd_linhas );
 int validationFinal = setEstadoFinal(fp, automato, qtd_linhas);
 listVectorEstados(automato,qtd_linhas);
 setFilhos(mat_estado,automato,qtd_linhas, tam_leitura);
       //listVectorEstados(automato,qtd_linhas);
    if(validationInicial==0){

        printf("Nao encontrou estado inicial, automato invalido!!!\n");
        exit(1);
    }
    if(validationFinal==0){

         printf("Nao encontrou estado FINAL, automato invalido!!!\n");
         exit(1);
    }

    //validando automato
    fp1 = fopen("teste.txt","r");   /* Arquivo ASCII, para leitura */
if(!fp1)
{
    printf( "Erro na abertura do arquivo");
    exit(0);
}

char c1;
int val;
 __dfa = __dfa_inicio;
 printf("\t\tLENDO ARQUVO DE TESTE!!!!!\n\n Automato INICIAL =>%s | %d\n\n",__dfa.estado , __dfa.tipo_estado);

 while((c1 = getc(fp1) ) != EOF){       
          
             printf("CARACTER =: %c\n",c1 );
              if(c1 == '\n'){
                  val =  validationAutomato('2', automato, vector_leitura, qtd_linhas, tam_leitura);
                  if(val == 1){
                     printf("AUTOMATO ACEITO\n");
                  }else if(val == 2){
                     printf("AUTOMATO NAO ACEITO\n");
                  }
                   __dfa = __dfa_inicio;
              }else{             

                  validationAutomato(c1, automato, vector_leitura, qtd_linhas, tam_leitura);
                  
              }          
    }
    

fclose(fp1);
fclose(fp);
return 0;

}