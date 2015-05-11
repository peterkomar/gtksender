/***************************************************************************
*            interface.h
*
*  Fri Sep 21 16:55:40 2007
*  Copyright  2007  Peter Komar
*  Email: markus_sksoft@mail.ru
 ****************************************************************************/
#include <X11/X.h>
#include <X11/Xlib.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "keyboard.h"
#include "xlib.h"
#include "interface.h"
#include "../conf/configfile.h"

#define R1LEN 13
#define R2LEN 12
#define R3LEN 11
#define R4LEN 10

GList* init_keys() //init keys keyboard
{
	char *ks1 = "`1234567890-=";
	char *ks1s = "~!@#$%^&*()_+";
	unsigned int kc1[R1LEN] = {49,10,11,12,13,14,15,16,17,18,19,20,21};

	char *ks2 = "qwertyuiop[]";
	char *ks2s = "QWERTYUIOP{}";
	unsigned int kc2[R2LEN] = {24,25,26,27,28,29,30,31,32,33,34,35};

	char *ks3 = "asdfghjkl;'";
	char *ks3s = "ASDFGHJKL:\"";
	unsigned int kc3[R3LEN] = {38,39,40,41,42,43,44,45,46,47,48};

	char *ks4 = "zxcvbnm,./";
	char *ks4s = "ZXCVBNM<>?";
	unsigned int kc4[R4LEN] = {52,53,54,55,56,57,58,59,60,61};
	
	GList *list=NULL;
	int i=0;
	//add normal
	char *pstr=NULL;
	pstr = ks1;
	for(i=0; i<R1LEN; ++i)
	{
		data_keys *data = g_malloc0(sizeof(data_keys));
		data->code = kc1[i];
		data->symbol = (*pstr);
		data->mod = 0;
		list = g_list_append(list,data);
		pstr++;
	}
	//add with shift
	pstr = ks1s;
	for ( i=0;i<R1LEN;i++ )
	{
		data_keys *data = g_malloc0(sizeof(data_keys));
		data->code = kc1[i];
		data->symbol = (*pstr);
		data->mod = 50;
		list = g_list_append(list,data);
		pstr++;
	}
	//add normal
	pstr = ks2;
	for ( i=0; i<R2LEN; i++ )
	{
		data_keys *data = g_malloc0(sizeof(data_keys));
		data->code = kc2[i];
		data->symbol = (*pstr);
		data->mod = 0;
		list = g_list_append(list,data);
		pstr++;
	}
	//add with shift
	pstr = ks2s;
	for ( i=0; i<R2LEN; i++ )
	{
		data_keys *data = g_malloc0(sizeof(data_keys));
		data->code = kc2[i];
		data->symbol = (*pstr);
		data->mod = 50;
		list = g_list_append(list,data);
		pstr++;
	}
	//add normal
	pstr = ks3;
	for ( i=0; i<R3LEN; i++ )
	{
		data_keys *data = g_malloc0(sizeof(data_keys));
		data->code = kc3[i];
		data->symbol = (*pstr);
		data->mod = 0;
		list = g_list_append(list,data);
		pstr++;
	}
	//add with shift
	pstr = ks3s;
	for ( i=0; i<R3LEN; i++ )
	{
		data_keys *data = g_malloc0(sizeof(data_keys));
		data->code = kc3[i];
		data->symbol = (*pstr);
		data->mod = 50;
		list = g_list_append(list,data);
		pstr++;
	}
	//add normal
	pstr = ks4;
	for ( i=0; i<R4LEN; i++ )
	{
		data_keys *data = g_malloc0(sizeof(data_keys));
		data->code = kc4[i];
		data->symbol = (*pstr);
		data->mod = 0;
		list = g_list_append(list,data);
		pstr++;
	}
	//add with shift
	pstr = ks4s;
	for ( i=0; i<R4LEN; i++ )
	{
		data_keys *data = g_malloc0(sizeof(data_keys));
		data->code = kc4[i];
		data->symbol = (*pstr);
		data->mod = 50;
		list = g_list_append(list,data);
		pstr++;
	}
	
	data_keys *data = g_malloc0(sizeof(data_keys));
	data->code = 51;
	data->symbol = '\\';
	data->mod = 0;
	list = g_list_append(list,data);
	
	data = g_malloc0(sizeof(data_keys));
	data->code = 51;
	data->symbol = '|';
	data->mod = 50;
	list = g_list_append(list,data);
		
	return list;   
}

