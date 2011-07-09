#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <ibus.h>

#include <stdio.h>
#define DEBUGM(X) fprintf(stderr, X)

#define IBUS_TYPE_CANNA_ENGINE (ibus_canna_engine_get_type ())

extern "C" {
GType ibus_canna_engine_get_type(void);
}

#endif
