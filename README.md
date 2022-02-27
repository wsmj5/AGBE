# AGBE
Advance Game Boy Engine. In C/C++. The tools are written in C++, and the engine itself is made in C.

GraohicsMaker (Unfinished):

CTRL + S to save.

    Terminal/CLI syntax:
    for modes 1, 2, 3
    GraphicsMaker <mode> <output file palette> <output file tile> <output file screen>
    for modes 3, 5
    GraphicsMaker <mode> <input 16BMP> <logic> <output BMP>
    for mode 4
    GraphicsMaker <mode> <input 16BMP> <output BMP> <output file palette>

    In the command line or terminal, you type the name of the program, then the mode of BG you what to make it for.

    mode 0 is also the basic mode for the GBA, tilemap of 16x16 (16 palettes of 16 colors) color tiles.
    You'll be making BG data for the equivalent if the following registers ("X" indicates that it doesn't matter concerning this program).
    DISPCNT = XXXXXXXXXXXXX000;
    BGCNT = XXXXXXX0XXXXXXXX;

    When editing the palette:
        r = Red at cursor is incremented (unsigned 5 bits)
        t = Red at cursor is decremented (unsigned 5 bits)
        g = Green at cursor is incremented (unsigned 5 bits)
        m = Green at cursor is decremented (unsigned 5 bits)
        b = Blue at cursor is incremented (unsigned 5 bits)
        y = Blue at cursor is decremented (unsigned 5 bits)
        left = decrements palette index selection (unsigned 4 bits)
        right = increments palette index selection (unsigned 4 bits)
        up = decrements palette selection (unsigned 4 bits)
        down = increments palette selection (unsigned 4 bits)

    When editing the tiles:
        w = decrement tile selection by 32 (unsigned 10 bits)
        s = increment tile selection by 32 (unsigned 10 bits)
        a = decrement tile selection (unsigned 10 bits)
        d = increment tile selection (unsigned 10 bits)
        left = decrements tile index selection (unsigned 3 bits)
        right = increments tile index selection (unsigned 3 bits)
        up = decrements tile index selection by 8 (unsigned 3 bits)
        down = increments tile index selection by 8 (unsigned 3 bits)
    
    When editing screen:
        up = decrement screen data selection by 32 (unsigned 10 bits)
        down = increment screen data selection by 32 (unsigned 10 bits)
        left = decrement screen data selection (unsigned 10 bits)
        right = increment screen data selection (unsigned 10 bits)
        h = reflect selected screen data horizanally
        v = reflect selected screen data vertically
        1 = flip lowest bit of palette
        2 = flip second lowest bit of palette
        3 = flip second highest bit of palette
        4 = flip highest bit of palette
        = = increment lower 5 bits of tile name (unsigned 5 bits)
        - = decrement lower 5 bits of tile name (unsigned 5 bits)
        + = increment higher 5 bits of tile name (unsigned 5 bits)
        _ = decrement hegher 5 bits of tile name (unsigned 5 bits)
        
    mode 1 is the second most basic. 256x1 (1 palette of 256 colors). The commands don't differ much from mode 0. Although you can't change palette. In addition, 5, 6, 7, and 8 can be used to flip the respective bits when editimg tiles.
    DISPCNT = XXXXXXXXXXXXX000;
    BGCNT = XXXXXXX1XXXXXXXX;
    
    mode 2 is like mode 1 however, you can't flip the tiles and you only have access to 256 tiles.
    DISPCNT = XXXXXXXXXXXXX010;
    BGCNT = XXXXXXXXXXXXXXXX;
    
    For modes 3, 4, and 5, there isn't much interaction. You just simply save if you're happy with the result.
    I should mention that for the LOGIC section of the CLI/Terminal, "b" means "blur", and "m" means "mosaic".
    Modes 3 and 5 scale the image to fit 240 x 160 or 160 x 128, however 4 assumes it's a 240 x 160 image.
