#ifndef G_INVENTORY_H
#define G_INVENTORY_H

namespace Cog
{
    struct Script;
}

namespace Game
{
    struct Item
    {
	    float value;
	    float min;
	    float max;
	    int flags;
        Cog::Script *cog;
    };

    void SetupInventory();

    void ActivateBin(int bin);
    void DeactivateBin(int bin);

    extern Item Items[200];
}
#endif