void send_key_to_win1 (data_keys *data)//send one key code to active window
{
	Window curr_focus;
	int revert_to;
	Display *dy = gdk_x11_get_default_xdisplay();
	
	XGetInputFocus ( dy, &curr_focus, &revert_to );
	
	if ( data->mod ) {
		XTestFakeKeyEvent ( dy, data->mod, TRUE, 0 );
	}
	
	XTestFakeKeyEvent ( dy, data->code, TRUE,1 );
	XTestFakeKeyEvent ( dy, data->code, FALSE, 2 );
	if ( data->mod ) {
	XTestFakeKeyEvent ( dy, data->mod, FALSE, 2 );
	}
	
	XFlush ( dy/*GDK_DISPLAY()*/ );
}
// converting string to keycodes 
GList* convert_text_to_X11_keys1(gchar* string)
{

	gchar* st = string;
	GList *list=NULL;

	GList *keys = init_keys();
	
	//printf("converting - %s leng list %d\n",st, g_list_length(keys));

	while( *st != '\0' )
	{	data_keys *data = g_malloc0(sizeof(data_keys));

		GList *p = g_list_first(keys);
		
		//printf("Count list %d\n",g_list_length(p));

		while(p != NULL)
		{
			data_keys *dat = (data_keys*)p->data;
		        //printf("Symbols %c -- %c\n",dat->symbol,*st);
		
			if( dat->symbol == (*st) )
			{
				data->code = dat->code;
				data->symbol = dat->symbol;
				data->mod = dat->mod;
				list = g_list_append(list,data);
				break;
			}
			
			p = g_list_next(p);
		}
			
		++st;
	}
			
	return list;
}
//send string to active window
void send_string_to_win1(gchar *str)
{
	gchar *st = str;

	GList *list_keys = NULL;

	//printf("sendet - %s\n",st);

	list_keys = convert_text_to_X11_keys1(st);//транслюємо стрічку посимвольно у список кодів
	
	GList *p = g_list_first(list_keys);

	while(p != NULL)
	{
		data_keys *dat = (data_keys*)p->data;
		
		send_key_to_win1(dat);// посилаємо кожен окремий символ актимвному вікну
		p = g_list_next(p);
	}
		
	g_list_free(list_keys);
}

//grabing keys inf i = 0 and ungrabing if i != 0
void glist_grab_ungrab_key (int i)
{
	int modifer=0;
	
	ConfigFile *c_data  = open_cfg_file(NULL);
	
	gchar *scodes = c_data->key_mod;//"56:64"; // code and modifers 

	gchar **key_modifer = g_strsplit(scodes,":",2);
	
	if(key_modifer[1]==NULL)
		modifer=0;
	else
			modifer=atoi(key_modifer[1]);
		if(i==0)
		{
		    grab_key (atoi(key_modifer[0]),modifer);
			
		}
		else
			ungrab_key(atoi(key_modifer[0]));

		g_strfreev(key_modifer);
}


void ungrab_key (int key_code)
{
	GdkWindow *root=gdk_get_default_root_window();
	
	gdk_error_trap_push ();
	XUngrabKey (GDK_DISPLAY (), key_code, AnyModifier,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())));
	XUngrabKey (GDK_DISPLAY (), AnyKey, AnyModifier, (GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())));


	gdk_flush ();
	if (gdk_error_trap_pop ()) 
	{
		gchar *error;
		gchar *key;

		key=g_strdup_printf ("%s",(XKeysymToString (XKeycodeToKeysym (GDK_DISPLAY (), key_code, 0)) != NULL)? 
		XKeysymToString (XKeycodeToKeysym (GDK_DISPLAY (),key_code, 0)):
		g_strdup_printf ("%d",key_code));
		
		error = g_strdup_printf
			("It seems that another application already has"
			   " access to the multimedia keys.\n"
			   "Key %s couldn't be bound.\n"
			   "Is another daemon already running ?\n",
			 key);
		printf("[Bik_bak][ERROR]:%s",error);
		show_error(error);
		g_free (key);
		g_free (error);
	}
}

void update_kbd ()
{
	glist_grab_ungrab_key(1);
}

void grab_key (int key_code,int state)
{
	GdkWindow *root=gdk_get_default_root_window();
	gdk_error_trap_push ();

	if(state>0)
	{
		XGrabKey (GDK_DISPLAY (), key_code,
			state,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), False,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			DellModifier(state),
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), False,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			AddCapsModifier(state),
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), False,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			AddNumModifier(state),
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), False,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			AddCapsNumModifier(state),
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), False,
			GrabModeAsync, GrabModeAsync);
	}
	else
	{
		XGrabKey (GDK_DISPLAY (), key_code,
			0,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), True,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			Mod2Mask,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), True,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			Mod5Mask,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), True,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			LockMask,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), True,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			Mod2Mask | LockMask,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), True,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			Mod5Mask | LockMask,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), True,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			Mod2Mask | Mod5Mask,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), True,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			Mod2Mask | Mod5Mask | LockMask,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), True,
			GrabModeAsync, GrabModeAsync);
	}

	gdk_flush ();
	if (gdk_error_trap_pop ())
	{
		gchar *error;
		gchar *str;
		gchar *key;

		str = g_new (gchar,513);
		 
		ModifierToString (state, str);
		key=g_strdup_printf ("%s%s%s",str,str[0] ? "+" : "",
		(XKeysymToString (XKeycodeToKeysym (GDK_DISPLAY (), key_code, 0)) != NULL)? 
		XKeysymToString (XKeycodeToKeysym (GDK_DISPLAY (),key_code, 0)):
		g_strdup_printf ("%d",key_code));
		
		error = g_strdup_printf
			("It seems that another application already has"
			   " access to the multimedia keys.\n"
			   "Key %s couldn't be bound.\n"
			   "Is another daemon already running ?\n",
			 key);
		//show_error(error);
		printf("[Bik-bak][ERROR]:%s",error);
		show_error(error);
		g_free (key);
		g_free (str);
		g_free (error);
	}
}


void init_kbd ()
{
	glist_grab_ungrab_key(0);
	//read keys from config
	
	gdk_window_add_filter (gdk_get_default_root_window(),window_event_filter,NULL);
}
