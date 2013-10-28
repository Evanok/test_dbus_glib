/*
 *  sh_daemon.c
 *
 *  This file contains the implementation of the dbus server
 *  for the Pace Security Framework.
 *
 *  Created by Pace on 05/07/2013.
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <dbus/dbus-glib.h>

#include "../include/sh_daemon.h"

DBusGObjectInfo dbus_glib_psf_object_info;
#define EXEC_PATH_SIZE 64
const char* exec_path = "\0exec\0S\0process_name\0I\0s\0args\0I\0as\0ret\0O\0F\0N\0i\0output\0O\0F\0N\0s\0\0\0";

void init_dbus_gobject_info (DBusGObjectInfo* dbus_glib_psf_object_info, const char* dbus_name)
{
  char* psf_dbus_name = malloc (strlen(dbus_name) + EXEC_PATH_SIZE + 1); // NEED TO FREE ? WHEN ?
  int i;
  char* tmp = NULL;

  strcpy (psf_dbus_name, dbus_name);
  tmp = psf_dbus_name + strlen(psf_dbus_name);
  for (i = 0; i < EXEC_PATH_SIZE; i++)
    tmp[i] = exec_path[i];
  tmp[i] = 0;

  dbus_glib_psf_object_info->format_version = 1;
  dbus_glib_psf_object_info->method_infos = dbus_glib_psf_methods;
  dbus_glib_psf_object_info->n_method_infos = 1;
  dbus_glib_psf_object_info->data = psf_dbus_name;
  dbus_glib_psf_object_info->exported_signals = "\0";
  dbus_glib_psf_object_info->exported_properties = "\0";
}

static void psf_init(Psf* obj)
{
  dbus_g_object_type_install_info (PSF_TYPE,
				   &dbus_glib_psf_object_info);
}

/*
 *
 *
 */
static void psf_class_init(PsfClass *klass)
{

}


gboolean psf_exec (Psf *obj,
		   const char* process_name,
		   char** args,
		   gint* ret,
		   char** output,
		   GError** error)
{
  int i;

  printf ("exec process : %s\n", process_name);
  for (i = 0; args[i] != NULL; i++)
    printf ("args of process : %s\n", args[i]);

  return TRUE;
}
 
/*
 *
 *
 */
int main(int argc , char **argv)
{
  GObject* psf;
  DBusGConnection* bus = NULL;
  DBusGProxy* busProxy = NULL;

  gboolean res;
  GMainLoop* mainloop = NULL;
  guint result;
  GError* error = NULL;
  int opt;
  const char *process = NULL;
  const char *dbus_path = NULL;
  const char *dbus_name = NULL;

  while ((opt = getopt(argc, argv, "e:n:p:")) != -1) {
    switch (opt) {
      case 'e':
	process=optarg;
	break;
      case 'n':
	dbus_name=optarg;
	break;
      case 'p':
	dbus_path=optarg;
	break;
      default: /* '?' */
	fprintf(stderr, "Usage: %s -e process -n dbus_name -p dbus_path\n",
		argv[0]);
	exit(EXIT_FAILURE);
    }
  }

  if (!process || !dbus_path || !dbus_name) {
    fprintf(stderr, "Usage: %s -e process -n dbus_name -p dbus_path\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  printf ("%s %s %s\n", process, dbus_path, dbus_name);

  init_dbus_gobject_info (&dbus_glib_psf_object_info, dbus_name);

  printf ("dbus path : %s\n", dbus_path);
  printf ("dbus name : %s\n", dbus_name);

  printf ("dbus gobject info data : %s\n", dbus_glib_psf_object_info.data);
  printf ("dbus gobject info export signals : %s\n", dbus_glib_psf_object_info.exported_signals);
  printf ("dbus gobject info export properties : %s\n", dbus_glib_psf_object_info.exported_properties);

  /* Initialize the GType/GObject system. */
  g_type_init();

  mainloop = g_main_loop_new(NULL, FALSE);
  if (mainloop == NULL) 
  {
    printf ("Couldn't create GMainLoop\n");
    return 1;
  }

  bus = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error);
  if (error != NULL) 
  {
    printf ("Couldn't connect to system bus\n");
    return 1;
  }

  busProxy = dbus_g_proxy_new_for_name(bus, DBUS_SERVICE_DBUS, dbus_path, DBUS_INTERFACE_DBUS);
  if (busProxy == NULL) 
  {
    printf ("Failed to get a proxy for D-Bus\n");
    return 1;
  }

  printf ("TUTU1\n");

  res = dbus_g_proxy_call(busProxy,
			  "RequestName",
			  &error,
			  G_TYPE_STRING,
			  dbus_name,
			  G_TYPE_UINT,
			  0,
			  G_TYPE_INVALID,
			  G_TYPE_UINT,
			  &result,
			  G_TYPE_INVALID);
  if (!res)
  {
    if (error != NULL)
    {
      g_warning ("Failed to acquire %s: %s",
		 dbus_name, error->message);
      g_error_free (error);
    }
    else
    {
      g_warning ("Failed to acquire %s", dbus_name);
    }
    return 1;
  }
  
  if (result != 1) 
  {
    printf ("Failed to get the primary well-known name.\n");
    return 1;
  }

  psf = g_object_new(PSF_TYPE, NULL);
  if (psf == NULL) 
  {
    printf ("Failed to create one Value instance.\n");
    return 1;
  }

  printf ("dbus path : %s\n", dbus_path);

  dbus_g_connection_register_g_object(bus,
                                      dbus_path,
                                      G_OBJECT(psf));
    
  printf ("TUTU3\n");

  g_main_loop_run(mainloop);

  return 0;
}

