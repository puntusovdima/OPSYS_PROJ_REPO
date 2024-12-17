#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "headers.h"
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);

int main()
{

    EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
    EXT_ENTRADA_DIR directorio[MAX_FICHEROS];

    EXT_BLQ_INODOS ext_blq_inodos;

    FILE *fent;
    fent = fopen("particion.bin", "r+b");
    fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);

    memcpy(&ext_blq_inodos, (EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
    // printf("size of ext_blq_inodos: %d bytes\n", sizeof(ext_blq_inodos));
    // printf("size of directorio: %d bytes\n", sizeof(directorio));
    memcpy(directorio, (EXT_ENTRADA_DIR *)&datosfich[3], sizeof(directorio));
    Directorio(directorio, &ext_blq_inodos);
    // printf("%d", ext_blq_inodos.blq_inodos[3].size_fichero);
    return 0;
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
        printf("%s\t", directorio->dir_nfich);
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