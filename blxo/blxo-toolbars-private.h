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

#ifndef __BLXO_TOOLBARS_PRIVATE_H__
#define __BLXO_TOOLBARS_PRIVATE_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

G_GNUC_INTERNAL GdkPixbuf *_blxo_toolbars_new_separator_pixbuf (void) G_GNUC_MALLOC;
G_GNUC_INTERNAL GtkWidget *_blxo_toolbars_new_separator_image  (void) G_GNUC_MALLOC;

G_GNUC_INTERNAL GtkAction *_blxo_toolbars_find_action          (GtkUIManager *ui_manager,
                                                               const gchar  *name);

G_GNUC_INTERNAL void       _blxo_toolbars_set_drag_cursor      (GtkWidget    *widget);
G_GNUC_INTERNAL void       _blxo_toolbars_unset_drag_cursor    (GtkWidget    *widget);

G_END_DECLS

#endif /* !__BLXO_TOOLBARS_PRIVATE_H__ */
