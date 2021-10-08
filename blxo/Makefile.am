
AM_CPPFLAGS = 								\
	-DGLIB_DISABLE_DEPRECATION_WARNINGS \
	-I$(top_srcdir)							\
	-DDATADIR=\"$(datadir)\"					\
	-DBLXO_COMPILATION						\
	-DG_LOG_DOMAIN=\"blxo\"						\
	-DHELPERDIR=\"$(HELPER_PATH_PREFIX)/xfce4/blxo-$(LIBBLXO2_VERSION_API)\"	\
	-DLIBBLXO_VERSION_API=\"$(LIBBLXO_VERSION_API)\"			\
	-DLIBBLXO2_VERSION_API=\"$(LIBBLXO2_VERSION_API)\"			\
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\"


libblxo_headers =							\
	blxo-binding.h							\
	blxo-cell-renderer-ellipsized-text.h				\
	blxo-cell-renderer-icon.h					\
	blxo-execute.h							\
	blxo-gdk-pixbuf-extensions.h					\
	blxo-gtk-extensions.h						\
	blxo-gobject-extensions.h					\
	blxo-icon-bar.h							\
	blxo-icon-chooser-dialog.h					\
	blxo-icon-view.h							\
	blxo-job.h							\
	blxo-simple-job.h						\
	blxo-string.h							\
	blxo-toolbars-editor-dialog.h					\
	blxo-toolbars-editor.h						\
	blxo-toolbars-model.h						\
	blxo-toolbars-view.h						\
	blxo-tree-view.h							\
	blxo-utils.h							\
	blxo-wrap-table.h						\
	blxo-xsession-client.h

libblxo_built_public_sources =						\
	blxo-enum-types.h

libblxo_built_sources =							\
	$(libblxo_built_public_sources)					\
	blxo-alias.h							\
	blxo-aliasdef.c							\
	blxo-enum-types.c						\
	blxo-marshal.c							\
	blxo-marshal.h

libblxoincludedir = $(includedir)/blxo-1/blxo

libblxoinclude_HEADERS =							\
	$(libblxo_built_public_sources)					\
	$(libblxo_headers)						\
	blxo.h								\
	blxo-config.h


##
## GTK+ 3 support library
##

libblxo_2_includedir = $(includedir)/blxo-2/blxo

lib_LTLIBRARIES = libblxo-2.la

libblxo_2_include_HEADERS =						\
	blxo.h								\
	blxo-binding.h							\
	blxo-config.h							\
	blxo-execute.h							\
	blxo-gdk-pixbuf-extensions.h					\
	blxo-gtk-extensions.h						\
	blxo-gobject-extensions.h					\
	blxo-job.h							\
	blxo-simple-job.h						\
	blxo-string.h							\
	blxo-utils.h							\
	blxo-icon-chooser-dialog.h					\
	blxo-icon-chooser-model.h					\
	blxo-icon-view.h							\
	blxo-enum-types.h						\
	blxo-toolbars-model.h						\
	blxo-cell-renderer-icon.h					\
	blxo-thumbnail.h							\
	blxo-thumbnail-preview.h						\
	blxo-tree-view.h

libblxo_2_la_SOURCES =							\
	$(libblxo_2_include_HEADERS)					\
	blxo-binding.c							\
	blxo-marshal.c							\
	blxo-marshal.h							\
	blxo-private.c							\
	blxo-private.h							\
	blxo-config.c							\
	blxo-execute.c							\
	blxo-gdk-pixbuf-extensions.c					\
	blxo-gtk-extensions.c						\
	blxo-gobject-extensions.c					\
	blxo-job.c							\
	blxo-simple-job.c						\
	blxo-string.c							\
	blxo-utils.c							\
	blxo-icon-chooser-dialog.c					\
	blxo-icon-chooser-model.c					\
	blxo-icon-view.c							\
	blxo-enum-types.c						\
	blxo-cell-renderer-icon.c					\
	blxo-thumbnail.c							\
	blxo-thumbnail-preview.c						\
	blxo-tree-view.c

