#include "C_Verb.h"

#include "JK_Level.h"
#include "W_Sector.h"

C_Type_Void SetSectorAdjoins( C_Type_Sector sector_num, C_Type_Int status )
{
	currentLevel.sectors[sector_num].RenderAdjoins( status );
}

C_Type_Void SectorAdjoins( C_Type_Sector sector_num, C_Type_Int status )
{
	SetSectorAdjoins( sector_num, status );
}

C_Type_Void SetSectorLight( C_Type_Sector sector_num, C_Type_Flex light, C_Type_Flex delay )
{
	currentLevel.sectors[sector_num].SetLight( light );
}

C_Type_Void SectorLight( C_Type_Sector sector_num, C_Type_Flex light, C_Type_Flex delay )
{
	SetSectorLight( sector_num, light, delay );
}
