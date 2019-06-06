/*
 * Rhythm_Game_Project.c
 *
 * Created: 6/3/2019 10:12:22 PM
 * Author : Phillip
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <stdbool.h>
#include "io.c"
#include "timer.h"
#include "scheduler.h"
#include "Shift.h"
#include "pwm.h"
#include "notes.h"
#include "music.h"

//DEFINES
//#define EZ_BUTTON (PINA & 0x10)
#define LEFT_BUTTON (PINA & 0x08)
#define DOWN_BUTTON (PINA & 0x04)
#define UP_BUTTON  (PINA & 0x02)
#define RIGHT_BUTTON (PINA & 0x01)



//Buttons
bool LEFT = false;
bool RIGHT = false;
bool UP = false;
bool DOWN = false;

//zybooks how to make
typedef struct song{
    double notes[SONG_LENGTH];
    unsigned char timing[SONG_LENGTH];
    unsigned char rest_timing[SONG_LENGTH];
    unsigned char hit[SONG_LENGTH];
    unsigned char timing_hit[SONG_LENGTH];
    unsigned char high_score;
    unsigned char length;
}song ;

enum LFT{LFT_INIT, LFT_WAIT};
enum RGHT{RGHT_INIT, RGHT_WAIT};
enum UPP{UP_INIT, UP_WAIT};
enum DOWNN{DOWN_INIT, DOWN_WAIT};
enum LCD_Menu {LCD_Menu_Init, LCD_Menu_Start, LCD_WAIT1, LCD_Menu_Songs, LCD_WAIT2, LCD_Wait_Mii, LCD_BACK, LCD_Menu_Zense, LCD_Menu_Mii, LCD_WAIT3, Play_CountDown, Play_Song, Score_Screen, LCD_High_Score,LCD_WAIT4};
enum RGB_Matrix{RGB_INIT, RGB_MENU, RGB_SONG, RGB_RESET};
enum Note_Play{Note_INIT, Note_Wait, Note_Play_Song, Note_Reset};
enum Player{Player_Init, Player_Press};
    
//Tick Functions
int Left_Tick(int state);
int Right_Tick(int state);
int Up_Tick(int state);
int Down_Tick(int state);
int LCD_Menu_Tick(int state);
int RGB_Matrix_Tick(int state);
int Note_Tick(int state);
int Player_Tick(int state);

//tasks
const unsigned long tasksPeriodGCD = 5;
const unsigned char tasksNum = 8;
task tasks[8];

//global variables
song songs[2];
unsigned char pic[8] = {4, 12, 24, 24, 24, 24, 12, 4};

unsigned char current_note = 0;
unsigned char current_song_choice = 0;

unsigned char player_button_press = 0;

unsigned char speed = 5;


unsigned char current_score = 0;
unsigned char RGB_DISPLAY_NOTES[8] = {0,0,0,0,0,0,0,0};

bool song_finished = false;
bool score_increase = false;
bool playing = false;



int main(void)
{
    //LCD PORTS & RGB MATRIX
    DDRD = 0xFF; PORTD = 0x00;
    DDRC = 0XFF; PORTC = 0x00; //0 & 1 are LCD Control Lines, 2,3,4 are RGB PORTS
    
    //BUTTON INPUT PORTS
    DDRA = 0x00; PORTA = 0xFF;
    
    //BUZZER PORT
    DDRB = 0x0FF; PORTB = 0x00; //PB6 is buzzer
    
    //Initialize LCD and Matrix
    LCD_init();
    Shift_Init();   
    Shift_transmit_data(0);
    Shift_transmit_data(255);
    
    //eeprom
    if(eeprom_read_byte((uint8_t*)1) == 255) {
        eeprom_update_byte((uint8_t*)1, (uint8_t) 0);
    }
    if(eeprom_read_byte((uint8_t*)2) == 255) {
        eeprom_update_byte((uint8_t*)2, (uint8_t) 0);
    }
    
    songs[0].high_score = (unsigned)(char)eeprom_read_byte((uint8_t*) 1);
    songs[1].high_score = (unsigned)(char)eeprom_read_byte((uint8_t*) 2);
    
    songs[0].length = SONG_LENGTH;
    songs[1].length = 101;    
    //tasks
    for(unsigned char j = 0; j < songs[0].length; j++) {
        songs[0].notes[j] = notes_zense[j];
        songs[0].timing[j] = timing_zense[j];
        songs[0].rest_timing[j] = rests_zense[j];
        songs[0].hit[j] = hit_zense[j];
        songs[0].timing_hit[j] = timing_hit_zense[j];
    }
    
    for(unsigned char i = 0; i < songs[1].length; i++) {
        songs[1].notes[i] = notes_mii[i];
        songs[1].timing[i] = timing_mii[i];
        songs[1].rest_timing[i] = rests_mii[i];
        songs[1].hit[i] = hit_zense[i];
        songs[1].timing_hit[i] = timing_hit_mii[i];
    }
    
    
    //Task Code
    unsigned char tasks_increment = 0;
    
    tasks[tasks_increment].state = LFT_INIT;
    tasks[tasks_increment].period = 20;
    tasks[tasks_increment].elapsedTime = tasks[tasks_increment].period;
    tasks[tasks_increment].TickFct = &Left_Tick;
    
    tasks_increment++;
    
    tasks[tasks_increment].state = RGHT_INIT;
    tasks[tasks_increment].period = 20;
    tasks[tasks_increment].elapsedTime = tasks[tasks_increment].period;
    tasks[tasks_increment].TickFct = &Right_Tick;
    
    tasks_increment++;
    
    tasks[tasks_increment].state = UP_INIT;
    tasks[tasks_increment].period = 20;
    tasks[tasks_increment].elapsedTime = tasks[tasks_increment].period;
    tasks[tasks_increment].TickFct = &Up_Tick;
    
    
    tasks_increment++;
    
    tasks[tasks_increment].state = DOWN_INIT;
    tasks[tasks_increment].period = 20;
    tasks[tasks_increment].elapsedTime = tasks[tasks_increment].period;
    tasks[tasks_increment].TickFct = &Down_Tick;
    
    
    tasks_increment++;
    
    tasks[tasks_increment].state = RGB_INIT;
    tasks[tasks_increment].period = 10;
    tasks[tasks_increment].elapsedTime = tasks[tasks_increment].period;
    tasks[tasks_increment].TickFct = &RGB_Matrix_Tick;
    
    
    tasks_increment++;
    
    tasks[tasks_increment].state = LCD_Menu_Init;
    tasks[tasks_increment].period = 30;
    tasks[tasks_increment].elapsedTime = tasks[tasks_increment].period;
    tasks[tasks_increment].TickFct = &LCD_Menu_Tick;
    
    tasks_increment++;
    
    tasks[tasks_increment].state = Note_INIT;
    tasks[tasks_increment].period = 10;
    tasks[tasks_increment].elapsedTime = tasks[tasks_increment].period;
    tasks[tasks_increment].TickFct = &Note_Tick;
    
    tasks_increment++;
    
    tasks[tasks_increment].state = Player_Init;
    tasks[tasks_increment].period = 5;
    tasks[tasks_increment].elapsedTime = tasks[tasks_increment].period;
    tasks[tasks_increment].TickFct = &Player_Tick;
    
    /*
    tasks_increment++;
    
    tasks[tasks_increment].state = EZ_INIT;
    tasks[tasks_increment].period = 60;
    tasks[tasks_increment].elapsedTime = tasks[tasks_increment].period;
    tasks[tasks_increment].TickFct = &EZ_TICK;
    */
    //Timer
    TimerSet(tasksPeriodGCD);
    TimerOn();
    
    
    while (1) 
    {
        
    }        
    return 0;
}

