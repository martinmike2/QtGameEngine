#include "ECMoveByKeyboardFourDirectional.h"
#include "Map.h"
#include <QTimer>
#include <cassert>
#include "Game.h"
#include "Sprite.h"
#include "Utilities.h"

ECMoveByKeyboardFourDirectional::ECMoveByKeyboardFourDirectional(Entity *entity):
    entity_(entity),
    stepSize_(15),
    moveTimer_(new QTimer(this))
{
    // make sure passed in Entity is not nullptr
    assert(entity != nullptr);

    // connect timer to move step
    connect(moveTimer_,&QTimer::timeout,this,&ECMoveByKeyboardFourDirectional::moveStep_);
    moveTimer_->start(secondsToMs(frequency(stepSize_,entity_->speed())));
}

/// See ECPathMover::setStepSize().
void ECMoveByKeyboardFourDirectional::setStepSize(double stepSize)
{
    stepSize_ = stepSize;
}

/// See ECPathMover::stepSize().
double ECMoveByKeyboardFourDirectional::stepSize()
{
    return stepSize_;
}

void ECMoveByKeyboardFourDirectional::moveStep_()
{
    // if the entity has been destroyed, stop
    if (entity_.isNull()){
        moveTimer_->disconnect();
        return;
    }

    // if currently not in a Map, do nothing
    Map* entitysMap = entity_->map();
    if (entitysMap == nullptr)
        return;

    // if entitysMap is not in a Game, do noting
    Game* entitysGame = entitysMap->game();
    if (entitysGame == nullptr)
        return;

    // temporarly disable pathingmap (will be automatically reenabled when moved)
    QPointF entitysPos = entity_->pointPos();

    std::vector<QRectF> entitysCellsAsRects = entity_->pathingMap().cellsAsRects();
    std::vector<QRectF> entitysFilledCellsAsRects;
    for (QRectF rect:entitysCellsAsRects){
        if (entity_->pathingMap().filled(rect)){
            // shift it and add it to filled collection
            rect.moveTopLeft(QPointF(entitysPos.x() + rect.x(), entitysPos.y() + rect.y()));
            entitysFilledCellsAsRects.push_back(rect);
        }
    }

    for (QRectF rect:entitysFilledCellsAsRects){
        std::vector<Node> intersectedCells = entitysMap->pathingMap().cells(rect);
        for (Node cell:intersectedCells){
            entitysMap->pathingMap().unfill(cell);
        }
    }

    // find out which keys are pressed during this step
    bool wPressed = entitysGame->keysPressed().count(Qt::Key_W);
    bool sPressed = entitysGame->keysPressed().count(Qt::Key_S);
    bool aPressed = entitysGame->keysPressed().count(Qt::Key_A);
    bool dPressed = entitysGame->keysPressed().count(Qt::Key_D);

    // move up if W is pressed
    if (wPressed){
        // find newPt to move to
        double newX = entity_->pointPos().x();
        double newY = entity_->pointPos().y() - stepSize_;
        QPointF newPt(newX,newY);

        // move if the new location is free
        if (entity_->canFit(newPt)){
            entity_->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walkUp")
                    && entity_->sprite()->hasAnimation("walkUp")){
                entity_->sprite()->play("walkUp",-1,100);
            }
        }
        return;
    }

    // move down if S is pressed
    if (sPressed){
        double newX = entity_->pointPos().x();
        double newY = entity_->pointPos().y() + stepSize_;
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walkDown")
                    && entity_->sprite()->hasAnimation("walkDown")){
                entity_->sprite()->play("walkDown",-1,100);
            }
        }
        return;
    }

    // move left if A is pressed
    if (aPressed){
        double newX = entity_->pointPos().x() - stepSize_;
        double newY = entity_->pointPos().y();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walkLeft")
                    && entity_->sprite()->hasAnimation("walkLeft")){
                entity_->sprite()->play("walkLeft",-1,100);
            }
        }
        return;
    }

    // move right if D is pressed
    if (dPressed){
        double newX = entity_->pointPos().x() + stepSize_;
        double newY = entity_->pointPos().y();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walkRight")
                    && entity_->sprite()->hasAnimation("walkRight")){
                entity_->sprite()->play("walkRight",-1,100);
            }
        }
        return;
    }

    // if none of the keys are pressed, play stand animation at currently facing direction
    if (!wPressed && !aPressed && !sPressed && !dPressed){
        const static double BUFFER = 10;
        double facingAngle = entity_->facingAngle();

        // right stand anim
        if ( facingAngle > 360 - BUFFER && facingAngle < 0 + BUFFER ){
            if (entity_->sprite()->playingAnimation() != std::string("standRight")
                    && entity_->sprite()->hasAnimation("standRight")){
                entity_->sprite()->play("standRight",-1,100);
            }
        }

        // left stand anim
        if (facingAngle > 180 - BUFFER && facingAngle < 180 + BUFFER){
            if (entity_->sprite()->playingAnimation() != std::string("standLeft")
                    && entity_->sprite()->hasAnimation("standLeft")){
                entity_->sprite()->play("standLeft",-1,100);
            }
        }

        // up stand anim
        if (facingAngle >  270 - BUFFER && facingAngle < 270 + BUFFER){
            if (entity_->sprite()->playingAnimation() != std::string("standUp")
                    && entity_->sprite()->hasAnimation("standUp")){
                entity_->sprite()->play("standUp",-1,100);
            }
        }

        // down stand anim
        if (facingAngle >  90 - BUFFER && facingAngle < 90 + BUFFER){
            if (entity_->sprite()->playingAnimation() != std::string("standDown")
                    && entity_->sprite()->hasAnimation("standDown")){
                entity_->sprite()->play("standDown",-1,100);
            }
        }

    }
}
