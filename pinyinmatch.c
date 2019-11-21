/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * Author : emptyhua@gmail.com
 * Create : 2011.9.26
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <getopt.h>
#include "pinyin.h"
#include "utf8vector.h"
#include "linereader.h"

#define DEBUG
#ifdef DEBUG
#define MYLOG(x, ARGS...) fprintf (stderr, "\n%d:"x"\n", __LINE__, ##ARGS)
#else
#define MYLOG(x, ARGS...)
#endif //DEBUG


int match_line_with_keyword(const char *line, int line_length, const char *keyword) {
    wchar_t line_char, keyword_char;
    int match_hanzi_count = 0;

    utf8vector line_vector = utf8vector_create(line, line_length);
    utf8vector keyword_vector = utf8vector_create(keyword, -1);

    int keyword_length = utf8vector_uni_count(keyword_vector);
    wchar_t *keyword_uni = malloc(sizeof(wchar_t) * keyword_length);

    int keyword_index = 0;
    while ((keyword_char = utf8vector_next_unichar(keyword_vector)) != '\0') {
        keyword_uni[keyword_index] = keyword_char;
        keyword_index ++;
    }

    int match_rt = 1;
    keyword_index = 0;

    while((line_char = utf8vector_next_unichar(line_vector)) != '\0' && keyword_index < keyword_length) {
        keyword_char = keyword_uni[keyword_index];
        if (pinyin_ishanzi(line_char)) {
            if (pinyin_ishanzi(keyword_char)) {
                if (line_char != keyword_char) {
                    match_rt = 0;
                    break;
                }
            } else if (pinyin_isabc(keyword_char)) {
                keyword_char = pinyin_lowercase(keyword_char);
                const char **pinyins;
                int count = pinyin_get_pinyins_by_unicode(line_char, &pinyins);
                int finded = 0;
                for (int i = 0; i < count; i++) {
                    if (keyword_char == pinyins[i][0]) {
                        finded = 1;
                        break;
                    }
                }
                free(pinyins);

                if (finded == 0) {
                    match_rt = 0;
                } else {
                    match_hanzi_count ++;
                }
                if (match_rt == 0) {
                    break;
                }
            } else {
                match_rt = 0;
                break;
            }
        } else {
            if (line_char != keyword_char) {
                match_rt = 0;
                break;
            }
        }
        keyword_index ++;
    }

    if (match_rt == 1 && keyword_index < keyword_length) {
        match_rt = 0;
    }

    free(keyword_uni);
    utf8vector_free(line_vector);
    utf8vector_free(keyword_vector);
    if (match_rt == 0) {
        return -1;
    } else {
        return match_hanzi_count;
    }
}

int main(int argc, char **argv) {
    char *keyword = argv[1];
    linereader reader = linereader_create(STDIN_FILENO);
    int count;
    MYLOG("keyword=%s", keyword);
    while ((count = linereader_readline(reader)) != -1)
    {
        const char *line = reader->line_buffer;
        int match_count = match_line_with_keyword(line, count, keyword);
        if (match_count >= 0) {
            MYLOG("match_count=%d, line=%s", match_count, line);
        }
        if (match_count > 0) {
            printf("%.*s\n", count, line);
        }
    }
    linereader_free(reader);
    return 0;
}
