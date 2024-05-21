// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "ImGui/Filter/Helpers/KLDebugTestSuiteTestFiltersHelpers.h"

#include "ImGui/Filter/Helpers/KLDebugTestSuiteTestFilters.h"

namespace KL::Debug::TestSuite::Filter::Helpers
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// private

    template <typename FilterClass>
    [[nodiscard]] KL::Debug::ImGui::Features::Types::FeatureOffset AllocateFilter(const KL::Debug::ImGui::Features::Types::FeatureOffset _CurrentFilterOffset, TArray<KL::Debug::ImGui::Features::Types::FilterPoolValue>& _FilterPool)
    {
        new (static_cast<void*>(&_FilterPool[_CurrentFilterOffset])) FilterClass();
        return _CurrentFilterOffset + sizeof(FilterClass);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// public

    void GetFilters(TArray<KL::Debug::ImGui::Features::Types::FilterPoolValue>& _FilterPool, TArray<KL::Debug::ImGui::Features::Types::FilterIndex>& _FilterOffsets)
    {
        static constexpr int32 FilterCount = 10;

        uint32 FullSize = 0;
        FullSize += sizeof(KLDebugImGuiTestFilterOne);
        FullSize += sizeof(KLDebugImGuiTestFilterTwo);
        FullSize += sizeof(KLDebugImGuiTestFilterThree);
        FullSize += sizeof(KLDebugImGuiTestFilterFour);
        FullSize += sizeof(KLDebugImGuiTestFilterFive);
        FullSize += sizeof(KLDebugImGuiTestFilterSix);
        FullSize += sizeof(KLDebugImGuiTestFilterSeven);
        FullSize += sizeof(KLDebugImGuiTestFilterHeight);
        FullSize += sizeof(KLDebugImGuiTestFilterNine);
        FullSize += sizeof(KLDebugImGuiTestFilterTen);

        _FilterOffsets.Reserve(FilterCount);
        _FilterPool.AddZeroed(FullSize);

        KL::Debug::ImGui::Features::Types::FeatureOffset FeatureOffset = 0;
        for (int32 i = 0; i < FilterCount; ++i)
        {
            _FilterOffsets.Emplace(FeatureOffset);

            switch (i)
            {
            case 0:
                FeatureOffset = AllocateFilter<KLDebugImGuiTestFilterOne>(FeatureOffset, _FilterPool);
                break;
            case 1:
                FeatureOffset = AllocateFilter<KLDebugImGuiTestFilterTwo>(FeatureOffset, _FilterPool);
                break;
            case 2:
                FeatureOffset = AllocateFilter<KLDebugImGuiTestFilterThree>(FeatureOffset, _FilterPool);
                break;
            case 3:
                FeatureOffset = AllocateFilter<KLDebugImGuiTestFilterFour>(FeatureOffset, _FilterPool);
                break;
            case 4:
                FeatureOffset = AllocateFilter<KLDebugImGuiTestFilterFive>(FeatureOffset, _FilterPool);
                break;
            case 5:
                FeatureOffset = AllocateFilter<KLDebugImGuiTestFilterSix>(FeatureOffset, _FilterPool);
                break;
            case 6:
                FeatureOffset = AllocateFilter<KLDebugImGuiTestFilterSeven>(FeatureOffset, _FilterPool);
                break;
            case 7:
                FeatureOffset = AllocateFilter<KLDebugImGuiTestFilterHeight>(FeatureOffset, _FilterPool);
                break;
            case 8:
                FeatureOffset = AllocateFilter<KLDebugImGuiTestFilterNine>(FeatureOffset, _FilterPool);
                break;
            case 9:
                FeatureOffset = AllocateFilter<KLDebugImGuiTestFilterTen>(FeatureOffset, _FilterPool);
                break;
            default:
                checkNoEntry();
                break;
            }
        }
    }
}    // namespace KL::Debug::TestSuite::Filter::Helpers