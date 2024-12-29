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
           char *nombre, FILE *fich);

int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino, FILE *fich);

void GrabarDatos(EXT_DATOS *memdatos, EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
                 EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich, EXT_DATOS *datosfich);

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
            Borrar(directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock, argumento1, fent);
        }
        if (strcmp(comando, "copy") == 0)
        {
            Copiar(directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock,
                   memdatos, argumento1, argumento2, fent);
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
            GrabarDatos(memdatos, directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock, fent, datosfich);
            fclose(fent);
            return 0;
        }
        // we free the user input
        strcpy(comando, "");
        strcpy(argumento1, "");
        strcpy(argumento2, "");
    }
    fclose(fent); // just in case(debuggin reasons)
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
    printf("Inodes in patrition = %d\n", psup->s_inodes_count);
    printf("Empty inodes = %d\n", psup->s_free_inodes_count);
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
    for (int i = 0; i < MAX_FICHEROS; i++)
    {
        // Check if the inode is NULL or reserved (e.g., inode 2 is often reserved as root)
        unsigned short int num_inode = directorio[i].dir_inodo;

        if (num_inode == 2 || num_inode == NULL_INODO)
        {
            continue; // Skip if inode is NULL or reserved
        }

        // Print filename and inode information
        printf("%s\t", directorio[i].dir_nfich);
        printf("size: %d\t", inodos->blq_inodos[num_inode].size_fichero);
        printf("inode: %d\t", directorio[i].dir_inodo);
        printf("blocks: ");

        // Loop through the blocks used by this inode
        for (int j = 0; j < MAX_NUMS_BLOQUE_INODO; j++)
        {
            if (inodos->blq_inodos[num_inode].i_nbloque[j] != NULL_BLOQUE)
            {
                printf(" %d ", inodos->blq_inodos[num_inode].i_nbloque[j]);
            }
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
    int i = 0;
    for (i = 0; i < MAX_FICHEROS; i++)
    {
        unsigned short int *num_inode = &directorio[i].dir_inodo;
        char *existingname = directorio[i].dir_nfich;
        if (*num_inode == 2 || *num_inode == NULL_INODO)
        {
            continue;
        }
        if (strcmp(existingname, nombreantiguo) == 0)
        {
            // printf("The name exists\n");
            name_exists = 1;
            break;
        }
    }
    if (!name_exists)
    {
        printf("The filename %s doesn't exist!\n", nombreantiguo);
        return 1;
    }

    EXT_ENTRADA_DIR *new_name_entry = &directorio[i];

    for (i = 0; i < MAX_FICHEROS; i++)
    {
        unsigned short int *num_inode = &directorio[i].dir_inodo;
        char *existingname = directorio[i].dir_nfich;
        if (*num_inode == 2 || *num_inode == NULL_INODO)
        {
            continue;
        }
        if (strcmp(existingname, nombrenuevo) == 0)
        {
            printf("The file with name: %s already exists\n", nombrenuevo);
            return 1;
        }
    }
    strcpy(new_name_entry->dir_nfich, nombrenuevo);
}

int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
             EXT_DATOS *memdatos, char *nombre)
{
    if (nombre[0] == '\0')
    {
        printf("ERROR(Print): incorrect argument, put a file name to print\n");
        return 1;
    }
    int name_exists = 0;
    int i;
    for (i = 0; i < MAX_FICHEROS; i++)
    {
        unsigned short int *num_inode = &directorio[i].dir_inodo;
        char *existingname = directorio[i].dir_nfich;
        if (*num_inode == 2 || *num_inode == NULL_INODO)
        {
            continue;
        }
        if (strcmp(existingname, nombre) == 0)
        {
            name_exists = 1;
            break;
        }
    }
    if (!name_exists)
    {
        printf("ERROR(Print): The filename %s doesn't exist!\n", nombre);
        return 1;
    }

    EXT_ENTRADA_DIR *entry_to_print = &directorio[i];
    int blocks_to_print[MAX_NUMS_BLOQUE_INODO + 1];
    unsigned short int num_inode = directorio[i].dir_inodo;
    unsigned short int *num_block = inodos->blq_inodos[num_inode].i_nbloque;

    i = 0;
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
           char *nombre, FILE *fich)
{
    if (nombre[0] == '\0')
    {
        printf("ERROR(Print): incorrect argument, put a file name to remove\n");
        return 1;
    }
    // Find the file we want to delete
    int exists = 0;
    int j = -1;
    for (int i = 0; i < MAX_FICHEROS; i++)
    {
        unsigned short int num_inode = directorio[i].dir_inodo;

        if (num_inode == 2 || num_inode == NULL_INODO)
        {
            continue; // Skip if inode is NULL or reserved
        }
        if (strcmp(directorio[i].dir_nfich, nombre) == 0)
        {
            exists = 1;
            j = i; // Save the index of the matching file
            break; // Exit loop since we've found the file
        }
    }

    if (!exists || j == -1)
    {
        printf("file %s does not exist\n", nombre);
        return 0; // File not found, return error
    }

    // Update the bytemap: set the inode to 0
    unsigned short int file_inode = directorio[j].dir_inodo;
    ext_bytemaps->bmap_inodos[file_inode] = 0;
    // Update the superblock free inodes count
    ext_superblock->s_free_inodes_count++;

    // Set the blocks used by this inode to 0 in the block bitmap
    for (int i = 0; i < MAX_NUMS_BLOQUE_INODO; i++)
    {
        unsigned short int block_num = inodos->blq_inodos[file_inode].i_nbloque[i];
        if (block_num != NULL_BLOQUE)
        {
            ext_bytemaps->bmap_bloques[block_num] = 0;
            // Update the superblock free blocks count
            ext_superblock->s_free_blocks_count++;
        }
    }

    // Clear the inode: set its size to 0 and all blocks to NULL_BLOQUE
    inodos->blq_inodos[file_inode].size_fichero = 0;
    for (int i = 0; i < MAX_NUMS_BLOQUE_INODO; i++)
    {
        inodos->blq_inodos[file_inode].i_nbloque[i] = NULL_BLOQUE;
    }

    // Delete the directory entry: clear the name and inode
    strcpy(directorio[j].dir_nfich, "");
    directorio[j].dir_inodo = NULL_INODO;

    return 1; // Success
}

