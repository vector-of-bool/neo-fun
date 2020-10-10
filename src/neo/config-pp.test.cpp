#include <neo/config-pp.hpp>

#define Foo Enabled
static_assert(NEO_IsEnabled(Foo));
static_assert(!NEO_IsDisabled(Foo));

#define foo_SettingVal_Color_Green +6
#define foo_SettingVal_Color_Red +3

#define foo_Setting_Color Red

static_assert(NEO_GetSettingValue(foo, Color) == foo_SettingVal_Color_Red);
static_assert(NEO_SettingValueEqual(foo, Color, Red));
static_assert(!NEO_SettingValueEqual(foo, Color, Green));

#if NEO_SettingValueEqual(foo, Color, Green)
#error "Fail"
#endif

#if !NEO_SettingValueEqual(foo, Color, Red)
#error "Fail2"
#endif
