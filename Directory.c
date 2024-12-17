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
    //printf("size of ext_blq_inodos: %d bytes\n", sizeof(ext_blq_inodos));
    //printf("size of directorio: %d bytes\n", sizeof(directorio));
    memcpy(directorio, (EXT_ENTRADA_DIR *)&datosfich[3], sizeof(directorio));
    Directorio(directorio, &ext_blq_inodos);
    
}

void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos)
{

    while (directorio->dir_inodo != NULL_INODO)
    {
        if (directorio->dir_inodo == 2)
        {
            directorio++;
            continue;
        }
        printf("%s\t size:%d\tinode: %d\n", directorio->dir_nfich, inodos->blq_inodos*SIZE_BLOQUE, directorio->dir_inodo);
        directorio++;
    }
}