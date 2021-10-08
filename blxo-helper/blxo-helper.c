/*-
 * Copyright (c) 2003-2006 Benedikt Meurer <benny@xfce.org>.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_GIO_UNIX
#include <gio/gdesktopappinfo.h>
#endif

#include <blxo-helper/blxo-helper.h>
#include <blxo-helper/blxo-helper-utils.h>



static void       blxo_helper_finalize   (GObject        *object);
static BlxoHelper *blxo_helper_new        (const gchar    *id,
                                         XfceRc         *rc);
static void       clear_bad_entries     (XfceRc         *rc);



struct _BlxoHelperClass
{
  GObjectClass __parent__;
};

struct _BlxoHelper
{
  GObject __parent__;

  guint             startup_notify : 1;

  gchar            *id;
  gchar            *icon;
  gchar            *name;
  gchar           **commands;
  gchar           **commands_with_parameter;
  gchar           **commands_with_flag;
  BlxoHelperCategory category;
};



G_DEFINE_TYPE (BlxoHelper, blxo_helper, G_TYPE_OBJECT)



static void
blxo_helper_class_init (BlxoHelperClass *klass)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = blxo_helper_finalize;
}



static void
blxo_helper_init (BlxoHelper *helpers)
{
}



static void
blxo_helper_finalize (GObject *object)
{
  BlxoHelper *helper = BLXO_HELPER (object);

  g_strfreev (helper->commands_with_flag);
  g_strfreev (helper->commands_with_parameter);
  g_strfreev (helper->commands);
  g_free (helper->name);
  g_free (helper->icon);
  g_free (helper->id);

  (*G_OBJECT_CLASS (blxo_helper_parent_class)->finalize) (object);
}



static gchar**
substitute_binary (const gchar *commands,
                   const gchar *binary)
{
  gchar **result;
  gchar **s, **t;
  gchar  *tmp;

  /* split the commands */
  result = g_strsplit (commands, ";", -1);

  /* (pre-)process the result */
  for (s = t = result; *s != NULL; ++s)
    {
      if (**s == '\0')
        {
          g_free (*s);
        }
      else if (binary != NULL)
        {
          tmp = blxo_str_replace (*s, "%B", binary);
          g_free (*s);
          *t++ = tmp;
        }
      else
        {
          *t++ = *s;
        }
    }
  *t = NULL;

  return result;
}



static BlxoHelper*
blxo_helper_new (const gchar *id,
                XfceRc      *rc)
{
  const gchar *commands_with_flag;
  const gchar *commands_with_parameter;
  const gchar *commands;
  const gchar *str;
  BlxoHelper   *helper;
  gchar      **binaries;
  gchar       *binary = NULL;
  guint        n;

  g_return_val_if_fail (id != NULL, NULL);
  g_return_val_if_fail (rc != NULL, NULL);

  xfce_rc_set_group (rc, "Desktop Entry");

  /* allocate a new helper */
  helper = g_object_new (BLXO_TYPE_HELPER, NULL);
  helper->id = g_strdup (id);
  helper->startup_notify = xfce_rc_read_bool_entry (rc, "StartupNotify", FALSE);

  /* verify the type of the desktop file */
  str = xfce_rc_read_entry_untranslated (rc, "Type", NULL);
  if (G_UNLIKELY (!blxo_str_is_equal (str, "X-XFCE-Helper")))
    goto failed;

  /* determine the category of the helper */
  str = xfce_rc_read_entry_untranslated (rc, "X-XFCE-Category", NULL);
  if (!blxo_helper_category_from_string (str, &helper->category))
    goto failed;

  /* determine the name of the helper */
  str = xfce_rc_read_entry (rc, "Name", NULL);
  if (G_UNLIKELY (blxo_str_is_empty (str)))
    goto failed;
  helper->name = g_strdup (str);

  /* determine the icon of the helper */
  str = xfce_rc_read_entry_untranslated (rc, "Icon", NULL);
  if (G_LIKELY (!blxo_str_is_empty (str)))
    helper->icon = g_strdup (str);

  /* determine the commands */
  commands = xfce_rc_read_entry_untranslated (rc, "X-XFCE-Commands", NULL);
  if (G_UNLIKELY (commands == NULL))
    goto failed;

  commands_with_flag = blxo_str_replace (commands, ";", " %s;");

  /* determine the commands (with parameter) */
  commands_with_parameter = xfce_rc_read_entry_untranslated (rc, "X-XFCE-CommandsWithParameter", NULL);
  if (G_UNLIKELY (commands_with_parameter == NULL))
    goto failed;

  /* check if we need binaries for substitution */
  if (strstr (commands, "%B") != NULL || strstr (commands_with_parameter, "%B") != NULL)
    {
      /* determine the binaries */
      str = xfce_rc_read_entry_untranslated (rc, "X-XFCE-Binaries", NULL);
      if (G_UNLIKELY (str == NULL))
        goto failed;

      /* determine the first available binary */
      binaries = g_strsplit (str, ";", -1);
      for (binary = NULL, n = 0; binaries[n] != NULL && binary == NULL; ++n)
        if (G_LIKELY (binaries[n][0] != '\0'))
          binary = g_find_program_in_path (binaries[n]);
      g_strfreev (binaries);

      /* check if we found a binary */
      if (G_UNLIKELY (binary == NULL))
        goto failed;
    }

  /* substitute the binary (if any) */
  helper->commands = substitute_binary (commands, binary);
  helper->commands_with_flag = substitute_binary (commands_with_flag, binary);
  helper->commands_with_parameter = substitute_binary (commands_with_parameter, binary);
  g_free (binary);

  /* verify that we have atleast one command */
  if (G_UNLIKELY (*helper->commands == NULL || *helper->commands_with_parameter == NULL))
    goto failed;

  return helper;

failed:
  g_object_unref (G_OBJECT (helper));
  return NULL;
}



