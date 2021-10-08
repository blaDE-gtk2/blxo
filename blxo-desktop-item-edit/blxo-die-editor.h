/*-
 * Copyright (c) 2006 Benedikt Meurer <benny@xfce.org>.
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

#ifndef __BLXO_DIE_EDITOR_H__
#define __BLXO_DIE_EDITOR_H__

#include <blxo-desktop-item-edit/blxo-die-enum-types.h>

G_BEGIN_DECLS;

typedef struct _BlxoDieEditorClass BlxoDieEditorClass;
typedef struct _BlxoDieEditor      BlxoDieEditor;

#define BLXO_DIE_TYPE_EDITOR             (blxo_die_editor_get_type ())
#define BLXO_DIE_EDITOR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_DIE_TYPE_EDITOR, BlxoDieEditor))
#define BLXO_DIE_EDITOR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_DIE_TYPE_EDITOR, BlxoDieEditorClass))
#define BLXO_DIE_IS_EDITOR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_DIE_TYPE_EDITOR))
#define BLXO_DIE_IS_EDITOR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_DIE_TYPE_EDITOR))
#define BLXO_DIE_EDITOR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_DIE_TYPE_EDITOR, BlxoDieEditorClass))

GType             blxo_die_editor_get_type     (void) G_GNUC_CONST;

GtkWidget        *blxo_die_editor_new          (void) G_GNUC_MALLOC;

gboolean          blxo_die_editor_get_complete (BlxoDieEditor     *editor);

BlxoDieEditorMode  blxo_die_editor_get_mode     (BlxoDieEditor     *editor);
void              blxo_die_editor_set_mode     (BlxoDieEditor     *editor,
                                               BlxoDieEditorMode  mode);

const gchar      *blxo_die_editor_get_name     (BlxoDieEditor     *editor);
void              blxo_die_editor_set_name     (BlxoDieEditor     *editor,
                                               const gchar      *name);

const gchar      *blxo_die_editor_get_comment  (BlxoDieEditor     *editor);
void              blxo_die_editor_set_comment  (BlxoDieEditor     *editor,
                                               const gchar      *comment);

const gchar      *blxo_die_editor_get_command  (BlxoDieEditor     *editor);
void              blxo_die_editor_set_command  (BlxoDieEditor     *editor,
                                               const gchar      *command);

const gchar      *blxo_die_editor_get_url      (BlxoDieEditor     *editor);
void              blxo_die_editor_set_url      (BlxoDieEditor     *editor,
                                               const gchar      *url);

const gchar      *blxo_die_editor_get_path     (BlxoDieEditor     *editor);
void              blxo_die_editor_set_path     (BlxoDieEditor     *editor,
                                               const gchar      *path);

const gchar      *blxo_die_editor_get_icon     (BlxoDieEditor     *editor);
void              blxo_die_editor_set_icon     (BlxoDieEditor     *editor,
                                               const gchar      *icon);

gboolean          blxo_die_editor_get_snotify  (BlxoDieEditor     *editor);
void              blxo_die_editor_set_snotify  (BlxoDieEditor     *editor,
                                               gboolean          snotify);

gboolean          blxo_die_editor_get_terminal (BlxoDieEditor     *editor);
void              blxo_die_editor_set_terminal (BlxoDieEditor     *editor,
                                               gboolean          terminal);

G_END_DECLS;

#endif /* !__BLXO_DIE_EDITOR_H__ */
