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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib.h>
#include <glib-object.h>

#include <gio/gio.h>

#include <blxo/blxo-config.h>
#include <blxo/blxo-job.h>
#include <blxo/blxo-private.h>
#include <blxo/blxo-string.h>
#include <blxo/blxo-alias.h>

/**
 * SECTION: blxo-job
 * @title: BlxoJob
 * @short_description: Base class for threaded/asynchronous jobs
 * @include: blxo/blxo.h
 * @see_also: <link linkend="BlxoSimpleJob">BlxoSimpleJob</link>
 *
 * <link linkend="BlxoJob">BlxoJob</link> is an abstract base class
 * intended to wrap threaded/asynchronous operations (called jobs here).
 * It was written because the ways of dealing with threads provided by
 * GLib are not exactly object-oriented.
 *
 * It can be used to wrap any kind of long-running or possibly-blocking
 * operation like file operations or communication with web services.
 * The benefit of using <link linkend="BlxoJob">BlxoJob</link> is that one
 * gets an object associated with each operation. After creating the job
 * the caller can connect to signals like <link linkend="BlxoJob::error">"error"
 * </link> or <link linkend="BlxoJob::percent">"percent"</link>. This
 * design integrates very well with the usual object-oriented design of
 * applications based on GObject.
 **/



/* Signal identifiers */
enum
{
  ERROR,
  FINISHED,
  INFO_MESSAGE,
  PERCENT,
  LAST_SIGNAL,
};



typedef struct _BlxoJobSignalData BlxoJobSignalData;



static void blxo_job_finalize   (GObject      *object);
static void blxo_job_error      (BlxoJob       *job,
                                const GError *error);
static void blxo_job_finished   (BlxoJob       *job);



struct _BlxoJobPrivate
{
  GIOSchedulerJob *scheduler_job;
  GCancellable    *cancellable;
  guint            running : 1;
  GError          *error;
  gboolean         failed;
  GMainContext    *context;
};

struct _BlxoJobSignalData
{
  gpointer instance;
  GQuark   signal_detail;
  guint    signal_id;
  va_list  var_args;
};



static guint job_signals[LAST_SIGNAL];



G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (BlxoJob, blxo_job, G_TYPE_OBJECT)



static void
blxo_job_class_init (BlxoJobClass *klass)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = blxo_job_finalize;

  klass->execute = NULL;
  klass->error = NULL;
  klass->finished = NULL;
  klass->info_message = NULL;
  klass->percent = NULL;

  /**
   * BlxoJob::error:
   * @job   : an #BlxoJob.
   * @error : a #GError describing the cause.
   *
   * Emitted whenever an error occurs while executing the @job. This signal
   * may not be emitted from within #BlxoJob subclasses. If a subclass wants
   * to emit an "error" signal (and thereby terminate the operation), it has
   * to fill the #GError structure and abort from its execute() method.
   * #BlxoJob will automatically emit the "error" signal when the #GError is
   * filled after the execute() method has finished.
   *
   * Callers interested in whether the @job was cancelled can connect to
   * the "cancelled" signal of the #GCancellable returned from
   * blxo_job_get_cancellable().
   **/
  job_signals[ERROR] =
    g_signal_new (I_("error"),
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_NO_HOOKS,
                  G_STRUCT_OFFSET (BlxoJobClass, error),
                  NULL, NULL,
                  g_cclosure_marshal_VOID__POINTER,
                  G_TYPE_NONE, 1, G_TYPE_POINTER);

  /**
   * BlxoJob::finished:
   * @job : an #BlxoJob.
   *
   * This signal will be automatically emitted once the @job finishes
   * its execution, no matter whether @job completed successfully or
   * was cancelled by the user. It may not be emitted by subclasses of
   * #BlxoJob as it is automatically emitted by #BlxoJob after the execute()
   * method has finished.
   **/
  job_signals[FINISHED] =
    g_signal_new (I_("finished"),
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_NO_HOOKS,
                  G_STRUCT_OFFSET (BlxoJobClass, finished),
                  NULL, NULL,
                  g_cclosure_marshal_VOID__VOID,
                  G_TYPE_NONE, 0);

  /**
   * BlxoJob::info-message:
   * @job     : an #BlxoJob.
   * @message : information to be displayed about @job.
   *
   * This signal is emitted to display information about the status of
   * the @job. Examples of messages are "Preparing..." or "Cleaning up...".
   *
   * The @message is garanteed to contain valid UTF-8, so it can be
   * displayed by #GtkWidget<!---->s out of the box.
   **/
  job_signals[INFO_MESSAGE] =
    g_signal_new (I_("info-message"),
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_NO_HOOKS,
                  G_STRUCT_OFFSET (BlxoJobClass, info_message),
                  NULL, NULL,
                  g_cclosure_marshal_VOID__STRING,
                  G_TYPE_NONE, 1, G_TYPE_STRING);

  /**
   * BlxoJob::percent:
   * @job     : an #BlxoJob.
   * @percent : the percentage of completeness.
   *
   * This signal is emitted to present the overall progress of the
   * operation. The @percent value is garantied to be a value between
   * 0.0 and 100.0.
   **/
  job_signals[PERCENT] =
    g_signal_new (I_("percent"),
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_NO_HOOKS,
                  G_STRUCT_OFFSET (BlxoJobClass, percent),
                  NULL, NULL,
                  g_cclosure_marshal_VOID__DOUBLE,
                  G_TYPE_NONE, 1, G_TYPE_DOUBLE);
}



