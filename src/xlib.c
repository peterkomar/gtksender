/***************************************************************************
*            interface.h
*
*  Fri Sep 21 16:55:40 2007
*  Copyright  2007  Peter Komar
*  Email: markus_sksoft@mail.ru
 ****************************************************************************/
 
 #include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/Xlibint.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#include "xlib.h"

/* modifer */

static char * modifier_string [] = {"Control", "Shift", "Alt","Win"};
static char * modifier_string_mouse [] = {"Mouse Left", "Mouse Center","Mouse Right","Scrool Up","Scrool Down","Scrool Left","Scrool Right"};

int DellModifier (unsigned int modifier)
{
	int mod = 0;

	if (modifier & ControlMask )
		mod += ControlMask;
  
	if (modifier & ShiftMask)
		mod += ShiftMask;

	if (modifier & Mod1Mask)
		mod += Mod1Mask;

	if (modifier & Mod4Mask)
		mod += Mod4Mask;
	
	return mod;
}
int AddCapsModifier (unsigned int modifier)
{
	int mod = DellModifier(modifier);

	mod += LockMask;
	return mod;
}
int AddNumModifier (unsigned int modifier)
{
	int mod = DellModifier(modifier);

	mod += Mod2Mask;
	return mod;
}
int AddCapsNumModifier (unsigned int modifier)
{
	int mod = DellModifier(modifier);

	mod += LockMask;
	mod += Mod2Mask;
	return mod;
}

int ModifierToString (unsigned int modifier, char * str)
{
	str[0] = '\0';
	int n=0;
	if (modifier & ControlMask )
	{
		if (str[0])
		strcat (str, "+");
		strcat (str, modifier_string[0]);
		n++;
	}
  
	if (modifier & ShiftMask)
	{
		if (str[0])
		strcat (str, "+");
		strcat (str, modifier_string[1]);
		n++;
	}

	if (modifier & Mod1Mask)
	{
		if (str[0])
		strcat (str, "+");
		strcat (str, modifier_string[2]);
		n++;
	}
	if (modifier & Mod4Mask)
	{
		if (str[0])
		strcat (str, "+");
		strcat (str, modifier_string[3]);
		n++;
	}
     return n;
}
int StringToModifier_Key (char * str,unsigned int * modifier)
{
	int key=0;
	gchar *temp;
 
	temp=strtok(str,"+");
	*modifier=0;
	
	while(temp!=NULL)
	{
		if (strcasecmp (temp, "control") == 0)
		  *modifier += ControlMask;
		else if (strcasecmp (temp, "shift") == 0)
		  *modifier += ShiftMask;
		else if (strcasecmp (temp, "mod1") == 0 || strcasecmp (temp, "alt") == 0)
		  *modifier += Mod1Mask;
		else if (strcasecmp (temp, "mod4") == 0 || strcasecmp (temp, "win") == 0)
		  *modifier += Mod4Mask;
		else
		{
			if(atoi(temp)!=0)
				 key=atoi(temp);
			else
			{
				key =XKeysymToKeycode(GDK_DISPLAY(),XStringToKeysym (temp));
				if (key == 0)
					break;
				temp=strtok(NULL,"+");
				    continue;
			}
		}
		temp=strtok(NULL,"+");
	}
	g_free(temp);
	temp=NULL;
	
	return key;
}


