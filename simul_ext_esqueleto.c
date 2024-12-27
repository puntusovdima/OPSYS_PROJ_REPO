#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "headers.h"

#define LONGITUD_COMANDO 100

// let us begin with the first and most essential function
int ComprobarCommando(char *strcomando, char *argumento1, char *argumento2);

// The second one will be the LeeSuperbloque Printbytemaps command
void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);

// Third and the fourth easy ones:
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
              char *nombreantiguo, char *nombrenuevo);
// This one was actually very easy as well:
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
             EXT_DATOS *memdatos, char *nombre);

// Those are the hard ones?
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           char *nombre,  FILE *fich);
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);

/*
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
              char *nombre);



void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);
*/

int main()
{
   // user input variables
   char raw[LONGITUD_COMANDO];
   char comando[LONGITUD_COMANDO];
   char argumento1[LONGITUD_COMANDO];
   char argumento2[LONGITUD_COMANDO];

   // i see the implementation of 'orden' unnecessary
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

   fent = fopen("particion.bin", "r+b");
   fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);

   memcpy(&ext_superblock, (EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
   memcpy(&directorio, (EXT_ENTRADA_DIR *)&datosfich[3], sizeof(directorio));
   memcpy(&ext_bytemaps, (EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
   memcpy(&ext_blq_inodos, (EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
   memcpy(&memdatos, (EXT_DATOS *)&datosfich[4], MAX_BLOQUES_DATOS * SIZE_BLOQUE);

   // main loop
   for (;;)
   {
      do
      {
         // this is the user input right here
         fflush(stdin);
         printf(">> ");
         fgets(raw, sizeof(raw), stdin);
         if (raw[strlen(raw) - 1] == '\n')
         {
            raw[strlen(raw) - 1] = '\0';
         }
         // removing the \n

         // parsing

         char *tk = strtok(raw, " ");
         int v = 0;
         while (tk != NULL)
         {
            switch (v)
            {
            case 0:
               strcpy(comando, tk);
               break;
            case 1:
               strcpy(argumento1, tk);
               break;
            case 2:
               strcpy(argumento2, tk);
               break;
            }
            v++;
            tk = strtok(NULL, " ");
         }
      } while (ComprobarCommando(comando, argumento1, argumento2) != 1);

      if (strcmp(comando, "info") == 0)
      {
         LeeSuperBloque(&ext_superblock);
         continue;
      }
      if (strcmp(comando, "bytemaps") == 0)
      {
         Printbytemaps(&ext_bytemaps);
         continue;
      }
      if (strcmp(comando, "dir") == 0)
      {
         Directorio(directorio, &ext_blq_inodos);
         continue;
      }
      if (strcmp(comando, "rename") == 0)
      {
         Renombrar(directorio, &ext_blq_inodos, argumento1, argumento2);
         continue;
      }
      if (strcmp(comando, "print") == 0)
      {
         Imprimir(directorio, &ext_blq_inodos, memdatos, argumento1);
      }
      if (strcmp(comando, "remove") == 0)
      {
         Borrar(directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock , argumento1, fent);
      }
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
         */
      // if the command is 'salir' (exit in english) we exit duh
      // we need the data and we close
      if (strcmp(comando, "exit") == 0)
      {
         // GrabarDatos(&memdatos,fent);
         fclose(fent);
         return 0;
      }
      // we free the user input
   }
}

int ComprobarCommando(char *strcomando, char *argumento1, char *argumento2)
{
   // we read the command and check
   // incase it doesnt match anything we just return a message
   // the while will keep asking until its !=0
   // and we return 0

   int i;
   char cmds[8][LONGITUD_COMANDO]; // will basically be a table with all the cmds
   strcpy(cmds[0], "info");
   strcpy(cmds[1], "bytemaps");
   strcpy(cmds[2], "dir");
   strcpy(cmds[3], "rename");
   strcpy(cmds[4], "print");
   strcpy(cmds[5], "remove");
   strcpy(cmds[6], "copy");
   strcpy(cmds[7], "exit");

   for (i = 0; i < 8; i++)
   {
      if (strcmp(cmds[i], strcomando) == 0)
      {
         return 1;
      }
   }

   printf("ERROR: Illegal command '%s'. [info, bytemaps, dir, rename, print, remove, copy, exit]\n", strcomando);
   return 0; // for the loop to proceed
}

void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup)
{

   printf("Block is: %d Bytes\n", psup->s_block_size);
   printf("inodes in patrition = %d\n", psup->s_inodes_count);
   printf("empty inodes = %d\n", psup->s_free_inodes_count);
   printf("Blocks in patrition = %d\n", psup->s_blocks_count);
   printf("Free blocks = %d\n", psup->s_free_blocks_count);
   printf("First block containing data = %d\n", psup->s_first_data_block);
}

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps)
{
   printf("Inodes: ");
   for (int i = 0; i < MAX_INODOS; i++)
   {
      ext_bytemaps->bmap_inodos[i] == 0 ? printf("0 ") : printf("1 ");
   }
   printf("\nBlocks  [0-25]: ");
   for (int i = 0; i <= 25; i++)
   {
      ext_bytemaps->bmap_bloques[i] == 0 ? printf("0 ") : printf("1 ");
   }
   printf("\n");
}

void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos)
{
    // We need to iterate over MAX_FICHEROS (total entries in the directory)
    for (int i = 0; i < MAX_FICHEROS; i++) {
        // Check if the inode is NULL or reserved (e.g., inode 2 is often reserved as root)
        unsigned short int num_inode = directorio[i].dir_inodo;
        
        if (num_inode == 2 || num_inode == NULL_INODO) {
            continue; // Skip if inode is NULL or reserved
        }

        // Print filename and inode information
        printf("%s\t", directorio[i].dir_nfich);
        printf("size: %d\t", inodos->blq_inodos[num_inode].size_fichero);
        printf("inode: %d\t", directorio[i].dir_inodo);
        printf("blocks: ");

        // Loop through the blocks used by this inode
        int j = 0;
        while (inodos->blq_inodos[num_inode].i_nbloque[j] != NULL_BLOQUE) {
            printf(" %d ", inodos->blq_inodos[num_inode].i_nbloque[j]);
            j++;
        }
        printf("\n");
    }
}

int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
              char *nombreantiguo, char *nombrenuevo)
{

   if (strlen(nombreantiguo) > (unsigned int)LEN_NFICH || strlen(nombrenuevo) > (unsigned int)LEN_NFICH)
   {
      printf("Maximum filename length is %d!\n", LEN_NFICH);
      return 1;
   }
   if (nombreantiguo[0] == '\0' || nombrenuevo[0] == '\0')
   {
      printf("Incorrect arguments, should be of type: rename <oldname> <newname>\n");
      return 1;
   }
   EXT_ENTRADA_DIR *directorioStart = directorio;
   int name_exists = 0;
   while (directorio->dir_inodo != NULL_INODO)
   {
      unsigned short int *num_inode = &directorio->dir_inodo;
      char *existingname = directorio->dir_nfich;
      if (*num_inode == 2 || *num_inode == NULL_INODO)
      {
         directorio++;
         continue;
      }
      if (strcmp(existingname, nombreantiguo) == 0)
      {
         // printf("The name exists\n");
         name_exists = 1;
         break;
      }
      directorio++;
   }
   if (!name_exists)
   {
      printf("The filename %s doesn't exist!\n", nombreantiguo);
      return 1;
   }

   EXT_ENTRADA_DIR *new_name_entry = directorio;
   directorio = directorioStart;

   while (directorio->dir_inodo != NULL_INODO)
   {
      unsigned short int *num_inode = &directorio->dir_inodo;
      char *existingname = directorio->dir_nfich;
      if (*num_inode == 2 || *num_inode == NULL_INODO)
      {
         directorio++;
         continue;
      }
      if (strcmp(existingname, nombrenuevo) == 0)
      {
         printf("The file with name: %s already exists\n", nombrenuevo);
         return 1;
      }
      directorio++;
   }
   strcpy(new_name_entry->dir_nfich, nombrenuevo);
}

int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
             EXT_DATOS *memdatos, char *nombre)
{
   int name_exists = 0;
   while (directorio->dir_inodo != NULL_INODO)
   {
      unsigned short int *num_inode = &directorio->dir_inodo;
      char *existingname = directorio->dir_nfich;
      if (*num_inode == 2 || *num_inode == NULL_INODO)
      {
         directorio++;
         continue;
      }
      if (strcmp(existingname, nombre) == 0)
      {
         name_exists = 1;
         break;
      }
      else
      {
         directorio++;
      }
   }
   if (!name_exists)
   {
      printf("ERROR(Print): The filename %s doesn't exist!\n", nombre);
      return 1;
   }

   EXT_ENTRADA_DIR *entry_to_print = directorio;
   int blocks_to_print[MAX_NUMS_BLOQUE_INODO + 1];
   unsigned short int num_inode = directorio->dir_inodo;
   unsigned short int *num_block = inodos->blq_inodos[num_inode].i_nbloque;

   int i = 0;
   while (*num_block != NULL_BLOQUE)
   {
      blocks_to_print[i] = *num_block;
      num_block++;
      i++;
   }

   blocks_to_print[i] = NULL_BLOQUE;
   for (int j = 0; j < i; j++)
   {
      printf("%s", memdatos[blocks_to_print[j] - PRIM_BLOQUE_DATOS]);
   }
   // Get length of the last block content
   char *last_block = (char *)&memdatos[blocks_to_print[i - 1] - PRIM_BLOQUE_DATOS];
   int len = strlen(last_block);
   // We add newline character only if the content doesn't have it
   if (len > 0 && last_block[len - 1] != '\n')
      printf("\n");
   return 0;
}

