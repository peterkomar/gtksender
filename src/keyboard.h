/***************************************************************************
*            interface.h
*
*  Fri Sep 21 16:55:40 2007
*  Copyright  2007  Peter Komar
*  Email: markus_sksoft@mail.ru
 ****************************************************************************/
#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_

#include <glib.h>

#include "interface.h"


typedef struct
{
int mod;//modifer
char symbol;//symbol 
int code; //code symbol
} data_keys;

GList* init_keys(); //init keys keyboard
void send_key_to_win1 (data_keys *data);//send one key code to active window
// converting string to keycodes 
GList* convert_text_to_X11_keys1(gchar* string);
//send string to active window
void send_string_to_win1(gchar *str);
void init_kbd ();
void grab_key (int key_code,int state);
void update_kbd ();
void ungrab_key (int key_code);
void glist_grab_ungrab_key (int i);

#endif /* __KEY_H_ */
