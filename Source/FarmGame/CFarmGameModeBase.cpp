#include "CFarmGameModeBase.h"
#include "Character/CCharacter.h"

ACFarmGameModeBase::ACFarmGameModeBase()
{
	ConstructorHelpers::FClassFinder<APawn> PlayerClass(TEXT("/Game/FarmCharacter/BP_FCharacter"));
	if (PlayerClass.Succeeded())
	{
		DefaultPawnClass = PlayerClass.Class;
	}
}