int Left_Tick(int state){
    switch(state){
        case LFT_INIT:
            state = LFT_WAIT;
            break;
        case LFT_WAIT:
            state = LFT_WAIT;
            break;
    }
    switch(state){
        case LFT_INIT:
            break;
        case LFT_WAIT:
            if(LEFT_BUTTON == 0x00){
                LEFT = true;
                break;
            }
            else{
                LEFT = false;
                break;
            }
    }
    
    return state;
}

int Right_Tick(int state){
    switch(state){
        case RGHT_INIT:
            state = RGHT_WAIT;
            break;
        case RGHT_WAIT:
            state = RGHT_WAIT;
            break;
    }
    switch(state){
        case RGHT_INIT:
            break;
        case RGHT_WAIT:
            if(RIGHT_BUTTON == 0x00){
                RIGHT = true;
                break;
            }
            else{
                RIGHT = false;
                break;
            }
    }
    
    return state;
}

int Up_Tick(int state){
    switch(state){
        case UP_INIT:
            state = UP_WAIT;
            break;
        case UP_WAIT:
            state = UP_WAIT;
            break;
    }
    switch(state){
        case UP_INIT:
            break;
        case UP_WAIT:
            if(UP_BUTTON == 0x00){
                UP = true;
                break;
            }
            else{
                UP = false;
                break;
            }
    }
    
    return state;
}