/**
 * blxo_helper_get_category:
 * @helper : an #BlxoHelper.
 *
 * Returns the #BlxoHelperCategory of @helper.
 *
 * Return value: the #BlxoHelperCategory of @helper.
 **/
BlxoHelperCategory
blxo_helper_get_category (const BlxoHelper *helper)
{
  g_return_val_if_fail (BLXO_IS_HELPER (helper), BLXO_HELPER_WEBBROWSER);
  return helper->category;
}



/**
 * blxo_helper_get_id:
 * @helper : an #BlxoHelper.
 *
 * Returns the unique id (the .desktop file basename) of @helper.
 *
 * Return value: the unique id of @helper.
 **/
const gchar*
blxo_helper_get_id (const BlxoHelper *helper)
{
  g_return_val_if_fail (BLXO_IS_HELPER (helper), NULL);
  return helper->id;
}



/**
 * blxo_helper_get_name:
 * @helper : an #BlxoHelper.
 *
 * Returns the (translated) name of the @helper.
 *
 * Return value: the name of @helper.
 **/
const gchar*
blxo_helper_get_name (const BlxoHelper *helper)
{
  g_return_val_if_fail (BLXO_IS_HELPER (helper), NULL);
  return helper->name;
}



/**
 * blxo_helper_get_icon:
 * @helper : an #BlxoHelper.
 *
 * Return the name of the themed icon for @helper or
 * the absolute path to an icon file, or %NULL if no
 * icon is available for @helper.
 *
 * Return value: the icon for @helper or %NULL.
 **/
const gchar*
blxo_helper_get_icon (const BlxoHelper *helper)
{
  g_return_val_if_fail (BLXO_IS_HELPER (helper), NULL);
  return helper->icon;
}



/**
 * blxo_helper_get_command:
 * @helper : an #BlxoHelper.
 *
 * Returns a reasonable command for @helper.
 *
 * Return value: a command for @helper.
 **/
const gchar*
blxo_helper_get_command (const BlxoHelper *helper)
{
  g_return_val_if_fail (BLXO_IS_HELPER (helper), NULL);
  return *helper->commands_with_parameter;
}

/* Set the DISPLAY variable, to be use by g_spawn_async. */
static void
set_environment (gchar *display)
{
  g_setenv ("DISPLAY", display, TRUE);
}

/**
 * blxo_helper_execute:
 * @helper    : an #BlxoHelper.
 * @screen    : the #GdkScreen on which to execute @helper or %NULL to use default.
 * @parameter : the parameter to pass to @helper (e.g. URL for WebBrowser) or %NULL
 *              to just run @helper.
 * @error     : return location for errors or %NULL.
 *
 * Executes @helper on @screen with the given @parameter. Returns %TRUE if the
 * execution succeed, else %FALSE and @error will be set.
 *
 * Return value: %TRUE on success, %FALSE if @error is set.
 **/
