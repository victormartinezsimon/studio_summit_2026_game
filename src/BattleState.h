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
        void UpdatePlayer(float deltaTime);
        void UpdateBullets(float deltaTime);
        void UpdateEnemies(float deltaTime);
        void UpdateMeteorites(float deltaTime);
    
    private:
        void ManageBulletCollisions(Bullet& bullet);
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
        bool TryDestroyBullet(const Bullet& bullet);

    private:
        void ConfigureMeteoriteSpawn(Meteorite& meteorite);

    private:
        Pool<Plane, PLANES_POOL_SIZE>* _enemiesPool;
        Pool<Bullet, BULLETS_POOL_SIZE>* _bulletsPool;
        std::function<void()> _damagePlayerCallback;
        std::function<void(float x, float y)> _damageEnemyCallback;
        long long* _score;
        float* _timeLeft;
        NumberManager* _numberManager;
        AlphaManager* _alphaManager;
        int _enemiesAlive;
       // Pool<Meteorite, TOTAL_METEORITES> _meteoritesPool;
        Pool<Explosion, TOTAL_EXPLOSIONS> _explosionPool;

        Spawner<Meteorite, TOTAL_METEORITES> _spawnerMeteorites;
        std::mt19937 _generator;
};