#include "assets/GameObjectsPrototypes.hpp"

#include "assets/Images.hpp"

const Object FISH_OBJECT_PROTOTYPE {(VisualAsset *) &FISH_VISUAL_ASSET, {0, 0, 13 * PIXEL_SIZE, 8 * PIXEL_SIZE}, ObjectTypes::FISH, {0, 0}};
const Object BACKGROUND_STAR_OBJECT_PROTOTYPE {(VisualAsset *) &BACKGROUND_STAR_VISUAL_ASSET, {0, 0, 0, 0}, ObjectTypes::BACKGROUND_STAR, {0, 0}};
const Object METEORITE_OBJECT_PROTOTYPE {(VisualAsset *) &METEORITE_VISUAL_ASSET, {0, 0, 23 * PIXEL_SIZE, 21 * PIXEL_SIZE}, ObjectTypes::METEORITE, {0, 0}};
const Object STAR_OBJECT_PROTOTYPE {(VisualAsset *) &STAR_VISUAL_ASSET, {0, 0, 18 * PIXEL_SIZE, 18 * PIXEL_SIZE}, ObjectTypes::STAR, {0, 0}};
const Object RAINBOW_OBJECT_PROTOTYPE {(VisualAsset *) &RAINBOW_VISUAL_ASSET, {0, 0, 0, 0}, ObjectTypes::RAINBOW, {0, 0}};
const Object CAT_OBJECT_PROTOTYPE {(VisualAsset *) &CAT_VISUAL_ASSET, {0, 0, 32 * PIXEL_SIZE, 18 * PIXEL_SIZE}, ObjectTypes::CAT, {0, 0}};