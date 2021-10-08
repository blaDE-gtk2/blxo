/*-
 * Copyright (c) 2004-2006 os-cillation e.K.
 *
 * Written by Benedikt Meurer <benny@xfce.org>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <blxo/blxo-config.h>
#include <blxo/blxo-private.h>
#include <blxo/blxo-string.h>
#include <blxo/blxo-xsession-client.h>
#include <blxo/blxo-alias.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif

/**
 * SECTION: blxo-xsession-client
 * @title: BlxoXsessionClient
 * @short_description: Lightweight session management support
 * @include: blxo/blxo.h
 *
 * This module provides application developers with lightweight
 * session management functions, based on the X11R5 session management
 * protocol. The X11R5 session management protocol is very limited in
 * its functionality and flexibility compared to the newer X11R6
 * session management protocol (XSMP), but - on the other hand - offers several
 * advantages for applications that do not need the complicated features
 * of the XSMP. Most importantly, the setup is much easier and
 * faster than with XSMP, because no special actions must be taken.
 *
 * So, in case your application is simple in its session management
 * requirements, e.g. it only needs to tell the session manager
 * its restart command, you may want to use the #BlxoXsessionClient
 * instead of a full featured XSMP client.
 *
 * Lets say, for example, you are developing a text editor, which
 * should provide basic session management support, limited to
 * proper restarting all editor windows that where left open
 * when you logged off the X session. In case the user was editing
 * a file when logging off, the same file should be opened in the
 * window on next startup.
 *
 * <example>
 * <title>Texteditor with <structname>BlxoXsessionClient</structname></title>
 * <programlisting>
 * static gchar *open_file_name = NULL;
 *
 * static void
 * save_yourself (BlxoXsessionClient *client)
 * {
 *   gchar *argv[2];
 *
 *   if (open_file_name != NULL)
 *     {
 *       argv[0] = "myeditor";
 *       argv[1] = open_file_name;
 *
 *       blxo_xsession_client_set_restart_command (client, argv, 2);
 *     }
 *   else
 *     {
 *       argv[0] = "myeditor";
 *
 *       blxo_xsession_client_set_restart_command (client, argv, 1);
 *     }
 * }
 *
 * // ...
 *
 * int
 * main (int argc, char **argv)
 * {
 *   BlxoXsessionClient *client;
 *   GdkDisplay        *display;
 *   GdkWindow         *leader;
 *   GtkWidget         *window;
 *
 *   gtk_init (&amp;argc, &amp;argv);
 *
 *   if (argc > 1)
 *     open_file_name = argv[1];
 *
 *   // create the main window
 *   window = create_window ();
 *
 *   // setup the session client
 *   display = gtk_widget_get_display (window);
 *   leader = gdk_display_get_default_group (display);
 *   client = blxo_xsession_client_new_with_group (leader);
 *   g_signal_connect (G_OBJECT (client), "save-yourself",
 *                     G_CALLBACK (save_yourself), NULL);
 *
 *   // ...
 * }
 * </programlisting>
 * </example>
 *
 * This example demonstrates the basic handling of #BlxoXsessionClient. It is
 * oversimplified, but we hope you get the point. The rule of thumb is, use
 * #BlxoXsessionClient if you can store all session data in the restart command,
 * else use a full-featured XSMP client.
 **/



enum
{
  PROP_0,
  PROP_GROUP,
  PROP_RESTART_COMMAND,
};

enum
{
  SAVE_YOURSELF,
  LAST_SIGNAL,
};



static void             blxo_xsession_client_dispose       (GObject                *object);
static void             blxo_xsession_client_get_property  (GObject                *object,
                                                           guint                   prop_id,
                                                           GValue                 *value,
                                                           GParamSpec             *pspec);
static void             blxo_xsession_client_set_property  (GObject                *object,
                                                           guint                   prop_id,
                                                           const GValue           *value,
                                                           GParamSpec             *pspec);
#ifdef GDK_WINDOWING_X11
static GdkFilterReturn  blxo_xsession_client_filter        (GdkXEvent              *xevent,
                                                           GdkEvent               *event,
                                                           gpointer                data);
#endif



