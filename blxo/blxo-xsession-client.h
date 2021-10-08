/*-
 * Copyright (c) 2004 os-cillation e.K.
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

#if !defined (BLXO_INSIDE_BLXO_H) && !defined (BLXO_COMPILATION)
#error "Only <blxo/blxo.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef __BLXO_XSESSION_CLIENT_H__
#define __BLXO_XSESSION_CLIENT_H__

#include <gdk/gdk.h>

G_BEGIN_DECLS

typedef struct _BlxoXsessionClientPrivate BlxoXsessionClientPrivate;
typedef struct _BlxoXsessionClientClass   BlxoXsessionClientClass;
typedef struct _BlxoXsessionClient        BlxoXsessionClient;

#define BLXO_TYPE_XSESSION_CLIENT            (blxo_xsession_client_get_type ())
#define BLXO_XSESSION_CLIENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_XSESSION_CLIENT, BlxoXsessionClient))
#define BLXO_XSESSION_CLIENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_XSESSION_CLIENT, BlxoXsessionClientClass))
#define BLXO_IS_XSESSION_CLIENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_XSESSION_CLIENT))
#define BLXO_IS_XSESSION_CLIENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_TYPE_XSESSION_CLIENT))
#define BLXO_XSESSION_CLIENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_XSESSION_CLIENT, BlxoXsessionClientClass))

struct _BlxoXsessionClientClass
{
  GObjectClass __parent__;

  /* signals */
  void (*save_yourself) (BlxoXsessionClient *client);

  void (*reserved1) (void);
  void (*reserved2) (void);
};

/**
 * BlxoXsessionClient:
 *
 * The BlxoXsessionClient struct contains only private fields and should
 * not be directly accessed.
 **/
struct _BlxoXsessionClient
{
  GObject __parent__;

  /*< private >*/
  BlxoXsessionClientPrivate *priv;
};

GType              blxo_xsession_client_get_type             (void) G_GNUC_CONST;

BlxoXsessionClient *blxo_xsession_client_new_with_group       (GdkWindow          *leader);

GdkWindow         *blxo_xsession_client_get_group            (BlxoXsessionClient  *client);
void               blxo_xsession_client_set_group            (BlxoXsessionClient  *client,
                                                             GdkWindow          *leader);

gboolean           blxo_xsession_client_get_restart_command  (BlxoXsessionClient  *client,
                                                             gchar            ***argv,
                                                             gint               *argc);
void               blxo_xsession_client_set_restart_command  (BlxoXsessionClient  *client,
                                                             gchar             **argv,
                                                             gint                argc);

G_END_DECLS

#endif /* !__BLXO_XSESSION_CLIENT_H__ */
