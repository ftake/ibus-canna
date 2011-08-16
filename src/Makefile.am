ibusenginedir=@IBUS_ENGINE_DIR@
ibusengine_PROGRAMS = ibus-engine-canna
ibus_engine_canna_SOURCES = engine.cpp main.cpp cannaui.cpp cannaui.h engine.h
ibus_engine_canna_CXXFLAGS = @IBUS_CFLAGS@ -DCOMPONENT_PATH="@COMPONENT_XML@" 
ibus_engine_canna_LDADD = @CANNA_LIBS@ @IBUS_LIBS@ @ICU_LIBS@
