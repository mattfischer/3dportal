#include "C_Verb.h"

#include "G_Inventory.h"

namespace Cog
{
    Type_Float GetInv( Type_Thing thing_num, Type_Int bin )
    {
        return Game::Items[bin].value;
    }

    Type_Float GetInvMax( Type_Thing thing_num, Type_Int bin )
    {
	    return Game::Items[bin].max;
    }

    Type_Float GetInvMin( Type_Thing thing_num, Type_Int bin )
    {
	    return Game::Items[bin].min;
    }

    Type_Float ChangeInv( Type_Thing thing_num, Type_Int bin, Type_Float delta )
    {
	    Game::Items[bin].value += delta;
	    if( Game::Items[bin].value > Game::Items[bin].max )
		    Game::Items[bin].value = Game::Items[bin].max;
	    if( Game::Items[bin].value < Game::Items[bin].min )
		    Game::Items[bin].value = Game::Items[bin].min;

	    return Game::Items[bin].value;
    }
}