int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           char *nombre,  FILE *fich)
{
   //we first find the file we want to delete
   int exists = 0;
   while (directorio->dir_inodo != NULL_INODO)
   {
      unsigned short int *num_inode = &directorio->dir_inodo;
      char *existingname = directorio->dir_nfich;
      if (*num_inode == 2 || *num_inode == NULL_INODO)
      {
         directorio++;
         continue;
      }
      if (strcmp(existingname, nombre) == 0)
      {
         exists = 1;
         break;
      }
      directorio++;
   }

   //we check it exists if not we return back error
   if(exists == 0){
      return 0;
   }

   //first off we update the bytemap
   //setting the inode to 0
   ext_bytemaps->bmap_inodos[directorio->dir_inodo] = 0;
   int i;
   //setting the blocks used to 0
   for(i=0;i<MAX_NUMS_BLOQUE_INODO;i++){
      if(inodos->blq_inodos[directorio->dir_inodo].i_nbloque[i] != NULL_BLOQUE){
         ext_bytemaps->bmap_bloques[inodos->blq_inodos[directorio->dir_inodo].i_nbloque[i]] = 0;
      }
   }

   //now we set the inode size to 0 and the blocks used
   inodos->blq_inodos[directorio->dir_inodo].size_fichero = 0;
   for(i=0;i<MAX_NUMS_BLOQUE_INODO;i++){
      inodos->blq_inodos[directorio->dir_inodo].i_nbloque[i] = NULL_BLOQUE;
   }
   
   
   //finally we delete the directory entry
   strcpy(directorio->dir_nfich, ""); //setting the name to nothing
   directorio->dir_inodo = NULL_INODO;
   
   return 1; //success
}