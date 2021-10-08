/*-
 * Copyright (c) 2004-2007 os-cillation e.K.
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

#ifndef __BLXO_H__
#define __BLXO_H__

/* be sure to have i18n macros available and libintl.h included! */
#if defined(GETTEXT_PACKAGE)
#include <glib/gi18n-lib.h>
#else
#include <glib/gi18n.h>
#endif

#include <gtk/gtk.h>

#define BLXO_INSIDE_BLXO_H

#include <blxo/blxo-config.h>

#include <blxo/blxo-binding.h>
#include <blxo/blxo-enum-types.h>
#include <blxo/blxo-cell-renderer-icon.h>
#include <blxo/blxo-gdk-pixbuf-extensions.h>
#include <blxo/blxo-gobject-extensions.h>
#include <blxo/blxo-execute.h>
#include <blxo/blxo-job.h>
#include <blxo/blxo-simple-job.h>
#include <blxo/blxo-string.h>
#include <blxo/blxo-utils.h>
#include <blxo/blxo-gtk-extensions.h>
#include <blxo/blxo-icon-chooser-dialog.h>
#include <blxo/blxo-icon-view.h>
#include <blxo/blxo-toolbars-model.h>
#include <blxo/blxo-tree-view.h>

#if !GTK_CHECK_VERSION (3, 0, 0)
#include <libbladeutil/libbladeutil.h>

#include <blxo/blxo-cell-renderer-ellipsized-text.h>
#include <blxo/blxo-icon-bar.h>
#include <blxo/blxo-toolbars-editor.h>
#include <blxo/blxo-toolbars-editor-dialog.h>
#include <blxo/blxo-toolbars-view.h>
#include <blxo/blxo-wrap-table.h>
#include <blxo/blxo-xsession-client.h>
#endif




#undef BLXO_INSIDE_BLXO_H

#endif /* !__BLXO_H__ */
