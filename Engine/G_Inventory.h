#ifndef G_INVENTORY_H
#define G_INVENTORY_H

struct C_Script;

struct G_Item
{
	float value;
	float min;
	float max;
	int flags;
	C_Script *cog;
};

void G_SetupInventory();

extern G_Item G_Items[200];
#endif