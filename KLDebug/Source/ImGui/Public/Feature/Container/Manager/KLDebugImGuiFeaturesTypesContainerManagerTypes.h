#pragma once

enum class KLDEBUGIMGUI_API EContainerType : uint8
{
    SELECTABLE_OBJECTS = 0,
    ENGINE_SUBSYTSTEM,
    WORLD_SUBSYSTEM,
    EDITOR_SUBSYSTEM,
    GAME_INSTANCE_SUBSYSTEM,
    LOCAL_PLAYER_SUBSYSTEM,

    COUNT
};