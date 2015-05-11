/***************************************************************************
*            interface.h
*
*  Fri Sep 21 16:55:40 2007
*  Copyright  2007  Peter Komar
*  Email: markus_sksoft@mail.ru
 ****************************************************************************/
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "configfile.h"
//#include <locale.h>

ConfigFile *open_cfg_file(gchar * filename)
{
	ConfigFile *cfg;

	FILE *file;
	char *buffer, **lines, *tmp;
	int i;
	struct stat stats;
	
	//g_return_val_if_fail(filename != NULL, FALSE);
	if(filename == NULL)
		filename = c_file;

	if (lstat(filename, &stats) == -1)
		return NULL;
	if (!(file = fopen(filename, "r")))
		return NULL;
		
	buffer = g_malloc(stats.st_size + 1);
	if (fread(buffer, 1, stats.st_size, file) != stats.st_size)
	{
		g_free(buffer);
		fclose(file);
		return NULL;
	}
	fclose(file);
	buffer[stats.st_size] = '\0';

	cfg = g_malloc0(sizeof (ConfigFile));
	lines = g_strsplit(buffer, "\n", 0);
	g_free(buffer);
	i = 0;
	while (lines[i])
	{
		if (lines[i][0] == '[')
		{
			if ((tmp = strchr(lines[i], ']')))
			{
				*tmp = '\0';
				cfg->key_mod = g_strdup(&lines[i][1]);
			}
		}
		else if (lines[i][0] != '#')
		{
			if ((tmp = strchr(lines[i], '=')))
			{
				if(strncasecmp((lines[i]),"password1",9) == 0)
				{
					*tmp = '\0';
					tmp++;
					cfg->password = g_strchug(g_strchomp(g_strdup(tmp)));
				}
				else
					if(strncasecmp((lines[i]),"device",6) == 0)
					{
						*tmp = '\0';
						tmp++;
						cfg->device = g_strchug(g_strchomp(g_strdup(tmp)));
					}
				
			}
		}
		i++;
	}
	g_strfreev(lines);
	
		
	return cfg;
}

gboolean write_cfg_file(ConfigFile * cfg, gchar * filename)
{
	FILE *file;
	
	g_return_val_if_fail(cfg != NULL, FALSE);
	//g_return_val_if_fail(filename != NULL, FALSE);
	if(filename == NULL)
		filename = c_file;	

	if (!(file = fopen(filename, "w")))
		return FALSE;
	
	fprintf(file, "[%s]\n", cfg->key_mod);
	fprintf(file, "password1=%s\n", cfg->password);
	fprintf(file, "device=%s\n", cfg->device);
	fprintf(file, "\n");
	
	fclose(file);
	return TRUE;
}
