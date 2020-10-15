#pragma once

#include "./pp.hpp"

/**
 * @brief Macro corresponding to a setting being enabled
 */
#define NEO_Enabled +

/**
 * @brief Macro corresponding to a setting being disabled
 */
#define NEO_Disabled -

/**
 * !NOTE If you get a syntax error pointing here, it means that the
 * corresponding enabled/disabled macro was #defined to an
 * invalid value, and must be one of literal 'Enabled' or 'Disabled'
 */
/// Check if the given macro is set to a literal 'Enabled'
#define NEO_IsEnabled(Val) ((1 NEO_CONCAT(NEO_, Val) 5) == 6)
/// Check if the given macro is set to a literal 'Disabled'
#define NEO_IsDisabled(Val) ((14 NEO_CONCAT(NEO_, Val) 1) == 13)

/**
 * @brief Check if the given library feature is 'Enabled'.
 *
 * @param Namespace A namespace for the feature.
 * @param Feature The identifier of the feature.
 */
#define NEO_FeatureIsEnabled(Namespace, Feature)                                                   \
    NEO_IsEnabled(NEO_CONCAT_3(Namespace, _ToggleFeature_, Feature))

/**
 * @brief Check if the given library feature is DISABLED.
 *
 * @param Namespace The namespace for the feature.
 * @param Feature The feature identifier.
 */
#define NEO_FeatureIsDisabled(Namespace, Feature)                                                  \
    NEO_IsDisabled(NEO_CONCAT_3(Namespace, _ToggleFeature_, Feature))

/// Get the prefix of the settings value for 'Setting' within 'Namespace'
#define NEO_GetSettingValPrefix(Namespace, Setting)                                                \
    NEO_CONCAT_4(Namespace, _SettingVal_, Setting, _)

/// Get the current literal definition of the Setting macro in Namespace
#define NEO_GetSettingIdent(Namespace, Setting) NEO_CONCAT_3(Namespace, _Setting_, Setting)

/// Get the current numeric value of the given Setting macro for Namespace
#define NEO_GetSettingValue(Namespace, Setting)                                                    \
    (0 NEO_CONCAT(NEO_GetSettingValPrefix(Namespace, Setting),                                     \
                  NEO_GetSettingIdent(Namespace, Setting)))

/// Test if the value of the Setting in Namespace is defined to be the given CheckValue
#define NEO_SettingValueEqual(Namespace, Setting, CheckValue)                                      \
    (NEO_GetSettingValue(Namespace, Setting)                                                       \
     == (0 NEO_CONCAT(NEO_GetSettingValPrefix(Namespace, Setting), CheckValue)))
