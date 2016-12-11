#include "Ability.h"
#include "assert.h"

/// Returns the owner of the Ability.
Entity *Ability::owner()
{
    return owner_;
}

/// Sets the owner of the Ability.
void Ability::setOwner(Entity &owner)
{
    owner_ = &owner;
}

/// Returns the icon of the Ability. If the Ability has no icon, returns nullptr.
Sprite *Ability::icon()
{
    return icon_;
}

/// Sets the icon of the Ability to the specified Sprite.
/// Pass in nullptr to give the Ability no icon.
void Ability::setIcon(Sprite *sprite)
{
    icon_ = sprite;
}

/// Constructs an Ability with the specified owner and icon.
Ability::Ability(Entity &owner, Sprite* icon):
    owner_(&owner),
    icon_(icon)
{
    // empty ctor body
}

/// Asserts that the owner of the Ability is still alive.
/// This function should be called by sub classes before actually
/// executing the use code.
void Ability::assertOwnerIsAlive_()
{
    assert(owner_ != nullptr);
}
