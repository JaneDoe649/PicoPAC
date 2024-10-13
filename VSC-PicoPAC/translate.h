#ifndef __TRANSLATE_H__
#define __TRANSLATE_H__
char transcode(char c);
unsigned char * gettitle(unsigned char* filename);
void translate(unsigned char *encodedtitle, unsigned char* filename, char isDir);
// unsigned char* to_uppercase(unsigned char* str);

unsigned int textpages[4][7] = {    //block, pages
    {0x1c80, 0x1d80, 0x1e80, 0x1f80, 0x0000, 0x0000, 0x0000},     // Pages 0 -> 3
    {0x1180, 0x1280, 0x1380, 0x1480, 0x1580, 0x1680, 0x1780},     // Pages 4 -> 10
    {0x0980, 0x0a80, 0x0b80, 0x0c80, 0x0d80, 0x0e80, 0x0f80},     // Pages 11 -> 17
    {0x0180, 0x0280, 0x0380, 0x0480, 0x0580, 0x0680, 0x0780}      // Pages 18 -> 24
};
#endif
    
