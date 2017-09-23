#ifndef ECMOVEBYKEYBOARDEIGHTDIRECTIONAL_H
#define ECMOVEBYKEYBOARDEIGHTDIRECTIONAL_H

#include <QObject>
#include <QPointer>
#include "Entity.h"

class QTimer;

/// An entity controller (TODO: link to documentation) that
/// moves an Entity relative to the screen in response to keyboard keys.
/// The entity can move in 4 directions only (up, down, left, and right) not diagonally.
///
/// Example usage:
/// ECMoveByKeyboardEightDirectional* c = new ECMoveByKeyboardEightDirectional(entity);
///
/// That is all that is needed. Once constructed, the Entity passed into the constructor will move
/// in response to the keyboard. The WASD keys are used to move the entity up, left, down, and right
/// relative to the screen. The entity is also moved diagonally if two of the keys is pressed simultaneously.
/// @author Abdullah Aghazadah
/// @date 11/21/16
class ECMoveByKeyboardEightDirectional: public QObject
{
    Q_OBJECT
public:
    // TODO factor out constructor for behaviors into a base EntityBehavior class
    ECMoveByKeyboardEightDirectional(Entity* entity);

    void setStepSize(double stepSize);
    double stepSize();
public slots:
    void moveStep_();
private:
    QPointer<Entity> entity_;   // TODO factor this attribute out into base EntityBehavior class
                                // this is a QPointer because we wanna know when the Entity is deleted
                                // so that we can stop moving it (dangling pointer!)
    double stepSize_;

    QTimer* moveTimer_;
};

#endif // ECMOVEBYKEYBOARDEIGHTDIRECTIONAL_H