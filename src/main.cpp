/* vim:set et sts=4: */

#include <ibus.h>
#include "engine.h"
#include <stdio.h>

static IBusBus *bus = NULL;
static IBusFactory *factory = NULL;

static void
ibus_disconnected_cb (IBusBus  *bus,
                      gpointer  user_data)
{
    ibus_quit ();
}

#define STR(x) _STR(x)
#define _STR(x) #x

static void init (void)
{
    IBusComponent *component;

    ibus_init ();

    bus = ibus_bus_new ();
    g_object_ref_sink (bus);
    g_signal_connect (bus, "disconnected", G_CALLBACK (ibus_disconnected_cb), NULL);
	
    factory = ibus_factory_new (ibus_bus_get_connection (bus));
    g_object_ref_sink (factory);
    ibus_factory_add_engine (factory, "ibus-canna", IBUS_TYPE_CANNA_ENGINE);

    int ret = ibus_bus_request_name (bus, "org.freedesktop.IBus.Canna", 0);
	if (ret == 0) {
//		DEBUGM("request_name failed\n");
	}
	
	//component = ibus_component_new_from_file(STR(COMPONENT_PATH));
	//fprintf(stderr, "xml file is: %s\n", STR(COMPONENT_PATH));
	//if (component == 0) {
	//	fprintf(stderr, "couldn't create ibus_component\n");
	//}
	component = ibus_component_new ("org.freedesktop.IBus.Canna",
                                    "Component for Canna",
                                    "0.1.0",
                                    "New BSD",
                                    "Fuminobu TAKEYAMA",
                                    "http://github.com/ftake/ibus-canna/",
                                    "",
                                    "ibus-canna");
     ibus_component_add_engine (component,
                                ibus_engine_desc_new ("ibus-canna",
                                                      "Canna",
                                                      "Component for Canna",
                                                      "ja",
                                                      "New BSD",
                                                      "Fuminobu TAKEYAMA",
                                                      "", //icon
                                                      "jp"));
    gboolean bret = ibus_bus_register_component (bus, component);
	if (bret == FALSE) {
		fprintf(stderr, "register_component failed.\n");
	}
}

int main()
{
	fprintf(stderr, "ibus-canna started up.\n");
    init ();
    ibus_main ();
	return 0;
}
