#include "C_Verb.h"

#include "G_Inventory.h"

C_Type_Float GetInv( C_Type_Thing thing_num, C_Type_Int bin )
{
	return G_Items[bin].value;
}

C_Type_Float GetInvMax( C_Type_Thing thing_num, C_Type_Int bin )
{
	return G_Items[bin].max;
}

C_Type_Float GetInvMin( C_Type_Thing thing_num, C_Type_Int bin )
{
	return G_Items[bin].min;
}

C_Type_Float ChangeInv( C_Type_Thing thing_num, C_Type_Int bin, C_Type_Float delta )
{
	G_Items[bin].value += delta;
	if( G_Items[bin].value > G_Items[bin].max )
		G_Items[bin].value = G_Items[bin].max;
	if( G_Items[bin].value < G_Items[bin].min )
		G_Items[bin].value = G_Items[bin].min;

	return G_Items[bin].value;
}
