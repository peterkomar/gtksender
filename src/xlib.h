/***************************************************************************
*            interface.h
*
*  Fri Sep 21 16:55:40 2007
*  Copyright  2007  Peter Komar
*  Email: markus_sksoft@mail.ru
 ****************************************************************************/
#ifndef __XLIB_H_
#define __XLIB_H_

int DellModifier (unsigned int modifier);
int AddCapsModifier (unsigned int modifier);
int AddNumModifier (unsigned int modifier);
int AddCapsNumModifier (unsigned int modifier);

int ModifierToString (unsigned int modifier, char * str);
int StringToModifier_Key (char * str,unsigned int * modifier);

#endif /* __XLIB_H_ */