libblxo_2_la_CFLAGS =							\
	$(LIBBLADEUTIL_CFLAGS)						\
	$(GIO_CFLAGS)							\
	$(GTK3_CFLAGS)							\
	$(LIBX11_CFLAGS)

libblxo_2_la_LDFLAGS =							\
	-export-dynamic							\
	-version-info $(LIBBLXO_VERINFO)					\
	-export-symbols-regex "^[^_].*"					\
	-no-undefined

libblxo_2_la_LIBADD =							\
	$(LIBBLADEUTIL_LIBS)						\
	$(GIO_LIBS)							\
	$(GTK3_LIBS)							\
	$(LIBX11_LIBS)							\
	-lm

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = blxo-2.pc

#
# Gtk+ 2 support library
#
if ENABLE_GTK2_LIBRARY

lib_LTLIBRARIES += libblxo-1.la

libblxo_1_la_SOURCES =							\
	$(libblxoinclude_HEADERS)					\
	$(libblxo_built_sources)						\
	blxo-binding.c							\
	blxo-cell-renderer-ellipsized-text.c				\
	blxo-cell-renderer-icon.c					\
	blxo-config.c							\
	blxo-execute.c							\
	blxo-gdk-pixbuf-extensions.c					\
	blxo-gtk-extensions.c						\
	blxo-gobject-extensions.c					\
	blxo-icon-bar.c							\
	blxo-icon-chooser-dialog.c					\
	blxo-icon-chooser-model.c					\
	blxo-icon-chooser-model.h					\
	blxo-icon-view.c							\
	blxo-job.c							\
	blxo-job.h							\
	blxo-simple-job.c						\
	blxo-private.c							\
	blxo-private.h							\
	blxo-string.c							\
	blxo-thumbnail-preview.c						\
	blxo-thumbnail-preview.h						\
	blxo-thumbnail.c							\
	blxo-thumbnail.h							\
	blxo-toolbars-editor-dialog.c					\
	blxo-toolbars-editor.c						\
	blxo-toolbars-model.c						\
	blxo-toolbars-private.c						\
	blxo-toolbars-private.h						\
	blxo-toolbars-view.c						\
	blxo-tree-view.c							\
	blxo-utils.c							\
	blxo-wrap-table.c						\
	blxo-xsession-client.c

libblxo_1_la_CFLAGS =							\
	$(GIO_CFLAGS)							\
	$(GTK2_CFLAGS)							\
	$(LIBX11_CFLAGS)						\
	$(LIBBLADEUTIL_CFLAGS)

libblxo_1_la_LDFLAGS =							\
	-export-dynamic							\
	-version-info $(LIBBLXO_VERINFO)					\
	-export-symbols-regex "^[^_].*"					\
	-no-undefined

libblxo_1_la_LIBADD =							\
	$(GIO_LIBS)							\
	$(GTK2_LIBS)							\
	$(LIBX11_LIBS)							\
	$(LIBBLADEUTIL_LIBS)						\
	-lm

pkgconfig_DATA += blxo-1.pc

endif

##
## Rules to auto-generate built sources
##
## This is a bit tricky with automake, and non-trivial to implement. The
## rules below seem to work fine and don't seem to break the build, but
## they are only enabled in maintainer mode, so arbitrary users don't get
## trapped in automake's oddities. Therefore we ship the autogenerated
## files as part of the dist tarball.
##
if MAINTAINER_MODE
CLEANFILES =								\
	actual-abi							\
	expected-abi							\
	xgen-eetc							\
	xgen-eeth							\
	xgen-emc							\
	xgen-emh

DISTCLEANFILES =							\
	stamp-blxo-enum-types.h						\
	stamp-blxo-marshal.h						\
	$(libblxo_built_sources)

BUILT_SOURCES =								\
	$(libblxo_built_sources)

