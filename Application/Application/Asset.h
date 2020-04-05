#ifndef ASSET_H
#define ASSET_H

#include <iostream>
#include <string>
#include <unordered_map>

#define AssetMap std::unordered_map<std::string, std::string>
#define MetalMap std::unordered_map<std::string, double>

class Asset
{

public:
    Asset();
    ~Asset();

    AssetMap * getResidences() const;
    AssetMap * getRentals() const;
    MetalMap * getMetals() const;

private:
    AssetMap * residences;
    AssetMap * rentals;
    MetalMap * metals;
};

#endif // ASSET_H
