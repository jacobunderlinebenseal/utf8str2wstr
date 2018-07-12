#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

int get_utf8_encode_size(char firstChar) {
    if ((firstChar & 0x80) == 0x00) {
        return 1;
    }
    else if ((firstChar & 0xe0) == 0xc0) {
        return 2;
    }
    else if ((firstChar & 0xf0) == 0xe0) {
        return 3;
    }
    else {
        return -1;
    }
}

std::wstring s2ws(std::string str) {
    int count = 0;
    std::wstring wstr;
    size_t p = 0;
    while (p < str.length()) {
        size_t utfbytes = get_utf8_encode_size(str[p]);
        wchar_t wchar = 0;
        unsigned char *pOutput = (unsigned char *)&wchar;
        if (utfbytes == 1) {
            wchar = str[p];
            wstr.push_back(wchar);
            p += utfbytes;
            count += utfbytes;
        }
        else if (utfbytes == 2 && p+1 < str.length()) {
            char b1 = str[p];
            char b2 = str[p+1];
            if ((b2 & 0xC0) != 0x80) {
                p++;
                printf("rkl %x\n", b2);
                continue;
            }
            *pOutput     = (b1 << 6) + (b2 & 0x3F);
            *(pOutput+1) = (b1 >> 2) & 0x07;
            wstr.push_back(wchar);
            p += utfbytes;
            count += utfbytes;
        }
        else if (utfbytes == 3 && p+2 < str.length()) {
            char b1 = str[p];
            char b2 = str[p+1];
            char b3 = str[p+2];
            if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)) {
                p++;
                continue;
            }
            *pOutput     = (b2 << 6) + (b3 & 0x3F);
            *(pOutput+1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
            wstr.push_back(wchar);
            p += utfbytes;
            count += utfbytes;
        }
        else {
            p++;
            continue;
        }
    }
    printf("count %d\n", count);
    return wstr;
}

int main()
{
    std::string str = "y=20-×是)不是正例数9.已知y=(k-3)x+k2-9是关于x的正比例函数求当x=-4时,y的值.";
    printf("str len %lu\n", str.length());
    std::wstring wstr = s2ws(str);
    printf("wstr len %lu\n", wstr.length());
    for (int i = 0; i < wstr.length(); i++)printf("\\u%04x ",wstr[i]);
    printf("\n");
    return 1;
}
