#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "headers.h"
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
             EXT_DATOS *memdatos, char *nombre);

int main()
{

    EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
    EXT_ENTRADA_DIR directorio[MAX_FICHEROS];

    EXT_BLQ_INODOS ext_blq_inodos;

    EXT_DATOS memdatos[MAX_BLOQUES_DATOS];

    FILE *fent;
    char *oldname, *newname;
    oldname = malloc(sizeof(char) * LEN_NFICH);
    newname = malloc(sizeof(char) * LEN_NFICH);

    oldname = "BelloGal.txt";
    newname = "HOLi.txt";
    fent = fopen("particion.bin", "r+b");
    fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);

    memcpy(&ext_blq_inodos, (EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
    memcpy(directorio, (EXT_ENTRADA_DIR *)&datosfich[3], sizeof(directorio));
    memcpy(&memdatos, (EXT_DATOS *)&datosfich[4], MAX_BLOQUES_DATOS * SIZE_BLOQUE);
    // Directorio(directorio, &ext_blq_inodos);
    char *text = "dkf";
    // printf("length of text: %d\n", strlen(text));
    Imprimir(directorio, &ext_blq_inodos, memdatos, oldname);
    // Renombrar(directorio, &ext_blq_inodos, oldname, newname);
    //   printf("%d", ext_blq_inodos.blq_inodos[3].size_fichero);
    return 0;
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
            // printf("The name exists\n");
            name_exists = 1;
            // directorio++;
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

    // directorio--;
    EXT_ENTRADA_DIR *entry_to_print = directorio;
    int blocks_to_print[MAX_NUMS_BLOQUE_INODO + 1];
    unsigned short int num_inode = directorio->dir_inodo;
    unsigned short int *num_block = inodos->blq_inodos[num_inode].i_nbloque;
    int i = 0;
    while (*num_block != NULL_BLOQUE)
    {
        blocks_to_print[i] = *num_block;
        // printf("File blocks: %d\n", *num_block);
        num_block++;
        i++;
    }
    blocks_to_print[i] = NULL_BLOQUE;
    // printf("Number of i-node: %d, file occupies:\n", num_inode);
    for (int j = 0; j < i; j++)
    {
        // printf("block: %d\n", blocks_to_print[j]);
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
