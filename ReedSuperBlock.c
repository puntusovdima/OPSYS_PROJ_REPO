#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "headers.h"

void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);

int main()
{
    EXT_SIMPLE_SUPERBLOCK ext_superblock;

    FILE *fent;
    EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];

    fent = fopen("particion.bin", "r+b");
    fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);
    memcpy(&ext_superblock, (EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
    LeeSuperBloque(&ext_superblock);
}

void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup)
{

    printf("Block %d Bytes\n", psup->s_block_size);
    printf("inodes in patrition = %d\n", psup->s_inodes_count);
    printf("empty inodes = %d\n", psup->s_free_inodes_count);
    printf("Blocks in patrition = %d\n", psup->s_blocks_count);
    printf("Free blocks = %d\n", psup->s_free_blocks_count);
    printf("First block containing data = %d\n", psup->s_first_data_block);
}