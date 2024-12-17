#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "headers.h"
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
              char *nombreantiguo, char *nombrenuevo);

int main()
{

    EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
    EXT_ENTRADA_DIR directorio[MAX_FICHEROS];

    EXT_BLQ_INODOS ext_blq_inodos;

    FILE *fent;
    char *oldname, *newname;
    oldname = malloc(sizeof(char) * LEN_NFICH);
    newname = malloc(sizeof(char) * LEN_NFICH);

    oldname = "HOLA.txt";
    newname = "HOLi.txt";
    fent = fopen("particion.bin", "r+b");
    fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);

    memcpy(&ext_blq_inodos, (EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
    memcpy(directorio, (EXT_ENTRADA_DIR *)&datosfich[3], sizeof(directorio));
    // Directorio(directorio, &ext_blq_inodos);
    Renombrar(directorio, &ext_blq_inodos, oldname, newname);
    // printf("%d", ext_blq_inodos.blq_inodos[3].size_fichero);
    return 0;
}

int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
              char *nombreantiguo, char *nombrenuevo)
{
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
            //printf("The name exists\n");
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
    //printf("new name: %s", new_name_entry->dir_nfich);
}

void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos)
{

    while (directorio->dir_inodo != NULL_INODO)
    {
        unsigned short int num_inode = directorio->dir_inodo;
        if (num_inode == 2 || num_inode == NULL_INODO)
        {
            directorio++;
            continue;
        }
        printf("%s\t", directorio->dir_nfich); // filename
        printf("size: %d\t", inodos->blq_inodos[num_inode].size_fichero);
        printf("inode: %d\t", directorio->dir_inodo);
        int j = 0;
        printf("blocks: ");
        unsigned short int *num_block = &inodos->blq_inodos[num_inode].i_nbloque[0];

        while (inodos->blq_inodos[num_inode].i_nbloque[j] != NULL_BLOQUE)
        {
            printf(" %d ", inodos->blq_inodos[num_inode].i_nbloque[j]);
            j++;
        }
        printf("\n");
        directorio++;
    }
}