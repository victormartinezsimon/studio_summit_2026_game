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
    _player->Update(deltaTime);

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
                p.GetWidth(), p.GetHeight(), posX, posY); });
    }

    {
        float playerX, playerY;
        _player->GetPaintPosition(playerX, playerY);
        _painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER, _player->GetWidth(), _player->GetHeight(), playerX, playerY);
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

    for(int metID = 0; metID < TOTAL_METEORITES; ++metID)
    {
        int id = _meteoritesPool.Get();
        _meteoritesPool.call_for_element(id, [](WorldObject& object)
        {
            object.SetSize(METERORITE_WIDTH, METERORITE_HEIGHT);
        });
    }

/*
    _meteorites[0].SetSize(METERORITE_WIDTH, METERORITE_HEIGHT);
    _meteorites[0].SetPosition(SCREEN_WIDTH + METERORITE_WIDTH, SCREEN_HEIGHT*0.4f);
    _meteorites[0].SetVelocities(-DEFAULT_BULLET_VEL_Y * 0.5,0);
    _meteorites[0].SetMoveLeft(true);

    _meteorites[1].SetSize(METERORITE_WIDTH, METERORITE_HEIGHT);
    _meteorites[1].SetPosition(-static_cast<int>(METERORITE_WIDTH), SCREEN_HEIGHT*0.6f);
    _meteorites[1].SetVelocities(DEFAULT_BULLET_VEL_Y * 0.55,0);
    _meteorites[1].SetMoveLeft(false);
*/


    _enemiesAlive = _enemiesPool->TotalInUse();

}
void BattleState::OnExit()
{
    _bulletsPool->ReturnAll();
    _enemiesPool->ReturnAll();
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

    if(!isDestroyed)
    {

        _meteoritesPool.for_each_active([&](Meteorite& meteorite)
            {
                 if (HasCollision(bullet, meteorite))
                {
                    if (!bullet.GetHasPenetration())
                    {
                        _bulletsPool->Release(bullet);
                        isDestroyed = true;
                    }
                }
            }
        );
    }

    
    if(!isDestroyed && bullet.GetPlayerTeam() == TEAM_PLAYER)
    {
        // check collision against enemies
        _enemiesPool->for_each_active([&](Plane &enemy)
                                    {
                                        bool solution = ManageCollisionBetweenBulletAndEnemy(bullet, enemy);
                                        isDestroyed |= solution; });
    }
    

    if(!isDestroyed && bullet.GetPlayerTeam() == TEAM_ENEMY)
    {
        //check player
        if (HasCollision(bullet, _player))
        {
            DamagePlayer();
            _bulletsPool->Release(bullet);
            isDestroyed = true;
        }
    }
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
    if (HasCollision(bullet, &enemy))
    {
        if (!bullet.GetHasPenetration())
        {
            _bulletsPool->Release(bullet);
            isBulletDestroyed = true;
        }
        ReturnEnemy(enemy);
    }
    return isBulletDestroyed;
}

bool BattleState::HasCollision(const Bullet &bullet, Plane *plane) const
{
    if (bullet.GetPlayerTeam() == plane->GetPlayerTeam())
    {
        return false;
    }
   
    bool hasCollsion = CollsisionDetection(bullet.GetX(), bullet.GetY(), bullet.GetWidth(), bullet.GetHeight(),
                               plane->GetX(), plane->GetY(), plane->GetWidth(), plane->GetHeight());

    if (hasCollsion && plane->GetHasShield())
    {
        plane->SetHasShield(false);
        return false;
    }
    return hasCollsion; 
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
    _damagePlayerCallback();
}

void BattleState::DoExplosion(Bullet &bullet)
{
    bullet.SetSize(EXPLOSION_SIZE, EXPLOSION_SIZE);

    //check explosion damage enemies
    _enemiesPool->for_each_active([&bullet, this](Plane &enemy)
                                  {
									  if (HasCollision(bullet, &enemy))
									  {
										  ReturnEnemy(enemy);
									  } });

    //check explosion damage player
    if (HasCollision(bullet, _player))
    {
        DamagePlayer();
    }
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
