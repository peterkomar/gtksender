/***************************************************************************
 *            interface.c
 *
 *  Fri Sep 21 16:58:18 2007
 *  Copyright  2007  Peter Komar
 *  Email: markus_sksoft@mail.ru
 ****************************************************************************/
//#include "keyboard.h"
#include "interface.h"

static GtkWidget *window1, *entry1;

GtkWidget* create_pixmap(GtkWidget *widget, const gchar *filename)
{
	GtkWidget *pixmap;
	
	if (!filename || !filename[0])
	return gtk_image_new ();
	
	/*pathname = find_pixmap_file (filename);
	
	if (!pathname)
	{
	     g_warning ("Couldn't find pixmap file: %s", filename);
	     return gtk_image_new ();
	}*/
        //GdkPixbuf *pix = gdk_pixbuf_new_from_file(filename,NULL);
	
	pixmap = gtk_image_new_from_file (filename);
        //pixmap = gtk_image_new_from_pixbuf(pix);

	
	return pixmap;
}

static void message_dialog (GtkMessageType type, const gchar * message)
{
  GtkWidget *dlg;

  dlg = gtk_message_dialog_new (NULL,
  GTK_DIALOG_MODAL,
  type, GTK_BUTTONS_CLOSE, message);
  gtk_label_set_use_markup (GTK_LABEL (GTK_MESSAGE_DIALOG (dlg)->label), TRUE);
  gtk_window_set_position (GTK_WINDOW (dlg), GTK_WIN_POS_CENTER);
  
  gtk_dialog_run (GTK_DIALOG (dlg));
  gtk_widget_destroy (dlg);
}

/* wrappers around gtk_message_dialog (OBSOLETE) */
void show_info (const char *text)
{
    message_dialog (GTK_MESSAGE_INFO, text);
}

void show_error (const char *text)
{
   message_dialog (GTK_MESSAGE_ERROR, text);
}

//call if user press enter
void enter_callback( GtkWidget *entry,
                     gpointer *data )
{
  gchar *entry_text;
  
  entry_text = gtk_entry_get_text(GTK_ENTRY(entry));//отримати текст і
  
  gtk_widget_hide(window1); 
   
  send_string_to_win1(entry_text); //послати активному вікну
  
  /*gtk_widget_destroy(window1);
   window1 = NULL;*/
  gtk_widget_hide(window1);  
}
//show string or no
void entry_toggle_visibility( GtkWidget *checkbutton,
                              GtkWidget *entry )
{
  gtk_entry_set_visibility(GTK_ENTRY(entry),
			 GTK_TOGGLE_BUTTON(checkbutton)->active);
}
//hide window if user press Esc
gboolean press_esk(GtkWidget   *widget, GdkEventKey *event, gpointer user_data)
{

if(event->keyval == GDK_Escape)
  {
     gtk_widget_hide(window1);     
     return TRUE;
  }
      
  return FALSE;
}

