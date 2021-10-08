## Process this file with automake to produce Makefile.in

# The name of the module.
DOC_MODULE=blxo

# Uncomment for versioned docs and specify the version of the module, e.g. '2'.
DOC_MODULE_VERSION=$(LIBBLXO_VERSION_API)

# The top-level SGML file.
DOC_MAIN_SGML_FILE=$(DOC_MODULE)-docs.xml

# Extra options to supply to gtkdoc-scan
SCAN_OPTIONS=--deprecated-guards="BLXO_DISABLE_DEPRECATED"

# The directory containing the source code. Relative to $(srcdir)
DOC_SOURCE_DIR=../..

# Extra options to supply to gtkdoc-mkdb
MKDB_OPTIONS=--output-format=xml --sgml-mode --name-space=blxo

# Extra options to supply to gtkdoc-fixref
FIXXREF_OPTIONS=

# Used for dependencies
HFILE_GLOB=$(top_srcdir)/blxo/*.h
CFILE_GLOB=$(top_srcdir)/blxo/*.c

# Header files to ignore when scanning
IGNORE_HFILES=

# Extra files to add when scanning (relative to $srcdir)
EXTRA_HFILES=

# Images to copy into HTML directory
HTML_IMAGES =								\
	$(srcdir)/images/blxo-gtk-file-chooser-add-thumbnail-preview.png	\
	$(srcdir)/images/blxo-icon-chooser-dialog.png			\
	$(srcdir)/images/blxo-toolbars-editor.png			\
	$(srcdir)/images/blxo-toolbars-editor-dialog.png			\
	$(srcdir)/images/blxo-toolbars-view.png				\
	$(srcdir)/images/blxo-wrap-table.png

# Extra SGML files that are included by DOC_MAIN_SGML_FILE
content_files = 							\
	blxo-csource.xml							\
	version.xml							\
	visual-index.xml

# CFLAGS and LDFLAGS for compiling scan program. Only needed
# if $(DOC_MODULE).types is non-empty.
# We need to use GTK2 blxo flags because the GTK3 version does not have
# all symbols (blxo_toolbars etc)
# We can't build the doc with --disable-gtk2 for now
GTKDOC_CFLAGS =	 							\
	-I$(top_srcdir) 						\
	-I$(top_builddir) 						\
	$(GTK2_CFLAGS)							\
	$(LIBBLADEUTIL_CFLAGS)

GTKDOC_LIBS =  								\
	$(top_builddir)/blxo/libblxo-$(LIBBLXO_VERSION_API).la \
	$(GTK2_LIBS)

include $(top_srcdir)/gtk-doc.make

# Other files to distribute
EXTRA_DIST += 								\
	blxo-csource.1							\
	blxo-open.1							\
	version.xml.in

# manual pages
man_MANS =								\
	blxo-csource.1							\
	blxo-open.1

if MAINTAINER_MODE
%.1: %.xml
	xsltproc -nonet http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl $<
endif

# required for gtk-doc
dist-hook: all

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
