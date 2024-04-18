#include <stdio.h>
#include <stdlib.h>

void flushBuffer(FILE* f, char* buf, int* count) {
    buf[*count] = 0;
    fprintf(f, "\n%s", buf);
    *count ^= *count;
}

void printHeader(FILE* f) {
    fprintf(f, "1556HL8 IMG to JED converter (c) 2022 Leonid Golouz, Computer Enthusiast Tips\n");
    fprintf(f, "YouTube channel: https://www.youtube.com/channel/UCz_ktTqWVekT0P4zVW8Xgcg\n\n");
}

int main(int argc, char** argv)
{
    printHeader(stdout);
    if (argc < 2) {
        printf("Run program with command line args, for ex.:\nimg2jed.exe input.img output.jed\n");
        return -2;
    }
    printf("Input IMG file: %s\nOutput JED file: %s\n", argv[1], argv[2]);
    char _02[2] = {2, 0};

    FILE* f = fopen(argv[1], "r");
    FILE* out = fopen(argv[2], "w");
    if (!f || !out) {
        printf("File open error!\n");
        return -1;
    }

    printHeader(out);
    fprintf(out, "%sst010/st007(tm) Version 3.20 JeDec file for: 1556HL8\n*\nQF2048*\nF0*\nL0", _02);

    fseek(f, 0L, SEEK_END);
    long size = ftell(f);
    fseek(f, 0L, SEEK_SET);

    unsigned char* buf = (unsigned char*) malloc(size);

    fread(buf, 1, size, f);
    char* stringBuf = (char*) malloc(32 + 1);
    int stringBufCount = 0;
    for (long i = 0; i < size; ++i) {
        if (buf[i] == 1) {
            stringBuf[stringBufCount] = '1';
        } else if (buf[i] == 0) {
            stringBuf[stringBufCount] = '0';
        } else {
            printf("Errornous character at pos: %ld!\n", i);
            stringBuf[stringBufCount] = 'X';
        }
        ++stringBufCount;
        if (stringBufCount == 32) {
            flushBuffer(out, stringBuf, &stringBufCount);
        }
    }
    if (stringBufCount != 0) {
        flushBuffer(out, stringBuf, &stringBufCount);
    }

    free(stringBuf);
    free(buf);
    fclose(f);

    char _03[2] = {3, 0};
    fprintf(out, "*\nC0000*%s0000", _03);
    printf("\nFile conversion finished!\n");
    fclose(out);
    return 0;
}
