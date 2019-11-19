#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include "utf8vector.h"

#define utf8vector_check_eof(num) \
if (vector->length == -1) { \
    for (int i = 0;i < num; i ++) { \
        if (utf[i] == '\0') return '\0'; \
    } \
} else { \
    if (vector->index + num > vector->length) return '\0'; \
}

utf8vector utf8vector_create(const char *utf8, int string_length) {
    utf8vector vector = (utf8vector)calloc(1, sizeof(struct _utf8vector));
    vector->utf8_str = utf8;
    vector->length = string_length;
    vector->index = 0;
    return vector;
}
void utf8vector_reset(utf8vector vector) {
    vector->index = 0;
}
int utf8vector_uni_count(utf8vector vector) {
    int count = 0;
    while (utf8vector_next_unichar(vector) != '\0') {
        count ++;
    }
    utf8vector_reset(vector);
    return count;
}
wchar_t utf8vector_next_unichar_with_raw(utf8vector vector, const char **raw, int *raw_length){
    if (vector->length == 0) {
        if (raw) {
            *raw = NULL;
            *raw_length = 0;
        }
        return '\0';
    }

    wchar_t uni;
    const char *utf = vector->utf8_str + vector->index;

    if (*utf >> 7 == 0) {
        utf8vector_check_eof(1);
        vector->index += 1;
        if (raw) {
            *raw = utf;
            *raw_length = 1;
        }
        return *utf;
    } else if ( (*utf & 0xE0) == 0xC0 ) { //两个字节
        utf8vector_check_eof(2);
        vector->index += 2;
        uni = ((utf[0] & 0x1F) << 6) | (utf[1] & 0x3F);
        if (raw) {
            *raw = utf;
            *raw_length = 2;
        }
        return uni;
    } else if ( (*utf & 0xF0) == 0xE0 ) { //三个字节
        utf8vector_check_eof(3);
        vector->index += 3;
        uni = (((utf[0] & 0x0F)) << 12) | (((utf[1] & 0x3F)) << 6) | (utf[2] & 0x3F);
        if (raw) {
            *raw = utf;
            *raw_length = 3;
        }
        return uni;
    } else if ( (*utf & 0xF8) == 0xF0 ) { //四个字节
        utf8vector_check_eof(4);
        vector->index += 4;
        uni = ((utf[0] & 0x07) << 18) | ((utf[1] & 0x3f) << 12) | ((utf[2] & 0x3f) << 6) | (utf[3] & 0x3f);

        if (raw) {
            *raw = utf;
            *raw_length = 4;
        }
        return uni;
    } else if ( (*utf & 0xFC) == 0xF8 ) { //五个字节
        utf8vector_check_eof(5);
        vector->index += 5;
        uni = ((utf[0] & 0x03) << 24) | ((utf[1] & 0x3f) << 18) | ((utf[2] & 0x3f) << 12) | ((utf[3] & 0x3f) << 6) |(utf[4] & 0x3f);

        if (raw) {
            *raw = utf;
            *raw_length = 5;
        }
        return uni;
    } else if ( (*utf & 0xFE) == 0xFC ) { //六个字节
        utf8vector_check_eof(6);
        vector->index += 6;
        uni = ((utf[0] & 0x01) << 30) | ((utf[1] & 0x3f) << 24) | ((utf[2] & 0x3f) << 18) | ((utf[3] & 0x3f) << 12) | ((utf[4] & 0x3f) << 6) | (utf[5] & 0x3f);

        if (raw) {
            *raw = utf;
            *raw_length = 6;
        }
        return uni;
    }
    return 0;
}
wchar_t utf8vector_next_unichar(utf8vector vector) {
    return utf8vector_next_unichar_with_raw(vector, NULL, NULL);
}
void utf8vector_free(utf8vector vector) {
    free(vector);
}
