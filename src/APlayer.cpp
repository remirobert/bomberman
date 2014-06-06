#include "APlayer.hpp"
# include "Bomb.hpp"

APlayer::APlayer(const glm::vec2 &pos, Map *map) : _vec(pos), _map(map), _time(2)
{
  _stock = 1;

  _obj = new Model(RES_MODEL "marvin.fbx");

  _obj->translate(glm::vec3(pos.x, -0.5, pos.y));
  _obj->scale(glm::vec3(0.0025, 0.0025, 0.0025));

  _status = STANDBY;
  _speed = 4;
  _way = UP;
  _size = 0.7;
  _lvl = 1;

  _obj->createSubAnim(0, "standby", 0, 0);
  _obj->createSubAnim(0, "walk", 13, 63);
  _obj->createSubAnim(0, "stop_walking", 64, 140);
  _obj->setCurrentSubAnim("standby");

  _moveKey.push_back(SDLK_UP);
  _moveKey.push_back(SDLK_DOWN);
  _moveKey.push_back(SDLK_LEFT);
  _moveKey.push_back(SDLK_RIGHT);

  _actionPtr[SDLK_SPACE] = &APlayer::bomb;

  _moveConf[SDLK_UP] = new movementCoef(0, glm::vec2(0.0, 1.0),
					glm::vec3(0, 0, 1),
					glm::vec2(0.7, 0.7), glm::vec2(0.2, 0.7));

  _moveConf[SDLK_DOWN] = new movementCoef(180, glm::vec2(0.0, -1.0),
					  glm::vec3(0, 0, -1),
					  glm::vec2(0.7, 0.2), glm::vec2(0.2, 0.2));

  _moveConf[SDLK_RIGHT] = new movementCoef(-90, glm::vec2(-1.0, 0.0),
					   glm::vec3(-1, 0, 0),
					   glm::vec2(0.2, 0.7), glm::vec2(0.2, 0.2));

  _moveConf[SDLK_LEFT] = new movementCoef(90, glm::vec2(1.0, 0.0),
					  glm::vec3(1, 0, 0),
					  glm::vec2(0.7, 0.7), glm::vec2(0.7, 0.2));
}

APlayer::~APlayer()
{
}

const glm::vec2	&APlayer::getPos() const
{
  return _vec;
}

void	APlayer::setPos(const glm::vec2 &new_pos)
{
  _vec = new_pos;
}

void	APlayer::draw(gdl::AShader *shader, const gdl::Clock& clock)
{
  _obj->draw(shader, clock);
}

bool	APlayer::movePlayer(const movementCoef *mcoef, float const distance)
{
  glm::vec2	toGoLeft;
  glm::vec2	toGoRight;
  bool	hasMoved = false;

  // reset rotation
  _obj->setRotation(glm::vec3(0,0,0));
  _obj->rotate(glm::vec3(0, 1, 0), mcoef->rotate);

  // get point to go left end right in front the player
  toGoLeft = _vec + (mcoef->dir * distance) + mcoef->distLeft;
  toGoRight = _vec + (mcoef->dir * distance) + mcoef->distRight;

  // if the points to go are on the same piece of field or
  // if the type to go is free --> move

  if ((glm::ivec2(toGoLeft) == glm::ivec2(_vec + mcoef->distLeft) &&
       glm::ivec2(toGoRight) == glm::ivec2(_vec + mcoef->distRight)) ||
      (_map->getTypeAt(toGoLeft.x, toGoLeft.y, _around) == NONE &&
       _map->getTypeAt(toGoRight.x, toGoRight.y, _around) == NONE))
    {
      _vec += mcoef->dir * distance;
      _obj->translate(mcoef->translate * distance);
      hasMoved = true;
    }
  return hasMoved;
}

void	APlayer::updateAnim(bool hasMoved, bool keyPressed)
{
  if (keyPressed == false)
    {
      if (_status == WALK)
	{
	  _obj->setCurrentSubAnim("stop_walking", false);
	  _status = STOP_WALK;
	}
      return;
    }
  if (_status != WALK && hasMoved)
    {
      _status = WALK;
      _obj->setCurrentSubAnim("walk");
    }
  else if (_status == WALK && !hasMoved)
    {
      _obj->setCurrentSubAnim("stop_walking", false);
      _status = STOP_WALK;
    }
}

void APlayer::createBomb()
{
  _bombList.push_back(_lvl);
}

bool APlayer::bomb()
{
  int x = _vec.x + _size;
  int y = _vec.y + _size;
  static int prevX = 0;
  static int prevY = 0;

  if (prevX == x && prevY == y)
    return false;
  if (!_bombList.empty()) {
      _map->addEntity(new Bomb(this, glm::vec2(x, y), _bombList.front(), _map));
      _bombList.pop_front();
      prevX = x;
      prevY = y;
  }
  return false;
}

void	APlayer::addEntityAround(const IEntity *entity)
{
  _around.push_back(entity);
}

void	APlayer::clearEntityAround()
{
  _around.clear();
}

IEntity::Type APlayer::getType() const
{
  return IEntity::PLAYER;
}

void	APlayer::setStatus(APlayer::Status st)
{
  _status = st;
}

IEntity::Status APlayer::getStatus() const
{
  return _statusOfObject;
}

void APlayer::setStatus(IEntity::Status status)
{
  _statusOfObject = status;
}
