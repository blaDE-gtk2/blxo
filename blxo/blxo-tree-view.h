/*-
 * Copyright (c) 2004-2006 Benedikt Meurer <benny@xfce.org>
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

#ifndef __BLXO_TREE_VIEW_H__
#define __BLXO_TREE_VIEW_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _BlxoTreeViewPrivate BlxoTreeViewPrivate;
typedef struct _BlxoTreeViewClass   BlxoTreeViewClass;
typedef struct _BlxoTreeView        BlxoTreeView;

#define BLXO_TYPE_TREE_VIEW            (blxo_tree_view_get_type ())
#define BLXO_TREE_VIEW(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_TREE_VIEW, BlxoTreeView))
#define BLXO_TREE_VIEW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_TREE_VIEW, BlxoTreeViewClass))
#define BLXO_IS_TREE_VIEW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_TREE_VIEW))
#define BLXO_IS_TREE_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_TYPE_TREE_VIEW))
#define BLXO_TREE_VIEW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_TREE_VIEW, BlxoTreeViewClass))

struct _BlxoTreeViewClass
{
  /*< private >*/
  GtkTreeViewClass __parent__;

  /*< private >*/
  void (*reserved1) (void);
  void (*reserved2) (void);
  void (*reserved3) (void);
  void (*reserved4) (void);
  void (*reserved5) (void);
  void (*reserved6) (void);
  void (*reserved7) (void);
  void (*reserved8) (void);
};

/**
 * BlxoTreeView:
 *
 * The #BlxoIconView struct contains only private fields and should
 * not be directly accessed.
 **/
struct _BlxoTreeView
{
  /*< private >*/
  GtkTreeView __parent__;

  /*< private >*/
  BlxoTreeViewPrivate *priv;
};

GType      blxo_tree_view_get_type                 (void) G_GNUC_CONST;

GtkWidget *blxo_tree_view_new                      (void) G_GNUC_MALLOC;

gboolean   blxo_tree_view_get_single_click         (const BlxoTreeView *tree_view);
void       blxo_tree_view_set_single_click         (BlxoTreeView       *tree_view,
                                                   gboolean           single_click);

guint      blxo_tree_view_get_single_click_timeout (const BlxoTreeView *tree_view);
void       blxo_tree_view_set_single_click_timeout (BlxoTreeView       *tree_view,
                                                   guint              single_click_timeout);

G_END_DECLS

#endif /* !__BLXO_TREE_VIEW_H__ */