static void
blxo_job_init (BlxoJob *job)
{
  job->priv = blxo_job_get_instance_private (job);
  job->priv->cancellable = g_cancellable_new ();
  job->priv->running = FALSE;
  job->priv->scheduler_job = NULL;
  job->priv->error = NULL;
  job->priv->failed = FALSE;
  job->priv->context = NULL;
}



static void
blxo_job_finalize (GObject *object)
{
  BlxoJob *job = BLXO_JOB (object);

  if (job->priv->running)
    blxo_job_cancel (job);

  if (job->priv->error != NULL)
    g_error_free (job->priv->error);

  g_object_unref (job->priv->cancellable);

  if (job->priv->context != NULL)
    g_main_context_unref (job->priv->context);

  (*G_OBJECT_CLASS (blxo_job_parent_class)->finalize) (object);
}



/**
 * blxo_job_async_ready:
 * @object : an #BlxoJob.
 * @result : the #GAsyncResult of the job.
 *
 * This function is called by the #GIOScheduler at the end of the
 * operation. It checks if there were errors during the operation
 * and emits "error" and "finished" signals.
 **/
static gboolean
blxo_job_async_ready (gpointer user_data)
{
  BlxoJob *job = BLXO_JOB (user_data);

  _blxo_return_val_if_fail (BLXO_IS_JOB (job), FALSE);

  if (job->priv->failed)
    {
      g_assert (job->priv->error != NULL);

      /* don't treat cancellation as an error */
      if (!g_error_matches (job->priv->error, G_IO_ERROR, G_IO_ERROR_CANCELLED))
        blxo_job_error (job, job->priv->error);

      /* cleanup */
      g_error_free (job->priv->error);
      job->priv->error = NULL;
    }

  blxo_job_finished (job);

  job->priv->running = FALSE;

  return FALSE;
}



/**
 * blxo_job_scheduler_job_func:
 * @scheduler_job : the #GIOSchedulerJob running the operation.
 * @cancellable   : the #GCancellable associated with the job.
 * @user_data     : an #BlxoJob.
 *
 * This function is called by the #GIOScheduler to execute the
 * operation associated with the job. It basically calls the
 * execute() function of #BlxoJobClass.
 *
 * Returns: %FALSE, to stop the thread at the end of the operation.
 **/
static gboolean
blxo_job_scheduler_job_func (GIOSchedulerJob *scheduler_job,
                            GCancellable    *cancellable,
                            gpointer         user_data)
{
  BlxoJob   *job = BLXO_JOB (user_data);
  GError   *error = NULL;
  gboolean  success;
  GSource  *source;

  _blxo_return_val_if_fail (BLXO_IS_JOB (job), FALSE);
  _blxo_return_val_if_fail (cancellable == NULL || G_IS_CANCELLABLE (cancellable), FALSE);

  job->priv->scheduler_job = scheduler_job;

  success = (*BLXO_JOB_GET_CLASS (job)->execute) (job, &error);

  if (!success)
    {
      /* clear existing error */
      if (G_UNLIKELY (job->priv->error != NULL))
        g_error_free (job->priv->error);

      /* take the error */
      job->priv->error = error;
    }

  job->priv->failed = !success;

  /* idle completion in the thread blxo job was started from */
  source = g_idle_source_new ();
  g_source_set_priority (source, G_PRIORITY_DEFAULT);
  g_source_set_callback (source, blxo_job_async_ready, g_object_ref (job),
                         g_object_unref);
  g_source_attach (source, job->priv->context);
  g_source_unref (source);

  return FALSE;
}



/**
 * blxo_job_emit_valist_in_mainloop:
 * @user_data : an #BlxoJobSignalData.
 *
 * Called from the main loop of the application to emit the signal
 * specified by the @user_data.
 *
 * Returns: %FALSE, to keep the function from being called
 *          multiple times in a row.
 **/
static gboolean
blxo_job_emit_valist_in_mainloop (gpointer user_data)
{
  BlxoJobSignalData *data = user_data;

  g_signal_emit_valist (data->instance, data->signal_id, data->signal_detail,
                        data->var_args);

  return FALSE;
}


