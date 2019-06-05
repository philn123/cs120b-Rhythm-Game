#ifndef _music_h_
#define _music_h_

//Current Length : 43
double notes_zense[SONG_LENGTH] =            {0, B3, CS4, DS4, B4, FS4, CS5, FS4, DS5, FS5, B4, DS5, B4, E5,
                                             DS5, B4, B3, CS4, DS4, B4, FS4, CS5, FS4, DS5, FS5, B4,
                                             DS5, B4, E5, DS5, B4, B3, CS4, DS4, B4, FS4, CS5, FS4, DS5,
                                             FS5, B4, DS5, B4};
                                             
                                             
unsigned char timing_zense[SONG_LENGTH] =    {20, 2,  2,  2,    2,   2,   2,   2,   2,   2,  2,   2,  2,  2,
                                             2,   2,  2,  2,    2,    2,   2,   2,   2,   2,   2,   2,  
                                             2,   2,  2,  2,    2,  2,  2,  2,    2,   2,   2,   2,   2,
                                             2,   2,  2,  2};
                                             
unsigned char rests_zense[SONG_LENGTH] =     {20, 0,  0,  2,    2,   2,   2,   2,   2,   6,  2,   2,  2,  2,
                                             2,   6,  0,  0,    2,    2,   2,   2,   2,   2,   6,   2,  
                                             2,  2,  2,  2,     6,  0,  0,  2,    2,   2,   2,   2,   2,
                                             6,  2,  2,  2};
                                             
unsigned char hit_zense[SONG_LENGTH] =       {20, 8,  4,  2,    1,   2,   1,   8,   2,   4,  8,   1,  2,  4,
                                             8,   4,  8,  4,    2,    1,   2,   1,   8,   2,   4,   8,  
                                             1,  2,  4,  4,     4,  8,  4,  2,    1,   2,   1,   8,   2,
                                             1,  8,  4,  8};
                                             
unsigned char timing_hit_zense[SONG_LENGTH] ={20, 1,  1,  1,    1,   1,   1,   1,   1,   2,  1,   1,  1,  1,
                                             1,   2,  1,  1,    1,    1,   1,   1,   1,   1,   2,   1,  
                                             1,  1,  1,  1,    2,  1,  1,  1,    1,   1,   1,   1,   1,
                                             2,  1,  1,  1};



#endif