#include "Axe.h"
#include "Sprite.h"
#include "Map.h"
#include <QGraphicsScene>
#include <QDebug> // TODO: remove
#include "Inventory.h"

Axe::Axe()
{
    // defaults swing parameters
    swingAngleEachStep_ = 5;
    swingFrequency_ = 19;
    alreadySwinging_ = false;
    maxDrawBackSteps_ = 9;
    maxDrawForwardSteps_ = 9;
    maxForwardSteps_ = 7;
    maxBackwardSteps_ = 7;

    // default sprite
    spr_ = new Sprite();
    pm_ = QPixmap(":resources/graphics/weapons/axe.png");
    spr_->setPixmap(pm_);
    setSprite(spr_);

    // default tip (length and width/2)
    resetTip();

    // default attachment point
    QPointF pt;
    pt.setX(pm_.width()/3);
    pt.setY(pm_.height()/2);
    setAttachmentPoint(pt);

    // default cast range
    setCastRange(100);

    // default damge
    damage_ = 5;

    timer_ = new QTimer(this);
}

/// The axe will start swinging. The "position" argument is ignored.
void Axe::attack(QPointF position)
{
    // if its already swinging, don't do anything
    if (alreadySwinging_){
        return;
    }

    // set initial variables
    currentDrawBackStep_ = 0;
    currentDrawForwardStep_ = 0;
    currentForwardStep_ = 0;
    currentBackwardStep_ = 0;
    currentStepToGoingBackToNeutral_ = 0;
    hitSomethingComingBackFromDraw_ = false;
    hitSomethingDuringForwardStep_ = false;
    alreadySwinging_ = true;

    // start swinging
    connect(timer_,SIGNAL(timeout()),this,SLOT(swingStep()));
    timer_->start(swingFrequency_);
}

void Axe::swingStep()
{
    // if we have hit something going forward
    if (hitSomethingDuringForwardStep_){
        setFacingAngle(facingAngle()-swingAngleEachStep_);
        currentStepToGoingBackToNeutral_++;
        if (currentStepToGoingBackToNeutral_ >= stepsToGoBackwardToNeutral_){
            timer_->disconnect();
            alreadySwinging_ = false;
            return;
        }
        return;
    }

    // if we have hit something coming back from draw
    if (hitSomethingComingBackFromDraw_){
        // keep moving towards neutral position (w/o damaging) and rest there
        setFacingAngle(facingAngle()+swingAngleEachStep_);
        currentDrawForwardStep_++;
        if (currentDrawForwardStep_ >= maxDrawForwardSteps_){
            timer_->disconnect();
            alreadySwinging_ = false;
        }
        return;
    }

    // if initially drawing
    if (currentDrawBackStep_ < maxDrawBackSteps_){
        setFacingAngle(facingAngle()-swingAngleEachStep_);
        currentDrawBackStep_++;
        return;
    }

    // if coming back from draw
    if (currentDrawForwardStep_ < maxDrawForwardSteps_){
        setFacingAngle(facingAngle()+swingAngleEachStep_);
        currentDrawForwardStep_++;

        // if hit something
        Entity* theOwner = inventory()->entity();
        std::unordered_set<Entity*> collidingEntities = map()->entities(mapToMap(tip()));
        for (Entity* e: collidingEntities){
            if (e != this && e != theOwner && e->parent() != theOwner){
                damage(e,damage_);
                hitSomethingComingBackFromDraw_ = true;
                return;
            }
        }
        return;
    }

    // if forward step
    if (currentForwardStep_ < maxForwardSteps_){
        setFacingAngle(facingAngle()+swingAngleEachStep_);
        currentForwardStep_++;

        // if hit something
        Entity* theOwner = inventory()->entity();
        std::unordered_set<Entity*> collidingEntities = map()->entities(mapToMap(tip()));
        for (Entity* e: collidingEntities){
            if (e != this && e != theOwner && e->parent() != theOwner){
                damage(e,damage_);
                hitSomethingDuringForwardStep_ = true;
                stepsToGoBackwardToNeutral_ = currentForwardStep_;
                return;
            }
        }
        return;
    }

    // if backward step
    if (currentBackwardStep_ < maxBackwardSteps_){
        setFacingAngle(facingAngle()-swingAngleEachStep_);
        currentBackwardStep_++;

        // if last backward step, stop
        if (currentBackwardStep_ >= maxBackwardSteps_){
            timer_->disconnect();
            alreadySwinging_ = false;
        }
        return;
    }
}