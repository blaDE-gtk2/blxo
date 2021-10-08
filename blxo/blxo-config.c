/*-
 * Copyright (c) 2004 os-cillation e.K.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <blxo/blxo-config.h>
#include <blxo/blxo-alias.h>

/**
 * SECTION: blxo-config
 * @title: Version Information
 * @short_description: Variables and macros to check the blxo version
 * @include: blxo/blxo.h
 *
 * Blxo provides version information, primarily useful in configure for
 * builds that have a configure script. Applications may use it to
 * check if a certain feature is available in the version of libblxo
 * they are being built against or being linked with.
 **/

/**
 * blxo_major_version:
 *
 * The major version number of the blxo library (e.g. in blxo 1.2.3 this is 1).
 *
 * This variable is in the library, and therefore represents the blxo
 * library you have linked against. Contrast with the #BLXO_MAJOR_VERSION
 * macro, which represents the major version of the libblxo headers you
 * have included.
 **/
const guint blxo_major_version = BLXO_MAJOR_VERSION;

/**
 * blxo_minor_version:
 *
 * The minor version number of the blxo library (e.g. in blxo 1.2.3 this is 2).
 *
 * This variable is in the library, and therefore represents the blxo
 * library you have linked against. Contrast with the #BLXO_MINOR_VERSION
 * macro, which represents the minor version of the libblxo headers you
 * have included.
 **/
const guint blxo_minor_version = BLXO_MINOR_VERSION;

/**
 * blxo_micro_version:
 *
 * The micro version number of the blxo library (e.g. in blxo 1.2.3 this is 3).
 *
 * This variable is in the library, and therefore represents the blxo
 * library you have linked against. Contrast with the #BLXO_MICRO_VERSION
 * macro, which represents the micro version of the libblxo headers you
 * have included.
 **/
const guint blxo_micro_version = BLXO_MICRO_VERSION;



/**
 * blxo_check_version:
 * @required_major : the required major version.
 * @required_minor : the required minor version.
 * @required_micro : the required micro version.
 *
 * Checks that the <systemitem class="library">blxo</systemitem> library
 * in use is compatible with the given version. Generally you would pass in
 * the constants #BLXO_MAJOR_VERSION, #BLXO_MINOR_VERSION and #BLXO_MICRO_VERSION
 * as the three arguments to this function; that produces
 * a check that the library in use is compatible with the version of
 * <systemitem class="library">blxo</systemitem> the application was
 * compiled against.
 *
 * <example>
 * <title>Checking the runtime version of the blxo library</title>
 * <programlisting>
 * const gchar *mismatch;
 * mismatch = blxo_check_version (BLXO_VERSION_MAJOR,
 *                               BLXO_VERSION_MINOR,
 *                               BLXO_VERSION_MICRO);
 * if (G_UNLIKELY (mismatch != NULL))
 *   g_error ("Version mismatch: %<!---->s", mismatch);
 * </programlisting>
 * </example>
 *
 * Returns: %NULL if the library is compatible with the given version,
 *          or a string describing the version mismatch. The returned
 *          string is owned by the library and must not be freed or
 *          modified by the caller.
 *
 * Since: 0.3.1
 **/
const gchar*
blxo_check_version (guint required_major,
                   guint required_minor,
                   guint required_micro)
{
  return NULL;
}



#define __BLXO_CONFIG_C__
#include <blxo/blxo-aliasdef.c>
