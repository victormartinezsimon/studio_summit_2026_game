#include "BattleState.h"
#include "PainterManager.h"
#include "Plane.h"
#include "GameConfig.h"
#include "Sprites.h"
#include "Bullet.h"

BattleState::BattleState(Plane *player, PainterManager *painter, Pool<Plane, PLANES_POOL_SIZE> *enemiesPool,
                         Pool<Bullet, BULLETS_POOL_SIZE> *bulletsPool, std::function<void()> damagePlayerCallback) : State(player, painter), _enemiesPool(enemiesPool), _bulletsPool(bulletsPool),
                                                                                                                     _damagePlayerCallback(damagePlayerCallback)
{
}

State::STATES BattleState::Update(const float deltaTime, float currentFrameInputValueNormalized, int currentFrameInputValue)
{
    // update player
    _player->Update(deltaTime);

    UpdateBullets(deltaTime);

    // update enemies
    UpdateEnemies(deltaTime);

    if (_enemiesPool->TotalInUse() == 0)
    {
        return STATES::IMPROVEMENT_SELECTOR;
    }

    return STATES::BATTLE;
}
void BattleState::Paint()
{
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
}
void BattleState::OnEnter()
{
    _player->SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
    _player->SetPositionY(SCREEN_HEIGHT * POSITION_Y_PLAYER);
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

    // check collision against enemies
    _enemiesPool->for_each_active([&](Plane &enemy)
                                  {
									bool solution = ManageCollisionBetweenBulletAndEnemy(bullet, enemy);
									isDestroyed |= solution; });

    if (HasCollision(bullet, _player))
    {
        DamagePlayer();
        _bulletsPool->Release(bullet);
        isDestroyed = true;
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
        _enemiesPool->Release(enemy);
    }
    return isBulletDestroyed;
}

bool BattleState::HasCollision(const Bullet &bullet, Plane *plane) const
{
    if (bullet.GetPlayerTeam() == plane->GetPlayerTeam())
    {
        return false;
    }

    if (plane->GetHasShield())
    {
        plane->SetHasShield(false);
        return false;
    }

    return CollsisionDetection(bullet.GetX(), bullet.GetY(), bullet.GetWidth(), bullet.GetHeight(),
                               plane->GetX(), plane->GetY(), plane->GetWidth(), plane->GetHeight());
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

    _enemiesPool->for_each_active([&bullet, this](Plane &enemy)
                                  {
									  if (HasCollision(bullet, &enemy))
									  {
										  _enemiesPool->Release(enemy);
									  } });

    if (HasCollision(bullet, _player))
    {
        DamagePlayer();
    }
}