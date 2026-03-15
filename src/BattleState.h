#pragma once
#include "State.h"
#include <functional>
#include "PainterManager.h"
#include "GameConfig.h"
#include "Pool.h"

class Plane;
class Bullet;

class BattleState: public State
{
    public:
        
        BattleState(Plane *player, PainterManager *painter, Pool<Plane, PLANES_POOL_SIZE> *enemiesPool,
                         Pool<Bullet, BULLETS_POOL_SIZE> *bulletsPool, std::function<void()> damagePlayerCallback);
        
    public:    
        STATES Update(const float deltaTime, float currentFrameInputValueNormalized,
	                        int currentFrameInputValue)override;
        void Paint()override;
        void OnEnter()override;
        void OnExit()override;

    private:
        void UpdateBullets(float deltaTime);
        void UpdateEnemies(float deltaTime);
        void ManageBulletCollisions(Bullet& bullet);
        bool ManageCollisionBetweenBulletAndEnemy(Bullet& bullet, Plane& enemy);
	    bool HasCollision(const Bullet& bullet, Plane* plane) const;
	    bool CollsisionDetection(float ax, float ay, float aw, float ah,
							 float bx, float by, float bw, float bh) const;
        void DamagePlayer();
        void DoExplosion(Bullet& bullet);

    private:
        Pool<Plane, PLANES_POOL_SIZE>* _enemiesPool;
        Pool<Bullet, BULLETS_POOL_SIZE>* _bulletsPool;
        std::function<void()> _damagePlayerCallback;
};