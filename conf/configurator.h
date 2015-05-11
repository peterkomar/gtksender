/***************************************************************************
*            interface.h
*
*  Fri Sep 21 16:55:40 2007
*  Copyright  2007  Peter Komar
*  Email: markus_sksoft@mail.ru
 ****************************************************************************/

#ifndef __CONFIGURATOR_H__
#define __CONFIGURATOR_H__


#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include <X11/X.h>
#include <gdk/gdkx.h>


typedef struct
{
 GtkWidget *pass_entry;
 GtkWidget *key_entry;
 GtkWidget *status;
 GtkWidget *dev_combo;
}  data_conf;

GtkWidget* create_dialog_config (data_conf *data);

#endif /* __SCONFDEVICE_H__ */