int Down_Tick(int state){
    switch(state){
        case DOWN_INIT:
            state = DOWN_WAIT;
            break;
        case DOWN_WAIT:
            state = DOWN_WAIT;
            break;
    }
    switch(state){
        case DOWN_INIT:
            break;
        case DOWN_WAIT:
            if(DOWN_BUTTON == 0x00){
                DOWN = true;
                break;
            }
            else{
                DOWN = false;
                break;
            }
    }
    
    return state;
}

int LCD_Menu_Tick(int state){
    
    static unsigned char count_down = 0;
    switch(state){ //transitions
        case LCD_Menu_Init:
            state = LCD_Menu_Start;
            break;
        case LCD_Menu_Start:
            if(LEFT && RIGHT && DOWN && UP){
                state = LCD_WAIT1;
                break;
            }
            else{
                state = LCD_Menu_Start;
                break;
            }
        case LCD_WAIT1:
            if(LEFT || RIGHT || UP || DOWN){
                state = LCD_WAIT1;
            }
            else{
                LCD_ClearScreen();
                state = LCD_Menu_Songs;
            }
            break;
        case LCD_Menu_Songs:
            if(LEFT && !RIGHT && !UP && !DOWN){
                LCD_ClearScreen();
                state = LCD_Menu_Start;
                break;
            }
            else if(DOWN && !UP && !LEFT && !RIGHT){
                if(current_song_choice){
                    current_song_choice = 1;
                }
                else{
                    current_song_choice += 1;
                }
                break;
            }
            else if(!DOWN && UP && !LEFT && !RIGHT){
                if(!current_song_choice){
                    current_song_choice = 0;
                }
                else{
                current_song_choice -= 1;
                
                }            
                break;
            }
            else if(!DOWN && !UP && !LEFT && RIGHT){
                if(!current_song_choice){
                    state = LCD_WAIT2;
                }
                else{
                    state = LCD_Wait_Mii;
                    break;
                    
                }
                break;
            }
            else{
                state = LCD_Menu_Songs;
                break;
            }    
        case LCD_Wait_Mii:
            if(LEFT || RIGHT || UP || DOWN){
                state = LCD_Wait_Mii;
            }
            else{
                LCD_ClearScreen();
                state = LCD_Menu_Mii;
            }
            break;
        case LCD_WAIT2:
            if(LEFT || RIGHT || UP || DOWN){
                state = LCD_WAIT2;
            }
            else{
                LCD_ClearScreen();
                state = LCD_Menu_Zense;
            }
            break;
        case LCD_BACK:
            if(LEFT || RIGHT || UP || DOWN){
                state = LCD_BACK;
            }
            else{
                LCD_ClearScreen();
                state = LCD_Menu_Songs;
            }
            break;
            
        case LCD_Menu_Zense:      
            if(LEFT && !RIGHT && !UP && !DOWN){
                state = LCD_BACK;
                break;
            }
            else if(!LEFT && RIGHT && !UP && !DOWN){
                LCD_ClearScreen();
                state = LCD_WAIT3;
                break;
            }
            state = LCD_Menu_Zense;
            break;    
        case LCD_Menu_Mii:
             if(LEFT && !RIGHT && !UP && !DOWN){
                 state = LCD_BACK;
                 break;
             }
             else if(!LEFT && RIGHT && !UP && !DOWN){
                 LCD_ClearScreen();
                 state = LCD_WAIT3;
                 break;
             }
             else{
                 state = LCD_Menu_Mii;
                 break;
             }             
        case LCD_WAIT3:
            if(LEFT || RIGHT || UP || DOWN){
                state = LCD_WAIT3;
            }
            else{
                state = Play_CountDown;
            }
            break;
        
        case Play_CountDown:
            if(count_down < 25){
                count_down++;
                state = Play_CountDown;
                break;
            }
            else{
                count_down = 0;
                playing = true;
                state = Play_Song;
                break;
            }
        case Play_Song:
            if((song_finished == false) || (playing == true)){
                state = Play_Song;
                break;
            }
            else{
                state = Score_Screen;
                break;
            }
        case Score_Screen:
            if(songs[current_song_choice].high_score < current_score){
                song_finished = false;
                score_increase = false;
                LCD_ClearScreen();
                state = LCD_High_Score;
                break;
            }
            else if(LEFT || RIGHT || UP || DOWN){
                song_finished = false;
                score_increase = false;
                state = LCD_WAIT4;
                break;
            }
            else{
                state = Score_Screen;
                break;
            }
        case LCD_High_Score:
            if(LEFT || RIGHT || UP || DOWN){
                state = LCD_WAIT4;
            }
            else{
                state = LCD_High_Score;
            }
            break;
            
        case LCD_WAIT4:
            if(LEFT || RIGHT || UP || DOWN){
                state = LCD_WAIT4;
            }
            else{
                LCD_ClearScreen();
                state = LCD_Menu_Start;
            }
            break;
        
    }
    switch(state){ //actions
        case LCD_Menu_Init:
            break;
        case LCD_Menu_Start:
            current_song_choice = 0;
            current_score = 0;
            LCD_Game_Menu();
            break;
        case LCD_WAIT1:
            break;
        case LCD_Menu_Songs:
            if(current_song_choice){
                LCD_Cursor(1);
                LCD_WriteData(' ');
                LCD_Cursor(17);
            }
            else{
                LCD_Cursor(17);
                LCD_WriteData(' ');
                LCD_Cursor(1);
            }
            LCD_WriteData(0xA5);
            LCD_Write_Single_Line(2,1, "ZEN ZEN ZENSE");
            LCD_Write_Single_Line(2,2, "MII");
            break;
        case LCD_WAIT2:
            break;
        case LCD_Wait_Mii:
            break;
        case LCD_Menu_Zense:
            LCD_Write_Single_Line(1,1, "Press - to play");
            LCD_Cursor(7);
            LCD_WriteData(0x00);
            LCD_Write_Single_Line(1,2, "High Score:");
            LCD_Cursor(28);
            LCD_WriteData(((songs[current_song_choice].high_score / 100) % 10) + '0');
            LCD_WriteData(((songs[current_song_choice].high_score/ 10) % 10) + '0'); //tens place
            LCD_WriteData((songs[current_song_choice].high_score % 10) + '0');
            LCD_Cursor(33);
            break;
        case LCD_Menu_Mii:
            LCD_Write_Single_Line(1,1, "Press - to play");
            LCD_Cursor(7);
            LCD_WriteData(0x00);
            LCD_Write_Single_Line(1,2, "High Score:");
            LCD_Cursor(28);
            LCD_WriteData(((songs[current_song_choice].high_score / 100) % 10) + '0');
            LCD_WriteData(((songs[current_song_choice].high_score/ 10) % 10) + '0'); //tens place
            LCD_WriteData((songs[current_song_choice].high_score % 10) + '0');
            LCD_Cursor(33);
            break;
        case LCD_WAIT3:
            break;
        case Play_CountDown:
            LCD_DisplayString(1, "Score:");
            LCD_Cursor(9);
            LCD_WriteData(current_score + '0');
            LCD_WriteData(current_score + '0');
            LCD_WriteData(current_score + '0');
            /*
            LCD_Cursor(17);
            LCD_WriteData(high_score + '0');
            */
            break;
        case Play_Song:
            if(score_increase == true){
                LCD_Cursor(9);
                LCD_WriteData(((current_score / 100) % 10) + '0'); //hundreds
                LCD_WriteData(((current_score / 10) % 10) + '0'); //tens place
                LCD_WriteData((current_score % 10) + '0'); //ones
                break;
            }
            else{
                if(speed == 6){
                    LCD_Write_Single_Line(1,2, "Moderate");
                }
                else{
                    LCD_Write_Single_Line(1,2, "HARD");
                }
                break;
            }
        case Score_Screen: 
            LCD_Write_Single_Line(1,2, "Try again :(");
            break;
        case LCD_High_Score:
            LCD_DisplayString(1, "HIGH SCORE!:");
            LCD_Cursor(13);
            LCD_WriteData(((current_score / 100) % 10) + '0');
            LCD_WriteData(((current_score / 10) % 10) + '0'); //tens place
            LCD_WriteData((current_score % 10) + '0');
            songs[current_song_choice].high_score = current_score;
            if(!current_song_choice){
                eeprom_update_byte((uint8_t*)1, (uint8_t) current_score);
            }
            else{
                eeprom_update_byte((uint8_t*)2, (uint8_t) current_score);
            }
      
           
            break;
        case LCD_WAIT4:
            break;
        
    }

    return state;
}

