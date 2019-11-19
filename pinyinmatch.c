#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "pinyin.h"
#include "utf8vector.h"
#include "linereader.h"

#define true 1
#define false 0
typedef int bool;

int indexOf(const char* str, int offset, char ch){
    int index = 0;
    while(str[index] != '\0'){
        if(index > offset && str[index] == ch){
            return index;
        }
        index++;
    }
    return -1;
}
bool match(char* parent, const char *child) {
    printf("-=====%s, %s\n", parent, child);
    if (parent[0] != child[0]) {
        return false;
    }
    int lastIndex = -1;
    int count = 0;
    int childLen = strlen(child);
    for (int i = 0; i < childLen; i++) {
        char ch = child[i];
        int index = indexOf(parent, lastIndex, ch);
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
    for (int i = 0; i < parentCount; i++) {
        parents[i] = (char *)calloc(size * 6 + 1, sizeof(char));
    }

    while((line_char = utf8vector_next_unichar(line_vector)) != '\0') {
        if (pinyin_ishanzi(line_char)) {
            const char **pinyins;
            int count = pinyin_get_pinyins_by_unicode(line_char, &pinyins);
            strcat(parents[0], pinyins[0]);
            free(pinyins);
        } else {
            parents[0][strlen(parents[0])] = line_char;
        }
    }
    int result = match(parents[0], word);
    utf8vector_free(line_vector);
    for (int i = 0; i < parentCount; i++) {
        free(parents[i]);
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
    return false;
}