gboolean
blxo_helper_execute (BlxoHelper   *helper,
                    GdkScreen   *screen,
                    const gchar *parameter,
                    GError     **error)
{
  GTimeVal      previous;
  GTimeVal      current;
  GdkDisplay   *display;
  gboolean      succeed = FALSE;
  GError       *err = NULL;
  gchar       **commands;
  gchar       **argv;
  gchar        *command;
  gchar        *display_name;
  guint         n;
  gint          status;
  gint          result;
  gint          pid;
  const gchar  *real_parameter = parameter;

  // FIXME: startup-notification

  g_return_val_if_fail (BLXO_IS_HELPER (helper), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);
  g_return_val_if_fail (screen == NULL || GDK_IS_SCREEN (screen), FALSE);

  /* fallback to default screen */
  if (G_UNLIKELY (screen == NULL))
    screen = gdk_screen_get_default ();

  /* strip the mailto part if needed */
  if (real_parameter != NULL && g_str_has_prefix (real_parameter, "mailto:"))
    real_parameter = parameter + 7;

  /* determine the command set to use */
  if (blxo_str_is_flag (real_parameter)) {
    commands = helper->commands_with_flag;
  } else if (blxo_str_is_empty (real_parameter)) {
    commands = helper->commands;
  } else {
    commands = helper->commands_with_parameter;
  }

  /* verify that we have atleast one command */
  if (G_UNLIKELY (*commands == NULL))
    {
      g_set_error (error, G_SPAWN_ERROR, G_SPAWN_ERROR_INVAL, _("No command specified"));
      return FALSE;
    }

  /* try to run the helper using the various given commands */
  for (n = 0; commands[n] != NULL; ++n)
    {
      /* reset the error */
      g_clear_error (&err);

      /* parse the command */
      command = !blxo_str_is_empty (real_parameter) ? blxo_str_replace (commands[n], "%s", real_parameter) : g_strdup (commands[n]);
      succeed = g_shell_parse_argv (command, NULL, &argv, &err);
      g_free (command);

      /* check if the parsing failed */
      if (G_UNLIKELY (!succeed))
        continue;

      /* set the display variable */
      display = gdk_screen_get_display (screen);
      display_name = g_strdup (gdk_display_get_name (display));

      /* try to run the command */
      succeed = g_spawn_async (NULL,
        argv,
        NULL,
        G_SPAWN_DO_NOT_REAP_CHILD | G_SPAWN_SEARCH_PATH,
        (GSpawnChildSetupFunc) set_environment,
        display_name,
        &pid,
        &err);

      /* cleanup */
      g_strfreev (argv);
      g_free (display_name);

      /* check if the execution was successful */
      if (G_LIKELY (succeed))
        {
          /* determine the current time */
          g_get_current_time (&previous);

          /* wait up to 5 seconds to see whether the command worked */
          for (;;)
            {
              /* check if the command exited with an error */
              result = waitpid (pid, &status, WNOHANG);
              if (result < 0)
                {
                  /* something weird happened */
                  err = g_error_new_literal (G_FILE_ERROR, g_file_error_from_errno (errno), g_strerror (errno));
                  succeed = FALSE;
                  break;
                }
              else if (result > 0 && status != 0)
                {
                  /* the command failed */
                  err = g_error_new_literal (G_FILE_ERROR, g_file_error_from_errno (EIO), g_strerror (EIO));
                  succeed = FALSE;
                  break;
                }
              else if (result == pid)
                {
                  /* the command succeed */
                  succeed = TRUE;
                  break;
                }

              /* determine the current time */
              g_get_current_time (&current);

              /* check if the command is still running after 5 seconds (which indicates that the command worked) */
              if (((current.tv_sec - previous.tv_sec) * 1000ll + (current.tv_usec - previous.tv_usec) / 1000ll) > 5000ll)
                break;

              /* wait some time */
              g_usleep (50 * 1000);
            }

          /* check if we should retry with the next command */
          if (G_LIKELY (succeed))
            break;
        }
    }

  /* propagate the error */
  if (G_UNLIKELY (!succeed))
    g_propagate_error (error, err);

  return succeed;
}




static void       blxo_helper_database_finalize    (GObject                *object);
static BlxoHelper *blxo_helper_database_lookup      (BlxoHelperDatabase      *database,
                                                   BlxoHelperCategory       category,
                                                   const gchar            *id);



struct _BlxoHelperDatabaseClass
{
  GObjectClass __parent__;
};

struct _BlxoHelperDatabase
{
  GObject     __parent__;
  GHashTable *helpers;
};



G_DEFINE_TYPE (BlxoHelperDatabase, blxo_helper_database, G_TYPE_OBJECT)



static void
blxo_helper_database_class_init (BlxoHelperDatabaseClass *klass)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = blxo_helper_database_finalize;
}



static void
blxo_helper_database_init (BlxoHelperDatabase *database)
{
  database->helpers = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_object_unref);
}



