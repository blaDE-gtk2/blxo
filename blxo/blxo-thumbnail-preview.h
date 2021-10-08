/*-
 * Copyright (c) 2006 Benedikt Meurer <benny@xfce.org>
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

#if !defined (BLXO_COMPILATION)
#error "Only <blxo/blxo.h> can be included directly, this file is not part of the public API."
#endif

#ifndef __BLXO_THUMBNAIL_PREVIEW_H__
#define __BLXO_THUMBNAIL_PREVIEW_H__

#include <blxo/blxo-config.h>

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _BlxoThumbnailPreviewClass BlxoThumbnailPreviewClass;
typedef struct _BlxoThumbnailPreview      BlxoThumbnailPreview;

#define BLXO_TYPE_THUMBNAIL_PREVIEW            (blxo_thumbnail_preview_get_type ())
#define BLXO_THUMBNAIL_PREVIEW(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_THUMBNAIL_PREVIEW, BlxoThumbnailPreview))
#define BLXO_THUMBNAIL_PREVIEW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_THUMBNAIL_PREVIEW, BlxoThumbnailPreviewClass))
#define BLXO_IS_THUMBNAIL_PREVIEW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_THUMBNAIL_PREVIEW))
#define BLXO_IS_THUMBNAIL_PREVIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_TYPE_THUMBNAIL_PREVIEW))
#define BLXO_THUMBNAIL_PREVIEW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_THUMBNAIL_PREVIEW, BlxoThumbnailPreviewClass))

G_GNUC_INTERNAL GType      blxo_thumbnail_preview_get_type  (void) G_GNUC_CONST;

G_GNUC_INTERNAL GtkWidget *_blxo_thumbnail_preview_new      (void) G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;

G_GNUC_INTERNAL void       _blxo_thumbnail_preview_set_uri  (BlxoThumbnailPreview *thumbnail_preview,
                                                            const gchar         *uri);

G_END_DECLS

#endif /* !__BLXO_THUMBNAIL_PREVIEW_H__ */
