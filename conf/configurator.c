/***************************************************************************
*            interface.h
*
*  Fri Sep 21 16:55:40 2007
*  Copyright  2007  Peter Komar
*  Email: markus_sksoft@mail.ru
 ****************************************************************************/

#include <gtk/gtksignal.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <X11/X.h>
#include <gdk/gdkx.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "configurator.h"
#include "../src/xlib.h"
#include "configfile.h"

/********************************Start create images********************************/
/* This is an internally used function to create pixmaps. */
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
/********************************End create images********************************/
/********************************Start Keyboard events********************************/
static void press_key (GtkWidget *widget,GdkEventKey *event,data_conf *data)
{
	char str[1000];
	char *mkey;

	ModifierToString (event->state, str);

	mkey=g_strdup_printf ("%s%s%s",str,str[0] ? "+" : "",
		      (XKeysymToString (XKeycodeToKeysym (GDK_DISPLAY (), event->hardware_keycode, 0)) != NULL)
		      ? XKeysymToString (XKeycodeToKeysym (GDK_DISPLAY (),event->hardware_keycode, 0))
		      : g_strdup_printf ("%d",event->hardware_keycode));

	gtk_entry_set_text (GTK_ENTRY (data->key_entry),mkey);
	g_free(mkey);
}
static void release_key (GtkWidget *widget,GdkEventKey *event,data_conf *data)
{
	guint i = 0;
	char **key_stat = g_strsplit(g_strdup(gtk_entry_get_text (GTK_ENTRY (data->key_entry))),"+",4);

	while (key_stat[i])
		i++;

	if((g_strcasecmp (key_stat[i-1],"Control")==0)|| (g_strcasecmp (key_stat[i-1],"Shift")==0)|| (g_strcasecmp (key_stat[i-1],"Alt")==0)|| (g_strcasecmp (key_stat[i-1],"Control_R")==0)|| (g_strcasecmp (key_stat[i-1],"Shift_R")==0)|| (g_strcasecmp (key_stat[i-1],"Alt_R")==0)|| (g_strcasecmp (key_stat[i-1],"Control_L")==0)|| (g_strcasecmp (key_stat[i-1],"Shift_L")==0)|| (g_strcasecmp (key_stat[i-1],"Alt_l")==0))
	gtk_entry_set_text (GTK_ENTRY (data->key_entry),"Press any key");
}


/***********************END Keyboard events*******************************************/

/************************** Start core code for get identificire process bik_bak_deamon **************************************/

static char* get_program_name(pid_t pid)
{
  char file_name[64];
  char status_info[256];
  int fd;
  int rval;
  char* open_parent;
  char* close_parent;
  char* result;
  
  snprintf(file_name, sizeof(file_name),"/proc/%d/stat",(int)pid);
  
  fd = open(file_name,O_RDONLY);
  if(fd == -1)
  	return NULL;
  rval = read(fd, status_info,sizeof(status_info)-1);
  close(fd);
  
    
  if(rval <= 0)
	return NULL;
  status_info[rval] = '\0';
  
  open_parent = strchr(status_info,'(');
  close_parent = strchr(status_info,')');
  
    
  if(open_parent == NULL || close_parent == NULL || close_parent < open_parent)
	return NULL;
  
  result = (char*)malloc(close_parent-open_parent);
  
   
  strncpy(result, open_parent+1, close_parent-open_parent-1);
  
  result[close_parent-open_parent-1] = '\0';
  
  return result;
}

static pid_t get_pid_deamon()
{
   DIR* proc_list;
   
   proc_list = opendir("/proc");
   
   if(proc_list == NULL)
	return 0;
   
      
   while(1)
   {
	struct dirent* data;
	const char* name;
	
	pid_t pid;
	
	data = readdir(proc_list);
	if(data == NULL)
	   break;
	
	name = data->d_name;
	
	if(strspn(name,"0123456789") != strlen(name))
		continue;
	pid = (pid_t)atoi(name);
	
	char* s_name = get_program_name(pid);
	
	if(strcmp(s_name,"bik_bak_deamon") == 0)
		return pid;
	
	free(s_name);
   }
   
   return 0;
}

/************************** End core code for list process **************************************/

