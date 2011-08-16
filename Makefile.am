SUBDIRS = src

component_DATA = canna.xml
componentdir = @IBUS_COMPONENT_DIR@

EXTRA_DIST = canna.xml.in
CLEANFILES = canna.xml

canna.xml: canna.xml.in
	$(AM_V_GEN) \
	( \
		libexecdir=${libexecdir}; \
		pkgdatadir=${pkgdatadir}; \
		s=`cat $<`; \
		eval "echo \"$${s}\""; \
	) > $@
