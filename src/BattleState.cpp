#include "BattleState.h"
#include "PainterManager.h"
#include "Plane.h"
#include "GameConfig.h"
#include "Sprites.h"
#include "Bullet.h"
#include "AlphaManager.h"

BattleState::BattleState(Plane *player, PainterManager *painter, Pool<Plane, PLANES_POOL_SIZE> *enemiesPool,
                         Pool<Bullet, BULLETS_POOL_SIZE> *bulletsPool, 
                         std::function<void()> damagePlayerCallback, 
                         std::function<void(float x, float y)> damageEnemy,
                         long long* score, float* time, 
                         NumberManager* numberManager, AlphaManager* alphaManager) 
                         : State(player, painter), _enemiesPool(enemiesPool), _bulletsPool(bulletsPool),
                         _damagePlayerCallback(damagePlayerCallback), _damageEnemyCallback(damageEnemy),
                         _score(score), _timeLeft(time), _numberManager(numberManager),_alphaManager(alphaManager)
{
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
        _meteoritesPool.for_each_active([&](const Meteorite& m)
        {
            float posX, posY;
            m.GetPaintPosition(posX, posY);
            _painterManager->AddToPaint(PainterManager::SPRITE_ID::METEORITE, 
                m.GetWidth(), m.GetHeight(), posX, posY);
        });
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

    int id0 = _meteoritesPool.Get();
    _meteoritesPool.call_for_element(id0, [](Meteorite& object)
    {
        object.SetSize(METERORITE_WIDTH, METERORITE_HEIGHT);
        object.SetPosition(SCREEN_WIDTH + METERORITE_WIDTH, SCREEN_HEIGHT*0.4f);
        object.SetVelocities(-DEFAULT_BULLET_VEL_Y * 0.5,0);
        object.SetMoveLeft(true);
    });

    int id1 = _meteoritesPool.Get();
    _meteoritesPool.call_for_element(id1, [](Meteorite& object)
    {
        object.SetSize(METERORITE_WIDTH, METERORITE_HEIGHT);
        object.SetPosition(-static_cast<int>(METERORITE_WIDTH), SCREEN_HEIGHT*0.6f);
        object.SetVelocities(DEFAULT_BULLET_VEL_Y * 0.55,0);
        object.SetMoveLeft(false);
    });

    _enemiesAlive = _enemiesPool->TotalInUse();

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

    _explosionPool.for_each_active([&] (Explosion& exp)
        {
            bool hasCollsion = CollsisionDetection(exp.GetX(), exp.GetY(), exp.GetWidth(), exp.GetHeight(),
                               _player->GetX(), _player->GetY(), _player->GetWidth(), _player->GetHeight());
            if (hasCollsion)
            {
                DamagePlayer();
            }
        }
    );
}