static void clicked_ok (GtkWidget *wid,data_conf *data)
{
   ConfigFile *cfg = g_malloc0(sizeof (ConfigFile));
   cfg->password = gtk_entry_get_text(GTK_ENTRY (data->pass_entry));
   gchar *str_key = gtk_entry_get_text(GTK_ENTRY (data->key_entry));
   cfg->device = gtk_combo_box_get_active_text(GTK_COMBO_BOX(data->dev_combo));
	
   int key, mod;
   key = StringToModifier_Key(str_key, &mod);
   cfg->key_mod = g_strdup_printf("%d:%d", key, mod);
   
   pid_t i_pid = get_pid_deamon();
   if(i_pid>0)
	   kill(i_pid,SIGUSR2); //stop
   
   write_cfg_file(cfg,NULL);
   
   g_free(cfg);
      
   if(i_pid>0)
      kill(i_pid,SIGUSR1);//start
   gtk_main_quit ();
}

static void clicked_cancel (GtkWidget *wid,data_conf *data)
{
  gtk_main_quit ();
}

void load_data_combo(GtkWidget *dev)
{
  DIR *dir;
  struct dirent *entry;
  char entry_path[PATH_MAX +1] = "/dev/\0";
  size_t path_len = strlen(entry_path);
  struct stat st;

  dir = opendir(entry_path);

  while((entry = readdir(dir)) != NULL)
  {
     //format full path name to file
     strncpy(entry_path+path_len, entry->d_name,sizeof(entry_path)-path_len); 
     //check file type
     lstat(entry_path,&st);
     //skip directoryes
     if(S_ISDIR(st.st_mode))
	continue;
     if((strncasecmp(entry_path,"/dev/tty",8)== 0) || (strncasecmp(entry_path,"/dev/usb",8)== 0))
          gtk_combo_box_append_text(GTK_COMBO_BOX(dev), entry_path);
  }
  closedir(dir);
}

static void set_gui_data(data_conf *data)
{
   ConfigFile *c_data  = open_cfg_file(NULL);
   
   if((c_data->password) && (c_data->password[0]!='\n') && (c_data->password[0]!=' '))
   {
	gtk_entry_set_text (GTK_ENTRY (data->pass_entry),c_data->password);
	gtk_label_set_text(GTK_LABEL(data->status), "Enterd");
   }

   gtk_combo_box_append_text(GTK_COMBO_BOX(data->dev_combo), c_data->device);
   gtk_combo_box_set_active (GTK_COMBO_BOX(data->dev_combo), 0);
   load_data_combo(data->dev_combo);
   if(c_data->key_mod)
   {
        int key_code=0;
	gchar **key_stat = g_strsplit(c_data->key_mod,":",2);
	key_code=atoi(key_stat[0]);
	int stat=0;
	gchar *key=NULL;
	char str[1000];
	
	if(key_stat[1]==NULL)
	   stat=0;
	else
	  stat=atoi(key_stat[1]);
	
	ModifierToString (stat, str);
	key=g_strdup_printf ("%s%s%s",str,str[0] ? "+" : "",
	(XKeysymToString (XKeycodeToKeysym (GDK_DISPLAY (), key_code, 0)) != NULL)
	? XKeysymToString (XKeycodeToKeysym (GDK_DISPLAY (),key_code, 0))
	: g_strdup_printf ("%d",key_code));
	
	gtk_entry_set_text (GTK_ENTRY (data->key_entry),key);
	
	g_free(key);
   }
   else
        gtk_entry_set_text (GTK_ENTRY (data->key_entry),"Press any key");
   free(c_data);
}

