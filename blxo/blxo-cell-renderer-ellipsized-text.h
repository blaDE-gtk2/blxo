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

#if !defined (BLXO_INSIDE_BLXO_H) && !defined (BLXO_COMPILATION)
#error "Only <blxo/blxo.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef __BLXO_CELL_RENDERER_ELLIPSIZED_TEXT_H__
#define __BLXO_CELL_RENDERER_ELLIPSIZED_TEXT_H__

#include <blxo/blxo-config.h>

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _BlxoCellRendererEllipsizedTextPrivate BlxoCellRendererEllipsizedTextPrivate;
typedef struct _BlxoCellRendererEllipsizedTextClass   BlxoCellRendererEllipsizedTextClass;
typedef struct _BlxoCellRendererEllipsizedText        BlxoCellRendererEllipsizedText;

#define BLXO_TYPE_CELL_RENDERER_ELLIPSIZED_TEXT            (blxo_cell_renderer_ellipsized_text_get_type ())
#define BLXO_CELL_RENDERER_ELLIPSIZED_TEXT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_CELL_RENDERER_ELLIPSIZED_TEXT, BlxoCellRendererEllipsizedText))
#define BLXO_CELL_RENDERER_ELLIPSIZED_TEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((obj), BLXO_TYPE_CELL_RENDERER_ELLIPSIZED_TEXT, BlxoCellRendererEllipsizedTextClass))
#define BLXO_IS_CELL_RENDERER_ELLIPSIZED_TEXT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_CELL_RENDERER_ELLIPSIZED_TEXT))
#define BLXO_IS_CELL_RENDERER_ELLIPSIZED_TEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), BLXO_TYPE_CELL_RENDERER_ELLIPSIZED_TEXT))
#define BLXO_CELL_RENDERER_ELLIPSIZED_TEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_CELL_RENDERER_ELLIPSIZED_TEXT, BlxoCellRendererEllipsizedTextClass))

struct _BlxoCellRendererEllipsizedTextClass
{
  /*< private >*/
  GtkCellRendererTextClass __parent__;
};

/**
 * BlxoCellRendererEllipsizedText:
 *
 * The #BlxoIconCellRendererEllipsizedText
 * struct contains only private fields and should not be directly accessed.
 **/
struct _BlxoCellRendererEllipsizedText
{
  /*< private >*/
  GtkCellRendererText                   __parent__;
  BlxoCellRendererEllipsizedTextPrivate *priv;
};

GType            blxo_cell_renderer_ellipsized_text_get_type (void) G_GNUC_CONST;

GtkCellRenderer *blxo_cell_renderer_ellipsized_text_new      (void) G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;

G_END_DECLS

#endif /* !__BLXO_CELL_RENDERER_ELLIPSIZED_TEXT_H__ */
