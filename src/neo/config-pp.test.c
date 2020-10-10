#include <neo/config-pp.hpp>

#define Foo Enabled
_Static_assert(NEO_IsEnabled(Foo), "Fail");
_Static_assert(!NEO_IsDisabled(Foo), "Fail");

#define foo_SettingVal_Color_Green +6
#define foo_SettingVal_Color_Red +3

#define foo_Setting_Color Red

_Static_assert(NEO_GetSettingValue(foo, Color) == foo_SettingVal_Color_Red, "Fail");
_Static_assert(NEO_SettingValueEqual(foo, Color, Red), "Fail");
_Static_assert(!NEO_SettingValueEqual(foo, Color, Green), "Fail");

#if NEO_SettingValueEqual(foo, Color, Green)
#error "Fail"
#endif

#if !NEO_SettingValueEqual(foo, Color, Red)
#error "Fail2"
#endif