/**
 * blxo_job_emit_valist:
 * @job           : an #BlxoJob.
 * @signal_id     : the signal id.
 * @signal_detail : the signal detail.
 * @var_args      : a list of parameters to be passed to the signal,
 *                  followed by a location for the return value. If the
 *                  return type of the signal is G_TYPE_NONE, the return
 *                  value location can be omitted.
 *
 * Sends a the signal with the given @signal_id and @signal_detail to the
 * main loop of the application and waits for listeners to handle it.
 **/
static void
blxo_job_emit_valist (BlxoJob *job,
                     guint   signal_id,
                     GQuark  signal_detail,
                     va_list var_args)
{
  BlxoJobSignalData data;

  _blxo_return_if_fail (BLXO_IS_JOB (job));
  _blxo_return_if_fail (job->priv->scheduler_job != NULL);

  data.instance = job;
  data.signal_id = signal_id;
  data.signal_detail = signal_detail;

  /* copy the variable argument list */
  G_VA_COPY (data.var_args, var_args);

  /* emit the signal in the main loop */
  blxo_job_send_to_mainloop (job,
                            blxo_job_emit_valist_in_mainloop,
                            &data, NULL);
}



/**
 * blxo_job_error:
 * @job   : an #BlxoJob.
 * @error : a #GError.
 *
 * Emits the "error" signal and passes the @error to it so that the
 * application can handle it (e.g. by displaying an error dialog).
 *
 * This function should never be called from outside the application's
 * main loop.
 **/
static void
blxo_job_error (BlxoJob       *job,
               const GError *error)
{
  _blxo_return_if_fail (BLXO_IS_JOB (job));
  _blxo_return_if_fail (error != NULL);

  g_signal_emit (job, job_signals[ERROR], 0, error);
}



/**
 * blxo_job_finished:
 * @job : an #BlxoJob.
 *
 * Emits the "finished" signal to notify listeners of the end of the
 * operation.
 *
 * This function should never be called from outside the application's
 * main loop.
 **/
static void
blxo_job_finished (BlxoJob *job)
{
  _blxo_return_if_fail (BLXO_IS_JOB (job));
  g_signal_emit (job, job_signals[FINISHED], 0);
}



/**
 * blxo_job_launch:
 * @job : an #BlxoJob.
 *
 * This functions schedules the @job to be run as soon as possible, in
 * a separate thread. The caller can connect to signals of the @job prior
 * or after this call in order to be notified on errors, progress updates
 * and the end of the operation.
 *
 * Returns: the @job itself.
 **/
BlxoJob *
blxo_job_launch (BlxoJob *job)
{
  _blxo_return_val_if_fail (BLXO_IS_JOB (job), NULL);
  _blxo_return_val_if_fail (!job->priv->running, NULL);
  _blxo_return_val_if_fail (BLXO_JOB_GET_CLASS (job)->execute != NULL, NULL);

  /* mark the job as running */
  job->priv->running = TRUE;

#if GLIB_CHECK_VERSION (2, 32, 0)
  job->priv->context = g_main_context_ref_thread_default ();
#else
  job->priv->context = g_main_context_get_thread_default ();
  if (job->priv->context == NULL)
    job->priv->context = g_main_context_default ();
  g_main_context_ref (job->priv->context);
#endif

G_GNUC_BEGIN_IGNORE_DEPRECATIONS
  g_io_scheduler_push_job (blxo_job_scheduler_job_func, g_object_ref (job),
                           (GDestroyNotify) g_object_unref,
                           G_PRIORITY_HIGH, job->priv->cancellable);
G_GNUC_END_IGNORE_DEPRECATIONS

  return job;
}



/**
 * blxo_job_cancel:
 * @job : a #BlxoJob.
 *
 * Attempts to cancel the operation currently performed by @job. Even
 * after the cancellation of @job, it may still emit signals, so you
 * must take care of disconnecting all handlers appropriately if you
 * cannot handle signals after cancellation.
 *
 * Calling this function when the @job has not been launched yet or
 * when it has already finished will have no effect.
 **/
void
blxo_job_cancel (BlxoJob *job)
{
  _blxo_return_if_fail (BLXO_IS_JOB (job));

  if (job->priv->running)
    g_cancellable_cancel (job->priv->cancellable);
}



/**
 * blxo_job_is_cancelled:
 * @job : a #BlxoJob.
 *
 * Checks whether @job was previously cancelled
 * by a call to blxo_job_cancel().
 *
 * Returns: %TRUE if @job is cancelled.
 **/
gboolean
blxo_job_is_cancelled (const BlxoJob *job)
{
  _blxo_return_val_if_fail (BLXO_IS_JOB (job), FALSE);
  return g_cancellable_is_cancelled (job->priv->cancellable);
}



