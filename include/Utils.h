//
// Created by svtem on 17.07.2019.
//

#ifndef UNO_SECURITY_UTILS_H
#define UNO_SECURITY_UTILS_H

void trim(char *str) {
    int i;
    int begin = 0;
    int end = strlen(str) - 1;

    while (isspace((unsigned char) str[begin]))
        begin++;

    while ((end >= begin) && isspace((unsigned char) str[end]))
        end--;

    // Shift all characters back to the start of the string array.
    for (i = begin; i <= end; i++)
        str[i - begin] = str[i];

    str[i - begin] = '\0'; // Null terminate string.
}

#endif //UNO_SECURITY_UTILS_H
