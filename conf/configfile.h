/***************************************************************************
*            interface.h
*
*  Fri Sep 21 16:55:40 2007
*  Copyright  2007  Peter Komar
*  Email: markus_sksoft@mail.ru
 ****************************************************************************/

 #include <glib.h>

typedef struct
{
	gchar *password;
	gchar *key_mod;
	gchar *device;
}
ConfigFile;

#ifdef __cplusplus
extern "C" {
#endif

static gchar *c_file="bik.cf";

ConfigFile *open_cfg_file(gchar * filename);
gboolean write_cfg_file(ConfigFile * cfg, gchar * filename);

#ifdef __cplusplus
};
#endif