int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino, FILE *fich)
{
    int o_exists = 0;
    int i;
    for (i = 0; i < MAX_FICHEROS; i++)
    {
        unsigned short int num_inode = directorio[i].dir_inodo;
        if (num_inode == 2 || num_inode == NULL_INODO)
        {
            continue; // Skip if inode is NULL or reserved
        }
        if (strcmp(directorio[i].dir_nfich, nombreorigen) == 0)
        {
            o_exists = 1;
            break;
        }
    }
    if (!o_exists)
    {
        printf("file %s does not exist\n", nombreorigen);
        return 0;
    }
    int origin = i; // being the file we copy from

    // first we find the first available inode
    i = 0;

    for (i = 0; i < MAX_INODOS; i++)
    {
        if (ext_bytemaps->bmap_inodos[i] == 0 && (i != 2))
        {
            break;
        }
    }
    ext_bytemaps->bmap_inodos[i] = 1; // we will put the inode here
    int to_inode = i;

    i = 0;
    // second we find the first available directory
    do
    {
        i++;
    } while (directorio[i].dir_inodo != NULL_INODO || directorio[i].dir_inodo == 2);
    int to_dir = i; // the destination where we will copy

    strcpy(directorio[to_dir].dir_nfich, nombredestino); // we assign the name
    directorio[to_dir].dir_inodo = to_inode;

    // we copy the size_fichero
    inodos->blq_inodos[directorio[to_dir].dir_inodo].size_fichero = inodos->blq_inodos[directorio[origin].dir_inodo].size_fichero;

    // how many blocks do we have?
    int to_blocks = 0;
    for (i = 0; i < MAX_NUMS_BLOQUE_INODO; i++)
    {
        if (inodos->blq_inodos[directorio[origin].dir_inodo].i_nbloque[i] != NULL_BLOQUE)
        {
            to_blocks++;
        }
    }

    int from_alloc[to_blocks];
    int from_alloc_n = 0;
    for (i = 0; i < MAX_NUMS_BLOQUE_INODO; i++)
    {
        if (inodos->blq_inodos[directorio[origin].dir_inodo].i_nbloque[i] != NULL_BLOQUE)
        {
            from_alloc[from_alloc_n] = i;
            from_alloc_n++;
            if (from_alloc_n >= to_blocks)
            {
                break;
            }
        }
    }

    int to_alloc[to_blocks];
    int to_alloc_n = 0;

    // finding the n free blocks
    for (i = 0; i < MAX_BLOQUES_PARTICION; i++)
    {
        if (ext_bytemaps->bmap_bloques[i] == 0)
        {
            to_alloc[to_alloc_n] = i; // Store block number
            to_alloc_n++;
            if (to_alloc_n >= to_blocks)
                break; // Stop when enough blocks are found
        }
    }

    // Check if we found enough blocks
    if (to_alloc_n < to_blocks)
    {
        printf("Error: Not enough free blocks available\n");
        return 0;
    }

    // Allocate blocks to the inode
    // Set unused entries in inode to NULL_BLOQUE
    for (i = to_blocks; i < MAX_NUMS_BLOQUE_INODO; i++)
    {
        inodos->blq_inodos[directorio[to_dir].dir_inodo].i_nbloque[i] = NULL_BLOQUE;
    }

    for (i = 0; i < to_blocks; i++)
    {

        // Mark block as used in the bytemap
        ext_bytemaps->bmap_bloques[to_alloc[i]] = 1;

        // Assign block number to the inode block array
        inodos->blq_inodos[directorio[to_dir].dir_inodo].i_nbloque[i] = to_alloc[i];
    }

    // this finalizes the memory allocation
    // we now have to populate said memory
    // similar to 'print'

    for (i = 0; i < to_blocks; i++)
    {
        // Calculate source and destination block indices directly
        int source_block = inodos->blq_inodos[directorio[origin].dir_inodo].i_nbloque[i];
        int dest_block = inodos->blq_inodos[directorio[to_dir].dir_inodo].i_nbloque[i];

        memcpy((char *)&memdatos[dest_block - PRIM_BLOQUE_DATOS].dato, (char *)&memdatos[source_block - PRIM_BLOQUE_DATOS].dato, SIZE_BLOQUE);
    }
}

void GrabarDatos(EXT_DATOS *memdatos, EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
                 EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich, EXT_DATOS *datosfich)
{
    memcpy(&datosfich[0].dato, ext_superblock, sizeof(EXT_SIMPLE_SUPERBLOCK));
    memcpy(&datosfich[1].dato, ext_bytemaps, sizeof(EXT_BYTE_MAPS));
    memcpy(&datosfich[2].dato, inodos, sizeof(EXT_BLQ_INODOS));
    memcpy(&datosfich[3].dato, directorio, sizeof(EXT_ENTRADA_DIR) * MAX_FICHEROS); // MAX_FICHEROS directories

    for (int i = 0; i < MAX_BLOQUES_DATOS; i++)
    {
        memcpy(&datosfich[PRIM_BLOQUE_DATOS + i].dato, memdatos[i].dato, SIZE_BLOQUE);
    }

    fseek(fich, 0, SEEK_SET);
    fwrite(datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fich);

    fflush(fich);
}