static void
blxo_helper_database_finalize (GObject *object)
{
  BlxoHelperDatabase *database = BLXO_HELPER_DATABASE (object);

  g_hash_table_destroy (database->helpers);

  (*G_OBJECT_CLASS (blxo_helper_database_parent_class)->finalize) (object);
}



static BlxoHelper*
blxo_helper_database_lookup (BlxoHelperDatabase *database,
                            BlxoHelperCategory  category,
                            const gchar       *id)
{
  BlxoHelper *helper;
  XfceRc    *rc;
  gchar     *file;
  gchar     *spec;

  g_return_val_if_fail (BLXO_IS_HELPER_DATABASE (database), NULL);
  g_return_val_if_fail (id != NULL, NULL);

  /* generate the spec for the helper */
  spec = g_strconcat ("xfce4/helpers/", id, ".desktop", NULL);

  /* try to find a cached version */
  helper = g_hash_table_lookup (database->helpers, spec);

  /* load the helper from the file */
  if (G_LIKELY (helper == NULL))
    {
      xfce_resource_push_path (XFCE_RESOURCE_DATA, DATADIR);
      file = xfce_resource_lookup (XFCE_RESOURCE_DATA, spec);
      xfce_resource_pop_path (XFCE_RESOURCE_DATA);

      if (G_LIKELY (file != NULL))
        {
          rc = xfce_rc_simple_open (file, TRUE);
          if (G_LIKELY (rc != NULL))
            {
              helper = blxo_helper_new (id, rc);
              xfce_rc_close (rc);
            }
          g_free (file);
        }

      /* add the loaded helper to the cache */
      if (G_LIKELY (helper != NULL))
        {
          g_hash_table_insert (database->helpers, spec, helper);
          spec = NULL;
        }
    }

  if (G_LIKELY (helper != NULL))
    {
      if (blxo_helper_get_category (helper) == category)
        g_object_ref (G_OBJECT (helper));
      else
        helper = NULL;
    }

  g_free (spec);

  return helper;
}



/**
 * blxo_helper_database_get:
 *
 * Returns a reference on the default #BlxoHelperDatabase
 * instance. The caller is responsible to free the
 * returned object using g_object_unref() when no longer
 * needed.
 *
 * Return value: a reference to the default #BlxoHelperDatabase.
 **/
BlxoHelperDatabase*
blxo_helper_database_get (void)
{
  static BlxoHelperDatabase *database = NULL;

  if (G_LIKELY (database == NULL))
    {
      database = g_object_new (BLXO_TYPE_HELPER_DATABASE, NULL);
      g_object_add_weak_pointer (G_OBJECT (database), (gpointer) &database);
    }
  else
    {
      g_object_ref (G_OBJECT (database));
    }

  return database;
}



/**
 * blxo_helper_database_get_default:
 * @database : an #BlxoHelperDatabase.
 * @category : an #BlxoHelperCategory.
 *
 * Returns a reference on the default #BlxoHelper for
 * the @category in @database or %NULL if no default
 * #BlxoHelper is registered for @category.
 *
 * The caller is responsible to free the returned
 * object using g_object_unref() when no longer needed.
 *
 * Return value: the default #BlxoHelper for @category
 *               or %NULL.
 **/
BlxoHelper*
blxo_helper_database_get_default (BlxoHelperDatabase *database,
                                 BlxoHelperCategory  category)
{
  const gchar *id;
  BlxoHelper   *helper = NULL;
  XfceRc      *rc;
  gchar       *key;

  g_return_val_if_fail (BLXO_IS_HELPER_DATABASE (database), NULL);
  g_return_val_if_fail (category < BLXO_HELPER_N_CATEGORIES, NULL);

  rc = xfce_rc_config_open (XFCE_RESOURCE_CONFIG, "xfce4/helpers.rc", TRUE);
  if (G_LIKELY (rc != NULL))
    {
      key = blxo_helper_category_to_string (category);
      id = xfce_rc_read_entry_untranslated (rc, key, NULL);
      if (G_LIKELY (id != NULL))
        helper = blxo_helper_database_lookup (database, category, id);
      xfce_rc_close (rc);
      g_free (key);
    }

  return helper;
}



static XfceRc*
mimeapps_open (gboolean readonly)
{
  XfceRc *rc;

  rc = xfce_rc_config_open (XFCE_RESOURCE_CONFIG, "mimeapps.list", readonly);
  if (G_UNLIKELY (rc == NULL))
    {
      /* deprecated location (glib < 2.41) */
      rc = xfce_rc_config_open (XFCE_RESOURCE_DATA, "applications/mimeapps.list", readonly);
    }

  return rc;
}



