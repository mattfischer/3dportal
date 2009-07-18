#include "C_Verb.h"

#include "G_Inventory.h"

namespace Cog
{
    Type_Float GetInv( Type_Thing thing_num, Type_Int bin )
    {
	    return G_Items[bin].value;
    }

    Type_Float GetInvMax( Type_Thing thing_num, Type_Int bin )
    {
	    return G_Items[bin].max;
    }

    Type_Float GetInvMin( Type_Thing thing_num, Type_Int bin )
    {
	    return G_Items[bin].min;
    }

    Type_Float ChangeInv( Type_Thing thing_num, Type_Int bin, Type_Float delta )
    {
	    G_Items[bin].value += delta;
	    if( G_Items[bin].value > G_Items[bin].max )
		    G_Items[bin].value = G_Items[bin].max;
	    if( G_Items[bin].value < G_Items[bin].min )
		    G_Items[bin].value = G_Items[bin].min;

	    return G_Items[bin].value;
    }
}