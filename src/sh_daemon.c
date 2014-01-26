#include "../include/sh_daemon.h"

#define ERROR_FORK 1
#define ERROR_WAITPID 2
#define ERROR_EXECVP 3
#define ERROR_PERMISSION_DENIED 4
#define ERROR_ARGS_NULL 5

DBusGObjectInfo dbus_glib_psf_object_info;
#define EXEC_PATH_SIZE 128
const char* exec_path = "\0exec\0S\0args\0I\0as\0ret\0O\0F\0N\0i\0output\0O\0F\0N\0s\0error\0O\0F\0N\0s\0\0\0";
const char *process = NULL;

void init_dbus_gobject_info (DBusGObjectInfo* dbus_glib_psf_object_info, const char* dbus_name)
{
  char* psf_dbus_name = malloc (strlen(dbus_name) + EXEC_PATH_SIZE + 1);
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
  pid_t pid;
  int status = -42;
  int pipefd_out[2];
  int pipefd_err[2];


  if (args[0] == NULL)
  {
    g_set_error (gerror, -1, ERROR_ARGS_NULL, "Binary to fork must be set as input\n");
    return FALSE;
  }

  if (strcmp (process, args[0]) != 0)
  {
    g_set_error (gerror, -1, ERROR_PERMISSION_DENIED, "Process %s : Permission denied\n", args[0]);
    *ret = -1;
    return FALSE;
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

    errno = 0;
    execvp (args[0], NULL);
    if (errno)
    {
      g_set_error (gerror, -1, ERROR_EXECVP, "Error during execvp : %d\n", errno);
      return FALSE;
    }
    return TRUE;
  }
  else if (pid == -1)
  {
    g_set_error (gerror, -1, ERROR_FORK, "Error during fork process\n");
    return FALSE;
  }

  char buf_stderr[1024];
  char buf_stdout[1024];
  close(pipefd_out[1]);
  close(pipefd_err[1]);
  while (read(pipefd_out[0], buf_stdout, sizeof(buf_stdout)) != 0);
  while (read(pipefd_err[0], buf_stderr, sizeof(buf_stderr)) != 0);

  if (waitpid(pid, &status, 0) == -1)
  {
    g_set_error (gerror, -1, ERROR_WAITPID, "Error during waitpid\n");
    return FALSE;
  }

  *ret = status;
  *output = strdup(buf_stdout);
  *error = strdup(buf_stderr);

  return TRUE;
}

int main(int argc , char **argv)
{
  GObject* psf = NULL;
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

  init_dbus_gobject_info (&dbus_glib_psf_object_info, dbus_name);

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

