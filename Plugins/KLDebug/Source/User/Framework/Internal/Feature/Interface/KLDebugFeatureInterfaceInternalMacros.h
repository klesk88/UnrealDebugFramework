// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// c++
#include <type_traits>

/*
 * macro for function that all base interface classes need to implement
 */
#define INTERNAL_KL_DEBUG_FEATURE_COMMON_FUNCTIONS(FeatureClass)                                                              \
protected:                                                                                                                    \
    template <typename ChildClass>                                                                                            \
    [[nodiscard]] inline constexpr bool RequireCanvasUpdateInternal() const                                                    \
    {                                                                                                                         \
        return !std::is_same_v<decltype(&FeatureClass::DrawOnCanvas), decltype(&ChildClass::DrawOnCanvas)>;                   \
    }                                                                                                                         \
                                                                                                                              \
    template <typename ChildClass>                                                                                            \
    [[nodiscard]] inline constexpr bool RequireSceneProxyInternal() const                                                      \
    {                                                                                                                         \
        return !std::is_same_v<decltype(&FeatureClass::CreateDebugSceneProxy), decltype(&ChildClass::CreateDebugSceneProxy)>; \
    }                                                                                                                         \
                                                                                                                              \
    template <typename ChildClass>                                                                                            \
    [[nodiscard]] inline constexpr bool RequireTickInternal() const                                                            \
    {                                                                                                                         \
        return !std::is_same_v<decltype(&FeatureClass::Tick), decltype(&ChildClass::Tick)>;                                   \
    }                                                                                                                         \
                                                                                                                              \
    template <typename ChildClass>                                                                                            \
    [[nodiscard]] inline constexpr bool RequireDrawImGuiInternal() const                                                       \
    {                                                                                                                         \
        return !std::is_same_v<decltype(&FeatureClass::DrawImGui), decltype(&ChildClass::DrawImGui)> ||                       \
               !std::is_same_v<decltype(&FeatureClass::DrawImGuiChild), decltype(&ChildClass::DrawImGuiChild)>;               \
    }                                                                                                                         \
                                                                                                                              \
    template <typename ChildClass>                                                                                            \
    [[nodiscard]] inline constexpr bool RequireRenderInternal() const                                                          \
    {                                                                                                                         \
        return !std::is_same_v<decltype(&FeatureClass::Render), decltype(&ChildClass::Render)>;                               \
    }
