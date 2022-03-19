int isNameInTable(char symbolName[MAX_LINE_LEN])
{
    symbol *sym = head;
    while (sym != NULL)
    {
        if (!strcmp(sym->symbol, symbolName))
            return 1;
        sym = sym->next;
    }
    return 0;
}