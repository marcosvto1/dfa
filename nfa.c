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
       struct Auto *lendo_vazio;
       struct Auto *lendo_0;
       struct Auto *lendo_1;
}typedef Automato;

//global vars
Automato __dfa_inicio;
Automato __dfa;
int __contador_estado_final=0;
int __estado_dual=0;
int aux_cont = 0;

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
 
           printf("%s\n",estado[linha].estado);  
           printf(" Filho 0 \t%s\n ",estado[linha].ponteiro_estado[LENDO_0].estado); 
           printf("Filho 1 \t%s\n ",estado[linha].ponteiro_estado[LENDO_1].estado); 
           printf("tipo estado  \t%d\n ",estado[linha].tipo_estado);
     }
}

Automato* getEstados(char mat_estado[][6],int qtd_linhas){
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

      // estado[contador] = mat_estado[]

  /*for(linha = 0 ; linha < qtd_linhas ; linha++){
     for(col = 0; col < 6 ; col++){
          printf("%c ",mat_estado[linha][col]);
       }
      printf("\n");
  } */
   
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
int vector[6*qtd_linhas];
char c;

 rewind(fp);
    while((c = getc(fp) ) != EOF){ 

        if(aux > 6){
          if(c != '\t' && c!='\n'){
            vector[contador]= c;
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
                 printf("esta %s\n",estado[indici].estado);
 
             }

             __dfa_inicio = estado[indici];
              printf("opa ->%s\n",__dfa_inicio.estado);
             return 1;
             break;
             

       }else if(vector[linha]  == '>'){
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
int linha=0;
int col=0;
int aux= 0;
int contador=0;
int vector[6*qtd_linhas];
char c;


 rewind(fp);
    while((c = getc(fp) ) != EOF){ 

        if(aux > 6){
          if(c != '\t' && c!='\n'){
            vector[contador]= c;
            contador++;
          }
        }
        aux++;     
    }
 int i;
for(i=0; i<contador;i++){

   printf("potaa ->%c\n",vector[i]);
}     
int indici;

__fpurge(stdin);
  for(linha =0 ; linha < contador; linha++){
        printf("%c\n", vector[linha] );  
      // linha == 0 ? linha : (linha = linha+3);
       if(vector[linha] == '*'){
             printf("encontrou estado inicial e final mesmo tempo\n" );
             //vali[1] = vector[linha+2];
             //vali[0] = vector[linha+3];
             
             if(vector[linha+1] == 'q' && vector[linha+2] =='1'){
              char *vali;
               vali = (char*)malloc(2*sizeof(char));                
                vali[0] = vector[linha+1];
                vali[1] = vector[linha+2];
                
                  indici = compara_estado(estado,qtd_linhas,vali);
             } 
           
             //printf(" achando estado final %s\n", vali );
             // indici = compara_estado(estado,qtd_linhas,vali);
              printf(" achando estado final -> indici %d\n", indici );
             if(indici != -1 ){                 
                 if(estado[indici].tipo_estado == DUAL_ESTADO){
                   continue;
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

void setFilhos(char mat_estado[][6], Automato automato[] ,int qtd_linhas){  
  
  int linha;
  int col;
  int contador=0;
  int indici;

  char val[3];

    for(linha = 0 ; linha < qtd_linhas ; linha++){
           
        //   printf("%c%c\n",mat_estado[linha][0], mat_estado[linha][1]);   
            automato[contador].ponteiro_estado= (Automato*)malloc(3*sizeof(Automato));
            val[0] = mat_estado[linha][2];
            val[1] = mat_estado[linha][3];
             printf("char em 0 ==> %s\n", val);
            indici = compara_estado(automato,qtd_linhas,val);
            if(indici == -1 ) printf("errro\n");     
            automato[contador].ponteiro_estado[LENDO_0] = automato[indici];

            val[0] = mat_estado[linha][4];
            val[1] = mat_estado[linha][5];
             printf("char em 1 ==> %s\n", val);
            indici = compara_estado(automato,qtd_linhas,val);
             if(indici == -1 ) printf("errro\n");
            automato[contador].ponteiro_estado[LENDO_1] =automato[indici];

            
            contador++;
    }
}
int validationAutomato(char c , Automato estado[] ,int qtd_linhas ){

  printf("Validando ++++=> %c\n",c  );
  int indici;
  if(c == '2'){
    
      if(__dfa.tipo_estado == FINAL || __dfa.tipo_estado == DUAL_ESTADO){

         return 1;
      }else{
         return 2;
      }
      
  }
    
    printf("entrando com estado -->%s\n",__dfa.estado);
    if(c=='0'){
      printf("entro aqui em validation 000\n" );
       indici = compara_estado(estado, qtd_linhas,__dfa.estado);
       if(indici == -1){
         printf("erro\n");
       }
       printf("indici e estado ->>%d , %s\n", indici , estado[indici].ponteiro_estado[LENDO_0].estado);
       __dfa = estado[indici].ponteiro_estado[LENDO_0];
        printf("saiu \n" );
    } 
    if(c=='1'){
      
      indici = compara_estado(estado, qtd_linhas,__dfa.estado);
       __dfa = estado[indici].ponteiro_estado[LENDO_1];
      
    }
     

  return 0;
}

int cont_virgula(char *s){
   int i;
   char c;
   int contador=0;
   int tamanho = strlen(s);
   for(i=0 ; i < tamanho ; i++){
       
       if(s[i] == ',')
        contador++;

   }
   return contador;
}

char * defineEstados(char *s){
     
     int tamanho = strlen(s);
     int qtd = cont_virgula(s);
    // char *str = "teste";
     //char str[(tamanho-(qtd+2))];
     char *str = (char*)malloc((tamanho-(qtd+2))*sizeof(char));


      int i;
      int contador =0;
      for(i=0; i < tamanho; i++){
          
           if(s[i] != '{' && s[i] != ',' && s[i] != '}'){
               printf("c ->%c\n", s[i]);
               str[contador++] =s[i];
           
           }
        
      }

      return str;
}


void setEstados(char *s,char *s0, char *s1, char *s2 ,Automato automato[])
{
    //estado pai
    automato[aux_cont].estado = s;
    int tamanho = strlen(s1);
    int qtd_virgula = cont_virgula(s0);
    char ch;
    int i = 0;
    //______________________________________________________
    //estado filhos_vazio-----------------------------------
      printf("CONFIGURANDO ESTADOS FILHOS LENDO VAZIO do -:>%s\n",automato[aux_cont].estado );
     
      int tam_vazio = strlen(s0);
      int cont_vazio=0;  
      int qtd_v_vazio = cont_virgula(s0); 
      automato[aux_cont].lendo_vazio = (Automato*) malloc ((qtd_v_vazio+1)*sizeof(Automato));     
      strcpy(s0,defineEstados(s0));
       for (i = 0; i < qtd_v_vazio+1; i++)
       {    
        automato[aux_cont].lendo_vazio[i].estado = (char*)malloc(2*sizeof(char));       
        automato[aux_cont].lendo_vazio[i].estado[0] = s0[cont_vazio]; 
        automato[aux_cont].lendo_vazio[i].estado[1] = s0[cont_vazio+1]; 
        cont_vazio+=2;
      }
      printf(" str => %s\n",s0 );

      for(i=0; i< qtd_v_vazio+1;i++){
        printf("%s\n",automato[aux_cont].lendo_vazio[i].estado);
      }
      scanf("%c",&ch);
    //______________________________________________________
    
    //--------- estados lendo 0-----------------------------
      printf("CONFIGURANDO ESTADOS FILHOS LENDO 0 do -:>%s\n",automato[aux_cont].estado );
      
      int tam_lendo_0 = strlen(s1);
      int cont_lendo_0=0;  
      int qtd_v_lendo_0 = cont_virgula(s1); 
      automato[aux_cont].lendo_0 = (Automato*) malloc ((qtd_v_lendo_0+1)*sizeof(Automato));     
      strcpy(s1,defineEstados(s1));
       for (i = 0; i < qtd_v_lendo_0+1; i++)
       {    
        automato[aux_cont].lendo_0[i].estado = (char*)malloc(2*sizeof(char));       
        automato[aux_cont].lendo_0[i].estado[0] = s1[cont_lendo_0]; 
        automato[aux_cont].lendo_0[i].estado[1] = s1[cont_lendo_0+1]; 
        cont_lendo_0+=2;
      }
      printf(" str => %s\n",s1 );

     for(i=0; i< qtd_v_lendo_0+1;i++){
        printf("%s\n",automato[aux_cont].lendo_vazio[i].estado);
     }
      scanf("%c",&ch);

    //______________________________________________________

    //----------estados lendo 1 ----------------------------
      printf("CONFIGURANDO ESTADOS FILHOS LENDO 1 do -:>%s\n",automato[aux_cont].estado );
      
      int tam_lendo_1 = strlen(s2);
      int cont_lendo_1=0;  
      int qtd_v_lendo_1 = cont_virgula(s2); 
      automato[aux_cont].lendo_1 = (Automato*) malloc ((qtd_v_lendo_1+1)*sizeof(Automato));     
      strcpy(s2,defineEstados(s2));
       for (i = 0; i < qtd_v_lendo_1+1; i++)
       {    
        automato[aux_cont].lendo_1[i].estado = (char*)malloc(2*sizeof(char));       
        automato[aux_cont].lendo_1[i].estado[0] = s2[cont_lendo_1]; 
        automato[aux_cont].lendo_1[i].estado[1] = s2[cont_lendo_1+1]; 
        cont_lendo_1+=2;
      }
      printf(" str => %s\n",s2 );

     for(i=0; i< qtd_v_lendo_1+1;i++){
        printf("%s\n",automato[aux_cont].lendo_1[i].estado);
     }
      scanf("%c",&ch);

    //________________________________________________________

   // int j = 0;
  //  int k=0;
   // for(j = 0 ; j < (qtd_virgula+1) ; j++){
        
    //     for(k = 0 ; k < tamanho ; k++){

               //automato[aux_cont].ponteiro_estado[]
     //    }
          
  //  }
   
    //printf("tamanho %d qtd_v %d  string-> %s \n", tamanho, qtd_virgula, s1);
    //printf("%s\n", automato[aux_cont].estado );   
    
        
    
}
main(){

FILE *fp;
FILE *fp1;
char c;
char vector[90];
int cont=0;
int contador=0;

fp = fopen("nfa_exemple.txt","r");   /* Arquivo ASCII, para leitura */
if(!fp)
{
    printf( "Erro na abertura do arquivo");
    exit(0);
}
char Linha[100];
int aux = 0;
int qtd_linhas = __get_qtd_linhas(fp);
rewind(fp);
char mat_estado[qtd_linhas][10];

//passar dados para um vector
int linha=0;
int col=0;
char s[3],s0[qtd_linhas+8], s1[qtd_linhas+8] , s2[qtd_linhas+8] , s3[qtd_linhas+8];
 printf("quantida=>%d\n", qtd_linhas);
 Automato automato[qtd_linhas-1];
    while( (fscanf(fp,"%s %s %s %s", s , s0 , s1 , s2 ))!=EOF ){
        
       // printf("%s\t\t\t\t%s\t\t\t\t%s\t\t\t\t\t%s\n", s, s0 ,s1 , s2 );
          
         setEstados(s,s0,s1,s2,automato);  

            /*int p = sizeof(s);
            printf("%s\n",s );

            automato[col].estado = (char*)malloc(p*sizeof(char));           
            strcpy(ch1,s);
            printf("contador-> %d\n",col );
            col++;*/
                        //printf("%s\t\t\t%s\t\t\t\t\t%s\n", s, s1 , s2 );
      
    }

//printf("linhas %d\n", col );
fclose(fp);
return 0;

}