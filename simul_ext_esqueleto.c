
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "headers.h"

#define LONGITUD_COMANDO 100

//let us begin with the first and most essential function
int ComprobarCommando(char *strcomando, char *argumento1, char *argumento2);

/*
void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombre);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombreantiguo, char *nombrenuevo);
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
             EXT_DATOS *memdatos, char *nombre);
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           char *nombre,  FILE *fich);
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);
*/

int main()
{
   //user input variables
   char raw[LONGITUD_COMANDO];
   char comando[LONGITUD_COMANDO];
   char argumento1[LONGITUD_COMANDO];
   char argumento2[LONGITUD_COMANDO];

   //i see the implementation of 'orden' unnecessary
   /*
   unsigned long int m;
   EXT_SIMPLE_SUPERBLOCK ext_superblock;
   EXT_BYTE_MAPS ext_bytemaps;
   EXT_BLQ_INODOS ext_blq_inodos;
   EXT_ENTRADA_DIR directorio[MAX_FICHEROS];
   EXT_DATOS memdatos[MAX_BLOQUES_DATOS];
   EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
   int entradadir;
   int grabardatos;
   FILE *fent;
      
   // Lectura del fichero completo de una sola vez
   //?...
      
   fent = fopen("particion.bin","r+b");
   fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);    
      
      
   memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
   memcpy(&directorio,(EXT_ENTRADA_DIR *)&datosfich[3], SIZE_BLOQUE);
   memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
   memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
   memcpy(&memdatos,(EXT_DATOS *)&datosfich[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);
   */
     
   // main loop
   for (;;){
      do {
         //this is the user input right here
         fflush(stdin);
         printf (">> ");
         fgets(raw,sizeof(raw),stdin);
         if(raw[strlen(raw)-1] == '\n'){
            raw[strlen(raw)-1] = '\0';
         }
         //removing the \n
         
         //parsing


         char* tk = strtok(raw," ");
         int v = 0;
         while(tk != NULL){
            switch(v){
               case 0:
               strcpy(comando,tk);
               break;
               case 1:
               strcpy(argumento1,tk);
               break;
               case 2:
               strcpy(argumento2,tk);
               break;
            }v++;
            tk = strtok(NULL," ");
         }
      } while (ComprobarCommando(comando,argumento1,argumento2) != 1);
         printf("command exists\n");
            /*if (strcmp(*orden,"dir")==0) {
                  Directorio(&directorio,&ext_blq_inodos);
                  continue;
                  }
               //?...
               // Escritura de metadatos en comandos rename, remove, copy     
               Grabarinodosydirectorio(&directorio,&ext_blq_inodos,fent);
               GrabarByteMaps(&ext_bytemaps,fent);
               GrabarSuperBloque(&ext_superblock,fent);
               if (grabardatos)
               GrabarDatos(&memdatos,fent);
               grabardatos = 0;

               //if the command is 'salir' (exit in english) we exit duh
               //we need the data and we close
               if (strcmp(orden,"salir")==0){
                  GrabarDatos(&memdatos,fent);
                  fclose(fent);
                  return 0;
               }*/
        //we free the user input
               
  }

}


int ComprobarCommando(char *strcomando, char *argumento1, char *argumento2)
{
   //we read the command and check
   //incase it doesnt match anything we just return a message
   //the while will keep asking until its !=0
   //and we return 0
   
   int i;
   char cmds[8][LONGITUD_COMANDO]; //will basically be a table with all the cmds
   strcpy(cmds[0],"info");
   strcpy(cmds[1],"bytemaps");
   strcpy(cmds[2],"dir");
   strcpy(cmds[3],"rename");
   strcpy(cmds[4],"print");
   strcpy(cmds[5],"remove");
   strcpy(cmds[6],"copy");
   strcpy(cmds[7],"exit");
   
   for(i=0;i<8;i++){
      if(strcmp(cmds[i],strcomando) == 0){
         return 1;
      }
   }
   printf("Syntax error the comand '%s' doesnt exist\n",strcomando);
   return 0; //for the loop to proceed
}