GtkWidget* create_dialog_config (data_conf *data)
{
        GtkWidget *window1;
	GtkWidget *frame1;
	GtkWidget *alignment1;
	GtkWidget *hbox1;
	GtkWidget *image1;
	GtkWidget *vbox1;
	GtkWidget *frame2;
	GtkWidget *alignment2;
	GtkWidget *vbox2;
	//GtkWidget *reader_pass;
	GtkWidget *frame5;
	GtkWidget *alignment5;
	//GtkWidget *status_pass;
	GtkWidget *label5;
	GtkWidget *frame6;
	GtkWidget *alignment6;
	GtkWidget *label7;
	GtkWidget *label2;
	GtkWidget *frame3;
	GtkWidget *alignment3;
	GtkWidget *frame4;
	GtkWidget *alignment4;
	//GtkWidget *key_code;
	GtkWidget *label4;
	GtkWidget *label3;
	GtkWidget *label1;
	
	window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window1), "Bik-bak configuration tool");
	gtk_signal_connect (GTK_OBJECT (window1), "destroy",GTK_SIGNAL_FUNC (clicked_cancel), NULL);
	gtk_window_set_position (GTK_WINDOW (window1), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_window_set_resizable(GTK_WINDOW (window1), FALSE);
	gtk_window_set_icon_from_file(GTK_WINDOW (window1), "../img/configure.png",NULL);
	
	GtkWidget *vbox_g = gtk_vbox_new (FALSE,0);
	gtk_widget_show (vbox_g);
	gtk_container_add (GTK_CONTAINER (window1), vbox_g);
	
	frame1 = gtk_frame_new (NULL);
	gtk_widget_show (frame1);
	gtk_container_add (GTK_CONTAINER (vbox_g), frame1);
	gtk_container_set_border_width (GTK_CONTAINER (frame1), 5);
	
	alignment1 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment1);
	gtk_container_add (GTK_CONTAINER (frame1), alignment1);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment1), 0, 0, 12, 0);
	
	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox1);
	gtk_container_add (GTK_CONTAINER (alignment1), hbox1);
	
	image1 = create_pixmap (window1, "../img/reder.png");
	gtk_widget_show (image1);
	gtk_box_pack_start (GTK_BOX (hbox1), image1, TRUE, TRUE, 0);
	
	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_box_pack_start (GTK_BOX (hbox1), vbox1, TRUE, TRUE, 0);
	
	frame2 = gtk_frame_new (NULL);
	gtk_widget_show (frame2);
	gtk_box_pack_start (GTK_BOX (vbox1), frame2, TRUE, TRUE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (frame2), 1);
	
	alignment2 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment2);
	gtk_container_add (GTK_CONTAINER (frame2), alignment2);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment2), 0, 0, 12, 0);
	
	vbox2 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox2);
	gtk_container_add (GTK_CONTAINER (alignment2), vbox2);
	
	data->pass_entry = gtk_entry_new ();
	gtk_widget_show (data->pass_entry);
	gtk_box_pack_start (GTK_BOX (vbox2), data->pass_entry, FALSE, FALSE, 0);
	gtk_entry_set_invisible_char (GTK_ENTRY (data->pass_entry), 9679);
	
	frame5 = gtk_frame_new (NULL);
	gtk_widget_show (frame5);
	gtk_box_pack_start (GTK_BOX (vbox2), frame5, TRUE, TRUE, 0);
	
	alignment5 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment5);
	gtk_container_add (GTK_CONTAINER (frame5), alignment5);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment5), 0, 0, 12, 0);
	
	data->status = gtk_label_new ("not enteret yet");
	gtk_widget_show (data->status);
	gtk_container_add (GTK_CONTAINER (alignment5), data->status);
	
	label5 = gtk_label_new ("<b>Status</b>");
	gtk_widget_show (label5);
	gtk_frame_set_label_widget (GTK_FRAME (frame5), label5);
	gtk_label_set_use_markup (GTK_LABEL (label5), TRUE);
	
	frame6 = gtk_frame_new (NULL);
	gtk_widget_show (frame6);
	gtk_box_pack_start (GTK_BOX (vbox2), frame6, TRUE, TRUE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (frame6), 3);
	gtk_frame_set_shadow_type (GTK_FRAME (frame6), GTK_SHADOW_NONE);
	
	alignment6 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment6);
	gtk_container_add (GTK_CONTAINER (frame6), alignment6);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment6), 0, 0, 12, 0);
	
	data->dev_combo = gtk_combo_box_entry_new_text();//gtk_combo_box_entry_new ();
	gtk_widget_show (data->dev_combo);
	gtk_container_add (GTK_CONTAINER (alignment6), data->dev_combo);
		
	label7 = gtk_label_new ("<b>Reader device</b>");
	gtk_widget_show (label7);
	gtk_frame_set_label_widget (GTK_FRAME (frame6), label7);
	gtk_label_set_use_markup (GTK_LABEL (label7), TRUE);
	
	label2 = gtk_label_new ("<b>Reader password</b>");
	gtk_widget_show (label2);
	gtk_frame_set_label_widget (GTK_FRAME (frame2), label2);
	gtk_label_set_use_markup (GTK_LABEL (label2), TRUE);
	
	frame3 = gtk_frame_new (NULL);
	gtk_widget_show (frame3);
	gtk_box_pack_start (GTK_BOX (vbox1), frame3, TRUE, TRUE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (frame3), 2);
	gtk_frame_set_shadow_type (GTK_FRAME (frame3), GTK_SHADOW_ETCHED_OUT);
	
	alignment3 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment3);
	gtk_container_add (GTK_CONTAINER (frame3), alignment3);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment3), 0, 0, 12, 0);
	
	frame4 = gtk_frame_new (NULL);
	gtk_widget_show (frame4);
	gtk_container_add (GTK_CONTAINER (alignment3), frame4);
	gtk_frame_set_shadow_type (GTK_FRAME (frame4), GTK_SHADOW_NONE);
	
	alignment4 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment4);
	gtk_container_add (GTK_CONTAINER (frame4), alignment4);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment4), 0, 0, 12, 0);
	
	data->key_entry = gtk_entry_new ();
	gtk_widget_show (data->key_entry);
	gtk_container_add (GTK_CONTAINER (alignment4), data->key_entry);
	gtk_entry_set_invisible_char (GTK_ENTRY (data->key_entry), 9679);
	gtk_signal_connect (GTK_OBJECT (data->key_entry), "key_press_event",
	GTK_SIGNAL_FUNC (press_key),data);
		
	gtk_signal_connect (GTK_OBJECT (data->key_entry), "key_release_event",
			GTK_SIGNAL_FUNC (release_key),data);
	
	label4 = gtk_label_new ("<b>Key code</b> (Please press key)");
	gtk_widget_show (label4);
	gtk_frame_set_label_widget (GTK_FRAME (frame4), label4);
	gtk_label_set_use_markup (GTK_LABEL (label4), TRUE);
	
	label3 = gtk_label_new ("<b>Call password window</b>");
	gtk_widget_show (label3);
	gtk_frame_set_label_widget (GTK_FRAME (frame3), label3);
	gtk_label_set_use_markup (GTK_LABEL (label3), TRUE);
	
	label1 = gtk_label_new ("<b>Configure Bik-bak deamon</b>");
	gtk_widget_show (label1);
	gtk_frame_set_label_widget (GTK_FRAME (frame1), label1);
	gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);
	
	GtkWidget  *bbox, *ok, *cancel, *sep;
	
	
	sep = gtk_hseparator_new ();
	gtk_box_pack_start (GTK_BOX (vbox_g), sep, FALSE, FALSE, 0);
	
	/* Command Buttons */
	bbox = gtk_hbutton_box_new ();
	gtk_button_box_set_layout (GTK_BUTTON_BOX (bbox), GTK_BUTTONBOX_END);
	gtk_button_box_set_spacing (GTK_BUTTON_BOX (bbox), 5);
	gtk_box_pack_start (GTK_BOX (vbox_g), bbox, FALSE, FALSE, 0);
	
	ok =  gtk_button_new_with_label ("Save");
	gtk_signal_connect (GTK_OBJECT (ok), "clicked",GTK_SIGNAL_FUNC (clicked_ok), data);
	GTK_WIDGET_SET_FLAGS (ok, GTK_CAN_DEFAULT);
	gtk_box_pack_start (GTK_BOX (bbox), ok, TRUE, TRUE, 0);
	gtk_widget_grab_default (ok);
	
	cancel = gtk_button_new_with_label("Close");
	gtk_signal_connect (GTK_OBJECT (cancel), "clicked",GTK_SIGNAL_FUNC(clicked_cancel), data);
	GTK_WIDGET_SET_FLAGS (cancel, GTK_CAN_DEFAULT);
	gtk_box_pack_start (GTK_BOX (bbox), cancel, TRUE, TRUE, 0);
	gtk_widget_show_all (bbox);
	
	set_gui_data(data);
	
  return window1;
} 



