/* $Id: exo-uri.h,v 1.2 2004/09/17 09:48:24 bmeurer Exp $ */
/*-
 * Copyright (c) 2004  Benedikt Meurer <benny@xfce.org>
 * Copyright (c) 2003  Marco Pesenti Gritti
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
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#if !defined (EXO_INSIDE_EXO_H) && !defined (EXO_COMPILATION)
#error "Only <exo/exo.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef __EXO_TOOLBARS_PRIVATE_H__
#define __EXO_TOOLBARS_PRIVATE_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS;

GdkPixbuf *_exo_toolbars_new_separator_pixbuf (void);
GtkWidget *_exo_toolbars_new_separator_image  (void);

GtkAction *_exo_toolbars_find_action          (GtkUIManager     *ui_manager,
                                               const gchar      *name);

void       _exo_toolbars_set_drag_cursor      (GtkWidget        *widget);
void       _exo_toolbars_unset_drag_cursor    (GtkWidget        *widget);

G_END_DECLS;

#endif /* !__EXO_TOOLBARS_PRIVATE_H__ */
