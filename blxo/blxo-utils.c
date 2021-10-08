/*-
 * Copyright (c) 2005 Benedikt Meurer <benny@xfce.org>.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* implement blxo-utils's inline functions */
#define G_IMPLEMENT_INLINES 1
#define __BLXO_UTILS_C__
#include <blxo/blxo-utils.h>
#include <blxo/blxo-alias.h>

/**
 * SECTION: blxo-utils
 * @title: Miscellaneous Utility Functions
 * @short_description: Various utility functions
 * @include: blxo/blxo.h
 * @see_also: <ulink type="http" url="http://library.gnome.org/devel/glib/stable/glib-Atomic-Operations.html">
 *            GLib Atomic Operations</ulink>
 *
 * This module contains various utility functions that extend the basic
 * utility functions provided by the <ulink type="http"
 * url="http://library.gnome.org/devel/glib/stable/">GLib</ulink> library.
 **/



/**
 * blxo_noop:
 *
 * This function has no effect. It does nothing but
 * returning instantly. It is mostly useful in
 * situations that require a function to be called,
 * but that function does not need to do anything
 * useful.
 *
 * Since: 0.3.1
 **/
void
blxo_noop (void)
{
}



/**
 * blxo_noop_one:
 *
 * This function has no effect but simply returns
 * the integer value %1. It is mostly useful in
 * situations where you just need a function that
 * returns %1, but don't want to perform any other
 * actions.
 *
 * Returns: the integer value %1.
 *
 * Since: 0.3.1
 **/
gint
blxo_noop_one (void)
{
  return 1;
}



/**
 * blxo_noop_zero:
 *
 * This function has no effect but simply returns
 * the integer value %0. It is mostly useful in
 * situations where you just need a function that
 * returns %0, but don't want to perform any other
 * actions.
 *
 * Returns: the integer value %0.
 *
 * Since: 0.3.1
 **/
gint
blxo_noop_zero (void)
{
  return 0;
}



/**
 * blxo_noop_null:
 *
 * This function has no effect but simply returns
 * a %NULL pointer. It is mostly useful in
 * situations where you just need a function that
 * returns %NULL, but don't want to perform any
 * other actions.
 *
 * Returns: a %NULL pointer.
 *
 * Since: 0.3.1
 **/
gpointer
blxo_noop_null (void)
{
  return NULL;
}



/**
 * blxo_noop_true:
 *
 * This function has no effect, but simply returns
 * the boolean value %TRUE. It is mostly useful in
 * situations where you just need a function that
 * returns %TRUE, but don't want to perform any
 * other actions.
 *
 * Returns: the boolean value %TRUE.
 *
 * Since: 0.3.1
 **/
gboolean
blxo_noop_true (void)
{
  return TRUE;
}



/**
 * blxo_noop_false:
 *
 * This function has no effect, but simply returns
 * the boolean value %FALSE. It is mostly useful in
 * situations where you just need a function that
 * returns %FALSE, but don't want to perform any
 * other actions.
 *
 * Returns: the boolean value %FALSE.
 *
 * Since: 0.3.1
 **/
gboolean
blxo_noop_false (void)
{
  return FALSE;
}



#define __BLXO_UTILS_C__
#include <blxo/blxo-aliasdef.c>

