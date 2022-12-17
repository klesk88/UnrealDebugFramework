// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Runtime/Public/Subsystems/KLDebugEngineSubsystem.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeKLDebugEngineSubsystem() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_UEngineSubsystem();
	KLDEBUGRUNTIME_API UClass* Z_Construct_UClass_UKLDebugEngineSubsystem();
	KLDEBUGRUNTIME_API UClass* Z_Construct_UClass_UKLDebugEngineSubsystem_NoRegister();
	UPackage* Z_Construct_UPackage__Script_KLDebugRuntime();
// End Cross Module References
	void UKLDebugEngineSubsystem::StaticRegisterNativesUKLDebugEngineSubsystem()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UKLDebugEngineSubsystem);
	UClass* Z_Construct_UClass_UKLDebugEngineSubsystem_NoRegister()
	{
		return UKLDebugEngineSubsystem::StaticClass();
	}
	struct Z_Construct_UClass_UKLDebugEngineSubsystem_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UKLDebugEngineSubsystem_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UEngineSubsystem,
		(UObject* (*)())Z_Construct_UPackage__Script_KLDebugRuntime,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UKLDebugEngineSubsystem_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "Subsystems/KLDebugEngineSubsystem.h" },
		{ "ModuleRelativePath", "Public/Subsystems/KLDebugEngineSubsystem.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UKLDebugEngineSubsystem_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UKLDebugEngineSubsystem>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UKLDebugEngineSubsystem_Statics::ClassParams = {
		&UKLDebugEngineSubsystem::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UKLDebugEngineSubsystem_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UKLDebugEngineSubsystem_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UKLDebugEngineSubsystem()
	{
		if (!Z_Registration_Info_UClass_UKLDebugEngineSubsystem.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UKLDebugEngineSubsystem.OuterSingleton, Z_Construct_UClass_UKLDebugEngineSubsystem_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UKLDebugEngineSubsystem.OuterSingleton;
	}
	template<> KLDEBUGRUNTIME_API UClass* StaticClass<UKLDebugEngineSubsystem>()
	{
		return UKLDebugEngineSubsystem::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UKLDebugEngineSubsystem);
	UKLDebugEngineSubsystem::~UKLDebugEngineSubsystem() {}
	struct Z_CompiledInDeferFile_FID_Projects_KLDebugFramework_KLDebugFramework_Plugins_KLDebug_Source_Runtime_Public_Subsystems_KLDebugEngineSubsystem_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Projects_KLDebugFramework_KLDebugFramework_Plugins_KLDebug_Source_Runtime_Public_Subsystems_KLDebugEngineSubsystem_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UKLDebugEngineSubsystem, UKLDebugEngineSubsystem::StaticClass, TEXT("UKLDebugEngineSubsystem"), &Z_Registration_Info_UClass_UKLDebugEngineSubsystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UKLDebugEngineSubsystem), 4035559941U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Projects_KLDebugFramework_KLDebugFramework_Plugins_KLDebug_Source_Runtime_Public_Subsystems_KLDebugEngineSubsystem_h_3308817196(TEXT("/Script/KLDebugRuntime"),
		Z_CompiledInDeferFile_FID_Projects_KLDebugFramework_KLDebugFramework_Plugins_KLDebug_Source_Runtime_Public_Subsystems_KLDebugEngineSubsystem_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Projects_KLDebugFramework_KLDebugFramework_Plugins_KLDebug_Source_Runtime_Public_Subsystems_KLDebugEngineSubsystem_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
