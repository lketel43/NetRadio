#include "message.h"
#include <string.h>
#include <stdio.h>

//We just need strtok to split the message, with the delim ' '


int count_words(char phrase[]){
    char delim = ' ';
    int counter = 0;
    int len_phrase = strlen(phrase);
    for(int i = 0; i < len_phrase; i++)
    {
        if(phrase[i] == delim && phrase[i-1] != delim && i > 0)
            counter++;
    }
    if(phrase[len_phrase-1] != delim && counter > 0)
        counter++;
    return counter;
}

enum msg_type get_msg_type(const char *phrase){
    char type[5];
    strncpy(type, phrase, 4);
    type[4] = '\0';
    if(strcmp("ACKM", type) == 0) return ACKM;
    else if(strcmp("DIFF", type) == 0) return DIFF;
    else if(strcmp("ENDM", type) == 0) return ENDM;
    else if(strcmp("IMOK", type) == 0) return IMOK;
    else if(strcmp("ITEM", type) == 0) return ITEM;
    else if(strcmp("LAST", type) == 0) return LAST;
    else if(strcmp("LINB", type) == 0) return LINB;
    else if(strcmp("LIST", type) == 0) return LIST;
    else if(strcmp("MESS", type) == 0) return MESS;
    else if(strcmp("OLDM", type) == 0) return OLDM;
    else if(strcmp("REGI", type) == 0) return REGI;
    else if(strcmp("RENO", type) == 0) return RENO;
    else if(strcmp("REOK", type) == 0) return REOK;
    else return RUOK;
}