#pragma once
#include "State.h"
#include <functional>
#include "PainterManager.h"
#include "GameConfig.h"
#include "Pool.h"
#include "NumberManager.h"
#include "Meteorite.h"
#include <array>

class Plane;
class Bullet;
class AlphaManager;

class BattleState: public State
{
    public:
        
        BattleState(Plane *player, PainterManager *painter, Pool<Plane, PLANES_POOL_SIZE> *enemiesPool,
                         Pool<Bullet, BULLETS_POOL_SIZE> *bulletsPool, 
                         std::function<void()> damagePlayerCallback, 
                         std::function<void(float x, float y)> damageEnemy,
                         long long* score, float* time, 
                         NumberManager* numberManager, AlphaManager* alphaManager);
        
    public:    
        STATES Update(const float deltaTime, float currentFrameInputValueNormalized,
	                        int currentFrameInputValue)override;
        void Paint()override;
        void OnEnter()override;
        void OnExit()override;

    private:
        void UpdateBullets(float deltaTime);
        void UpdateEnemies(float deltaTime);
        void UpdateMeteorites(float deltaTime);
        void ManageBulletCollisions(Bullet& bullet);
        bool ManageCollisionBetweenBulletAndEnemy(Bullet& bullet, Plane& enemy);
	    bool HasCollision(const Bullet& bullet, Plane* plane) const;
        bool HasCollision(const Bullet& bullet, const Meteorite& meteorite) const;
	    bool CollsisionDetection(float ax, float ay, float aw, float ah,
							 float bx, float by, float bw, float bh) const;
        void DamagePlayer();
        void DoExplosion(Bullet& bullet);

        void ReturnEnemy(Plane& plane);

    private:
        Pool<Plane, PLANES_POOL_SIZE>* _enemiesPool;
        Pool<Bullet, BULLETS_POOL_SIZE>* _bulletsPool;
        std::function<void()> _damagePlayerCallback;
        std::function<void(float x, float y)> _damageEnemyCallback;
        long long* _score;
        float* _timeLeft;
        NumberManager* _numberManager;
        std::array<Meteorite, TOTAL_METEORITES> _meteorites;
        AlphaManager* _alphaManager;
        int _enemiesAlive;
};