// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "ImGuiDelegates.h"
#include "ImGuiDelegatesContainer.h"

#include <Engine/World.h>


FOnImGuiDelegate& FImGuiDelegates::OnWorldEarlyDebug()
{
	return OnWorldEarlyDebug(GWorld);
}

FOnImGuiDelegate& FImGuiDelegates::OnWorldEarlyDebug(UWorld* World)
{
	return FImGuiDelegatesContainer::Get().OnWorldEarlyDebug(World);
}

FOnImGuiDelegate& FImGuiDelegates::OnMultiContextEarlyDebug()
{
	return FImGuiDelegatesContainer::Get().OnMultiContextEarlyDebug();
}

FOnImGuiDelegate& FImGuiDelegates::OnWorldDebug()
{
	return OnWorldDebug(GWorld);
}

FOnImGuiDelegate& FImGuiDelegates::OnWorldDebug(UWorld* World)
{
	return FImGuiDelegatesContainer::Get().OnWorldDebug(World);
}

FOnImGuiDelegate& FImGuiDelegates::OnMultiContextDebug()
{
	return FImGuiDelegatesContainer::Get().OnMultiContextDebug();
}
