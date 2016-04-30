#ifndef PROJECTILECOLLISIONBEHAVIOR_H
#define PROJECTILECOLLISIONBEHAVIOR_H

#include <unordered_set>

class Projectile;
class Entity;

/// Represents a behavior that defines how the projectile should respond to
/// colliding with Entities.
class ProjectileCollisionBehavior
{
    friend class Projectile;
public:
    virtual void onCollisionWith(std::unordered_set<Entity*> entities) = 0;
protected:
    Projectile* projectile_;
};

#endif // PROJECTILECOLLISIONBEHAVIOR_H