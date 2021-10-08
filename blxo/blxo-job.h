/* vi:set et ai sw=2 sts=2 ts=2: */
/*-
 * Copyright (c) 2009 Jannis Pohlmann <jannis@xfce.org>
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#if !defined (BLXO_INSIDE_BLXO_H) && !defined (BLXO_COMPILATION)
#error "Only <blxo/blxo.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef __BLXO_JOB_H__
#define __BLXO_JOB_H__

#include <gio/gio.h>

G_BEGIN_DECLS

#define BLXO_TYPE_JOB            (blxo_job_get_type ())
#define BLXO_JOB(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_JOB, BlxoJob))
#define BLXO_JOB_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_JOB, BlxoJobClass))
#define BLXO_IS_JOB(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_JOB))
#define BLXO_IS_JOB_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_TYPE_JOB)
#define BLXO_JOB_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_JOB, BlxoJobClass))

typedef struct _BlxoJobPrivate BlxoJobPrivate;
typedef struct _BlxoJobClass   BlxoJobClass;
typedef struct _BlxoJob        BlxoJob;

struct _BlxoJobClass
{
  GObjectClass __parent__;

  /* virtual methods */
  gboolean   (*execute)      (BlxoJob      *job,
                              GError     **error);

  /* signals */
  void       (*error)        (BlxoJob      *job,
                              GError      *error);
  void       (*finished)     (BlxoJob      *job);
  void       (*info_message) (BlxoJob      *job,
                              const gchar *message);
  void       (*percent)      (BlxoJob      *job,
                              gdouble      percent);
};

/**
 * BlxoJob:
 *
 * The #BlxoJob struct contains only private fields and should not be
 * directly accessed.
 **/
struct _BlxoJob
{
  GObject __parent__;

  /*< private >*/
  BlxoJobPrivate *priv;
};

GType           blxo_job_get_type               (void) G_GNUC_CONST;

BlxoJob         *blxo_job_launch                 (BlxoJob        *job);
void            blxo_job_cancel                 (BlxoJob        *job);
gboolean        blxo_job_is_cancelled           (const BlxoJob  *job);
GCancellable   *blxo_job_get_cancellable        (const BlxoJob  *job);
gboolean        blxo_job_set_error_if_cancelled (BlxoJob        *job,
                                                GError       **error);
void            blxo_job_emit                   (BlxoJob        *job,
                                                guint          signal_id,
                                                GQuark         signal_detail,
                                                ...);
void            blxo_job_info_message           (BlxoJob        *job,
                                                const gchar   *format,
                                                ...);
void            blxo_job_percent                (BlxoJob        *job,
                                                gdouble        percent);
gboolean        blxo_job_send_to_mainloop       (BlxoJob        *job,
                                                GSourceFunc    func,
                                                gpointer       user_data,
                                                GDestroyNotify destroy_notify);

G_END_DECLS

#endif /* !__BLXO_JOB_H__ */
