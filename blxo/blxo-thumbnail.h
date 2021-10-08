/*-
 * Copyright (c) 2005-2006 Benedikt Meurer <benny@xfce.org>
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

#ifndef __BLXO_THUMBNAIL_H__
#define __BLXO_THUMBNAIL_H__

#include <blxo/blxo-config.h>

#include <gdk-pixbuf/gdk-pixbuf.h>

G_BEGIN_DECLS

/**
 * BlxoThumbnailSize:
 * @BLXO_THUMBNAIL_SIZE_NORMAL : normal sized thumbnails (up to 128px).
 * @BLXO_THUMBNAIL_SIZE_LARGE  : large sized thumbnails.
 *
 * Thumbnail sizes used by the thumbnail database.
 **/
typedef enum /*< skip >*/
{
  BLXO_THUMBNAIL_SIZE_NORMAL = 128,
  BLXO_THUMBNAIL_SIZE_LARGE  = 256,
} BlxoThumbnailSize;

G_GNUC_INTERNAL GdkPixbuf *_blxo_thumbnail_get_for_file (const gchar     *filename,
                                                        BlxoThumbnailSize size,
                                                        GError         **error) G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;
G_GNUC_INTERNAL GdkPixbuf *_blxo_thumbnail_get_for_uri  (const gchar     *uri,
                                                        BlxoThumbnailSize size,
                                                        GError         **error) G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;

G_END_DECLS

#endif /* !__BLXO_THUMBNAIL_H__ */
