#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "pinyin.h"
#include "utf8vector.h"
#include "linereader.h"


int indexOf(const char* str, char ch){
    int index = 0;
    while(str[index] != '\0'){
        if(str[index] == ch){
            return index;
        }
        index++;
    }
    return -1;
}
int match(pinyinvector parents, int size, const char *child) {
    pinyinvector parent = parents;
    for (int i = 0; i < size; i++) {




        parent += sizeof(struct _pinyinvector);
    }

    if (parent[0] != child[0]) {
        return 0;
    }
    int lastIndex = -1;
    int count = 0;
    int childLen = strlen(child);
    for (int i = 0; i < childLen; i++) {
        char ch = child[i];
        int index = indexOf(parent, ch);
        if (index > lastIndex) {
            count++;
        }
        lastIndex = index;
    }
    return count == childLen;
}

int match_line(const char *line, int line_length, const char *word) {
    wchar_t line_char;
    utf8vector line_vector = utf8vector_create(line, line_length);
    int size = utf8vector_uni_count(line_vector);

    int parentCount = 1;
    while((line_char = utf8vector_next_unichar(line_vector)) != '\0') {
        if (pinyin_ishanzi(line_char)) {
             parentCount *= get_pinyin_count(line_char);
        }
    }
    utf8vector_reset(line_vector);
    char **parents = (char **)calloc(parentCount, sizeof(char *));

    while((line_char = utf8vector_next_unichar(line_vector)) != '\0') {
        if (pinyin_ishanzi(line_char)) {
            parent->count = pinyin_get_pinyins_by_unicode(line_char, &(parent->pinyins));
        } else {
            parent->ch = line_char;
        }
        parent += sizeof(struct _pinyinvector);
    }
    int result = match(parents, size, word);
    utf8vector_free(line_vector);
    parent = parents;
    for (int i = 0; i < size; i++) {
        free(parent->pinyins);
        parent += sizeof(struct _pinyinvector);
    }
    free(parents);

    return result;
}

int main(int argc, char **argv)
{
    char *word = argv[1];
    int wordLen = strlen(word);
    int select = (wordLen > 0 && word[wordLen-1] > 48 && word[wordLen-1] < 58) ? word[wordLen-1] - 48 : 0; // 查看最后一位是否为数字
    if (select > 0) {
        word[wordLen-1] = '\0';
    }

    int count;
    int showIndex = 0;
    linereader reader = linereader_create(STDIN_FILENO);
    while ((count = linereader_readline(reader)) != -1) {
        const char *line = reader->line_buffer;
        if (wordLen == 0 || match_line(line, count, word)) {
            if (select == 0) {
                printf("%s\n", line);
            } else {
                showIndex++;
                if (showIndex == select) {
                    printf("%s\n", line);
                    break;
                }
            }
        }
    }
    linereader_free(reader);
    return 0;
}
