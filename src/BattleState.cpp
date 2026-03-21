#include "BattleState.h"
#include "PainterManager.h"
#include "Plane.h"
#include "GameConfig.h"
#include "Sprites.h"
#include "Bullet.h"
#include "AlphaManager.h"
#include "EasingManager.h"

BattleState::BattleState(Plane *player, PainterManager *painter, Pool<Plane, PLANES_POOL_SIZE> *enemiesPool,
                         Pool<Bullet, BULLETS_POOL_SIZE> *bulletsPool, 
                         std::function<void()> damagePlayerCallback, 
                         std::function<void(float x, float y)> damageEnemy,
                         long long* score, float* time, 
                         NumberManager* numberManager, AlphaManager* alphaManager, EasingManager* easingManager) 
                         : State(player, painter), _enemiesPool(enemiesPool), _bulletsPool(bulletsPool),
                         _damagePlayerCallback(damagePlayerCallback), _damageEnemyCallback(damageEnemy),
                         _score(score), _timeLeft(time), _numberManager(numberManager),_alphaManager(alphaManager),
                         _generator(std::random_device{}()), _spawnerMeteorites(TIME_SPAWN_METEORITE, painter), _easingManager(easingManager)
{
    _spawnerMeteorites.SetCallbackConfiguration([this](Meteorite& m){ConfigureMeteoriteSpawn(m);});
}

State::STATES BattleState::Update(const float deltaTime, float currentFrameInputValueNormalized, int currentFrameInputValue)
{
    // update player
    UpdatePlayer(deltaTime);

    UpdateMeteorites(deltaTime);

    UpdateBullets(deltaTime);

    UpdateEnemies(deltaTime);

    if (_enemiesAlive == 0)
    {
        return STATES::IMPROVEMENT_SELECTOR;
    }

    return STATES::BATTLE;
}
void BattleState::Paint()
{

    {
        float playerX, playerY;
        float currentTimeInmortal = _player->GetTimeInmortal();
        if( currentTimeInmortal <= 0)
        {
            _player->GetPaintPosition(playerX, playerY);
            _painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER, _player->GetWidth(), _player->GetHeight(), playerX, playerY);
        }
        else
        {
            float playerX, playerY;
            _player->GetPaintPosition(playerX, playerY);
            float percent = currentTimeInmortal / TIME_INMORTAL;

            int mask = 0;
            if((percent >= 0.25 && percent <= 0.50) || (percent >= 0.75 && percent <= 1.0))
            {
                mask = 1;
            }

            _painterManager->AddToPaintWithAlpha(PainterManager::SPRITE_ID::PLAYER, 
                _player->GetWidth(), _player->GetHeight(), playerX, playerY, mask);
        }

        if(_player->GetHasShield())
        {
            _painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER_SHIELD,
                SHIELD_PLAYER_WIDTH,SHIELD_PLAYER_HEIGHT, playerX, playerY );
        }
    }

    {
        _spawnerMeteorites.Paint();
    }
    
    {
        _bulletsPool->for_each_active([&](const Bullet &bullet)
                                      {
            float posX, posY;
            bullet.GetPaintPosition(posX, posY);
            _painterManager->AddToPaint(PainterManager::SPRITE_ID::BULLET, 
                bullet.GetWidth(), bullet.GetHeight(), posX, posY); });
    }

    {
        _enemiesPool->for_each_active([this](const Plane &p)
            {
                float posX, posY;
                p.GetPaintPosition(posX, posY);
                _painterManager->AddToPaint(PainterManager::SPRITE_ID::ENEMY, 
                    p.GetWidth(), p.GetHeight(), posX, posY);

                if(p.GetHasShield())
                {
                    _painterManager->AddToPaint(PainterManager::SPRITE_ID::ENEMY_SHIELD,
                        SHIELD_ENEMY_WIDTH,SHIELD_ENEMY_HEIGHT, posX, posY );
                }
             }
        );
    }

    {
        long long value = *_score;
        _numberManager->PaintNumber(value, SCREEN_WIDTH, NUMBER_0_HEIGHT, 3, NumberManager::PIVOT::RIGHT);
    }

    {
        float value = *_timeLeft;
        _numberManager->PaintNumber(MAX_SECS_PLAYING - value,0, NUMBER_0_HEIGHT, 3, NumberManager::PIVOT::LEFT);
    }
}

void BattleState::OnEnter()
{
    _player->SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
    _player->SetPositionY( POSITION_Y_PLAYER);

    _enemiesAlive = _enemiesPool->TotalInUse();
    _spawnerMeteorites.Reset();

    _destinyManager.Reset();
    _enemiesPool->for_each_active([this](Plane& p)
    {
        _destinyManager.AddPosition(p.GetX(), p.GetY());
    });

    _enemiesPool->for_each_active([this](Plane& p)
    {
        ConfigureRandomMovement(p);
    });
}
void BattleState::OnExit()
{
    _bulletsPool->ReturnAll();
    _enemiesPool->ReturnAll();
    _explosionPool.ReturnAll();
}