/**
 * blxo_helper_database_set_default:
 * @database : an #BlxoHelperDatabase.
 * @category : an #BlxoHelperCategory.
 * @helper   : an #BlxoHelper.
 * @error    : return location for errors or %NULL.
 *
 * Sets the default #BlxoHelper for @category in @database to
 * @helper. Returns %TRUE on success, %FALSE if @error is set.
 *
 * Return value: %TRUE on success, %FALSE if @error is set.
 **/
gboolean
blxo_helper_database_set_default (BlxoHelperDatabase *database,
                                 BlxoHelperCategory  category,
                                 BlxoHelper         *helper,
                                 GError           **error)
{
  XfceRc       *rc, *desktop_file;
  gchar        *key;
  const gchar  *filename;
  gchar       **mimetypes;
  guint         i;
  gchar        *path;
  gchar        *entry;

  g_return_val_if_fail (category < BLXO_HELPER_N_CATEGORIES, FALSE);
  g_return_val_if_fail (BLXO_IS_HELPER_DATABASE (database), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);
  g_return_val_if_fail (BLXO_IS_HELPER (helper), FALSE);

  /* open the helpers.rc for writing */
  rc = xfce_rc_config_open (XFCE_RESOURCE_CONFIG, "xfce4/helpers.rc", FALSE);
  if (G_UNLIKELY (rc == NULL))
    {
      g_set_error (error, G_FILE_ERROR, G_FILE_ERROR_IO, _("Failed to open %s for writing"), "helpers.rc");
      return FALSE;
    }

  /* save the new setting */
  key = blxo_helper_category_to_string (category);
  xfce_rc_write_entry (rc, key, blxo_helper_get_id (helper));
  g_free (key);

  /* clear the dismissed preference */
  key = g_strconcat (blxo_helper_category_to_string (category), "Dismissed", NULL);
  xfce_rc_delete_entry (rc, key, FALSE);
  xfce_rc_close (rc);
  g_free (key);

  /* get the desktop filename */
  switch (category)
    {
      case BLXO_HELPER_WEBBROWSER:
        filename = "blxo-web-browser.desktop";
        break;

      case BLXO_HELPER_MAILREADER:
        filename = "blxo-mail-reader.desktop";
        break;

      case BLXO_HELPER_FILEMANAGER:
        filename = "blxo-file-manager.desktop";
        break;

      default:
        /* no mimetype support for terminals */
        return TRUE;
    }

  /* open the mimeapp.list file to set the default handler of the mime type */
  rc = mimeapps_open (FALSE);
  if (G_UNLIKELY (rc == NULL))
    {
      g_set_error (error, G_FILE_ERROR, G_FILE_ERROR_IO, _("Failed to open %s for writing"), "mimeapps.list");
      return FALSE;
    }

  /* open the blxo desktop file to read the mimetypes the file supports */
  path = g_build_filename ("applications", filename, NULL);
  desktop_file = xfce_rc_config_open (XFCE_RESOURCE_DATA, path, TRUE);
  g_free (path);

  if (G_UNLIKELY (desktop_file != NULL))
    {
      xfce_rc_set_group (desktop_file, "Desktop Entry");
      mimetypes = xfce_rc_read_list_entry (desktop_file, "X-XFCE-MimeType", ";");
      if (mimetypes != NULL)
        {
#ifdef HAVE_GIO_UNIX
          GDesktopAppInfo *info = g_desktop_app_info_new (filename);
#endif

          xfce_rc_set_group (rc, "Added Associations");

          for (i = 0; mimetypes[i] != NULL; i++)
            if (!blxo_str_is_empty (mimetypes[i]))
              {
                entry = g_strconcat (filename, ";", NULL);
                xfce_rc_write_entry (rc, mimetypes[i], entry);
                g_free (entry);

#ifdef HAVE_GIO_UNIX
                if (info != NULL)
                  {
                    g_app_info_set_as_default_for_type (G_APP_INFO (info),
                                                        mimetypes[i],
                                                        NULL);
                  }
#endif
              }
          g_strfreev (mimetypes);
#ifdef HAVE_GIO_UNIX
          if (info != NULL)
            {
              g_object_unref (info);
            }
#endif
        }

      xfce_rc_close (desktop_file);
    }

  clear_bad_entries (rc);

  xfce_rc_close (rc);

  return TRUE;
}



/**
 * blxo_helper_database_clear_default:
 * @database : an #BlxoHelperDatabase.
 * @category : an #BlxoHelperCategory.
 * @error    : return location for errors or %NULL.
 *
 * Clears the default #BlxoHelper for @category in @database.
 * Returns %TRUE on success, %FALSE if @error is set.
 *
 * Return value: %TRUE on success, %FALSE if @error is set.
 *
 * Since: 0.11.3
 **/
