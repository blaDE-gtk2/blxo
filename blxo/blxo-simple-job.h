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

#ifndef __BLXO_SIMPLE_JOB_H__
#define __BLXO_SIMPLE_JOB_H__

#include <blxo/blxo-job.h>

G_BEGIN_DECLS

/**
 * BlxoSimpleJobFunc:
 * @job            : an #BlxoJob.
 * @param_values   : a #GValueArray of the #GValue<!---->s passed to
 *                   blxo_simple_job_launch().
 * @error          : return location for errors.
 *
 * Used by the #BlxoSimpleJob to process the @job. See blxo_simple_job_launch()
 * for further details.
 *
 * Returns: %TRUE on success, %FALSE in case of an error.
 **/
typedef gboolean (*BlxoSimpleJobFunc) (BlxoJob      *job,
                                      GValueArray *param_values,
                                      GError     **error);


#define BLXO_TYPE_SIMPLE_JOB            (blxo_simple_job_get_type ())
#define BLXO_SIMPLE_JOB(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_SIMPLE_JOB, BlxoSimpleJob))
#define BLXO_SIMPLE_JOB_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_SIMPLE_JOB, BlxoSimpleJobClass))
#define BLXO_IS_SIMPLE_JOB(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_SIMPLE_JOB))
#define BLXO_IS_SIMPLE_JOB_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_TYPE_SIMPLE_JOB))
#define BLXO_SIMPLE_JOB_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_SIMPLE_JOB, BlxoSimpleJobClass))

typedef struct _BlxoSimpleJobClass BlxoSimpleJobClass;
typedef struct _BlxoSimpleJob      BlxoSimpleJob;

GType   blxo_simple_job_get_type (void) G_GNUC_CONST;

BlxoJob *blxo_simple_job_launch   (BlxoSimpleJobFunc func,
                                 guint            n_param_values,
                                 ...) G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;

G_END_DECLS

#endif /* !__BLXO_SIMPLE_JOB_H__ */