struct _BlxoXsessionClientPrivate
{
#ifdef GDK_WINDOWING_X11
  Atom        wm_protocols;
  Atom        wm_save_yourself;
#endif
  GdkWindow  *leader;
};



static guint client_signals[LAST_SIGNAL];



G_DEFINE_TYPE_WITH_PRIVATE (BlxoXsessionClient, blxo_xsession_client, G_TYPE_OBJECT)



static void
blxo_xsession_client_class_init (BlxoXsessionClientClass *klass)
{
  GObjectClass *gobject_class;

  /* initialize the libblxo i18n support */
  _blxo_i18n_init ();

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->dispose = blxo_xsession_client_dispose;
  gobject_class->get_property = blxo_xsession_client_get_property;
  gobject_class->set_property = blxo_xsession_client_set_property;

  /**
   * BlxoXsessionClient:group:
   **/
  g_object_class_install_property (gobject_class,
                                   PROP_GROUP,
                                   g_param_spec_object ("group",
                                                        _("Window group"),
                                                        _("Window group leader"),
                                                        GDK_TYPE_WINDOW,
                                                        BLXO_PARAM_READWRITE));

  /**
   * BlxoXsessionClient:restart-command:
   **/
  g_object_class_install_property (gobject_class,
                                   PROP_RESTART_COMMAND,
                                   g_param_spec_boxed ("restart-command",
                                                       _("Restart command"),
                                                       _("Session restart command"),
                                                       G_TYPE_STRV,
                                                       BLXO_PARAM_READWRITE));

  /**
   * BlxoXsessionClient::save-yourself:
   * @client  : An #BlxoXsessionClient.
   *
   * This signal is emitted when @client receives a %WM_SAVE_YOURSELF
   * message from the session manager or the window manager on the
   * specified client leader window.
   **/
  client_signals[SAVE_YOURSELF] =
    g_signal_new (I_("save-yourself"),
                  G_TYPE_FROM_CLASS (gobject_class),
                  G_SIGNAL_RUN_FIRST,
                  G_STRUCT_OFFSET (BlxoXsessionClientClass, save_yourself),
                  NULL, NULL,
                  g_cclosure_marshal_VOID__VOID,
                  G_TYPE_NONE, 0);
}



static void
blxo_xsession_client_init (BlxoXsessionClient *client)
{
  client->priv = blxo_xsession_client_get_instance_private (client);
}



static void
blxo_xsession_client_dispose (GObject *object)
{
  BlxoXsessionClient *client = BLXO_XSESSION_CLIENT (object);

  blxo_xsession_client_set_group (client, NULL);

  (*G_OBJECT_CLASS (blxo_xsession_client_parent_class)->dispose) (object);
}



