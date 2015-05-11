/* Bik-bak technologies
 * main.h
 *
 * Copyright (C) 2007 Peter kievomar
 * mailto: markus_sksoft@mail.ru
*/

#include "configurator.h"

int main( int   argc,
          char *argv[] )
{
  GtkWidget *window1;
  gtk_init (&argc, &argv);
  
  data_conf *data = g_malloc(sizeof(data_conf));
  
  window1 = create_dialog_config(data);
  gtk_widget_show (window1);
  gtk_main();
  g_free(data);
  return(0);
}
