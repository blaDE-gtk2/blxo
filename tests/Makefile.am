
SUBDIRS =								\
	data

AM_CPPFLAGS =								\
	-DGLIB_DISABLE_DEPRECATION_WARNINGS \
	-I$(top_srcdir)							\
	-DG_LOG_DOMAIN=\"blxo-tests\"

TESTS =									\
	test-blxo-csource						\
	test-blxo-noop							\
	test-blxo-string

check_PROGRAMS =							\
	test-blxo-csource						\
	test-blxo-noop							\
	test-blxo-string							\
	test-blxo-icon-chooser-dialog					\
	test-blxo-icon-chooser-dialog-gtk3					\
	test-blxo-wrap-table

test_blxo_csource_SOURCES =						\
	test-blxo-csource.c						\
	test-blxo-csource-data.c

test_blxo_csource_CFLAGS =						\
	$(GLIB_CFLAGS)

test_blxo_csource_LDADD =						\
	$(GLIB_LIBS)

test_blxo_noop_SOURCES =							\
	test-blxo-noop.c

test_blxo_noop_CFLAGS =							\
	$(GTK2_CFLAGS)							\
	$(LIBBLADEUTIL_CFLAGS)

test_blxo_noop_DEPENDENCIES =						\
	$(top_builddir)/blxo/libblxo-$(LIBBLXO_VERSION_API).la

test_blxo_noop_LDADD =							\
	$(GLIB_LIBS)							\
	$(top_builddir)/blxo/libblxo-$(LIBBLXO_VERSION_API).la

test_blxo_string_SOURCES =						\
	test-blxo-string.c

test_blxo_string_CFLAGS =						\
	$(GTK2_CFLAGS)							\
	$(LIBBLADEUTIL_CFLAGS)

test_blxo_string_DEPENDENCIES =						\
	$(top_builddir)/blxo/libblxo-$(LIBBLXO_VERSION_API).la

test_blxo_string_LDADD =							\
	$(GLIB_LIBS)							\
	$(top_builddir)/blxo/libblxo-$(LIBBLXO_VERSION_API).la

test_blxo_icon_chooser_dialog_SOURCES =					\
	test-blxo-icon-chooser-dialog.c

test_blxo_icon_chooser_dialog_CFLAGS =					\
	$(GTK2_CFLAGS)							\
	$(LIBBLADEUTIL_CFLAGS)

test_blxo_icon_chooser_dialog_DEPENDENCIES =				\
	$(top_builddir)/blxo/libblxo-$(LIBBLXO_VERSION_API).la

test_blxo_icon_chooser_dialog_LDADD =					\
	$(GTK2_LIBS)							\
	$(top_builddir)/blxo/libblxo-$(LIBBLXO_VERSION_API).la

test_blxo_icon_chooser_dialog_gtk3_SOURCES =					\
	test-blxo-icon-chooser-dialog.c

test_blxo_icon_chooser_dialog_gtk3_CFLAGS =					\
	$(GTK3_CFLAGS)							\
	$(LIBBLADEUTIL_CFLAGS)

test_blxo_icon_chooser_dialog_gtk3_DEPENDENCIES =				\
	$(top_builddir)/blxo/libblxo-2.la

test_blxo_icon_chooser_dialog_gtk3_LDADD =					\
	$(GTK3_LIBS)							\
	$(top_builddir)/blxo/libblxo-2.la

test_blxo_wrap_table_SOURCES =						\
	test-blxo-wrap-table.c

test_blxo_wrap_table_CFLAGS =						\
	$(GTK2_CFLAGS)							\
	$(LIBBLADEUTIL_CFLAGS)

test_blxo_wrap_table_DEPENDENCIES =					\
	$(top_builddir)/blxo/libblxo-$(LIBBLXO_VERSION_API).la

test_blxo_wrap_table_LDADD =						\
	$(GTK2_LIBS)							\
	$(top_builddir)/blxo/libblxo-$(LIBBLXO_VERSION_API).la

DISTCLEANFILES = test-blxo-csource-data.c
BUILT_SOURCES = test-blxo-csource-data.c

test-blxo-csource-data.c: Makefile $(srcdir)/data/data.bin $(srcdir)/data/data.txt
	$(top_builddir)/blxo-csource/blxo-csource --extern --build-list data_bin $(srcdir)/data/data.bin data_txt $(srcdir)/data/data.txt > test-blxo-csource-data.c

clean-local:
	rm -f *.core core core.*

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
