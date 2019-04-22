#include "alttab-handler_plugin.h"
#include "alttabhandler.h"

#include <qqml.h>

void Alttab_HandlerPlugin::registerTypes(const char *uri)
{
    // @uri com.alttabhandler
    qmlRegisterType<AltTabHandler>(uri, 1, 0, "AltTabHandler");
}