void BattleState::UpdatePlayer(float deltaTime)
{
    _player->Update(deltaTime);

    bool someCollision = ManagePlaneCollisions(*_player);
    if(someCollision)
    {
        DamagePlayer();
    }
}

void BattleState::UpdateBullets(float deltaTime)
{
    _bulletsPool->for_each_active([deltaTime](Bullet &bullet)
                                  { bullet.Update(deltaTime); });

    _bulletsPool->for_each_active([this, deltaTime](Bullet &bullet)
                                  { ManageBulletCollisions(bullet); });
                                  
}
void BattleState::UpdateEnemies(float deltaTime)
{
    /*
    _enemiesPool->for_each_active([deltaTime](Plane &enemy)
                                  { enemy.Update(deltaTime); });

    */
    //check collision
    _enemiesPool->for_each_active([&](Plane &enemy)
    { 
        bool someCollision = ManagePlaneCollisions(enemy);
        if(someCollision)
        {
            ReturnEnemy(enemy);
        }
        
    });

}

 bool BattleState::ManagePlaneCollisions(Plane& plane)
 {
    bool someCollision = false;

    //check explosion
     _explosionPool.for_each_active([&] (Explosion& explosion)
        {
            if(someCollision){return;}
            bool explosionCollision = ManageExplosionPlaneCollision(explosion, plane);
            if(explosionCollision)
            {
                someCollision = true;
            }
        }
    );

    //check explosion
    _bulletsPool->for_each_active([this, &someCollision, &plane](Bullet &bullet)
    {
        if(someCollision){return;}
        bool bulletCollision = ManageBulletPlaneCollision(bullet, plane);
        if(bulletCollision)
        {
            someCollision = true;
        }
    });

    return someCollision;
 }

void BattleState::ManageBulletCollisions(Bullet &bullet)
{
    bool isDestroyed = false;
    // check out of screen
    if (bullet.GetY() < 0 || bullet.GetY() > SCREEN_HEIGHT)
    {
        _bulletsPool->Release(bullet);
        isDestroyed = true;
    }

    //check meteorites
    if(!isDestroyed)
    {
        _spawnerMeteorites.for_each_active([&](Meteorite& meteorite)
            {
                if(isDestroyed){return;}
                isDestroyed = ManageMeteoriteBulletCollision(meteorite, bullet);
            }
        );
    }
    
    //manage explosion
    if (isDestroyed)
    {
        if (bullet.GetHasExplostion())
        {
            DoExplosion(bullet);
        }
    }
}

 bool BattleState::ManageBulletPlaneCollision(const Bullet& bullet, Plane& plane)
 {
    bool hasShield = plane.GetHasShield();
    bool hasCollision = HasCollision(bullet, plane);

    if(hasCollision && hasShield)
    {
        plane.SetHasShield(false);
        TryDestroyBullet(bullet);
        return false;
    }

    if( hasCollision )
    {
        bool bulletDestroyed = TryDestroyBullet(bullet);
        if (bulletDestroyed && bullet.GetHasExplostion())
        {
            DoExplosion(bullet);
        }
    }
    return hasCollision;
 }

bool BattleState::ManageExplosionPlaneCollision(const Explosion& explosion, const Plane& plane)
{
    bool hasCollision = CollsisionDetection(explosion.GetX(), explosion.GetY(), explosion.GetWidth(), explosion.GetHeight(),
                    plane.GetX(), plane.GetY(), plane.GetWidth(), plane.GetHeight());
    return hasCollision;
}

bool BattleState::ManageMeteoriteBulletCollision(const Meteorite& meteorite, const Bullet& bullet)
{
    if (HasCollision(bullet, meteorite))
    {
        return TryDestroyBullet(bullet);
    }
    return false;
}


bool BattleState::TryDestroyBullet(const Bullet& bullet)
{
    if (!bullet.GetHasPenetration())
    {
        _bulletsPool->Release(bullet);
        return true;
    }
    return false;
}


bool BattleState::HasCollision(const Bullet &bullet, const Plane& plane) const
{
    if (bullet.GetPlayerTeam() == plane.GetPlayerTeam())
    {
        return false;
    }
   
    return CollsisionDetection(bullet.GetX(), bullet.GetY(), bullet.GetWidth(), bullet.GetHeight(),
                               plane.GetX(), plane.GetY(), plane.GetWidth(), plane.GetHeight());
}

 bool BattleState::HasCollision(const Bullet& bullet, const Meteorite& meteorite) const
 {
    return CollsisionDetection(bullet.GetX(), bullet.GetY(), bullet.GetWidth(), bullet.GetHeight(),
                               meteorite.GetX(), meteorite.GetY(), meteorite.GetWidth(), meteorite.GetHeight());
 }

bool BattleState::CollsisionDetection(float ax, float ay, float aw, float ah,
                                      float bx, float by, float bw, float bh) const
{
    return (std::abs(ax - bx) < (aw + bw) / 2.0f &&
            std::abs(ay - by) < (ah + bh) / 2.0f);
}

