#pragma once
#include "State.h"
#include <functional>
#include "PainterManager.h"
#include "GameConfig.h"
#include "Pool.h"
#include "NumberManager.h"
#include "Meteorite.h"
#include <array>
#include "Explosion.h"
#include "Spawner.h"
#include <random>

class Plane;
class Bullet;
class AlphaManager;
class EasingManager;
class RandomManager;
class TrailManager;

class BattleState: public State
{
    public:
        
        BattleState(Plane *player, PainterManager *painter, 
        NumberManager* numberManager, AlphaManager* alphaManager,
        EasingManager* easingManager, RandomManager* randomManager, ButtonA* buttonAManager,
        Pool<Plane, PLANES_POOL_SIZE> *enemiesPool,
        Pool<Bullet, BULLETS_POOL_SIZE> *bulletsPool, 
        std::function<void()> damagePlayerCallback, 
        std::function<void(float x, float y)> damageEnemy,
        long long* score, float* time,
        Spawner<Meteorite, TOTAL_METEORITES>* spawnerMeteorites,
        TrailManager* trailManager
        );
        
    public:    
        STATES Update(const float deltaTime, float currentFrameInputValueNormalized)override;
        void Paint()override;
        void PaintUI()override;
        void OnEnter()override;
        void OnExit()override;

    private:
        void UpdatePlayer(float deltaTime);
        void UpdateBullets(float deltaTime);
        void UpdateEnemies(float deltaTime);
        bool UpdateBullet(float deltaTime, Bullet& bullet);
    
    private:
        bool ManageBulletCollisions(Bullet& bullet);
        bool ManagePlaneCollisions(Plane& plane);

        bool ManageBulletPlaneCollision(const Bullet& bullet, Plane& plane);
        bool ManageExplosionPlaneCollision(const Explosion& explosion, const Plane& plane);
        bool ManageMeteoriteBulletCollision(const Meteorite& meteorite, const Bullet& plane);
    
    private:
	    bool HasCollision(const Bullet& bullet, const Plane& plane) const;
        bool HasCollision(const Bullet& bullet, const Meteorite& meteorite) const;
	    bool CollsisionDetection(float ax, float ay, float aw, float ah,
							 float bx, float by, float bw, float bh) const;
        
    private:    
        void DamagePlayer();
        void ReturnEnemy(Plane& plane);
        void DoExplosion(const Bullet& bullet);

    private:
        void EndExplosion(Explosion& exp);
        void ConfigureExplosion(const int id, Explosion& exp, const Bullet& bullet);
        bool TryDestroyBullet(const Bullet& bullet, bool isAsteroid);

    private:
        void ConfigureRandomMovement(Plane& plane);

    private:
        void GetMinMaxXPosiblePositionForEnemies(float &minX, float &maxX) const;

    public:
        void SetCurrentLevel(int value);
 
    private:
        Pool<Plane, PLANES_POOL_SIZE>* _enemiesPool;
        Pool<Bullet, BULLETS_POOL_SIZE>* _bulletsPool;
        std::function<void()> _damagePlayerCallback;
        std::function<void(float x, float y)> _damageEnemyCallback;
        long long* _score;
        float* _timeLeft;
        int _enemiesAlive;
        Pool<Explosion, TOTAL_EXPLOSIONS> _explosionPool;
        int _currentLevel;
        Spawner<Meteorite, TOTAL_METEORITES>* _spawnerMeteorites;
        TrailManager* _trailManager;
};