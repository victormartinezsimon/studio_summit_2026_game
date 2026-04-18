#include "BattleState.h"
#include "PainterManager.h"
#include "Plane.h"
#include "GameConfig.h"
#include "Bullet.h"
#include "EasingManager.h"
#include "RandomManager.h"
#include "TrailManager.h"

BattleState::BattleState(
    Plane *player, PainterManager *painter,
    NumberManager *numberManager,
    EasingManager *easingManager, RandomManager *randomManager, ButtonA *buttonAManager,
    Pool<Plane, PLANES_POOL_SIZE> *enemiesPool,
    Pool<Bullet, BULLETS_POOL_SIZE> *bulletsPool,
    std::function<void()> damagePlayerCallback,
    std::function<void(float x, float y)> damageEnemy,
    long long *score, float *time, 
    Spawner<Meteorite, TOTAL_METEORITES> *spawnerMeteorites,
    TrailManager* trailManager)
    : State(player, painter, numberManager,
            easingManager, randomManager, buttonAManager),
      _enemiesPool(enemiesPool), _bulletsPool(bulletsPool),
      _damagePlayerCallback(damagePlayerCallback), _damageEnemyCallback(damageEnemy),
      _score(score), _timeLeft(time), _spawnerMeteorites(spawnerMeteorites),_trailManager(trailManager)
{
    _countDownNumbers.Configure(_painterManager, PainterManager::SPRITE_ID::NUMBERS_BIG, 5, 2, -1);
}

State::STATES BattleState::Update(const float deltaTime, float currentFrameInputValueNormalized)
{
    UpdateExplosions(deltaTime);

    UpdatePlayer(deltaTime);

    UpdateBullets(deltaTime);

    UpdateEnemies(deltaTime);

    if ((_enemiesDeathAnimation.TotalInUse() + _enemiesPool->TotalInUse() )== 0 || _enemiesAlive == 0)
    {
        return STATES::IMPROVEMENT_SELECTOR;
    }

    return STATES::BATTLE;
}
void BattleState::Paint()
{
    _player->Paint(_painterManager);
    _bulletsPool->Paint(_painterManager);
    _enemiesPool->Paint(_painterManager);
    _explosionPool.Paint(_painterManager);
}

void BattleState::PaintUI()
{
    {
        long long value = *_score;
        _numberManager->PaintNumber(value, SCORE_POSITION_X, SCORE_POSITION_Y, 3, NumberManager::PIVOT::RIGHT);
    }

    {
        int value = std::round(MAX_SECS_PLAYING - *_timeLeft);
        _numberManager->PaintNumber(value, TIME_POSITION_X, SCORE_POSITION_Y, 3, NumberManager::PIVOT::LEFT);
        
        if(value < 10 && SHOW_FINAL_COUNTDOWN)
        {
            _countDownNumbers.PaintFrame(_painterManager, SCREEN_WIDTH*0.5, SCREEN_HEIGHT*0.5, value, 0.5f);
        }
    }

    _enemiesDeathAnimation.Paint(_painterManager);
}

void BattleState::OnEnter()
{
    _explosionPool.ReturnAll();

    _player->SetPositionY(POSITION_Y_PLAYER);
    _player->ConfigureSprite(_painterManager);

    _enemiesAlive = _enemiesPool->TotalInUse();

    if (_currentLevel >= MIN_LEVEL_WITH_MOVEMENT)
    {
        _enemiesPool->for_each_active([this](Plane &p)
                                      { ConfigureRandomMovement(p); });
    }
}
void BattleState::OnExit()
{
    _bulletsPool->ReturnAll();
    _enemiesPool->ReturnAll();
    _explosionPool.ReturnAll();
    _player->SetTimeInmortal(0);
	_player->SetAlpha(1);
	_player->SetHasShield(false);//remove because I dont want shield in other screens
}

void BattleState::UpdateExplosions(float deltaTime)
{
    _explosionPool.for_each_active([&](Explosion& exp)
    {
        exp.Update(deltaTime);
    });
}


void BattleState::UpdatePlayer(float deltaTime)
{
    _player->Update(deltaTime);

    bool someCollision = ManagePlaneCollisions(*_player);
    if (someCollision)
    {
        DamagePlayer(); 
    }
}

void BattleState::UpdateEnemies(float deltaTime)
{

    _enemiesPool->for_each_active([deltaTime](Plane &enemy)
                                  { enemy.Update(deltaTime); });

    // check collision
    _enemiesPool->for_each_active([&](Plane &enemy)
                                  { 
        bool someCollision = ManagePlaneCollisions(enemy);
        if(someCollision)
        {
            ReturnEnemy(enemy);
        } });
}

