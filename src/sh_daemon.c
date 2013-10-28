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
#define EXEC_PATH_SIZE 128
const char* exec_path = "\0exec\0S\0args\0I\0as\0ret\0O\0F\0N\0i\0output\0O\0F\0N\0s\0error\0O\0F\0N\0s\0\0\0";
const char *process = NULL;

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
		   char** args,
		   gint* ret,
		   char** output,
		   char** error,
		   GError** gerror)
{
  int i, j;
  pid_t pid;
  int status = -42;
  int pipefd_out[2];
  int pipefd_err[2];

  if (strcmp (process, args[0]) != 0)
  {
    fprintf(stderr, "Process %s : Permission denied\n", args[0]);
    return TRUE;
  }

  pipe(pipefd_out);
  pipe(pipefd_err);
  pid = fork ();

  if (pid == 0)
  {
    close(pipefd_out[0]);
    close(pipefd_err[0]);

    dup2(pipefd_out[1], 1);
    dup2(pipefd_err[1], 2);

    close(pipefd_out[1]);
    close(pipefd_err[1]);

    execvp (process, args);
    perror ("execv");
    return TRUE;
  }
  else if (pid == -1)
  {
    perror ("fork");
    return TRUE;
  }
  else
  {
    if (waitpid(pid, &status, 0) == -1)
    {
      perror ("waitpid");
    return TRUE;
    }
    else
    {
      char buf_stderr[1024];
      char buf_stdout[1024];
      close(pipefd_out[1]);
      close(pipefd_err[1]);
      while (read(pipefd_out[0], buf_stdout, sizeof(buf_stdout)) != 0);
      while (read(pipefd_err[0], buf_stderr, sizeof(buf_stderr)) != 0);
      printf ("_______\n");
      printf ("output : \n"); printf ("%s", buf_stdout);
      printf ("_______\n");
      printf ("error : \n"); printf ("%s", buf_stderr);
      printf ("_______\n");
      printf ("code retour : %d\n", status);
      printf ("_______\n");
      *ret = status;
    }
  }

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
    g_warning ("Couldn't create GMainLoop\n");
    return 1;
  }

  bus = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error);
  if (error != NULL)
  {
    g_warning ("Couldn't connect to system bus\n");
    return 1;
  }

  busProxy = dbus_g_proxy_new_for_name(bus, DBUS_SERVICE_DBUS, dbus_path, DBUS_INTERFACE_DBUS);
  if (busProxy == NULL)
  {
    g_warning ("Failed to get a proxy for D-Bus\n");
    return 1;
  }

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
    g_warning ("Failed to get the primary well-known name.\n");
    return 1;
  }

  psf = g_object_new(PSF_TYPE, NULL);
  if (psf == NULL)
  {
    g_warning ("Failed to create one Value instance.\n");
    return 1;
  }

  dbus_g_connection_register_g_object(bus,
                                      dbus_path,
                                      G_OBJECT(psf));

  g_main_loop_run(mainloop);

  return 0;
}

