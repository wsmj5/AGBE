#include <fstream>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

unsigned char mode = 0;
char charlist[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
struct sPal{unsigned short r:5 = 0, g:5 = 0, b:5 = 0;}__attribute__((packed));
struct sScrn{unsigned short nameL:5 = 0, nameH:5 = 0, h:1 = 0, v:1 = 0, pal:4 = 0;}__attribute__((packed));
sPal pal[256];
sScrn scrn[1024];
unsigned char tildath[1024][64] = {0};
unsigned char* chars = (unsigned char*)calloc(0x10000, 1);
unsigned short SData[1024] = {0};
unsigned char* VRAM = (unsigned char*)calloc(0x9600, 2);
char* name;
char* name1;
char* name2;
unsigned char namelen;

class GraphicsMaker : public olc::PixelGameEngine
{
    public:
	GraphicsMaker()
	{
		sAppName = "GraphicsMaker";
	}
	unsigned uint1632(sPal pixel){
        unsigned char r = pixel.r / 31.0 * 255, g = pixel.g / 31.0 * 255, b = pixel.b / 31.0 * 255;
        return (0xFF << 24) | (b << 16) | (g << 8) | r;}
    void printtile(unsigned char x, unsigned char y){
        sScrn tile = scrn[y * 32 + x];
        short xdirect = tile.h * -2 + 1;
        short ydirect = tile.v * -2 + 1;
        for(int i = 0; i < 8; i++)
        {
            Draw((8 + 0 * xdirect - tile.h) % 8 + x * 8 + 512, (8 + i * ydirect - tile.v) % 8 + y * 8 + 256, uint1632(pal[tile.pal * 16 + tildath[tile.nameH << 5 | tile.nameL][i * 8]]));
            Draw((8 + 1 * xdirect - tile.h) % 8 + x * 8 + 512, (8 + i * ydirect - tile.v) % 8 + y * 8 + 256, uint1632(pal[tile.pal * 16 + tildath[tile.nameH << 5 | tile.nameL][i * 8 + 1]]));
            Draw((8 + 2 * xdirect - tile.h) % 8 + x * 8 + 512, (8 + i * ydirect - tile.v) % 8 + y * 8 + 256, uint1632(pal[tile.pal * 16 + tildath[tile.nameH << 5 | tile.nameL][i * 8 + 2]]));
            Draw((8 + 3 * xdirect - tile.h) % 8 + x * 8 + 512, (8 + i * ydirect - tile.v) % 8 + y * 8 + 256, uint1632(pal[tile.pal * 16 + tildath[tile.nameH << 5 | tile.nameL][i * 8 + 3]]));
            Draw((8 + 4 * xdirect - tile.h) % 8 + x * 8 + 512, (8 + i * ydirect - tile.v) % 8 + y * 8 + 256, uint1632(pal[tile.pal * 16 + tildath[tile.nameH << 5 | tile.nameL][i * 8 + 4]]));
            Draw((8 + 5 * xdirect - tile.h) % 8 + x * 8 + 512, (8 + i * ydirect - tile.v) % 8 + y * 8 + 256, uint1632(pal[tile.pal * 16 + tildath[tile.nameH << 5 | tile.nameL][i * 8 + 5]]));
            Draw((8 + 6 * xdirect - tile.h) % 8 + x * 8 + 512, (8 + i * ydirect - tile.v) % 8 + y * 8 + 256, uint1632(pal[tile.pal * 16 + tildath[tile.nameH << 5 | tile.nameL][i * 8 + 6]]));
            Draw((8 + 7 * xdirect - tile.h) % 8 + x * 8 + 512, (8 + i * ydirect - tile.v) % 8 + y * 8 + 256, uint1632(pal[tile.pal * 16 + tildath[tile.nameH << 5 | tile.nameL][i * 8 + 7]]));
        }}
    unsigned char palpos = 0;
    unsigned short tilpos = 0;
    unsigned short tilposMACHO = 0;
    unsigned short scrnpos = 0;

public:
	bool OnUserCreate() override
	{
		return true;
	}
    float timelapse = 0;
    char edit = 0;
	bool OnUserUpdate(float fElapsedTime) override
	{
	    timelapse += fElapsedTime;
        switch(mode){
            case 0:{
                if(GetKey(olc::CTRL).bHeld && GetKey(olc::S).bPressed){
                    std::ofstream ofilepal(name, std::ios::binary);
                    std::ofstream ofiletil(name1, std::ios::binary);
                    std::ofstream ofilescrn(name2, std::ios::binary);
                    ofilepal.write((char*)pal, 512);
                    ofilescrn.write((char*)scrn, 2048);
                    ofilepal.close();
                    ofilescrn.close();
                    for(int i = 0; i < 1024 * 32; i++){
                        unsigned char wbyte = tildath[i >> 5][i * 2 & 0x1F] | tildath[i >> 5][i * 2 + 1 & 0x1F] << 4;
                        ofiletil.write((char*)&wbyte, 1);}
                    ofiletil.close();}
                if(GetKey(olc::TAB).bPressed){
                    edit++, edit %= 3;}
                for(int i = 0; i < 0x400; i++) printtile(i & 0x1F, i >> 5 & 0x1F);
                switch(edit){
                    case 0:{
                        for(int i = 0; i < 0x100; i++){
                            for(int j = 0; j < 16; j++)
                                for(int k = 0; k < 16; k++)
                                    Draw((i & 0xF) * 16 + k + 512, (i >> 4 & 0xF) * 16 + j, uint1632(pal[i]));}
                        if((int)(timelapse * 2) & 1){
                            for(int j = 0; j < 2; j++)
                                for(int k = 0; k < 2; k++)
                                    Draw((palpos & 0xF) * 16 + 2 + k + 512, (palpos >> 4 & 0xF) * 16 + 2 + j, 0xFFFFFF ^ uint1632(pal[palpos]));}
        if(GetKey(olc::LEFT).bPressed)
        {
            unsigned char temppos = palpos & 0xF;
            palpos ^= temppos;
            temppos--, temppos &= 0xF;
            palpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::RIGHT).bPressed)
        {
            unsigned char temppos = palpos & 0xF;
            palpos ^= temppos;
            temppos++, temppos &= 0xF;
            palpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::UP).bPressed)
        {
            unsigned char temppos = palpos >> 4 & 0xF;
            palpos ^= temppos << 4;
            temppos--, temppos &= 0xF;
            palpos |= temppos << 4;
            timelapse = 0;
        }
        if(GetKey(olc::DOWN).bPressed)
        {
            unsigned char temppos = palpos >> 4 & 0xF;
            palpos ^= temppos << 4;
            temppos++, temppos &= 0xF;
            palpos |= temppos << 4;
            timelapse = 0;
        }
        if(GetKey(olc::R).bPressed) pal[palpos].r++;
        if(GetKey(olc::G).bPressed) pal[palpos].g++;
        if(GetKey(olc::B).bPressed) pal[palpos].b++;
        if(GetKey(olc::T).bPressed) pal[palpos].r--;
        if(GetKey(olc::M).bPressed) pal[palpos].g--;
        if(GetKey(olc::Y).bPressed) pal[palpos].b--;
                        break;}
                    case 1:{
                        for(int j = 0; j < 0x400; j++)
                            for(int i = 0; i < 0x40; i++)
                                for(int k = 0; k < 2; k++)
                                    for(int l = 0; l < 2; l++)
                                        Draw(((j & 0x1F) * 8 + (i & 0x7)) * 2 + l, ((j >> 5 & 0x1F) * 8 + (i >> 3 & 0x7)) * 2 + k, (0xFF000000 |
                                            tildath[j][i] << 20 | tildath[j][i] << 16 | tildath[j][i] << 12 | tildath[j][i] << 8 | tildath[j][i] << 4 | tildath[j][i]));
//                                        (tildath[j][i] & 0x8) << 20 | (tildath[j][i] & 0x8) << 19 | (tildath[j][i] & 0x8) << 19 | (tildath[j][i] & 0x4) << 18 |
//                                        (tildath[j][i] & 0x4) << 17 | (tildath[j][i] & 0x4) << 16 | (tildath[j][i] & 0x4) << 15 | (tildath[j][i] & 0x4) << 14 |
//                                        (tildath[j][i] & 0x2) << 6 | (tildath[j][i] & 0x2) << 5 | (tildath[j][i] & 0x1) << 5 | (tildath[j][i] & 0x1) << 4 |
//                                        (tildath[j][i] & 0x1) << 3 | (tildath[j][i] & 0x1) << 2 | (tildath[j][i] & 0x1) << 1 | (tildath[j][i] & 0x1)));
                        if((int)(timelapse * 2) & 1) Draw(((tilposMACHO & 0x1F) * 8 + (tilpos & 0x7)) * 2, ((tilposMACHO >> 5 & 0x1F) * 8 + (tilpos >> 3 & 0x7)) * 2, 0xFF00FF00);
        if(GetKey(olc::A).bPressed)
        {
            unsigned char temppos = tilposMACHO & 0x1F;
            tilposMACHO ^= temppos;
            temppos--, temppos &= 0x1F;
            tilposMACHO |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::D).bPressed)
        {
            unsigned char temppos = tilposMACHO & 0x1F;
            tilposMACHO ^= temppos;
            temppos++, temppos &= 0x1F;
            tilposMACHO |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::W).bPressed)
        {
            unsigned char temppos = tilposMACHO >> 5 & 0x1F;
            tilposMACHO ^= temppos << 5;
            temppos--, temppos &= 0x1F;
            tilposMACHO |= temppos << 5;
            timelapse = 0;
        }
        if(GetKey(olc::S).bPressed && !GetKey(olc::CTRL).bHeld)
        {
            unsigned char temppos = tilposMACHO >> 5 & 0x1F;
            tilposMACHO ^= temppos << 5;
            temppos++, temppos &= 0x1F;
            tilposMACHO |= temppos << 5;
            timelapse = 0;
        }
        if(GetKey(olc::LEFT).bPressed)
        {
            unsigned char temppos = tilpos & 0x7;
            tilpos ^= temppos;
            temppos--, temppos &= 0x7;
            tilpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::RIGHT).bPressed)
        {
            unsigned char temppos = tilpos & 0x7;
            tilpos ^= temppos;
            temppos++, temppos &= 0x7;
            tilpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::UP).bPressed)
        {
            unsigned char temppos = tilpos >> 3 & 0x7;
            tilpos ^= temppos << 3;
            temppos--, temppos &= 0x7;
            tilpos |= temppos << 3;
            timelapse = 0;
        }
        if(GetKey(olc::DOWN).bPressed)
        {
            unsigned char temppos = tilpos >> 3 & 0x7;
            tilpos ^= temppos << 3;
            temppos++, temppos &= 0x7;
            tilpos |= temppos << 3;
            timelapse = 0;
        }
        if(GetKey(olc::K1).bPressed) tildath[tilposMACHO][tilpos] ^= 1;
        if(GetKey(olc::K2).bPressed) tildath[tilposMACHO][tilpos] ^= 2;
        if(GetKey(olc::K3).bPressed) tildath[tilposMACHO][tilpos] ^= 4;
        if(GetKey(olc::K4).bPressed) tildath[tilposMACHO][tilpos] ^= 8;
                        break;}
                    case 2:{
        if((int)(timelapse * 2) & 1) {Draw((scrnpos & 0x1F) * 8 + 512, (scrnpos >> 5 & 0x1F) * 8 + 256);}
        else {Draw((scrnpos & 0x1F) * 8 + 512, (scrnpos >> 5 & 0x1F) * 8 + 256, olc::RED);}
        if(GetKey(olc::LEFT).bPressed)
        {
            unsigned char temppos = scrnpos & 0x1F;
            scrnpos ^= temppos;
            temppos--, temppos &= 0x1F;
            scrnpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::RIGHT).bPressed)
        {
            unsigned char temppos = scrnpos & 0x1F;
            scrnpos ^= temppos;
            temppos++, temppos &= 0x1F;
            scrnpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::UP).bPressed)
        {
            unsigned char temppos = scrnpos >> 5 & 0x1F;
            scrnpos ^= temppos << 5;
            temppos--, temppos &= 0x1F;
            scrnpos |= temppos << 5;
            timelapse = 0;
        }
        if(GetKey(olc::DOWN).bPressed)
        {
            unsigned char temppos = scrnpos >> 5 & 0x1F;
            scrnpos ^= temppos << 5;
            temppos++, temppos &= 0x1F;
            scrnpos |= temppos << 5;
            timelapse = 0;
        }
        if(GetKey(olc::EQUALS).bPressed && !GetKey(olc::SHIFT).bHeld) scrn[scrnpos].nameL++;
        if(GetKey(olc::MINUS).bPressed && !GetKey(olc::SHIFT).bHeld) scrn[scrnpos].nameL--;
        if(GetKey(olc::EQUALS).bPressed && GetKey(olc::SHIFT).bHeld) scrn[scrnpos].nameH++;
        if(GetKey(olc::MINUS).bPressed && GetKey(olc::SHIFT).bHeld) scrn[scrnpos].nameH--;
        if(GetKey(olc::H).bPressed) scrn[scrnpos].h ^= 1;
        if(GetKey(olc::V).bPressed) scrn[scrnpos].v ^= 1;
        if(GetKey(olc::K1).bPressed) scrn[scrnpos].pal ^= 1;
        if(GetKey(olc::K2).bPressed) scrn[scrnpos].pal ^= 0b10;
        if(GetKey(olc::K3).bPressed) scrn[scrnpos].pal ^= 0b100;
        if(GetKey(olc::K4).bPressed) scrn[scrnpos].pal ^= 0b1000;
                        break;}}
                break;}
            case 1:{
                if(GetKey(olc::CTRL).bHeld && GetKey(olc::S).bPressed){
                    std::ofstream ofilepal(name, std::ios::binary);
                    std::ofstream ofiletil(name1, std::ios::binary);
                    std::ofstream ofilescrn(name2, std::ios::binary);
                    ofilepal.write((char*)pal, 512);
                    ofilescrn.write((char*)scrn, 2048);
                    ofilepal.close();
                    ofilescrn.close();
                    ofiletil.write((char*)tildath, 1024 * 64);
                    ofiletil.close();}
                if(GetKey(olc::TAB).bPressed){
                    edit++, edit %= 3;}
                for(int i = 0; i < 0x400; i++) printtile(i & 0x1F, i >> 5 & 0x1F);
                switch(edit){
                    case 0:{
                        for(int i = 0; i < 0x100; i++){
                            for(int j = 0; j < 16; j++)
                                for(int k = 0; k < 16; k++)
                                    Draw((i & 0xF) * 16 + k + 512, (i >> 4 & 0xF) * 16 + j, uint1632(pal[i]));}
                        if((int)(timelapse * 2) & 1){
                            for(int j = 0; j < 2; j++)
                                for(int k = 0; k < 2; k++)
                                    Draw((palpos & 0xF) * 16 + 2 + k + 512, (palpos >> 4 & 0xF) * 16 + 2 + j, 0xFFFFFF ^ uint1632(pal[palpos]));}
        if(GetKey(olc::LEFT).bPressed)
        {
            unsigned char temppos = palpos & 0xF;
            palpos ^= temppos;
            temppos--, temppos &= 0xF;
            palpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::RIGHT).bPressed)
        {
            unsigned char temppos = palpos & 0xF;
            palpos ^= temppos;
            temppos++, temppos &= 0xF;
            palpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::UP).bPressed)
        {
            unsigned char temppos = palpos >> 4 & 0xF;
            palpos ^= temppos << 4;
            temppos--, temppos &= 0xF;
            palpos |= temppos << 4;
            timelapse = 0;
        }
        if(GetKey(olc::DOWN).bPressed)
        {
            unsigned char temppos = palpos >> 4 & 0xF;
            palpos ^= temppos << 4;
            temppos++, temppos &= 0xF;
            palpos |= temppos << 4;
            timelapse = 0;
        }
        if(GetKey(olc::R).bPressed) pal[palpos].r++;
        if(GetKey(olc::G).bPressed) pal[palpos].g++;
        if(GetKey(olc::B).bPressed) pal[palpos].b++;
        if(GetKey(olc::T).bPressed) pal[palpos].r--;
        if(GetKey(olc::M).bPressed) pal[palpos].g--;
        if(GetKey(olc::Y).bPressed) pal[palpos].b--;
                        break;}
                    case 1:{
                        for(int j = 0; j < 0x400; j++)
                            for(int i = 0; i < 0x40; i++)
                                for(int k = 0; k < 2; k++)
                                    for(int l = 0; l < 2; l++)
                                        Draw(((j & 0x1F) * 8 + (i & 0x7)) * 2 + l, ((j >> 5 & 0x1F) * 8 + (i >> 3 & 0x7)) * 2 + k, uint1632(pal[tildath[j][i]]));
//                                        (tildath[j][i] & 0x8) << 20 | (tildath[j][i] & 0x8) << 19 | (tildath[j][i] & 0x8) << 19 | (tildath[j][i] & 0x4) << 18 |
//                                        (tildath[j][i] & 0x4) << 17 | (tildath[j][i] & 0x4) << 16 | (tildath[j][i] & 0x4) << 15 | (tildath[j][i] & 0x4) << 14 |
//                                        (tildath[j][i] & 0x2) << 6 | (tildath[j][i] & 0x2) << 5 | (tildath[j][i] & 0x1) << 5 | (tildath[j][i] & 0x1) << 4 |
//                                        (tildath[j][i] & 0x1) << 3 | (tildath[j][i] & 0x1) << 2 | (tildath[j][i] & 0x1) << 1 | (tildath[j][i] & 0x1)));
                        if((int)(timelapse * 2) & 1) Draw(((tilposMACHO & 0x1F) * 8 + (tilpos & 0x7)) * 2, ((tilposMACHO >> 5 & 0x1F) * 8 + (tilpos >> 3 & 0x7)) * 2, 0xFF00FF00);
        if(GetKey(olc::A).bPressed)
        {
            unsigned char temppos = tilposMACHO & 0x1F;
            tilposMACHO ^= temppos;
            temppos--, temppos &= 0x1F;
            tilposMACHO |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::D).bPressed)
        {
            unsigned char temppos = tilposMACHO & 0x1F;
            tilposMACHO ^= temppos;
            temppos++, temppos &= 0x1F;
            tilposMACHO |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::W).bPressed)
        {
            unsigned char temppos = tilposMACHO >> 5 & 0x1F;
            tilposMACHO ^= temppos << 5;
            temppos--, temppos &= 0x1F;
            tilposMACHO |= temppos << 5;
            timelapse = 0;
        }
        if(GetKey(olc::S).bPressed && !GetKey(olc::CTRL).bHeld)
        {
            unsigned char temppos = tilposMACHO >> 5 & 0x1F;
            tilposMACHO ^= temppos << 5;
            temppos++, temppos &= 0x1F;
            tilposMACHO |= temppos << 5;
            timelapse = 0;
        }
        if(GetKey(olc::LEFT).bPressed)
        {
            unsigned char temppos = tilpos & 0x7;
            tilpos ^= temppos;
            temppos--, temppos &= 0x7;
            tilpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::RIGHT).bPressed)
        {
            unsigned char temppos = tilpos & 0x7;
            tilpos ^= temppos;
            temppos++, temppos &= 0x7;
            tilpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::UP).bPressed)
        {
            unsigned char temppos = tilpos >> 3 & 0x7;
            tilpos ^= temppos << 3;
            temppos--, temppos &= 0x7;
            tilpos |= temppos << 3;
            timelapse = 0;
        }
        if(GetKey(olc::DOWN).bPressed)
        {
            unsigned char temppos = tilpos >> 3 & 0x7;
            tilpos ^= temppos << 3;
            temppos++, temppos &= 0x7;
            tilpos |= temppos << 3;
            timelapse = 0;
        }
        if(GetKey(olc::K1).bPressed) tildath[tilposMACHO][tilpos] ^= 1;
        if(GetKey(olc::K2).bPressed) tildath[tilposMACHO][tilpos] ^= 2;
        if(GetKey(olc::K3).bPressed) tildath[tilposMACHO][tilpos] ^= 4;
        if(GetKey(olc::K4).bPressed) tildath[tilposMACHO][tilpos] ^= 8;
        if(GetKey(olc::K5).bPressed) tildath[tilposMACHO][tilpos] ^= 0x10;
        if(GetKey(olc::K6).bPressed) tildath[tilposMACHO][tilpos] ^= 0x20;
        if(GetKey(olc::K7).bPressed) tildath[tilposMACHO][tilpos] ^= 0x40;
        if(GetKey(olc::K8).bPressed) tildath[tilposMACHO][tilpos] ^= 0x80;
                        break;}
                    case 2:{
        if((int)(timelapse * 2) & 1) {Draw((scrnpos & 0x1F) * 8 + 512, (scrnpos >> 5 & 0x1F) * 8 + 256);}
        else {Draw((scrnpos & 0x1F) * 8 + 512, (scrnpos >> 5 & 0x1F) * 8 + 256, olc::RED);}
        if(GetKey(olc::LEFT).bPressed)
        {
            unsigned char temppos = scrnpos & 0x1F;
            scrnpos ^= temppos;
            temppos--, temppos &= 0x1F;
            scrnpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::RIGHT).bPressed)
        {
            unsigned char temppos = scrnpos & 0x1F;
            scrnpos ^= temppos;
            temppos++, temppos &= 0x1F;
            scrnpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::UP).bPressed)
        {
            unsigned char temppos = scrnpos >> 5 & 0x1F;
            scrnpos ^= temppos << 5;
            temppos--, temppos &= 0x1F;
            scrnpos |= temppos << 5;
            timelapse = 0;
        }
        if(GetKey(olc::DOWN).bPressed)
        {
            unsigned char temppos = scrnpos >> 5 & 0x1F;
            scrnpos ^= temppos << 5;
            temppos++, temppos &= 0x1F;
            scrnpos |= temppos << 5;
            timelapse = 0;
        }
        if(GetKey(olc::EQUALS).bPressed && !GetKey(olc::SHIFT).bHeld) scrn[scrnpos].nameL++;
        if(GetKey(olc::MINUS).bPressed && !GetKey(olc::SHIFT).bHeld) scrn[scrnpos].nameL--;
        if(GetKey(olc::EQUALS).bPressed && GetKey(olc::SHIFT).bHeld) scrn[scrnpos].nameH++;
        if(GetKey(olc::MINUS).bPressed && GetKey(olc::SHIFT).bHeld) scrn[scrnpos].nameH--;
        if(GetKey(olc::H).bPressed) scrn[scrnpos].h ^= 1;
        if(GetKey(olc::V).bPressed) scrn[scrnpos].v ^= 1;
                        break;}}
                break;}
            case 2:{
                if(GetKey(olc::CTRL).bHeld && GetKey(olc::S).bPressed){
                    std::ofstream ofilepal(name, std::ios::binary);
                    std::ofstream ofiletil(name1, std::ios::binary);
                    std::ofstream ofilescrn(name2, std::ios::binary);
                    ofilepal.write((char*)pal, 512);
                    //ofilescrn.write((char*)scrn, 2048);
                    for(int i = 0; i < 1024; i++){
                        ofilescrn.write((char*)(scrn + i), 1);}
                    ofilepal.close();
                    ofilescrn.close();
                    ofiletil.write((char*)tildath, 256 * 64);
                    std::cout << "YES!";
                    ofiletil.close();}
                if(GetKey(olc::TAB).bPressed){
                    edit++, edit %= 3;}
                for(int i = 0; i < 0x400; i++) printtile(i & 0x1F, i >> 5 & 0x1F);
                switch(edit){
                    case 0:{
                        for(int i = 0; i < 0x100; i++){
                            for(int j = 0; j < 16; j++)
                                for(int k = 0; k < 16; k++)
                                    Draw((i & 0xF) * 16 + k + 512, (i >> 4 & 0xF) * 16 + j, uint1632(pal[i]));}
                        if((int)(timelapse * 2) & 1){
                            for(int j = 0; j < 2; j++)
                                for(int k = 0; k < 2; k++)
                                    Draw((palpos & 0xF) * 16 + 2 + k + 512, (palpos >> 4 & 0xF) * 16 + 2 + j, 0xFFFFFF ^ uint1632(pal[palpos]));}
        if(GetKey(olc::LEFT).bPressed)
        {
            unsigned char temppos = palpos & 0xF;
            palpos ^= temppos;
            temppos--, temppos &= 0xF;
            palpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::RIGHT).bPressed)
        {
            unsigned char temppos = palpos & 0xF;
            palpos ^= temppos;
            temppos++, temppos &= 0xF;
            palpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::UP).bPressed)
        {
            unsigned char temppos = palpos >> 4 & 0xF;
            palpos ^= temppos << 4;
            temppos--, temppos &= 0xF;
            palpos |= temppos << 4;
            timelapse = 0;
        }
        if(GetKey(olc::DOWN).bPressed)
        {
            unsigned char temppos = palpos >> 4 & 0xF;
            palpos ^= temppos << 4;
            temppos++, temppos &= 0xF;
            palpos |= temppos << 4;
            timelapse = 0;
        }
        if(GetKey(olc::R).bPressed) pal[palpos].r++;
        if(GetKey(olc::G).bPressed) pal[palpos].g++;
        if(GetKey(olc::B).bPressed) pal[palpos].b++;
        if(GetKey(olc::T).bPressed) pal[palpos].r--;
        if(GetKey(olc::M).bPressed) pal[palpos].g--;
        if(GetKey(olc::Y).bPressed) pal[palpos].b--;
                        break;}
                    case 1:{
                        for(int j = 0; j < 0x100; j++)
                            for(int i = 0; i < 0x40; i++)
                                for(int k = 0; k < 4; k++)
                                    for(int l = 0; l < 4; l++)
                                        Draw(((j & 0xF) * 8 + (i & 0x7)) * 4 + l, ((j >> 4 & 0xF) * 8 + (i >> 3 & 0x7)) * 4 + k, uint1632(pal[tildath[j][i]]));
//                                        (tildath[j][i] & 0x8) << 20 | (tildath[j][i] & 0x8) << 19 | (tildath[j][i] & 0x8) << 19 | (tildath[j][i] & 0x4) << 18 |
//                                        (tildath[j][i] & 0x4) << 17 | (tildath[j][i] & 0x4) << 16 | (tildath[j][i] & 0x4) << 15 | (tildath[j][i] & 0x4) << 14 |
//                                        (tildath[j][i] & 0x2) << 6 | (tildath[j][i] & 0x2) << 5 | (tildath[j][i] & 0x1) << 5 | (tildath[j][i] & 0x1) << 4 |
//                                        (tildath[j][i] & 0x1) << 3 | (tildath[j][i] & 0x1) << 2 | (tildath[j][i] & 0x1) << 1 | (tildath[j][i] & 0x1)));
                        if((int)(timelapse * 2) & 1) Draw(((tilposMACHO & 0xF) * 8 + (tilpos & 0x7)) * 4, ((tilposMACHO >> 4 & 0xF) * 8 + (tilpos >> 3 & 0x7)) * 4, 0xFF00FF00);
        if(GetKey(olc::A).bPressed)
        {
            unsigned char temppos = tilposMACHO & 0x1F;
            tilposMACHO ^= temppos;
            temppos--, temppos &= 0x1F;
            tilposMACHO |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::D).bPressed)
        {
            unsigned char temppos = tilposMACHO & 0x1F;
            tilposMACHO ^= temppos;
            temppos++, temppos &= 0x1F;
            tilposMACHO |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::W).bPressed)
        {
            unsigned char temppos = tilposMACHO >> 5 & 0x1F;
            tilposMACHO ^= temppos << 5;
            temppos--, temppos &= 0x1F;
            tilposMACHO |= temppos << 5;
            timelapse = 0;
        }
        if(GetKey(olc::S).bPressed && !GetKey(olc::CTRL).bHeld)
        {
            unsigned char temppos = tilposMACHO >> 5 & 0x1F;
            tilposMACHO ^= temppos << 5;
            temppos++, temppos &= 0x1F;
            tilposMACHO |= temppos << 5;
            timelapse = 0;
        }
        if(GetKey(olc::LEFT).bPressed)
        {
            unsigned char temppos = tilpos & 0x7;
            tilpos ^= temppos;
            temppos--, temppos &= 0x7;
            tilpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::RIGHT).bPressed)
        {
            unsigned char temppos = tilpos & 0x7;
            tilpos ^= temppos;
            temppos++, temppos &= 0x7;
            tilpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::UP).bPressed)
        {
            unsigned char temppos = tilpos >> 3 & 0x7;
            tilpos ^= temppos << 3;
            temppos--, temppos &= 0x7;
            tilpos |= temppos << 3;
            timelapse = 0;
        }
        if(GetKey(olc::DOWN).bPressed)
        {
            unsigned char temppos = tilpos >> 3 & 0x7;
            tilpos ^= temppos << 3;
            temppos++, temppos &= 0x7;
            tilpos |= temppos << 3;
            timelapse = 0;
        }
        if(GetKey(olc::K1).bPressed) tildath[tilposMACHO][tilpos] ^= 1;
        if(GetKey(olc::K2).bPressed) tildath[tilposMACHO][tilpos] ^= 2;
        if(GetKey(olc::K3).bPressed) tildath[tilposMACHO][tilpos] ^= 4;
        if(GetKey(olc::K4).bPressed) tildath[tilposMACHO][tilpos] ^= 8;
        if(GetKey(olc::K5).bPressed) tildath[tilposMACHO][tilpos] ^= 0x10;
        if(GetKey(olc::K6).bPressed) tildath[tilposMACHO][tilpos] ^= 0x20;
        if(GetKey(olc::K7).bPressed) tildath[tilposMACHO][tilpos] ^= 0x40;
        if(GetKey(olc::K8).bPressed) tildath[tilposMACHO][tilpos] ^= 0x80;
                        break;}
                    case 2:{
        if((int)(timelapse * 2) & 1) {Draw((scrnpos & 0x1F) * 8 + 512, (scrnpos >> 5 & 0x1F) * 8 + 256);}
        else {Draw((scrnpos & 0x1F) * 8 + 512, (scrnpos >> 5 & 0x1F) * 8 + 256, olc::RED);}
        if(GetKey(olc::LEFT).bPressed)
        {
            unsigned char temppos = scrnpos & 0x1F;
            scrnpos ^= temppos;
            temppos--, temppos &= 0x1F;
            scrnpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::RIGHT).bPressed)
        {
            unsigned char temppos = scrnpos & 0x1F;
            scrnpos ^= temppos;
            temppos++, temppos &= 0x1F;
            scrnpos |= temppos;
            timelapse = 0;
        }
        if(GetKey(olc::UP).bPressed)
        {
            unsigned char temppos = scrnpos >> 5 & 0x1F;
            scrnpos ^= temppos << 5;
            temppos--, temppos &= 0x1F;
            scrnpos |= temppos << 5;
            timelapse = 0;
        }
        if(GetKey(olc::DOWN).bPressed)
        {
            unsigned char temppos = scrnpos >> 5 & 0x1F;
            scrnpos ^= temppos << 5;
            temppos++, temppos &= 0x1F;
            scrnpos |= temppos << 5;
            timelapse = 0;
        }
        if(GetKey(olc::EQUALS).bPressed && !GetKey(olc::SHIFT).bHeld) scrn[scrnpos].nameL++;
        if(GetKey(olc::MINUS).bPressed && !GetKey(olc::SHIFT).bHeld) scrn[scrnpos].nameL--;
        if(GetKey(olc::EQUALS).bPressed && GetKey(olc::SHIFT).bHeld) scrn[scrnpos].nameH++, scrn[scrnpos].nameH & 7;
        if(GetKey(olc::MINUS).bPressed && GetKey(olc::SHIFT).bHeld) scrn[scrnpos].nameH--, scrn[scrnpos].nameH & 7;
                        break;}}
                break;}
            case 3:{
                if(GetKey(olc::CTRL).bHeld && GetKey(olc::S).bPressed){
                    std::ofstream ofile(name, std::ios::binary);
                    ofile.write((char*)VRAM, 0x9600 * 2);
                    ofile.close();}
                for(int i = 0; i < 0x9600; i++){
                    Draw(i % 240, i / 240, uint1632(*((sPal*)VRAM + i)));}
                break;}
            case 4:{
                if(GetKey(olc::CTRL).bHeld && GetKey(olc::S).bPressed){
                    std::ofstream ofilebmp(name1, std::ios::binary);
                    std::ofstream ofilepal(name2, std::ios::binary);
                    ofilebmp.write((char*)VRAM, 240 * 160);
                    ofilepal.write((char*)pal, 256 * 2);
                    ofilebmp.close();
                    ofilepal.close();}
                for(int i = 0; i < 160; i++){
                    for(int j = 0; j < 240; j++){
                        Draw(j, i, uint1632(pal[*(VRAM + i * 240 + j)]));}}
                break;}
            case 5:{
                for(int i = 0; i < 0x5000; i++){
                    Draw(i % 160, i / 160, uint1632(*((sPal*)VRAM + i)));}
                if(GetKey(olc::CTRL).bHeld && GetKey(olc::S).bPressed){
                    std::ofstream ofile(name, std::ios::binary);
                    ofile.write((char*)VRAM, 0x5000 * 2);
                    ofile.close();}
                break;}}
        return true;
	}
};
int main(int argc, char** argv){
    mode = *argv[1] - 0x30;
    name = argv[2];
    while(argv[2][namelen]) namelen++;
    GraphicsMaker inst;
    switch(mode){
        case 0:{
            name1 = argv[3], name2 = argv[4];
            if(inst.Construct(768, 512, 2, 2))
                inst.Start();
            break;}
        case 1:{
            name1 = argv[3], name2 = argv[4];
            if(inst.Construct(768, 512, 2, 2))
                inst.Start();
            break;}
        case 2:{
            name1 = argv[3], name2 = argv[4];
            if(inst.Construct(768, 512, 2, 2))
                inst.Start();
            break;}
        case 3:{
            std::ifstream ifile(argv[2], std::ios::binary);
            unsigned x, y;
            unsigned beg;
            ifile.seekg(0x0A);
            ifile.read((char*)&beg, 4);
            ifile.seekg(0x12);
            ifile.read((char*)&x, 4);
            ifile.seekg(0x16);
            ifile.read((char*)&y, 4);
            unsigned short tempBMP[x * y] = {0};
            ifile.seekg(beg);
            int row = 0;
            char funnel;
            for(int i = 0; i < y; i++){
                ifile.read((char*)(tempBMP + (y - 1) * x - i * x), x * 2);
                ifile.read(&funnel, 2 * (x & 1));}
            for(int i = 0; i < x * y; i++){
                tempBMP[i] = tempBMP[i] >> 10 & 0x1F | tempBMP[i] & 0x3E0 | tempBMP[i] << 10 & 0x7C00;}
            unsigned short BMP[240 * 160] = {0};
//            unsigned short BLDBMProw[240 * x] = {0};
//            unsigned short BLDBMPcol[160 * y] = {0};
//            for(int i = 0; i < y; i++){
//                for(int j = 0; j < x; j++){
//                    for(int k = 0; k < 160; k++){
//                        for(int l = 0; l < 240; l++){
//                            interBMP[i * 160 + k][j * 240 + l] = tempBMP[i * x + j];}}}}
//            unsigned int colorbuffer[x + y - 1][3];
            switch((*argv[3] == 'b') << 1 | (*argv[3] == 'm')){
                case 0:{

                    break;}
                case 1:{
                    for(int i = 0; i < 160; i++){
                        for(int j = 0; j < 240; j++){
                            BMP[i * 240 + j] = tempBMP[(int)(y * i / 160.0) * x + (int)(x * j / 240.0)];}}
                    break;}
                case 2:{
                    unsigned short BLDbuff[x * y] = {0};
                    unsigned colortotal[3] = {0};
                    for(int i = 0; i < 160; i++){
        //                for(int j = 0; j < 240 * x; j++){
        //                    BLDBMProw[j] = tempBMP[(int)(y / 160.0 * i) * x + j / x];}
                        for(int j = 0; j < 240; j++){
        //                    for(int k = 0; k < 160 * y; k++){
        //                        BLDBMProw[j] = tempBMP[k / y * x + (int)(x / 240.0 * j)];}
                            for(int k = 0; k < y; k++){
                                for(int l = 0; l < x; l++){
                                    BLDbuff[k * x + l] = tempBMP[(int)((i * y + k) / 160.0) * x + (int)((j * x + l) / 240.0)];}}
                            colortotal[0] = 0, colortotal[1] = 0, colortotal[2] = 0;
                            for(int k = 0; k < y * x; k++){
                                colortotal[0] += BLDbuff[k] & 0x1F;
                                colortotal[1] += BLDbuff[k] >> 5 & 0x1F;
                                colortotal[2] += BLDbuff[k] >> 10 & 0x1F;}
                            colortotal[0] /= x * y;
                            colortotal[1] /= x * y;
                            colortotal[2] /= x * y;
                            BMP[i * 240 + j] = colortotal[0] | colortotal[1] << 5 | colortotal[2] << 10;}}
                    break;}}
            for(int i = 0; i < 240 * 160; i++){
                *(unsigned short*)(VRAM + i * 2) = BMP[i];}
            if(inst.Construct(240, 160, 6, 6))
                inst.Start();
            break;}
        case 4:{
            name1 = argv[3], name2 = argv[3];
            std::ifstream ifile(name, std::ios::binary);
            ifile.read((char*)pal, 2);
            unsigned beg;
            ifile.seekg(0x0A);
            ifile.read((char*)&beg, 4);
            ifile.seekg(beg);
            int row = 0;
            char funnel;
            for(int i = 0; i < 160; i++){
                ifile.read((char*)(VRAM + (160 - 1 - i) * 240 * 2), 240 * 2);}
            unsigned short BMP[240 * 160] = {0};
            unsigned char palcount = 0;
            for(int i = 0; i < 240 * 160; i++){
                unsigned short testcolor = *(unsigned short*)(VRAM + i * 2);
                bool found = 0;
                unsigned char colsel = 0;
                for(int j = 0; j <= palcount; j++){
                    if(testcolor == *(unsigned short*)&pal[j]){
                        *(VRAM + i) = j;
                        found = 1;
                        break;}}
                if(!found){
                    palcount++;
                    *(unsigned short*)&pal[palcount] = testcolor;
                    *(VRAM + i) = palcount;}}
            for(int i = 0; i < 256; i++){
                *(unsigned short*)&pal[i] = *(unsigned short*)&pal[i] >> 10 & 0x1F | *(unsigned short*)&pal[i] & 0x3E0 | *(unsigned short*)&pal[i] << 10 & 0x7C00;}
            if(inst.Construct(240, 160, 6, 6))
                inst.Start();
            break;}
        case 5:{
            std::ifstream ifile(argv[2], std::ios::binary);
            unsigned x, y;
            unsigned beg;
            ifile.seekg(0x0A);
            ifile.read((char*)&beg, 4);
            ifile.seekg(0x12);
            ifile.read((char*)&x, 4);
            ifile.seekg(0x16);
            ifile.read((char*)&y, 4);
            unsigned short tempBMP[x * y] = {0};
            ifile.seekg(beg);
            int row = 0;
            char funnel;
            for(int i = 0; i < y; i++){
                ifile.read((char*)(tempBMP + (y - 1) * x - i * x), x * 2);
                ifile.read(&funnel, 2 * (x & 1));}
            for(int i = 0; i < x * y; i++){
                tempBMP[i] = tempBMP[i] >> 10 & 0x1F | tempBMP[i] & 0x3E0 | tempBMP[i] << 10 & 0x7C00;}
            unsigned short BMP[160 * 128] = {0};
//            unsigned short BLDBMProw[240 * x] = {0};
//            unsigned short BLDBMPcol[160 * y] = {0};
//            for(int i = 0; i < y; i++){
//                for(int j = 0; j < x; j++){
//                    for(int k = 0; k < 160; k++){
//                        for(int l = 0; l < 240; l++){
//                            interBMP[i * 160 + k][j * 240 + l] = tempBMP[i * x + j];}}}}
//            unsigned int colorbuffer[x + y - 1][3];
            switch((*argv[3] == 'b') << 1 | (*argv[3] == 'm')){
                case 0:{

                    break;}
                case 1:{
                    for(int i = 0; i < 128; i++){
                        for(int j = 0; j < 160; j++){
                            BMP[i * 160 + j] = tempBMP[(int)(y * i / 128.0) * x + (int)(x * j / 160.0)];}}
                    break;}
                case 2:{
                    unsigned short BLDbuff[x * y] = {0};
                    unsigned colortotal[3] = {0};
                    for(int i = 0; i < 128; i++){
        //                for(int j = 0; j < 240 * x; j++){
        //                    BLDBMProw[j] = tempBMP[(int)(y / 160.0 * i) * x + j / x];}
                        for(int j = 0; j < 160; j++){
        //                    for(int k = 0; k < 160 * y; k++){
        //                        BLDBMProw[j] = tempBMP[k / y * x + (int)(x / 240.0 * j)];}
                            for(int k = 0; k < y; k++){
                                for(int l = 0; l < x; l++){
                                    BLDbuff[k * x + l] = tempBMP[(int)((i * y + k) / 128.0) * x + (int)((j * x + l) / 160.0)];}}
                            colortotal[0] = 0, colortotal[1] = 0, colortotal[2] = 0;
                            for(int k = 0; k < y * x; k++){
                                colortotal[0] += BLDbuff[k] & 0x1F;
                                colortotal[1] += BLDbuff[k] >> 5 & 0x1F;
                                colortotal[2] += BLDbuff[k] >> 10 & 0x1F;}
                            colortotal[0] /= x * y;
                            colortotal[1] /= x * y;
                            colortotal[2] /= x * y;
                            BMP[i * 160 + j] = colortotal[0] | colortotal[1] << 5 | colortotal[2] << 10;}}
                    break;}}
            for(int i = 0; i < 160 * 128; i++){
                *(unsigned short*)(VRAM + i * 2) = BMP[i];}
            if(inst.Construct(160, 128, 6, 6))
                inst.Start();
            break;}}
    return 0;}