int RGB_Matrix_Tick(int state){
    static unsigned char play_note;
    static unsigned char rest_note;
    static unsigned char note_to_hit;
    switch(state){
        case RGB_INIT:
            state = RGB_MENU;
            break;
        case RGB_MENU:
            if(playing){
                if(current_song_choice){
                    speed = 6;
                }
                play_note = 0;
                rest_note = 0;
                state = RGB_SONG;
                break;
            }
            else{
                state = RGB_MENU;
                break;
            }    
        case RGB_SONG:
            if(song_finished || !playing){
                state = RGB_RESET;
                break;
            }        
            else{
                state = RGB_SONG;
                break;
            }
        case RGB_RESET:
            state = RGB_MENU;
            break;
    }
    switch(state){
        case RGB_INIT:
            break;
        case RGB_MENU:
            if(LEFT || RIGHT || UP || DOWN){
                for(unsigned char i = 0; i < 8; i++){
                    Shift_transmit_data(128 >> i);
                    Shift_transmit_data(~pic[i]);
                    delay_ms(3);
                }
            }
            Shift_transmit_data(0);
            Shift_transmit_data(0);
            
            break;
        case RGB_SONG:
            for(unsigned j = 0; j < 8; j++){
                RGB_DISPLAY_NOTES[j] <<= 1;
            }
            if(play_note < songs[current_song_choice].timing[current_note]){
                note_to_hit = songs[current_song_choice].hit[current_note];
                
                if(note_to_hit == 8){
                    RGB_DISPLAY_NOTES[7] |= 3;
                    RGB_DISPLAY_NOTES[6] |= 3;
                }
                else if(note_to_hit == 4){
                    RGB_DISPLAY_NOTES[5] |= 3;
                    RGB_DISPLAY_NOTES[4] |= 3;
                }
                else if(note_to_hit == 2){
                    RGB_DISPLAY_NOTES[3] |= 3;
                    RGB_DISPLAY_NOTES[2] |= 3;
                }    
                else if(note_to_hit == 1){
                    RGB_DISPLAY_NOTES[1] |= 3;
                    RGB_DISPLAY_NOTES[0] |= 3;
                }    
                play_note++;        
            }
            else{
                note_to_hit = 0;
                if(rest_note < songs[current_song_choice].rest_timing[current_note]){
                    rest_note++;
                }
                else{
                    play_note = 0;
                    rest_note = 0;   
                }     
            }                   
            for(unsigned char i = 0; i < 8; i++){
                Shift_transmit_data(128 >> i);
                Shift_transmit_data(~RGB_DISPLAY_NOTES[i]);
                delay_ms(speed);
            
            }
            break;
        case RGB_RESET:
            for(unsigned char r = 0; r < 8; r++){
                RGB_DISPLAY_NOTES[r] = 0;
            }
            break;
    }
    
    return state;
}

