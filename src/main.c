/* Bik-bak technologies
 * main.h
 *
 * Copyright (C) 2007 Peter kievomar
 * mailto: markus_sksoft@mail.ru
*/
#include <signal.h>

#include "interface.h"
#include "keyboard.h"

static void
start_as_daemon (void)
{
pid_t pid;
int i;

pid = fork ();
if (pid < 0)
	  {
	  perror ("Could not fork");
	  }
	  if (pid > 0)
	{
	exit (EXIT_SUCCESS);
	}
	
	setsid ();
	
	pid = fork ();
	if (pid < 0)
		  {
		  perror ("Could not fork");
		  }
		  if (pid > 0)
		{
		exit (EXIT_SUCCESS);
		}
		
		for (i = getdtablesize (); i >= 0; i--)
		{
		close (i);
		}
		
		i = open ("/dev/null", O_RDWR);
		dup (i);
		dup (i);
}

void stop_grab(int sig)
{
   //printf("Stop grabing old key\n");
   glist_grab_ungrab_key (1); 
}

void start_grab(int sig)
{
   //printf("Start grabing new key\n");
   glist_grab_ungrab_key (0);  
}

int main( int   argc,
          char *argv[] )
{
//start_as_daemon();

signal(SIGUSR2, stop_grab);
signal(SIGUSR1, start_grab);
      
//while( XOpenDisplay ( "localhost:0.0" ) == NULL ) printf("Wait for open display\n");
      
  gtk_init (&argc, &argv);
  
  init_kbd ();
  create_input_widget();
  
   show_about_dlg();
    
   gtk_main();
   return(0);
}
