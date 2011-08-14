SUBDIRS = src

component_DATA = canna.xml
componentdir = @datadir@/ibus/component

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