int Note_Tick(int state){
    static unsigned char play_note;
    static unsigned char rest_note;
    static unsigned char note_to_hit;
    static unsigned char hitting_note;
    static bool already_hit;
    
    switch(state){
        case Note_INIT:
            state = Note_Wait;
            break;
        case Note_Wait:
            if(playing){
                PWM_on();
                state = Note_Play_Song;
                break;
            }
            else{
                state = Note_Wait;
                break;
            }
        case Note_Play_Song:
            if(song_finished){
                state = Note_Reset;
                break;
            }
            else{
                state = Note_Play_Song;
                break;
            }
        case Note_Reset:
            state = Note_Wait;
            break;
    }
    switch(state){
        case Note_INIT:
            break;
        case Note_Wait:
            play_note = 0;
            rest_note = 0;
            hitting_note = 0;
            already_hit = false;
            break;
        case Note_Play_Song:
            note_to_hit = songs[current_song_choice].hit[current_note];
            if(hitting_note < songs[current_song_choice].timing_hit[current_note] && (already_hit == false)){
                if(player_button_press == note_to_hit){
                    current_score++;
                    score_increase = true;
                }
                else{
                    score_increase = false;
                }
                hitting_note++;
            }
            if(play_note < songs[current_song_choice].timing[current_note]){
                if(player_button_press == note_to_hit){
                    current_score++;
                    score_increase = true;
                }
                else{
                    score_increase = false;
                }
                set_PWM(songs[current_song_choice].notes[current_note]);
                play_note++;
                break;
            }
            else{
                set_PWM(0);
                if(rest_note < songs[current_song_choice].rest_timing[current_note]){
                    rest_note++;
                }
                else{
                    play_note = 0;
                    rest_note = 0;
                    hitting_note = 0;
                    already_hit = false;
                    if(current_note < songs[current_song_choice].length){
                        current_note++;
                    }
                    else{
                        playing = false;
                        song_finished = true;
                        break;
                    }
                }
            }
            break;
        case Note_Reset:
            PWM_off();
            current_note = 0;
            score_increase = false;
            break;
    }
    return state;
}

int Player_Tick(int state){
    switch(state){
        case Player_Init:
            state = Player_Press;
            break;
        case Player_Press:
            state = Player_Press;
            break;
    }
    switch(state){
        case Player_Init:
            break;
        case Player_Press:
            if(LEFT && !RIGHT && !UP && !DOWN){
                player_button_press = 8;
            }
            else if(!LEFT && !RIGHT && !UP && DOWN){
                player_button_press = 4;
            }
            else if(!LEFT && !RIGHT && UP && !DOWN){
                player_button_press = 2;
            }
            else if(!LEFT && RIGHT && !UP && !DOWN){
                player_button_press = 1;
            }
            else{
                player_button_press = 0;
            }
            
            break;
    }
    return state;
}

