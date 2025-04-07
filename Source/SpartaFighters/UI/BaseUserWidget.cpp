#include "BaseUserWidget.h"
#include "Framework/SFGameInstanceSubsystem.h"

UUIManager* UBaseUserWidget::ResolveUIManager() const
{
	if (const UWorld* World = GetWorld())
	{
		if (const UGameInstance* GI = World->GetGameInstance())
		{
			if (const USFGameInstanceSubsystem* Subsystem = GI->GetSubsystem<USFGameInstanceSubsystem>())
			{
				return Subsystem->GetUIManager();
			}
		}
	}
	return nullptr;
}