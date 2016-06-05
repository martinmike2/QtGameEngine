#include "ItemRainOfSpears.h"
#include "Sprite.h"
#include <QPixmap>
#include "SpearProjectile.h"
#include <cassert>
#include "DynamicEntity.h"
#include "Inventory.h"
#include "stdlib.h" // TODO: use C++ random number generatio instead

ItemRainOfSpears::ItemRainOfSpears()
{
    // set sprite
    Sprite* sprite = new Sprite(QPixmap(":/resources/graphics/weapons/trippleSpear.png"));
    setSprite(sprite);
}

/// Executed when the ItemRainOfSpears is asked to be used. Will generate a
/// bunch of falling spears.
void ItemRainOfSpears::use()
{
    // make sure the item is in an inventory
    assert(inventory() != nullptr);

    // make sure the inventory has an owner
    DynamicEntity* owner = inventory()->entity();
    assert(owner != nullptr);

    int NUM_SPEARS_TO_GENERATE = 10;
    int X_OFFSET_RANGE = 1000; // number of pixels around the owner's x position to spawn
                               // projectiles at
    int Y_OFFSET = 1000; // number of pixels ABOVE the owner to spawn projectiles
    int Y_OFFSET_RANGE = 600; // the y value "thickness" of possible y locations projectiles can spawn
    for (int i = 0, n = NUM_SPEARS_TO_GENERATE; i < n; i++){
        // get a random position some where above the entity.
        int randomXoffset = (rand() % X_OFFSET_RANGE) - (X_OFFSET_RANGE/2);
        int randomYoffset = rand() % Y_OFFSET_RANGE - Y_OFFSET;
        int posX = owner->pointPos().x() + randomXoffset;
        int posY = owner->pointPos().y() + randomYoffset;
        QPointF randomPos(posX,posY);

        // spawn projectile
        QPointF targetPos = randomPos;
        targetPos.setY(targetPos.y() + 20);
        targetPos.setX(targetPos.x() + 10);
        std::unordered_set<Entity*> noDmgList;
        noDmgList.insert(owner);
        noDmgList.insert(this);
        SpearProjectile* spearProjectile = new SpearProjectile(randomPos,
                                                               targetPos,
                                                               2000,
                                                               5,
                                                               noDmgList,
                                                               owner->map());
        spearProjectile->startMoving();
    }
}