void BattleState::DamagePlayer()
{
    if(_player->GetTimeInmortal() > 0){return;}
    
    _damagePlayerCallback();
    _player->SetTimeInmortal(TIME_INMORTAL);
}

void BattleState::UpdateMeteorites(float deltaTime)
{
    _spawnerMeteorites.Update(deltaTime);
}

void BattleState::ReturnEnemy(Plane& enemy)
{
    _enemiesPool->Release(enemy);

    _easingManager->KillEase(enemy.GetRandomMovementID());

    float x, y;
    enemy.GetPaintPosition(x, y);
    int id = _alphaManager->AddAlpha(ALPHA_TIME_DESTROY_PLANE, x, y,
        ENEMY_WIDTH, ENEMY_HEIGHT, PainterManager::SPRITE_ID::ENEMY);
    _alphaManager->AddCallback(id, [this]()
    {
        _enemiesAlive--;
    });
    _damageEnemyCallback(x, y);
}

void BattleState::DoExplosion(const Bullet &bullet)
{
    int id = _explosionPool.Get();
    _explosionPool.call_for_element(id, [&](Explosion& exp){ConfigureExplosion(id, exp, bullet);});
}

void BattleState::EndExplosion(Explosion& exp)
{
    _explosionPool.Release(exp);
}
void BattleState::ConfigureExplosion(const int id, Explosion& exp ,const Bullet& bullet)
{
    float x = bullet.GetX();
    float y = bullet.GetY();
    if(y <= 0)
    {
        y += EXPLOSION_HEIGHT/2;
    }
    if(y >= SCREEN_HEIGHT)
    {
        y -= EXPLOSION_HEIGHT/2;
    }
    exp.SetPosition(x, y);
    x -= EXPLOSION_WIDTH/2;
    y -= EXPLOSION_HEIGHT/2;

    exp.SetID(id);
    exp.SetSize(EXPLOSION_WIDTH, EXPLOSION_HEIGHT);
    exp.SetPlayerTeam(bullet.GetPlayerTeam());
    
    auto alphaID =_alphaManager->AddAlpha(EXPLOSION_DURATION, 
        x, 
        y, 
        EXPLOSION_WIDTH, EXPLOSION_HEIGHT, 
        PainterManager::SPRITE_ID::EXPLOSION);
    
    _alphaManager->AddCallback(alphaID, 
        [&]()
        {
            EndExplosion(exp);
        }
    );
}

void BattleState::ConfigureMeteoriteSpawn(Meteorite& meteorite)
{
    meteorite.SetSize(METERORITE_WIDTH, METERORITE_HEIGHT);

    bool goingLeft = _generator()% 2;   

    std::uniform_real_distribution<float> velocityDist(MIN_VELOCITY_METEORITE, MAX_VELOCITY_METEORITE);
    float velocity = velocityDist(_generator);

    std::uniform_real_distribution<float> heightDist(MIN_HEIGHT_METEORITE, MAX_HEIGHT_METEORITE);
    float height = heightDist(_generator);


    if(goingLeft)
    {
        meteorite.SetSize(METERORITE_WIDTH, METERORITE_HEIGHT);
        meteorite.SetPosition(SCREEN_WIDTH + METERORITE_WIDTH, SCREEN_HEIGHT*height);
        meteorite.SetVelocities(-DEFAULT_BULLET_VEL_Y * velocity, 0);
        meteorite.SetMoveLeft(true);
    }
    else
    {
        meteorite.SetSize(METERORITE_WIDTH, METERORITE_HEIGHT);
        meteorite.SetPosition(-static_cast<int>(METERORITE_WIDTH), SCREEN_HEIGHT*height);
        meteorite.SetVelocities(DEFAULT_BULLET_VEL_Y * velocity,0);
        meteorite.SetMoveLeft(false);
    }
}

 void BattleState::ConfigureRandomMovement(Plane& plane)
 {
    if(_destinyManager.GetTotalPositions() == 0){return;}

    float newX, newY;
    _destinyManager.GetRandomPosition(newX, newY);

    float duration = 10;

    std::uniform_int_distribution<int> typeDist(0, 3);
	int type = typeDist(_generator);
    Ease::EASE_TYPES easeType = Ease::EASE_TYPES::INOUTQUINT;

    switch (type)
    {
        case 0: easeType = Ease::EASE_TYPES::INOUTSINE; break;
        case 1: easeType = Ease::EASE_TYPES::INOUTCUBE; break;
        case 2: easeType = Ease::EASE_TYPES::INOUTQUINT; break;
        case 3: easeType = Ease::EASE_TYPES::INOUTCIRC; break;
    }

    int easeID = _easingManager->AddEase(duration, plane.GetX(), plane.GetY(), newX, newY, easeType,
        [&](){ ConfigureRandomMovement(plane); },
        [&plane](float x, float y) { plane.SetPosition(x, y); }
    );
    plane.SetRandomMovementID(easeID);
 }