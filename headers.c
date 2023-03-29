#include "headers.h"

// ls and discover commands helper function- returns sorted names irrespective of case or '.' in the beginning
int alphasort(const struct dirent **a, const struct dirent **b)
{
    char tempa[30], tempb[30];
    const char *aname = (char *)malloc(sizeof(char));
    const char *bname = (char *)malloc(sizeof(char));
    aname = (*a)->d_name;
    bname = (*b)->d_name;
    int indexa = 0, indexb = 0;

    for (int i = 0; i < strlen(aname); i++)
    {
        if (aname[i] != '.')
            tempa[indexa++] = aname[i];
    }
    tempa[indexa] = '\0';

    for (int i = 0; i < strlen(bname); i++)
    {
        if (bname[i] != '.')
        {
            tempb[indexb++] = bname[i];
        }
    }
    tempb[indexb] = '\0';
    return strcasecmp(tempa, tempb);
}