
AM_CPPFLAGS = 								\
	-DGLIB_DISABLE_DEPRECATION_WARNINGS \
	-I$(top_srcdir)							\
	-DG_LOG_DOMAIN=\"blxo-open\"					\
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\"

bin_PROGRAMS =								\
	blxo-open

blxo_open_SOURCES =							\
	main.c

blxo_open_CFLAGS =							\
	$(GTK3_CFLAGS)							\
	$(LIBBLADEUTIL_CFLAGS)						\
	$(GIO_CFLAGS)							\
	$(GIO_UNIX_CFLAGS)

blxo_open_LDFLAGS =							\
	-no-undefined

blxo_open_LDADD =							\
	$(GTK3_LIBS)							\
	$(LIBBLADEUTIL_LIBS)						\
	$(GIO_LIBS)							\
	$(GIO_UNIX_LIBS)						\
	$(top_builddir)/blxo/libblxo-2.la

desktopdir = $(datadir)/applications
desktop_in_files =							\
	blxo-file-manager.desktop.in					\
	blxo-mail-reader.desktop.in					\
	blxo-terminal-emulator.desktop.in				\
	blxo-web-browser.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

EXTRA_DIST =								\
	$(desktop_in_files)

DISTCLEANFILES =							\
	$(desktop_DATA)

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
