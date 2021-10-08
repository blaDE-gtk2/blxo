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

#ifndef __BLXO_TOOLBARS_EDITOR_H__
#define __BLXO_TOOLBARS_EDITOR_H__

#include <blxo/blxo-toolbars-view.h>

G_BEGIN_DECLS

#define BLXO_TYPE_TOOLBARS_EDITOR            (blxo_toolbars_editor_get_type ())
#define BLXO_TOOLBARS_EDITOR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_TOOLBARS_EDITOR, BlxoToolbarsEditor))
#define BLXO_TOOLBARS_EDITOR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_TOOLBARS_EDITOR, BlxoToolbarsEditorClass))
#define BLXO_IS_TOOLBARS_EDITOR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_TOOLBARS_EDITOR))
#define BLXO_IS_TOOLBARS_EDITOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_TYPE_TOOLBARS_EDITOR))
#define BLXO_TOOLBARS_EDITOR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_TOOLBARS_EDITOR, BlxoToolbarsEditorClass))

typedef struct _BlxoToolbarsEditorPrivate BlxoToolbarsEditorPrivate;
typedef struct _BlxoToolbarsEditorClass   BlxoToolbarsEditorClass;
typedef struct _BlxoToolbarsEditor        BlxoToolbarsEditor;

struct _BlxoToolbarsEditorClass
{
  GtkVBoxClass __parent__;

  void  (*reserved1)  (void);
  void  (*reserved2)  (void);
  void  (*reserved3)  (void);
  void  (*reserved4)  (void);
};

/**
 * BlxoToolbarsEditor:
 *
 * The #BlxoToolbarsEditor struct contains only private fields and
 * should not be directly accessed.
 **/
struct _BlxoToolbarsEditor
{
  GtkVBox __parent__;

  /*< private >*/
  BlxoToolbarsEditorPrivate *priv;
};


GType             blxo_toolbars_editor_get_type        (void) G_GNUC_CONST;
GtkWidget        *blxo_toolbars_editor_new             (GtkUIManager       *ui_manager);
GtkWidget        *blxo_toolbars_editor_new_with_model  (GtkUIManager       *ui_manager,
                                                       BlxoToolbarsModel   *model);

BlxoToolbarsModel *blxo_toolbars_editor_get_model       (BlxoToolbarsEditor  *editor);
void              blxo_toolbars_editor_set_model       (BlxoToolbarsEditor  *editor,
                                                       BlxoToolbarsModel   *model);

GtkUIManager     *blxo_toolbars_editor_get_ui_manager  (BlxoToolbarsEditor  *editor);
void              blxo_toolbars_editor_set_ui_manager  (BlxoToolbarsEditor  *editor,
                                                       GtkUIManager       *ui_manager);

G_END_DECLS

#endif /* !__BLXO_TOOLBARS_EDITOR_H__ */
