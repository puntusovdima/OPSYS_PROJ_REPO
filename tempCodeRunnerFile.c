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
            printf("The filename %s already exists\n", nombrenuevo);
            return;
        }
        directorio++;
    }
    strcpy(new_name_entry->dir_nfich, nombrenuevo);
    printf("new name: %s", directorio->dir_nfich);
}