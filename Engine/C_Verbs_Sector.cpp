#include "C_Verb.h"

#include "JK_Level.h"
#include "W_Sector.h"

namespace Cog
{
    Type_Void SetSectorAdjoins( Type_Sector sector_num, Type_Int status )
    {
	    currentLevel.sectors[sector_num].RenderAdjoins( status );
    }

    Type_Void SectorAdjoins( Type_Sector sector_num, Type_Int status )
    {
	    SetSectorAdjoins( sector_num, status );
    }

    Type_Void SetSectorLight( Type_Sector sector_num, Type_Flex light, Type_Flex delay )
    {
	    currentLevel.sectors[sector_num].SetLight( light );
    }

    Type_Void SectorLight( Type_Sector sector_num, Type_Flex light, Type_Flex delay )
    {
	    SetSectorLight( sector_num, light, delay );
    }
}