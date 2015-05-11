/***************************************************************************
 *            interface.h
 *
 *  Fri Sep 21 16:55:40 2007
 *  Copyright  2007  Peter Komar
 *  Email: markus_sksoft@mail.ru
 ****************************************************************************/

#ifndef _INTERFACE_H
#define _INTERFACE_H

#include <gdk/gdkkeysyms.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/XTest.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//call if user press enter
void enter_callback( GtkWidget *entry,
                     gpointer *data );

void show_info (const char *text);
void show_error (const char *text);

//show string or no
void entry_toggle_visibility( GtkWidget *checkbutton,
                              GtkWidget *entry );

//hide window if user press Esc
gboolean press_esk(GtkWidget   *widget, GdkEventKey *event, gpointer user_data);

//Створення віджету
void create_input_widget();

GdkFilterReturn window_event_filter(GdkXEvent *xevent, GdkEvent *gdkevent, gpointer data);

void show_about_dlg();

#endif /* _INTERFACE_H */