//Створення віджету
void create_input_widget()
{

  //GtkWidget *window1;
  if(window1)
       return;

  GtkWidget *vbox1;
  GtkWidget *label1;
  GtkWidget *frame1;
  GtkWidget *alignment1;
  //GtkWidget *entry1;
  GtkWidget *label2;
  GtkWidget *hbox1;
  GtkWidget *checkbutton1;
  GtkWidget *frame2;
  GtkWidget *alignment2;
  GtkWidget *key_read_prog;
  GtkWidget *label3;
  GtkWidget *hbox2;
  GtkWidget *image1;
  GtkWidget *frame3;
  GtkWidget *alignment3;
  GtkWidget *l_keys;
  GtkWidget *label4;

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_signal_connect(GTK_OBJECT (window1), "key-press-event",(GtkSignalFunc) press_esk, NULL);
  gtk_window_set_decorated(GTK_WINDOW(window1),FALSE);
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_MOUSE);
  gtk_container_set_border_width (GTK_CONTAINER (window1), 5);
  //gtk_window_stick(GTK_WINDOW(window1));
  gtk_window_set_keep_above(GTK_WINDOW(window1), TRUE);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (window1), vbox1);

  label1 = gtk_label_new ("Please input BIK-BAK key or/and type the password.\nPress <b>Enter</b> to confirm or <b>Esc</b> to cancel");
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (vbox1), label1, FALSE, FALSE, 0);
  gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);
  gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_CENTER);
  gtk_label_set_line_wrap (GTK_LABEL (label1), TRUE);

  frame1 = gtk_frame_new (NULL);
  gtk_widget_show (frame1);
  gtk_box_pack_start (GTK_BOX (vbox1), frame1, TRUE, TRUE, 0);
  gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_NONE);

  alignment1 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment1);
  gtk_container_add (GTK_CONTAINER (frame1), alignment1);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment1), 0, 0, 12, 0);

  entry1 = gtk_entry_new ();
  gtk_widget_show (entry1);
  gtk_container_add (GTK_CONTAINER (alignment1), entry1);
  gtk_entry_set_visibility (GTK_ENTRY (entry1), FALSE);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry1), 9679);
  gtk_signal_connect(GTK_OBJECT(entry1), "activate", GTK_SIGNAL_FUNC(enter_callback), NULL);

  label2 = gtk_label_new ("<b>Password</b>");
  gtk_widget_show (label2);
  gtk_frame_set_label_widget (GTK_FRAME (frame1), label2);
  gtk_label_set_use_markup (GTK_LABEL (label2), TRUE);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);

  checkbutton1 = gtk_check_button_new_with_mnemonic ("Show password");
  gtk_widget_show (checkbutton1);
  gtk_box_pack_start (GTK_BOX (hbox1), checkbutton1, FALSE, FALSE, 0);
  gtk_signal_connect (GTK_OBJECT(checkbutton1), "toggled", GTK_SIGNAL_FUNC(entry_toggle_visibility), entry1);

  frame2 = gtk_frame_new (NULL);
  gtk_widget_show (frame2);
  gtk_box_pack_start (GTK_BOX (hbox1), frame2, TRUE, TRUE, 0);
  gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_NONE);

  alignment2 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment2);
  gtk_container_add (GTK_CONTAINER (frame2), alignment2);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment2), 0, 0, 12, 0);

  key_read_prog = gtk_progress_bar_new ();
  gtk_widget_show (key_read_prog);
  gtk_container_add (GTK_CONTAINER (alignment2), key_read_prog);

  label3 = gtk_label_new ("<b>Read key progress</b>");
  gtk_widget_show (label3);
  gtk_frame_set_label_widget (GTK_FRAME (frame2), label3);
  gtk_label_set_use_markup (GTK_LABEL (label3), TRUE);

  hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox2, TRUE, TRUE, 0);

  image1 = create_pixmap (window1, "../img/key.png");
  gtk_widget_show (image1);
  gtk_box_pack_start (GTK_BOX (hbox2), image1, TRUE, TRUE, 0);

  frame3 = gtk_frame_new (NULL);
  gtk_widget_show (frame3);
  gtk_box_pack_start (GTK_BOX (hbox2), frame3, TRUE, TRUE, 0);
  gtk_frame_set_shadow_type (GTK_FRAME (frame3), GTK_SHADOW_NONE);

  alignment3 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment3);
  gtk_container_add (GTK_CONTAINER (frame3), alignment3);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment3), 0, 0, 12, 0);

  l_keys = gtk_label_new ("0");
  gtk_widget_show (l_keys);
  gtk_container_add (GTK_CONTAINER (alignment3), l_keys);

  label4 = gtk_label_new ("<b>Key count's</b>");
  gtk_widget_show (label4);
  gtk_frame_set_label_widget (GTK_FRAME (frame3), label4);
  gtk_label_set_use_markup (GTK_LABEL (label4), TRUE);
  //gtk_widget_show_all(window1);
  //gdk_window_set_accept_focus (window1->window,TRUE);
  //gdk_window_set_skip_taskbar_hint(window1->window,TRUE);
  //gtk_window_present(GTK_WINDOW(window1));
  gtk_widget_hide(window1);
  //gdk_window_hide(window1->window);
}


 
GdkFilterReturn window_event_filter(GdkXEvent *xevent, GdkEvent *gdkevent, gpointer data)
{

     XEvent *event= (XEvent *)xevent;
          	
	 switch (event->type) 
     {
		 case KeyPress:
			 if (event->xkey.send_event == TRUE)
				return GDK_FILTER_REMOVE;
				 				 
				//create_input_widget();
			 gtk_entry_set_text(GTK_ENTRY(entry1),"");
			 //gdk_window_show(GDK_WINDOW(window1->window));
			 //gdk_window_set_accept_focus (window1->window,TRUE);
			 //gdk_window_focus(window1->window,10);
			 gtk_window_present(GTK_WINDOW(window1));
				//gtk_window_present_with_time(GTK_WINDOW(window1),1);
				 
				return GDK_FILTER_REMOVE;
			 break;
	  }
   return GDK_FILTER_CONTINUE;
}

void show_about_dlg()
{
   GtkWidget *dialog = NULL;
   const gchar *str="Deamon bik-bak is runing.";
   dialog = gtk_message_dialog_new (NULL,  GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,str);
   gtk_widget_show(dialog);
   gtk_window_present(GTK_WINDOW(dialog));
   gtk_dialog_run (GTK_DIALOG (dialog));
   gtk_widget_destroy (dialog);
}