blxo-alias.h: make-blxo-alias.pl blxo.symbols
	$(AM_V_GEN) $(PERL) $(srcdir)/make-blxo-alias.pl < $(srcdir)/blxo.symbols > blxo-alias.h

blxo-aliasdef.c: make-blxo-alias.pl blxo.symbols
	$(AM_V_GEN) $(PERL) $(srcdir)/make-blxo-alias.pl -def < $(srcdir)/blxo.symbols > blxo-aliasdef.c

blxo-enum-types.h: stamp-blxo-enum-types.h
	@true
stamp-blxo-enum-types.h: $(libblxo_headers) Makefile
	$(AM_V_GEN) ( cd $(srcdir) && glib-mkenums \
		--fhead "#ifndef __BLXO_ENUM_TYPES_H__\n#define __BLXO_ENUM_TYPES_H__\n#include <libbladeutil/libbladeutil.h>\nG_BEGIN_DECLS\n" \
		--fprod "/* enumerations from \"@filename@\" */\n" \
		--vhead "GType @enum_name@_get_type (void) G_GNUC_CONST;\n#define BLXO_TYPE_@ENUMSHORT@ (@enum_name@_get_type())\n" \
		--ftail "G_END_DECLS\n\n#endif /* !__BLXO_ENUM_TYPES_H__ */" \
		$(libblxo_headers) ) >> xgen-eeth \
	&& (cmp -s xgen-eeth blxo-enum-types.h || cp xgen-eeth blxo-enum-types.h ) \
	&& rm -f xgen-eeth \
	&& echo timestamp > $(@F)

blxo-enum-types.c: $(libblxo_headers) Makefile
	$(AM_V_GEN) ( cd $(srcdir) && glib-mkenums \
		--fhead "#undef GTK_DISABLE_DEPRECATED\n#define GTK_ENABLE_BROKEN\n#include <blxo/blxo.h>\n#include <blxo/blxo-alias.h>\n" \
		--fprod "\n/* enumerations from \"@filename@\" */" \
		--vhead "GType\n@enum_name@_get_type (void)\n{\n\tstatic GType type = 0;\n\tif (type == 0) {\n\tstatic const G@Type@Value values[] = {"\
		--vprod "\t{ @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
		--vtail "\t{ 0, NULL, NULL }\n\t};\n\ttype = g_@type@_register_static (\"@EnumName@\", values);\n  }\n\treturn type;\n}\n" \
		--ftail "\n#define __BLXO_ENUM_TYPES_C__\n#include <blxo/blxo-aliasdef.c>\n" \
		$(libblxo_headers) ) >> xgen-eetc \
	&& cp xgen-eetc blxo-enum-types.c \
	&& rm -f xgen-eetc

blxo-marshal.h: stamp-blxo-marshal.h
	@true
stamp-blxo-marshal.h: blxo-marshal.list Makefile
	$(AM_V_GEN) ( cd $(srcdir) && glib-genmarshal \
		--prefix=_blxo_marshal \
		--header blxo-marshal.list ) >> xgen-emh \
	&& ( cmp -s xgen-emh blxo-marshal.h || cp xgen-emh blxo-marshal.h ) \
	&& rm -f xgen-emh \
	&& echo timestamp > $(@F)

blxo-marshal.c: blxo-marshal.list Makefile
	$(AM_V_GEN) echo "#include <blxo/blxo-marshal.h>" > xgen-emc \
	&& ( cd $(srcdir) && glib-genmarshal \
		--prefix=_blxo_marshal \
		--body blxo-marshal.list ) >> xgen-emc \
	&& cp xgen-emc blxo-marshal.c \
	&& rm -f xgen-emc
endif


# required for gtk-doc
dist-hook: all

EXTRA_DIST = 								\
	abicheck.sh							\
	blxo.symbols							\
	blxo-alias.h							\
	blxo-aliasdef.c							\
	blxo-icon-view-accessible.c					\
	blxo-marshal.list						\
	make-blxo-alias.pl

if HAVE_GNUC_VISIBILITY
TESTS =									\
	abicheck.sh
endif

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
