// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"

class UObject;

class KLDEBUGEXAMPLES_API FKLDebugUtilsPickerScoredObjects final : public FNoncopyable
{
public:
    explicit FKLDebugUtilsPickerScoredObjects(const float _Distance, UObject& _Object);

    [[nodiscard]] UObject* GetObject() const;
    [[nodiscard]] float GetDistance() const;

    [[nodiscard]] float GetScore() const;
    void SetScore(const float _Score);

private:
    UObject* mObject = nullptr;
    float mDistance = 0.f;
    float mScore = 0.f;
};

inline UObject* FKLDebugUtilsPickerScoredObjects::GetObject() const
{
    return mObject;
}

inline float FKLDebugUtilsPickerScoredObjects::GetDistance() const
{
    return mDistance;
}

inline float FKLDebugUtilsPickerScoredObjects::GetScore() const
{
    return mScore;
}

inline void FKLDebugUtilsPickerScoredObjects::SetScore(const float _Score)
{
    mScore = _Score;
}
