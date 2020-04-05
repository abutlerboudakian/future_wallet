#include "Asset.h"

Asset::Asset()
{
    residences = new AssetMap;
    rentals = new AssetMap;
    metals = new MetalMap;
}

Asset::~Asset()
{
    delete residences;
    delete rentals;
    delete metals;
}

AssetMap * Asset::getResidences() const
{
    return this->residences;
}

AssetMap * Asset::getRentals() const{
    return this->rentals;
}

MetalMap * Asset::getMetals() const
{
    return this->metals;
}