static void
blxo_xsession_client_get_property (GObject     *object,
                                  guint        prop_id,
                                  GValue      *value,
                                  GParamSpec  *pspec)
{
  BlxoXsessionClient *client = BLXO_XSESSION_CLIENT (object);
  gchar            **argv;

  switch (prop_id)
    {
    case PROP_GROUP:
      g_value_set_object (value, client->priv->leader);
      break;

    case PROP_RESTART_COMMAND:
      if (!blxo_xsession_client_get_restart_command (client, &argv, NULL))
        {
          argv = g_new (gchar *, 1);
          argv[0] = NULL;
        }
      g_value_take_boxed (value, argv);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}



static void
blxo_xsession_client_set_property (GObject       *object,
                                  guint          prop_id,
                                  const GValue  *value,
                                  GParamSpec    *pspec)
{
  BlxoXsessionClient *client = BLXO_XSESSION_CLIENT (object);

  switch (prop_id)
    {
    case PROP_GROUP:
      blxo_xsession_client_set_group (client, g_value_get_object (value));
      break;

    case PROP_RESTART_COMMAND:
      blxo_xsession_client_set_restart_command (client, g_value_get_boxed (value), -1);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}



#ifdef GDK_WINDOWING_X11
static GdkFilterReturn
blxo_xsession_client_filter (GdkXEvent *xevent,
                            GdkEvent  *event,
                            gpointer   data)
{
  XClientMessageEvent *xev = (XClientMessageEvent *) xevent;
  BlxoXsessionClient   *client = BLXO_XSESSION_CLIENT (data);

  if (xev->type == ClientMessage
      && xev->message_type == client->priv->wm_protocols
      && xev->format == 32
      && xev->data.l[0] == (glong) client->priv->wm_save_yourself)
    {
      g_signal_emit (G_OBJECT (client), client_signals[SAVE_YOURSELF], 0);
      return GDK_FILTER_REMOVE;
    }

  return GDK_FILTER_CONTINUE;
}
#endif



/**
 * blxo_xsession_client_new_with_group:
 * @leader  : The client leader window of the group.
 *
 * Creates a new #BlxoXsessionClient and associates it
 * with the group, which is lead by @leader.
 *
 * Returns: A newly allocated #BlxoXsessionClient.
 **/
BlxoXsessionClient*
blxo_xsession_client_new_with_group (GdkWindow *leader)
{
  g_return_val_if_fail (GDK_IS_WINDOW (leader), NULL);

  return g_object_new (BLXO_TYPE_XSESSION_CLIENT,
                       "group", leader,
                       NULL);
}



/**
 * blxo_xsession_client_get_group:
 * @client : An #BlxoXsessionClient.
 *
 * Returns the client leader window of the group with which
 * the @client is associated or %NULL if @client is not
 * associated with any group.
 *
 * Returns: The client leader window of the group with which @client is
 *          associated or %NULL.
 **/
GdkWindow*
blxo_xsession_client_get_group (BlxoXsessionClient *client)
{
  g_return_val_if_fail (BLXO_IS_XSESSION_CLIENT (client), NULL);
  return client->priv->leader;
}



/**
 * blxo_xsession_client_set_group:
 * @client  : An #BlxoXsessionClient.
 * @leader  : The client leader window of a group or %NULL.
 *
 * Sets the group according to the specified @leader.
 **/
void
blxo_xsession_client_set_group (BlxoXsessionClient *client,
                               GdkWindow         *leader)
{
#ifdef GDK_WINDOWING_X11
  const char  *atom_names[2] = { "WM_PROTOCOLS", "WM_SAVE_YOURSELF" };
  Atom         atoms[2];
  Atom        *protocols;
  Atom        *protocols_custom;
  int          nprotocols;
  int          n;
  int          m;

  g_return_if_fail (BLXO_IS_XSESSION_CLIENT (client));
  g_return_if_fail (GDK_IS_WINDOW (leader) || leader == NULL);

  if (G_UNLIKELY (client->priv->leader == leader))
    return;

  if (client->priv->leader != NULL)
    {
      /* remove WM_SAVE_YOURSELF protocol property */
      if (XGetWMProtocols (GDK_DRAWABLE_XDISPLAY (client->priv->leader),
                           GDK_DRAWABLE_XID (client->priv->leader),
                           &protocols, &nprotocols))
        {
          for (n = 0, m = 0; n < nprotocols; ++n)
            if (protocols[n] != client->priv->wm_save_yourself)
              protocols[m++] = protocols[n];
          nprotocols = m;

          if (G_LIKELY (nprotocols > 0))
            {
              XSetWMProtocols (GDK_DRAWABLE_XDISPLAY (client->priv->leader),
                               GDK_DRAWABLE_XID (client->priv->leader),
                               protocols, nprotocols);
            }

          XFree ((void *) protocols);
        }

      gdk_window_remove_filter (client->priv->leader, blxo_xsession_client_filter, client);
      g_object_unref (G_OBJECT (client->priv->leader));
    }

  client->priv->leader = leader;

  if (leader != NULL)
    {
      XInternAtoms (GDK_DRAWABLE_XDISPLAY (leader), (char **) atom_names, 2, False, atoms);
      client->priv->wm_protocols = atoms[0];
      client->priv->wm_save_yourself = atoms[1];

      /* setup WM_SAVE_YOURSELF protocol property */
      if (XGetWMProtocols (GDK_DRAWABLE_XDISPLAY (leader),
                           GDK_DRAWABLE_XID (leader),
                           &protocols, &nprotocols))
        {
          protocols_custom = g_newa (Atom, nprotocols + 1);
          memcpy (protocols_custom, protocols, nprotocols * sizeof (*protocols));
          protocols_custom[nprotocols++] = client->priv->wm_save_yourself;

          XSetWMProtocols (GDK_DRAWABLE_XDISPLAY (leader),
                           GDK_DRAWABLE_XID (leader),
                           protocols_custom, nprotocols);

          XFree ((void *) protocols);
        }

      gdk_window_add_filter (leader, blxo_xsession_client_filter, client);
      g_object_ref (G_OBJECT (leader));
    }

  g_object_notify (G_OBJECT (client), "group");
#endif
}



/**
 * blxo_xsession_client_get_restart_command:
 * @client  : An #BlxoXsessionClient.
 * @argv    : Pointer to the location where the
 *            pointer to the argument vector should
 *            be stored to.
 * @argc    : Pointer to the location where the
 *            number of arguments should be stored
 *            to or %NULL.
 *
 * Retrieves the restart command previously set on @client. The
 * result is stored in @argv and should be freed using
 * g_strfreev() when no longer needed.
 *
 * See blxo_xsession_client_set_restart_command() for further
 * explanation.
 *
 * Returns: %TRUE on success, else %FALSE.
 **/
gboolean
blxo_xsession_client_get_restart_command (BlxoXsessionClient  *client,
                                         gchar            ***argv,
                                         gint               *argc)
{
#ifdef GDK_WINDOWING_X11
  gchar **argv_return;
  gint    argc_return;

  g_return_val_if_fail (BLXO_IS_XSESSION_CLIENT (client), FALSE);
  g_return_val_if_fail (argv != NULL, FALSE);

  if (G_UNLIKELY (client->priv->leader == NULL))
    {
      g_warning ("Tried to get the restart command for an BlxoXsessionClient "
                 "instance, which is not connected to any client leader "
                 "window.");
      return FALSE;
    }

  if (XGetCommand (GDK_DRAWABLE_XDISPLAY (client->priv->leader),
                   GDK_DRAWABLE_XID (client->priv->leader),
                   &argv_return, &argc_return))
    {
      if (argc != NULL)
        *argc = argc_return;
      *argv = blxo_strndupv (argv_return, argc_return);
      XFreeStringList (argv_return);
      return TRUE;
    }
#endif

  return FALSE;
}



/**
 * blxo_xsession_client_set_restart_command:
 * @client  : An #BlxoXsessionClient.
 * @argv    : The argument vector.
 * @argc    : The number of arguments in @argv or -1.
 *
 * Sets the %WM_COMMAND property on the client leader window,
 * which instructs the session manager (or session-enabled window
 * manager) how to restart the application on next login.
 *
 * This function can only be used if @client is associated with
 * a client leader window.
 *
 * If @argc is specify as -1, the argument vector @argv is expected
 * to be %NULL<!-- -->-terminated and @argc will be automatically
 * calculated from @argv.
 *
 * Please take note, that gtk_init() automatically sets the
 * %WM_COMMAND property on all client leader windows that are
 * implicitly created by Gtk+. So, you may only need to call
 * this function in response to the ::save-yourself signal.
 **/
void
blxo_xsession_client_set_restart_command (BlxoXsessionClient *client,
                                         gchar            **argv,
                                         gint               argc)
{
  g_return_if_fail (BLXO_IS_XSESSION_CLIENT (client));
  g_return_if_fail (argv != NULL);
  g_return_if_fail (argc != 0);

  if (G_UNLIKELY (client->priv->leader == NULL))
    {
      g_warning ("Tried to set the restart command for an BlxoXsessionClient "
                 "instance, which is not connected to any client leader "
                 "window.");
      return;
    }

  /* count the arguments if caller doesn't specify the argc */
  if (G_UNLIKELY (argc < 0))
    for (argc = 0; argv[argc] != NULL; ++argc)
      ;

#ifdef GDK_WINDOWING_X11
  XSetCommand (GDK_DRAWABLE_XDISPLAY (client->priv->leader),
               GDK_DRAWABLE_XID (client->priv->leader),
               argv, argc);
#endif
}



#define __BLXO_XSESSION_CLIENT_C__
#include <blxo/blxo-aliasdef.c>