void BattleState::UpdateBullets(float deltaTime)
{
    _bulletsPool->for_each_active([this, deltaTime](Bullet &bullet)
                                  {
                                    bullet.Update(deltaTime);
                                    bool isDestroyed = UpdateBullet(deltaTime, bullet);
                                    if(!isDestroyed && SHOW_TRAIL_BULLETS)
                                    {
                                        _trailManager->AddTrail(_painterManager, bullet.GetX(), bullet.GetY(), 
                                        bullet.GetWidth(), bullet.GetHeight(),
                                        TRAIL_LIVE_BULLETS, 
                                        PainterManager::SPRITE_ID::BULLET,
                                        PainterManager::SPRITE_ID::BULLET_SMALL);
                                    }
                                  });
}

bool BattleState::UpdateBullet(float deltaTime, Bullet &bullet)
{
    bool isDestroyed = ManageBulletCollisions(bullet);
    if (isDestroyed)
    {
        DoExplosion(bullet);
        return true;
    }

    // Enemy
    if (bullet.GetPlayerTeam() == TEAM_PLAYER)
    {
        _enemiesPool->for_each_active([&](Plane &enemy)
                                      {
                                            if(isDestroyed){return;}
                                            isDestroyed = ManageBulletPlaneCollision(bullet, enemy);
                                            if(isDestroyed)
                                            {
                                                ReturnEnemy(enemy); 
                                            }
                                      });
    }
    if (isDestroyed)
    {
        DoExplosion(bullet);
        return true;
    }

    // PLAYER
    if (bullet.GetPlayerTeam() == TEAM_ENEMY)
    {
        isDestroyed = ManageBulletPlaneCollision(bullet, *_player);
        if(isDestroyed)
        {
            DamagePlayer();
        }
    }
    if (isDestroyed)
    {
        DoExplosion(bullet);
    }

    return isDestroyed;
}
bool BattleState::ManagePlaneCollisions(Plane &plane)
{
    bool someCollision = false;

    // check explosion
    _explosionPool.for_each_active([&](Explosion &explosion)
                                   {
            if(someCollision){return;}
            if(explosion.GetPlayerTeam() == plane.GetPlayerTeam()){return;}
            bool explosionCollision = ManageExplosionPlaneCollision(explosion, plane);
            bool hasShield = plane.GetHasShield();

            if(explosionCollision)
            {
                if(hasShield)
                {
                    plane.SetHasShield(false);
                }
                else
                {
                    someCollision = true;
                }
            } }
        );

    return someCollision;
}

bool BattleState::ManageBulletCollisions(Bullet &bullet)
{
    bool isDestroyed = false;
    // check out of screen
    if (bullet.GetY() < 0 || bullet.GetY() > SCREEN_HEIGHT)
    {
        _bulletsPool->Release(bullet);
        isDestroyed = true;
    }

    // check meteorites
    if (!isDestroyed)
    {
        _spawnerMeteorites->for_each_active([&](Meteorite &meteorite)
                                            {
                if(isDestroyed){return;}
                isDestroyed = ManageMeteoriteBulletCollision(meteorite, bullet); });
    }

    return isDestroyed;
}

bool BattleState::ManageBulletPlaneCollision(const Bullet &bullet, Plane &plane)
{
    bool hasShield = plane.GetHasShield();
    bool hasCollision = HasCollision(bullet, plane);

    if (hasCollision && hasShield)
    {
        plane.SetHasShield(false);
        TryDestroyBullet(bullet, false);
        return false;
    }

    if (hasCollision)
    {
        TryDestroyBullet(bullet, false);
    }
    return hasCollision;
}

bool BattleState::ManageExplosionPlaneCollision(const Explosion &explosion, const Plane &plane)
{
    bool hasCollision = CollisionDetection(explosion.GetX(), explosion.GetY(), explosion.GetWidth(), explosion.GetHeight(),
                                            plane.GetX(), plane.GetY(), plane.GetWidth(), plane.GetHeight());
    return hasCollision;
}

bool BattleState::ManageMeteoriteBulletCollision(const Meteorite &meteorite, const Bullet &bullet)
{
    if (HasCollision(bullet, meteorite))
    {
        return TryDestroyBullet(bullet, true);
    }
    return false;
}

bool BattleState::TryDestroyBullet(const Bullet &bullet, bool isAsteroid)
{
    if (isAsteroid && bullet.GetHasPenetration())
    {
        return false;
    }

    _bulletsPool->Release(bullet);
    return true;
}

bool BattleState::HasCollision(const Bullet &bullet, const Plane &plane) const
{
    if (bullet.GetPlayerTeam() == plane.GetPlayerTeam())
    {
        return false;
    }

    return CollisionDetection(bullet.GetX(), bullet.GetY(), bullet.GetWidth(), bullet.GetHeight(),
                               plane.GetX(), plane.GetY(), plane.GetWidth(), plane.GetHeight());
}

bool BattleState::HasCollision(const Bullet &bullet, const Meteorite &meteorite) const
{
    return CollisionDetection(bullet.GetX(), bullet.GetY(), bullet.GetWidth(), bullet.GetHeight(),
                               meteorite.GetX(), meteorite.GetY(), meteorite.GetWidth(), meteorite.GetHeight());
}

