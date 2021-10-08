
AM_CPPFLAGS = 								\
	-DGLIB_DISABLE_DEPRECATION_WARNINGS \
	-I$(top_srcdir)							\
	-DG_LOG_DOMAIN=\"blxo-csource\"					\
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\"

bin_PROGRAMS =								\
	blxo-csource

blxo_csource_SOURCES =							\
	main.c

blxo_csource_CFLAGS =							\
	$(GLIB_CFLAGS)

blxo_csource_LDFLAGS =							\
	-no-undefined

blxo_csource_LDADD =							\
	$(GLIB_LIBS)

# require to make dist in tests/
dist-hook: all

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
