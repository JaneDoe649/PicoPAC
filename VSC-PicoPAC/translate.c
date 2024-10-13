#include <string.h>
#include "translate.h"
#include "game_title.h"

char transcode(char c) {
    switch (c) {
        case 'A': return 0x20; break;
        case 'B': return 0x25; break;
        case 'C': return 0x23; break;
        case 'D': return 0x1A; break;
        case 'E': return 0x12; break;
        case 'F': return 0x1B; break;
        case 'G': return 0x1C; break;
        case 'H': return 0x1D; break;
        case 'I': return 0x16; break;
        case 'J': return 0x1E; break;
        case 'K': return 0x1F; break;
        case 'L': return 0x0E; break;
        case 'M': return 0x26; break;
        case 'N': return 0x2D; break;
        case 'O': return 0x17; break;
        case 'P': return 0x0F; break;
        case 'Q': return 0x18; break;
        case 'R': return 0x13; break;
        case 'S': return 0x19; break;
        case 'T': return 0x14; break;
        case 'U': return 0x15; break;
        case 'V': return 0x24; break;
        case 'X': return 0x22; break;
        case 'W': return 0x11; break;
        case 'Y': return 0x2C; break;
        case 'Z': return 0x21; break;
        case '0': return 0x0; break;
        case '1': return 0x1; break;
        case '2': return 0x2; break;
        case '3': return 0x3; break;
        case '4': return 0x4; break;
        case '5': return 0x5; break;
        case '6': return 0x6; break;
        case '7': return 0x7; break;
        case '8': return 0x8; break;
        case '9': return 0x9; break;
        case '_': return 0x28; break;
        case '-': return 0x28; break;
        case '$': return 0x0b; break;
        case ':': return 0x0a; break;
        case ' ': return 0x0c; break;
        case '?': return 0x0d; break;
        case '.': return 0x27; break;
        case '*': return 0x29; break;
        case '/': return 0x2e; break;
        case '=': return 0x2b; break;
        case '\\': return 0x3b; break;
        default: return 0;
    }
}

unsigned char * gettitle(unsigned char* filename) {
    for (int i=0; strcmp(game_title[i][0], "") != 0; i++) {
        if (strcasecmp(filename, game_title[i][0]) == 0) {
            return game_title[i][1];
        }
    }
    // No match
    return filename;
}

void translate(unsigned char *encodedtitle, unsigned char* filename, char isDir) {
    char *title = gettitle(filename);
    char v;
    encodedtitle[0] = 0x0c;    // Title start with space
    if (isDir) {
        encodedtitle[1] = 0x33; // menu start with walking guy
    }
    for (int i=0; i<16; i++) {
        if (i<strlen(title)) {
            v = title[i];
            if(v >= 'a' && v <= 'z') {
                v = v - 32;
            }
        } else {
                v = ' ';
            }
        if (!isDir) {
            encodedtitle[i+1] = transcode(v);
        } else {
            encodedtitle[i+2] = transcode(v);
        }
    }
}

/* unsigned char* to_uppercase(unsigned char* str) {
    unsigned char v;
    for (int i = 0; str[i] != '\0'; i++) {
        v = str[i];
        if(v >= 'a' && v <= 'z') {
                v = v - 32;
            }
        str[i] = v;
    }
    return str;
}*/ 
