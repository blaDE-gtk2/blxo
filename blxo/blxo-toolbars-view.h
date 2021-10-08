/*-
 * Copyright (c) 2004 os-cillation e.K.
 * Copyright (c) 2003 Marco Pesenti Gritti
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

#ifndef __BLXO_TOOLBARS_VIEW_H__
#define __BLXO_TOOLBARS_VIEW_H__

#include <blxo/blxo-toolbars-model.h>

G_BEGIN_DECLS

#define BLXO_TYPE_TOOLBARS_VIEW            (blxo_toolbars_view_get_type ())
#define BLXO_TOOLBARS_VIEW(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_TOOLBARS_VIEW, BlxoToolbarsView))
#define BLXO_TOOLBARS_VIEW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_TOOLBARS_VIEW, BlxoToolbarsViewClass))
#define BLXO_IS_TOOLBARS_VIEW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_TOOLBARS_VIEW))
#define BLXO_IS_TOOLBARS_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_TYPE_TOOLBARS_VIEW))
#define BLXO_TOOLBARS_VIEW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_TOOLBARS_VIEW, BlxoToolbarsViewClass))

typedef struct _BlxoToolbarsViewPrivate BlxoToolbarsViewPrivate;
typedef struct _BlxoToolbarsViewClass   BlxoToolbarsViewClass;
typedef struct _BlxoToolbarsView        BlxoToolbarsView;

struct _BlxoToolbarsViewClass
{
  GtkVBoxClass __parent__;

  /* signals */
  void  (*action_request) (BlxoToolbarsView  *view,
                           const gchar      *action_name);
  void  (*customize)      (BlxoToolbarsView  *view);

  void  (*reserved1)      (void);
  void  (*reserved2)      (void);
  void  (*reserved3)      (void);
  void  (*reserved4)      (void);
};

/**
 * BlxoToolbarsView:
 *
 * The #BlxoToolbarsView struct contains only private fields and should
 * not be directly accessed.
 **/
struct _BlxoToolbarsView
{
  GtkVBox __parent__;

  /*< private >*/
  BlxoToolbarsViewPrivate *priv;
};


GType             blxo_toolbars_view_get_type        (void) G_GNUC_CONST;
GtkWidget        *blxo_toolbars_view_new             (GtkUIManager         *ui_manager);
GtkWidget        *blxo_toolbars_view_new_with_model  (GtkUIManager         *ui_manager,
                                                     BlxoToolbarsModel     *model);

gboolean          blxo_toolbars_view_get_editing     (BlxoToolbarsView      *view);
void              blxo_toolbars_view_set_editing     (BlxoToolbarsView      *view,
                                                     gboolean              editing);

BlxoToolbarsModel *blxo_toolbars_view_get_model       (BlxoToolbarsView      *view);
void              blxo_toolbars_view_set_model       (BlxoToolbarsView      *view,
                                                     BlxoToolbarsModel     *model);

GtkUIManager     *blxo_toolbars_view_get_ui_manager  (BlxoToolbarsView      *view);
void              blxo_toolbars_view_set_ui_manager  (BlxoToolbarsView      *view,
                                                     GtkUIManager         *ui_manager);

G_END_DECLS

#endif /* !__BLXO_TOOLBARS_VIEW_H__ */