void BattleState::UpdateBullets(float deltaTime)
{
    _bulletsPool->for_each_active([deltaTime](Bullet &bullet)
                                  { bullet.Update(deltaTime); });

    // check if it should be deleted any bullet
    
    _bulletsPool->for_each_active([this, deltaTime](Bullet &bullet)
                                  { ManageBulletCollisions(bullet); });
                                  
}
void BattleState::UpdateEnemies(float deltaTime)
{
    _enemiesPool->for_each_active([deltaTime](Plane &enemy)
                                  { enemy.Update(deltaTime); });

    
    _explosionPool.for_each_active([&] (Explosion& exp)
        {
            _enemiesPool->for_each_active([&](Plane& enemy)
            {
                bool hasCollsion = CollsisionDetection(exp.GetX(), exp.GetY(), exp.GetWidth(), exp.GetHeight(),
                                enemy.GetX(), enemy.GetY(), enemy.GetWidth(), enemy.GetHeight());
                if (hasCollsion)
                {
                    ReturnEnemy(enemy);
                }
            }
            );
        }
    );
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
        _meteoritesPool.for_each_active([&](Meteorite& meteorite)
            {
                if (HasCollision(bullet, meteorite))
                {
                    if(TryDestroyBullet(bullet))
                    {
                        isDestroyed = true;
                    }
                }
            }
        );
    }

    //check enemies
    if(!isDestroyed && bullet.GetPlayerTeam() == TEAM_PLAYER)
    {
        // check collision against enemies
        _enemiesPool->for_each_active([&](Plane &enemy)
                                    {
                                        if(isDestroyed){return;}//only affect one enemy
                                        bool solution = ManageCollisionBetweenBulletAndEnemy(bullet, enemy);
                                        isDestroyed |= solution; 
                                    });
    }
    
    //check player
    if(!isDestroyed && bullet.GetPlayerTeam() == TEAM_ENEMY)
    {
        //check player
        bool hasShield = _player->GetHasShield();
        bool hasCollision = HasCollision(bullet, _player);

        if(hasCollision && hasShield)
        {
            _player->SetHasShield(false);
            hasCollision = false;
            isDestroyed = true;
        }

        if (hasCollision)
        {
            DamagePlayer();
            _bulletsPool->Release(bullet);
            isDestroyed = true;
        }
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

bool BattleState::ManageCollisionBetweenBulletAndEnemy(Bullet &bullet, Plane &enemy)
{
    bool isBulletDestroyed = false;
    bool hasShield = enemy.GetHasShield();
    bool hasCollision = HasCollision(bullet, &enemy);

    if(hasCollision && hasShield)
    {
        enemy.SetHasShield(false);
        if (TryDestroyBullet(bullet))
        {
            isBulletDestroyed = true;
        }
        hasCollision = false;
    }

    if (hasCollision)
    {
        if (TryDestroyBullet(bullet))
        {
            isBulletDestroyed = true;
        }
        ReturnEnemy(enemy);
    }
    return isBulletDestroyed;
}

bool BattleState::TryDestroyBullet(Bullet& bullet)
{
    if (!bullet.GetHasPenetration())
    {
        _bulletsPool->Release(bullet);
        return false;
    }
    return true;
}


bool BattleState::HasCollision(const Bullet &bullet, Plane *plane) const
{
    if (bullet.GetPlayerTeam() == plane->GetPlayerTeam())
    {
        return false;
    }
   
    return CollsisionDetection(bullet.GetX(), bullet.GetY(), bullet.GetWidth(), bullet.GetHeight(),
                               plane->GetX(), plane->GetY(), plane->GetWidth(), plane->GetHeight());
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
    _meteoritesPool.for_each_active([&](Meteorite& m)
    {
        m.Update(deltaTime);

        float posX, posY;
        m.GetPaintPosition(posX, posY);
        if(posX + m.GetWidth() < 0 && m.GetMoveLeft())
        {   
            m.SetPositionX( SCREEN_WIDTH + m.GetWidth() / 2);
        }

        if(posX > SCREEN_WIDTH && !m.GetMoveLeft())
        {   
            float positionX = -static_cast<int>(m.GetWidth()) / 2;
            m.SetPositionX( positionX );
        }
    });
}

void BattleState::ReturnEnemy(Plane& enemy)
{
    _enemiesPool->Release(enemy);
    float x, y;
    enemy.GetPaintPosition(x, y);
    int id = _alphaManager->AddAlpha(ALPHA_TIME_DESTROY_PLANE, x, y, false, 
        ENEMY_WIDTH, ENEMY_HEIGHT, PainterManager::SPRITE_ID::ENEMY);
    _alphaManager->AddCallback(id, [this]()
    {
        _enemiesAlive--;
    });
    _damageEnemyCallback(x, y);
}

void BattleState::DoExplosion(Bullet &bullet)
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
    exp.SetPosition(x, y);
    exp.SetID(id);
    exp.SetSize(EXPLOSION_WIDTH, EXPLOSION_HEIGHT);
    exp.SetPlayerTeam(bullet.GetPlayerTeam());
    
    auto alphaID =_alphaManager->AddAlpha(EXPLOSION_DURATION, 
        x - EXPLOSION_WIDTH/2, 
        y - EXPLOSION_HEIGHT/2, 
        true, EXPLOSION_WIDTH, EXPLOSION_HEIGHT, 
        PainterManager::SPRITE_ID::EXPLOSION);
    
    _alphaManager->AddCallback(alphaID, 
        [&]()
        {
            EndExplosion(exp);
        }
    );
}
