
#include <string.h>


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

void translate(char *d, char* s) {
    d[0] = 0x0c;    //Title start with space
    for (int i=0; i<16; i++) {
        char v = s[i];
        if (i < strlen(s))  {
            if(v >= 'a' && v <= 'z') {
                v = v - 32;
            }
        } else {
            v = ' ';
        }
        d[i+1] = transcode(v);
    }



    /*d[0] = 0x0c;    //Title start with space
    for (int i=0; i<strlen(s); i++) {
        if(s[i] >= 'a' && s[i] <= 'z') {
            s[i] = s[i] -32;
        }
        d[i+1] = transcode(s[i]);
    }*/
}

unsigned int textpages[4][7] = {    //block, pages
    {0x1c80, 0x1d80, 0x1e80, 0x1f80, 0x0000, 0x0000, 0x0000},     // Pages 0 -> 3
    {0x1180, 0x1280, 0x1380, 0x1480, 0x1580, 0x1680, 0x1780},     // Pages 4 -> 10
    {0x0980, 0x0a80, 0x0b80, 0x0c80, 0x0d80, 0x0e80, 0x0f80},     // Pages 11 -> 17
    {0x0180, 0x0280, 0x0380, 0x0480, 0x0580, 0x0680, 0x0780}      // Pages 18 -> 24
};

    
