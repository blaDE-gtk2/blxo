/*-
 * Copyright (c) 2000 Ramiro Estrugo <ramiro@eazel.com>
 * Copyright (c) 2005 Benedikt Meurer <benny@xfce.org>
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

#ifndef __BLXO_WRAP_TABLE_H__
#define __BLXO_WRAP_TABLE_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _BlxoWrapTablePrivate BlxoWrapTablePrivate;
typedef struct _BlxoWrapTableClass   BlxoWrapTableClass;
typedef struct _BlxoWrapTable        BlxoWrapTable;

#define BLXO_TYPE_WRAP_TABLE             (blxo_wrap_table_get_type ())
#define BLXO_WRAP_TABLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_WRAP_TABLE, BlxoWrapTable))
#define BLXO_WRAP_TABLE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_WRAP_TABLE, BlxoWrapTableClass))
#define BLXO_IS_WRAP_TABLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_WRAP_TABLE))
#define BLXO_IS_WRAP_TABLE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_TYPE_WRAP_TABLE))
#define BLXO_WRAP_TABLE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_WRAP_TABLE, BlxoWrapTableClass))

struct _BlxoWrapTableClass
{
  /*< private >*/
  GtkContainerClass __parent__;

  /* padding for further expansion */
  void (*reserved1) (void);
  void (*reserved2) (void);
  void (*reserved3) (void);
  void (*reserved4) (void);
};

/**
 * BlxoWrapTable:
 *
 *  The #BlxoWrapTable struct contains only private fields
* and should not be directly accessed.
 **/
struct _BlxoWrapTable
{
  /*< private >*/
  GtkContainer         __parent__;
  BlxoWrapTablePrivate *priv;
};

GType      blxo_wrap_table_get_type        (void) G_GNUC_CONST;

GtkWidget *blxo_wrap_table_new             (gboolean            homogeneous) G_GNUC_MALLOC;

guint      blxo_wrap_table_get_col_spacing (const BlxoWrapTable *table);
void       blxo_wrap_table_set_col_spacing (BlxoWrapTable       *table,
                                           guint               col_spacing);

guint      blxo_wrap_table_get_row_spacing (const BlxoWrapTable *table);
void       blxo_wrap_table_set_row_spacing (BlxoWrapTable       *table,
                                           guint               row_spacing);

gboolean   blxo_wrap_table_get_homogeneous (const BlxoWrapTable *table);
void       blxo_wrap_table_set_homogeneous (BlxoWrapTable       *table,
                                           gboolean            homogeneous);

G_END_DECLS

#endif /* !__BLXO_WRAP_TABLE_H__ */