bool BattleState::CollisionDetection(float ax, float ay, float aw, float ah,
                                      float bx, float by, float bw, float bh) const
{
    return (std::abs(ax - bx) < (aw + bw) / 2.0f &&
            std::abs(ay - by) < (ah + bh) / 2.0f);
}

void BattleState::DamagePlayer()
{
    if (_player->GetTimeInmortal() > 0)
    {
        return;
    }

    _damagePlayerCallback();
    _player->SetTimeInmortal(TIME_INMORTAL);
}

void BattleState::ReturnEnemy(Plane &enemy)
{
    _easingManager->KillEase(enemy.GetRandomMovementID());

    float enemyX = enemy.GetX();
    float enemyY = enemy.GetY();

    int idAnimDeath = _enemiesDeathAnimation.Get();
    _enemiesDeathAnimation.call_for_element(idAnimDeath, [&](WorldObject& obj)
		{
			SpriteSheetController* sprite = obj.GetSpriteController();
			sprite->Configure(_painterManager, enemy.GetSpriteController());
		});
		
    int idEase = _easingManager->AddEase(DURATION_EASING_SCORE, enemyX, enemyY, enemyX, enemyY, Ease::EASE_TYPES::LINEAL, 
                [&](bool forced, int idNum)
                {
                    _enemiesDeathAnimation.Release(idNum);
                    --_enemiesAlive;
                },
                [&](float x, float y, Ease& ease, float percent)
                {
                    int numberAnimID = ease.GetReferenceID();
                    _enemiesDeathAnimation.call_for_element(numberAnimID, [&](WorldObject& obj){
                        obj.SetAlpha(1- percent);
                        obj.SetPosition(x, y);
                    });
                });
    _easingManager->SetReferenceIDToEase(idEase, idAnimDeath);

    _enemiesPool->Release(enemy);

    _damageEnemyCallback(enemyX, enemyY);
}

void BattleState::DoExplosion(const Bullet &bullet)
{
    if (!bullet.GetHasExplostion())
    {
        return;
    }

    int id = _explosionPool.Get();
    if (id != -1)
    {
        _explosionPool.call_for_element(id, [&](Explosion &exp)
                                        { ConfigureExplosion(id, exp, bullet); });
    }
}

void BattleState::EndExplosion(Explosion &exp)
{
    _explosionPool.Release(exp);
}
void BattleState::ConfigureExplosion(const int id, Explosion &exp, const Bullet &bullet)
{
    float x = bullet.GetX();
    float y = bullet.GetY();

    if (y <= 0)
    {
        y = 0;
    }
    if (y >= SCREEN_HEIGHT)
    {
        y = SCREEN_HEIGHT;
    }
    
    exp.SetPosition(x, y);
    exp.SetID(id);
    exp.ConfigureSprite(_painterManager);
    exp.SetPlayerTeam(bullet.GetPlayerTeam());

    exp.SetCallbackEnd([&](Explosion& exp)
    {
        EndExplosion(exp);
    });

}

void BattleState::ConfigureRandomMovement(Plane &plane)
{
    float minX;
    float maxX;
    GetMinMaxXPosiblePositionForEnemies(minX, maxX);

    float minY = SCREEN_HEIGHT * MIN_Y_ENEMY;
    float maxY = SCREEN_HEIGHT * MAX_Y_ENEMY;

    float nextX = _randomManager->GetValue(minX, maxX, 100.0f);
    float nextY = _randomManager->GetValue(minY, maxY, 100.0f);

    int movementType = _randomManager->GetValue(0, 3);

    Ease::EASE_TYPES easeType = Ease::EASE_TYPES::INOUTQUINT;

    switch (movementType)
    {
    case 0:
        easeType = Ease::EASE_TYPES::INOUTSINE;
        break;
    case 1:
        easeType = Ease::EASE_TYPES::INOUTCUBE;
        break;
    case 2:
        easeType = Ease::EASE_TYPES::INOUTQUINT;
        break;
    case 3:
        easeType = Ease::EASE_TYPES::INOUTCIRC;
        break;
    }

    float duration = _randomManager->GetValue(MIN_DURATION_MOVEMENT_ENEMY, MAX_DURATION_MOVEMENT_ENEMY, 100.0f);

    int easeID = _easingManager->AddEase(duration, plane.GetX(), plane.GetY(), nextX, nextY, easeType, 
            [&](bool forced, int noUsed) { if(!forced){ConfigureRandomMovement(plane);} }, 
            [&plane](float x, float y, Ease &ease, float percent) { plane.SetPosition(x, y); });
    plane.SetRandomMovementID(easeID);
}

void BattleState::GetMinMaxXPosiblePositionForEnemies(float &minX, float &maxX) const
{
    minX = 0 + ENEMY_WIDTH / 2;
    maxX = SCREEN_WIDTH - ENEMY_WIDTH / 2;
}

void BattleState::SetCurrentLevel(int value)
{
    _currentLevel = value;
}
