/*
 *  sh_daemon.h
 *
 *
 *  Created by Pace on 05/07/2013.
 *
 */

#ifndef SH_DAEMON_H
#define SH_DAEMON_H

typedef struct {
  GObject parent;
} Psf;

typedef struct {
  GObjectClass parent;
} PsfClass;

GType psf_get_type(void);

#define PSF_TYPE (psf_get_type())

#define PSF(object)            (G_TYPE_CHECK_INSTANCE_CAST((object),PSF_TYPE, Psf))
#define PSF_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST((klass), PSF_TYPE, PsfClass))
#define IS_PSF(object)         (G_TYPE_CHECK_INSTANCE_TYPE((object), PSF_TYPE))
#define IS_PSF_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE((klass), PSF_TYPE))
#define PSF_GET_CLASS(object)  (G_TYPE_INSTANCE_GET_CLASS((object), PSF_TYPE, PsfClass))

G_DEFINE_TYPE(Psf, psf, G_TYPE_OBJECT)

gboolean psf_exec (Psf *obj,
		   char** args,
		   gint* ret,
		   char** output,
		   char** error,
		   GError** gerror);

#ifndef __dbus_glib_marshal_psf_MARSHAL_H__
#define __dbus_glib_marshal_psf_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

#ifdef G_ENABLE_DEBUG
#define g_marshal_value_peek_boolean(v)  g_value_get_boolean (v)
#define g_marshal_value_peek_char(v)     g_value_get_schar (v)
#define g_marshal_value_peek_uchar(v)    g_value_get_uchar (v)
#define g_marshal_value_peek_int(v)      g_value_get_int (v)
#define g_marshal_value_peek_uint(v)     g_value_get_uint (v)
#define g_marshal_value_peek_long(v)     g_value_get_long (v)
#define g_marshal_value_peek_ulong(v)    g_value_get_ulong (v)
#define g_marshal_value_peek_int64(v)    g_value_get_int64 (v)
#define g_marshal_value_peek_uint64(v)   g_value_get_uint64 (v)
#define g_marshal_value_peek_enum(v)     g_value_get_enum (v)
#define g_marshal_value_peek_flags(v)    g_value_get_flags (v)
#define g_marshal_value_peek_float(v)    g_value_get_float (v)
#define g_marshal_value_peek_double(v)   g_value_get_double (v)
#define g_marshal_value_peek_string(v)   (char*) g_value_get_string (v)
#define g_marshal_value_peek_param(v)    g_value_get_param (v)
#define g_marshal_value_peek_boxed(v)    g_value_get_boxed (v)
#define g_marshal_value_peek_pointer(v)  g_value_get_pointer (v)
#define g_marshal_value_peek_object(v)   g_value_get_object (v)
#define g_marshal_value_peek_variant(v)  g_value_get_variant (v)
#else /* !G_ENABLE_DEBUG */
/* WARNING: This code accesses GValues directly, which is UNSUPPORTED API.
 *          Do not access GValues directly in your code. Instead, use the
 *          g_value_get_*() functions
 */
#define g_marshal_value_peek_boolean(v)  (v)->data[0].v_int
#define g_marshal_value_peek_char(v)     (v)->data[0].v_int
#define g_marshal_value_peek_uchar(v)    (v)->data[0].v_uint
#define g_marshal_value_peek_int(v)      (v)->data[0].v_int
#define g_marshal_value_peek_uint(v)     (v)->data[0].v_uint
#define g_marshal_value_peek_long(v)     (v)->data[0].v_long
#define g_marshal_value_peek_ulong(v)    (v)->data[0].v_ulong
#define g_marshal_value_peek_int64(v)    (v)->data[0].v_int64
#define g_marshal_value_peek_uint64(v)   (v)->data[0].v_uint64
#define g_marshal_value_peek_enum(v)     (v)->data[0].v_long
#define g_marshal_value_peek_flags(v)    (v)->data[0].v_ulong
#define g_marshal_value_peek_float(v)    (v)->data[0].v_float
#define g_marshal_value_peek_double(v)   (v)->data[0].v_double
#define g_marshal_value_peek_string(v)   (v)->data[0].v_pointer
#define g_marshal_value_peek_param(v)    (v)->data[0].v_pointer
#define g_marshal_value_peek_boxed(v)    (v)->data[0].v_pointer
#define g_marshal_value_peek_pointer(v)  (v)->data[0].v_pointer
#define g_marshal_value_peek_object(v)   (v)->data[0].v_pointer
#define g_marshal_value_peek_variant(v)  (v)->data[0].v_pointer
#endif /* !G_ENABLE_DEBUG */


extern void dbus_glib_marshal_psf_BOOLEAN__BOXED_POINTER_POINTER_POINTER_POINTER (GClosure     *closure,
                                                                                  GValue       *return_value,
                                                                                  guint         n_param_values,
                                                                                  const GValue *param_values,
                                                                                  gpointer      invocation_hint,
                                                                                  gpointer      marshal_data);
void
dbus_glib_marshal_psf_BOOLEAN__BOXED_POINTER_POINTER_POINTER_POINTER (GClosure     *closure,
                                                                      GValue       *return_value G_GNUC_UNUSED,
                                                                      guint         n_param_values,
                                                                      const GValue *param_values,
                                                                      gpointer      invocation_hint G_GNUC_UNUSED,
                                                                      gpointer      marshal_data)
{
  typedef gboolean (*GMarshalFunc_BOOLEAN__BOXED_POINTER_POINTER_POINTER_POINTER) (gpointer     data1,
                                                                                   gpointer     arg_1,
                                                                                   gpointer     arg_2,
                                                                                   gpointer     arg_3,
                                                                                   gpointer     arg_4,
                                                                                   gpointer     arg_5,
                                                                                   gpointer     data2);
  register GMarshalFunc_BOOLEAN__BOXED_POINTER_POINTER_POINTER_POINTER callback;
  register GCClosure *cc = (GCClosure*) closure;
  register gpointer data1, data2;
  gboolean v_return;

  g_return_if_fail (return_value != NULL);
  g_return_if_fail (n_param_values == 6);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = g_value_peek_pointer (param_values + 0);
    }
  else
    {
      data1 = g_value_peek_pointer (param_values + 0);
      data2 = closure->data;
    }
  callback = (GMarshalFunc_BOOLEAN__BOXED_POINTER_POINTER_POINTER_POINTER) (marshal_data ? marshal_data : cc->callback);

  v_return = callback (data1,
                       g_marshal_value_peek_boxed (param_values + 1),
                       g_marshal_value_peek_pointer (param_values + 2),
                       g_marshal_value_peek_pointer (param_values + 3),
                       g_marshal_value_peek_pointer (param_values + 4),
                       g_marshal_value_peek_pointer (param_values + 5),
                       data2);

  g_value_set_boolean (return_value, v_return);
}

G_END_DECLS

#endif /* __dbus_glib_marshal_psf_MARSHAL_H__ */

#include <dbus/dbus-glib.h>
static const DBusGMethodInfo dbus_glib_psf_methods[] = {
  { (GCallback) psf_exec, dbus_glib_marshal_psf_BOOLEAN__BOXED_POINTER_POINTER_POINTER_POINTER, 0 },
};
#endif