gboolean
blxo_helper_database_clear_default (BlxoHelperDatabase *database,
                                   BlxoHelperCategory  category,
                                   GError           **error)
{
  XfceRc       *rc, *desktop_file;
  gchar        *key;
  const gchar  *filename;
  gchar       **mimetypes;
  guint         i;
  gchar        *path;

  g_return_val_if_fail (category < BLXO_HELPER_N_CATEGORIES, FALSE);
  g_return_val_if_fail (BLXO_IS_HELPER_DATABASE (database), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  /* open the helpers.rc for writing */
  rc = xfce_rc_config_open (XFCE_RESOURCE_CONFIG, "xfce4/helpers.rc", FALSE);
  if (G_UNLIKELY (rc == NULL))
    {
      g_set_error (error, G_FILE_ERROR, G_FILE_ERROR_IO, _("Failed to open %s for writing"), "helpers.rc");
      return FALSE;
    }

  /* save the new setting */
  key = blxo_helper_category_to_string (category);
  xfce_rc_delete_entry (rc, key, FALSE);
  g_free (key);

  /* clear the dismissed preference */
  key = g_strconcat (blxo_helper_category_to_string (category), "Dismissed", NULL);
  xfce_rc_delete_entry (rc, key, FALSE);
  xfce_rc_close (rc);
  g_free (key);

  /* get the desktop filename */
  switch (category)
    {
      case BLXO_HELPER_WEBBROWSER:
        filename = "blxo-web-browser.desktop";
        break;

      case BLXO_HELPER_MAILREADER:
        filename = "blxo-mail-reader.desktop";
        break;

      case BLXO_HELPER_FILEMANAGER:
        filename = "blxo-file-manager.desktop";
        break;

      default:
        /* no mimetype support for terminals */
        return TRUE;
    }

  /* open the mimeapp.list file to set the default handler of the mime type */
  rc = mimeapps_open (FALSE);
  if (G_UNLIKELY (rc == NULL))
    {
      g_set_error (error, G_FILE_ERROR, G_FILE_ERROR_IO, _("Failed to open %s for writing"), "mimeapps.list");
      return FALSE;
    }

  /* open the blxo desktop file to read the mimetypes the file supports */
  path = g_build_filename ("applications", filename, NULL);
  desktop_file = xfce_rc_config_open (XFCE_RESOURCE_DATA, path, TRUE);
  g_free (path);

  if (G_UNLIKELY (desktop_file != NULL))
    {
      xfce_rc_set_group (desktop_file, "Desktop Entry");
      mimetypes = xfce_rc_read_list_entry (desktop_file, "X-XFCE-MimeType", ";");
      if (mimetypes != NULL)
        {
          xfce_rc_set_group (rc, "Added Associations");

          for (i = 0; mimetypes[i] != NULL; i++)
            if (!blxo_str_is_empty (mimetypes[i]))
              xfce_rc_delete_entry (rc, mimetypes[i], FALSE);
          g_strfreev (mimetypes);
        }

      xfce_rc_close (desktop_file);
    }

  clear_bad_entries (rc);

  xfce_rc_close (rc);

  return TRUE;
}



static void
clear_bad_entry (XfceRc *rc,
                 gchar  *key,
                 gchar  *filename)
{
  gchar **values;

  if (xfce_rc_has_entry (rc, key))
    {
      values = xfce_rc_read_list_entry (rc, key, ";");
      if (values != NULL)
        {
          GSList *list = NULL, *item = NULL;
          gint i;

          for (i = 0; values[i] != NULL; i++)
            {
              if (!blxo_str_is_empty(values[i]) && g_strcmp0(values[i], filename) != 0)
                {
                  list = g_slist_append (list, g_strdup(values[i]));
                }
            }
          g_strfreev(values);

          if (list == NULL)
            {
              xfce_rc_delete_entry (rc, key, FALSE);
            }
          else
            {
              gchar   *value;
              GString *string = g_string_new (NULL);
              for (item = list; item != NULL; item = g_slist_next (item))
                {
                  g_string_append_printf (string, "%s;", (gchar *)item->data);
                }
              value = g_string_free (string, FALSE);
              xfce_rc_write_entry (rc, key, value);
              g_slist_free_full (list, g_free);
              g_free (value);
            }
        }
    }
}



static void
clear_bad_entries (XfceRc *rc)
{
  xfce_rc_set_group (rc, "Added Associations");
  clear_bad_entry (rc, "x-scheme-handler/file", "blxo-file-manager.desktop"); // Xfce #7257
}



static gint
helper_compare (gconstpointer a,
                gconstpointer b)
{
  return g_utf8_collate (blxo_helper_get_name (a), blxo_helper_get_name (b));
}



/**
 * blxo_helper_database_get_all:
 * @database : an #BlxoHelperDatabase.
 * @category : an #BlxoHelperCategory.
 *
 * Looks up all available helpers for @category
 * in @database, sorted in alphabetic order.
 *
 * The returned list keeps references on the included
 * helpers, so be sure to run
 * <informalexample><programlisting>
 *  g_list_foreach (list, (GFunc) g_object_unref, NULL);
 *  g_list_free (list);
 * </programlisting></informalexample>
 * when you are done.
 *
 * Return value: The list of all helpers available
 *               in @category.
 **/
GList*
blxo_helper_database_get_all (BlxoHelperDatabase *database,
                             BlxoHelperCategory  category)
{
  BlxoHelper *helper;
  GList     *helpers = NULL;
  gchar    **specs;
  gchar     *id;
  gchar     *s;
  guint      n;

  g_return_val_if_fail (BLXO_IS_HELPER_DATABASE (database), NULL);
  g_return_val_if_fail (category < BLXO_HELPER_N_CATEGORIES, NULL);

  xfce_resource_push_path (XFCE_RESOURCE_DATA, DATADIR);
  specs = xfce_resource_match (XFCE_RESOURCE_DATA, "xfce4/helpers/*.desktop", TRUE);
  xfce_resource_pop_path (XFCE_RESOURCE_DATA);

  for (n = 0; specs[n] != NULL; ++n)
    {
      s = strrchr (specs[n], '.');
      if (G_LIKELY (s != NULL))
        *s = '\0';

      id = strrchr (specs[n], '/');
      id = (id != NULL) ? id + 1 : specs[n];

      helper = blxo_helper_database_lookup (database, category, id);
      if (G_LIKELY (helper != NULL))
        helpers = g_list_insert_sorted (helpers, helper, helper_compare);

      g_free (specs[n]);
    }
  g_free (specs);

  return helpers;
}



/**
 * blxo_helper_database_get_custom:
 * @database : an #BlxoHelperDatabase.
 * @category : an #BlxoHelperCategory.
 *
 * Returns the custom #BlxoHelper set for @database
 * or %NULL if no custom #BlxoHelper is set for
 * @category.
 *
 * The caller is responsible to free the returned
 * object using g_object_unref() when no longer
 * needed.
 *
 * Return value: the custom #BlxoHelper for @category
 *               in @database or %NULL.
 **/
BlxoHelper*
blxo_helper_database_get_custom (BlxoHelperDatabase *database,
                                BlxoHelperCategory  category)
{
  gchar *string;
  gchar  id[256];

  g_return_val_if_fail (BLXO_IS_HELPER_DATABASE (database), NULL);
  g_return_val_if_fail (category < BLXO_HELPER_N_CATEGORIES, NULL);

  /* determine the id for the custom helper */
  string = blxo_helper_category_to_string (category);
  g_snprintf (id, sizeof (id), "custom-%s", string);
  g_free (string);

  return blxo_helper_database_lookup (database, category, id);
}



/**
 * blxo_helper_database_set_custom:
 * @database : an #BlxoHelperDatabase.
 * @category : an #BlxoHelperCategory.
 * @command  : the custom command.
 *
 * Sets the custom helper for @category in @database
 * to @command.
 **/
void
blxo_helper_database_set_custom (BlxoHelperDatabase *database,
                                BlxoHelperCategory  category,
                                const gchar       *command)
{
  XfceRc *rc;
  gchar **argv;
  gchar  *category_string;
  gchar  *name;
  gchar  *cmdline;
  gchar  *file;
  gchar   spec[256];

  g_return_if_fail (BLXO_IS_HELPER_DATABASE (database));
  g_return_if_fail (category < BLXO_HELPER_N_CATEGORIES);
  g_return_if_fail (!blxo_str_is_empty (command));

  /* determine the spec for the custom helper */
  category_string = blxo_helper_category_to_string (category);
  g_snprintf (spec, sizeof (spec), "xfce4/helpers/custom-%s.desktop", category_string);

  /* lookup the resource save location */
  file = xfce_resource_save_location (XFCE_RESOURCE_DATA, spec, TRUE);

  /* write the custom helper file */
  rc = xfce_rc_simple_open (file, FALSE);
  if (G_LIKELY (rc != NULL))
    {
      xfce_rc_set_group (rc, "Desktop Entry");
      xfce_rc_write_bool_entry (rc, "NoDisplay", TRUE);
      xfce_rc_write_entry (rc, "Version", "1.0");
      xfce_rc_write_entry (rc, "Encoding", "UTF-8");
      xfce_rc_write_entry (rc, "Type", "X-XFCE-Helper");
      xfce_rc_write_entry (rc, "X-XFCE-Category", category_string);

      /* check if the command includes a parameter */
      cmdline = (strstr (command, "%s") != NULL) ? g_strdup (command) : g_strconcat (command, " \"%s\"", NULL);

      /* use the command line for the CommandsWithParameter */
      xfce_rc_write_entry (rc, "X-XFCE-CommandsWithParameter", cmdline);

      /* try to parse the command line */
      if (g_shell_parse_argv (cmdline, NULL, &argv, NULL))
        {
          /* use the basename for Name and Icon */
          name = g_path_get_basename (*argv);
          xfce_rc_write_entry (rc, "Icon", name);
          xfce_rc_write_entry (rc, "Name", name);
          g_free (name);

          if (strstr (command, "%s") == NULL) {
            /* trust the user, they defined the command without a parameter (bug #4093) */
            xfce_rc_write_entry (rc, "X-XFCE-Commands", command);
          } else {
            /* use only the binary for the Commands */
            xfce_rc_write_entry (rc, "X-XFCE-Commands", *argv);
          }

          /* cleanup */
          g_strfreev (argv);
        }
      else
        {
          xfce_rc_write_entry (rc, "Name", command);
          xfce_rc_write_entry (rc, "X-XFCE-Commands", command);
        }

      /* save the helper file */
      xfce_rc_close (rc);

      /* cleanup */
      g_free (cmdline);
    }

  /* ditch any cached object */
  g_hash_table_remove (database->helpers, spec);

  /* cleanup */
  g_free (category_string);
  g_free (file);
}

/**
 * blxo_helper_database_get_dismissed:
 * @database : an #BlxoHelperDatabase.
 * @category : an #BlxoHelperCategory.
 *
 * Returns %TRUE if errors should no longer be displayed
 * on the default #BlxoHelper for the @category in @database.
 *
 * Return value: %TRUE if dismissed, %FALSE otherwise.
 **/
gboolean blxo_helper_database_get_dismissed (BlxoHelperDatabase *database,
                                            BlxoHelperCategory  category)
{
  XfceRc      *rc;
  gchar       *key;
  gboolean     dismissed = FALSE;

  g_return_val_if_fail (BLXO_IS_HELPER_DATABASE (database), FALSE);
  g_return_val_if_fail (category < BLXO_HELPER_N_CATEGORIES, FALSE);

  rc = xfce_rc_config_open (XFCE_RESOURCE_CONFIG, "xfce4/helpers.rc", TRUE);
  if (G_LIKELY (rc != NULL))
    {
      key = g_strconcat (blxo_helper_category_to_string (category), "Dismissed", NULL);
      dismissed = xfce_rc_read_bool_entry (rc, key, FALSE);
      xfce_rc_close (rc);
      g_free (key);
    }

  return dismissed;
}

/**
 * blxo_helper_database_set_dismissed:
 * @database  : an #BlxoHelperDatabase.
 * @category  : an #BlxoHelperCategory.
 * @dismissed : TRUE if the errr should no longer be displayed.
 * @error     : return location for errors or %NULL.
 *
 * Dismisses future errors related to the selected helper category.
 * This setting is cleared any time a new default is configured.
 * Returns %TRUE on success, %FALSE if @error is set.
 *
 * Return value: %TRUE on success, %FALSE if @error is set.
 **/
gboolean
blxo_helper_database_set_dismissed (BlxoHelperDatabase *database,
                                   BlxoHelperCategory  category,
                                   gboolean           dismissed)
{
  XfceRc       *rc;
  gchar        *key;

  g_return_val_if_fail (category < BLXO_HELPER_N_CATEGORIES, FALSE);
  g_return_val_if_fail (BLXO_IS_HELPER_DATABASE (database), FALSE);

  /* open the helpers.rc for writing */
  rc = xfce_rc_config_open (XFCE_RESOURCE_CONFIG, "xfce4/helpers.rc", FALSE);
  if (G_UNLIKELY (rc == NULL))
    {
      return FALSE;
    }

  /* save the new setting */
  key = g_strconcat (blxo_helper_category_to_string (category), "Dismissed", NULL);
  xfce_rc_write_bool_entry (rc, key, dismissed);
  xfce_rc_close (rc);
  g_free (key);

  return TRUE;
}
