#include <SkyrimScripting/Plugin.h>
#include "Hooks.h"

OnInit {
	Hooks::Install();
}
