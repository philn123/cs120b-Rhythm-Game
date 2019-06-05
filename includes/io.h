#ifndef __io_h__
#define __io_h__

void LCD_init();
void LCD_ClearScreen(void);
void LCD_WriteCommand (unsigned char Command);
void LCD_Cursor (unsigned char column);
void LCD_DisplayString(unsigned char column ,const unsigned char *string);
void delay_ms(int miliSec);

//custom functions
void LCD_Custom_Character (unsigned char loc, unsigned char *msg); //builds characters
void LCD_Write_Single_Line(unsigned char column, unsigned line_num, const unsigned char* string); //displays strings on one line
void LCD_Game_Menu(); //initializes main menu
#endif

