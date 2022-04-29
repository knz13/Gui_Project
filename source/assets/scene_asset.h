#pragma once
#include "asset_object.h"



class SceneAssetStorage {

};


class SceneAsset : public AssetObjectSpecifier<SceneAsset,SceneAssetStorage> {
public:
	SceneAsset(entt::entity e);

private:

};

