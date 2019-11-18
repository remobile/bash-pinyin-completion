#include <stdio.h>
#include <stdlib.h>
#include "pinyin.h"
#include "pinyin_data.h"

int pinyin_get_tones_by_unicode(wchar_t uni, char **tones_out)
{
    if (!pinyin_ishanzi(uni))
    {
        *tones_out = NULL;
        return 0;
    }

    int count = pinyin_count[uni - 19968];

    if (count == 0)
    {
        *tones_out = NULL;
        return 0;
    }

    *tones_out = strdup(pinyin_tones[uni - 19968]);

    return count;
}

int get_pinyin_count(wchar_t uni) {
    return pinyin_count[uni - 19968];
}

int pinyin_get_pinyins_by_unicode(wchar_t uni, const char ***pinyins_out) {
    int count = pinyin_count[uni - 19968];
    const char *indexs = pinyin_index[uni - 19968];
    const char **pinyins = (const char **)malloc(sizeof(const char *) * count);
    for (int i = 0; i < count; i++) {
        int start = i * 2;
        pinyins[i] = pinyin_all_pinyin[(unsigned char)indexs[start] + (unsigned char)indexs[start + 1]];
    }
    *pinyins_out = pinyins;
    return count;
}
