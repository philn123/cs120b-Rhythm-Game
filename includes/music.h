#ifndef _music_h_
#define _music_h_

double notesDIV[SONG_LENGTH] =        { 0, A4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,A4,D4,D4,D4,D4,D4,D4,D4,A4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,CS4,D4,D4,D4,D4,D4,D4,D4,A4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,A4,D4,D4,D4,D4,D4,D4,D4,A4,FS4,FS4,FS4,FS4,FS4,CS5,D5,B4,FS4,B4,A4,A4,A4,A4,A4,A4, 0};
unsigned char timesDIV[SONG_LENGTH] = {16,  4,  4,  2,  4,  2,  2,  4,  2,  2,  2, 4, 4, 2, 4, 2, 2, 4, 8, 4,  4,  2,  4,  2,  2,  4,  2,  2,  2,  4, 4, 2, 4, 2, 2, 4, 8, 4,  4,  2,  4,  2,  2,  4,  2,  2,  2, 4, 4, 2, 4, 2, 2, 4, 8, 4,  4,  2,  4,  2,  2,  4, 4, 4,  4, 4, 4, 2, 4, 2, 2, 4, 0};
unsigned char restsDIV[SONG_LENGTH] = {16,  4,  9,  2,  8,  1,  2,  4,  2,  2,  2, 4, 8, 2, 8, 2, 2, 9, 0, 4,  8,  2,  9,  2,  2,  4,  2,  2,  2,  4, 8, 2, 8, 2, 2, 9, 0, 4,  9,  2,  8,  1,  2,  4,  2,  2,  2, 4, 8, 2, 8, 2, 2, 9, 0, 4,  9,  2,  8,  1,  2,  4, 0, 0,  0, 4, 8, 2, 8, 2, 2, 0,32};
unsigned char pressDIV[SONG_LENGTH] = {16,  1,  2,  2,  2,  2,  2,  2,  2,  2,  2, 1, 4, 4, 4, 4, 4, 4, 4, 1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  8, 4, 4, 4, 4, 4, 4, 4, 1,  2,  2,  2,  2,  2,  2,  2,  2,  2, 1, 4, 4, 4, 4, 4, 4, 4, 1,  2,  2,  2,  2,  2,  1, 2, 4,  8, 4, 8, 8, 8, 8, 8, 8,240};

double notes_zense[SONG_LENGTH] =           {0, B3, CS4, DS4, B4, FS4, CS5};
unsigned char timing_zense[SONG_LENGTH] =   {20, 2,  2,  2,    2,   2,   2};
unsigned char rests_zense[SONG_LENGTH] =    {20, 0,  0,  2,    2,   2,   2};
unsigned char hit_zense[SONG_LENGTH] =      {20, 1,  2,  4,    8,   4,   8};



#endif