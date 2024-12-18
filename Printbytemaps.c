#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "headers.h"

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);

int main()
{
    EXT_BYTE_MAPS ext_bytemaps;
    EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
    FILE *fent;

    fent = fopen("particion.bin", "r+b");
    fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);
    memcpy(&ext_bytemaps, (EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);


    Printbytemaps(&ext_bytemaps);
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
}