/**
 * blxo_job_get_cancellable:
 * @job : an #BlxoJob.
 *
 * Returns the #GCancellable that can be used to cancel the @job.
 *
 * Returns: the #GCancellable associated with the @job. It
 *          is owned by the @job and must not be released.
 **/
GCancellable *
blxo_job_get_cancellable (const BlxoJob *job)
{
  _blxo_return_val_if_fail (BLXO_IS_JOB (job), NULL);
  return job->priv->cancellable;
}



/**
 * blxo_job_set_error_if_cancelled:
 * @job   : an #BlxoJob.
 * @error : error to be set if the @job was cancelled.
 *
 * Sets the @error if the @job was cancelled. This is a convenience
 * function that is equivalent to
 * <informalexample><programlisting>
 * GCancellable *cancellable;
 * cancellable = blxo_job_get_cancllable (job);
 * g_cancellable_set_error_if_cancelled (cancellable, error);
 * </programlisting></informalexample>
 *
 * Returns: %TRUE if the job was cancelled and @error is now set,
 *          %FALSE otherwise.
 **/
gboolean
blxo_job_set_error_if_cancelled (BlxoJob  *job,
                                GError **error)
{
  _blxo_return_val_if_fail (BLXO_IS_JOB (job), FALSE);
  return g_cancellable_set_error_if_cancelled (job->priv->cancellable, error);
}



/**
 * blxo_job_emit:
 * @job           : an #BlxoJob.
 * @signal_id     : the signal id.
 * @signal_detail : the signal detail.
 * @...           : a list of parameters to be passed to the signal,
 *                  followed by a location for the return value. If the
 *                  return type of the signal is G_TYPE_NONE, the return
 *                  value location can be omitted.
 *
 * Sends the signal with @signal_id and @signal_detail to the application's
 * main loop and waits for listeners to handle it.
 **/
void
blxo_job_emit (BlxoJob *job,
              guint   signal_id,
              GQuark  signal_detail,
              ...)
{
  va_list var_args;

  _blxo_return_if_fail (BLXO_IS_JOB (job));

  va_start (var_args, signal_detail);
  blxo_job_emit_valist (job, signal_id, signal_detail, var_args);
  va_end (var_args);
}



/**
 * blxo_job_info_message:
 * @job     : an #BlxoJob.
 * @format  : a format string.
 * @...     : parameters for the format string.
 *
 * Generates and emits an "info-message" signal and sends it to the
 * application's main loop.
 **/
void
blxo_job_info_message (BlxoJob      *job,
                      const gchar *format,
                      ...)
{
  va_list var_args;
  gchar  *message;

  _blxo_return_if_fail (BLXO_IS_JOB (job));
  _blxo_return_if_fail (format != NULL);

  va_start (var_args, format);
  message = g_strdup_vprintf (format, var_args);

  blxo_job_emit (job, job_signals[INFO_MESSAGE], 0, message);

  g_free (message);
  va_end (var_args);
}



/**
 * blxo_job_percent:
 * @job     : an #BlxoJob.
 * @percent : percentage of completeness of the operation.
 *
 * Emits a "percent" signal and sends it to the application's main
 * loop. Also makes sure that @percent is between 0.0 and 100.0.
 **/
void
blxo_job_percent (BlxoJob *job,
                 gdouble percent)
{
  _blxo_return_if_fail (BLXO_IS_JOB (job));

  percent = CLAMP (percent, 0.0, 100.0);
  blxo_job_emit (job, job_signals[PERCENT], 0, percent);
}



/**
 * blxo_job_send_to_mainloop:
 * @job            : an #BlxoJob.
 * @func           : a #GSourceFunc callback that will be called in the main thread.
 * @user_data      : data to pass to @func.
 * @destroy_notify : a #GDestroyNotify for @user_data, or %NULL.
 *
 * This functions schedules @func to be run in the main loop (main thread),
 * waiting for the result (and blocking the job in the meantime).
 *
 * Returns: The return value of @func.
 **/
gboolean
blxo_job_send_to_mainloop (BlxoJob        *job,
                          GSourceFunc    func,
                          gpointer       user_data,
                          GDestroyNotify destroy_notify)
{
  _blxo_return_val_if_fail (BLXO_IS_JOB (job), FALSE);
  _blxo_return_val_if_fail (job->priv->scheduler_job != NULL, FALSE);

G_GNUC_BEGIN_IGNORE_DEPRECATIONS
  return g_io_scheduler_job_send_to_mainloop (job->priv->scheduler_job, func, user_data,
                                              destroy_notify);
G_GNUC_END_IGNORE_DEPRECATIONS
}




#define __BLXO_JOB_C__
#include <blxo/blxo-aliasdef.c>