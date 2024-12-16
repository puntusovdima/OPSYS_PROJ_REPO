#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "headers.h"

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);

int main() {
    EXT_BYTE_MAPS ext_bytemaps;
    EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
    FILE *fent;


   fent = fopen("particion.bin","r+b");
   fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);    
   memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);

}

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){

    
}