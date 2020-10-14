#include <neo/config-pp.hpp>

#define Foo Enabled
#if !(NEO_IsEnabled(Foo))
#error "Fail"
#endif

#if !(!NEO_IsDisabled(Foo))
#error "Fail"
#endif

#define foo_SettingVal_Color_Green +6
#define foo_SettingVal_Color_Red +3

#define foo_Setting_Color Red

#if !(NEO_GetSettingValue(foo, Color) == foo_SettingVal_Color_Red)
#error "Fail"
#endif

#if !(NEO_SettingValueEqual(foo, Color, Red))
#error "Fail"
#endif

#if !(!NEO_SettingValueEqual(foo, Color, Green))
#error "Fail"
#endif

#if NEO_SettingValueEqual(foo, Color, Green)
#error "Fail"
#endif

#if !NEO_SettingValueEqual(foo, Color, Red)
#error "Fail2"
#endif

// Workaround warning about empty translation unit
void foo() {}
