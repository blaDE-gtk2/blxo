
AM_CPPFLAGS = 								\
	-DGLIB_DISABLE_DEPRECATION_WARNINGS \
	-I$(top_builddir)						\
	-I$(top_srcdir)							\
	-DBINDIR=\"$(bindir)\"						\
	-DDATADIR=\"$(datadir)\"					\
	-DG_LOG_DOMAIN=\"blxo-desktop-item-edit\"			\
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\"

bin_PROGRAMS =								\
	blxo-desktop-item-edit

blxo_desktop_item_edit_SOURCES =						\
	blxo-die-command-entry.c						\
	blxo-die-command-entry.h						\
	blxo-die-command-model.c						\
	blxo-die-command-model.h						\
	blxo-die-desktop-model.c						\
	blxo-die-desktop-model.h						\
	blxo-die-editor.c						\
	blxo-die-editor.h						\
	blxo-die-enum-types.c						\
	blxo-die-enum-types.h						\
	blxo-die-utils.c							\
	blxo-die-utils.h							\
	main.c

blxo_desktop_item_edit_CFLAGS =						\
	$(GTK3_CFLAGS)							\
	$(GTHREAD_CFLAGS)						\
	$(LIBBLADEUTIL_CFLAGS)						\
	$(GIO_CFLAGS)							\
	$(LIBBLADEUI2_CFLAGS)

blxo_desktop_item_edit_LDFLAGS =						\
	-no-undefined

blxo_desktop_item_edit_DEPENDENCIES =					\
	$(top_builddir)/blxo/libblxo-2.la

blxo_desktop_item_edit_LDADD =						\
	$(GTK3_LIBS)							\
	$(GTHREAD_LIBS)							\
	$(LIBBLADEUTIL_LIBS)						\
	$(GIO_LIBS)							\
	$(LIBBLADEUI2_LIBS)						\
	$(top_builddir)/blxo/libblxo-2.la

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
