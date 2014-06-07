#include <iostream>
#include "Bomb.hpp"

Bomb::Bomb(APlayer *player, const glm::vec2 &pos, int lvl, Map *map) :
  _vec(pos), _lvl(lvl), _map(map), _time(2), _range(2 + lvl)
{
  _player = player;
  _status = OK;
  _speed = _range * 10;
  _distance = 0;
  _obj = new Model(RES_MODEL "bomb.fbx");
  _obj->scale(glm::vec3(0.0025f, 0.0025f, 0.0025f));
  _obj->translate(glm::vec3(pos.x, 0, pos.y));

  // _lastPos.push_back(_vec);
  // _lastPos.push_back(_vec);
  // _lastPos.push_back(_vec);
  // _lastPos.push_back(_vec);

  _lastPosUp = _vec;
  _lastPosDown = _vec;
  _lastPosLeft = _vec;
  _lastPosRight = _vec;

  _fireList.push_back(new Fire(_vec));
}

Bomb::~Bomb()
{

}

const glm::vec2	&Bomb::getPos() const
{
  return _vec;
}

void	Bomb::setPos(const glm::vec2 &new_pos)
{
  _vec = new_pos;
}

void	Bomb::update(UNUSED gdl::Input &input, gdl::Clock const &clock)
{
  if (_status == BURNING || _time.update(clock.getElapsed()))
    this->explode(clock);
}

void	Bomb::explode(gdl::Clock const &clock)
{
  if (_status == DESTROY)
    return ;
  _status = BURNING;
  _distance += clock.getElapsed() * _speed;
  _totalDistance += _distance;
  std::cout << "distance = " << _distance << std::endl;

  // this->spreadTop();
  // this->spreadLeft();
  // this->spreadDown();
  // this->spreadRight();

  std::cout << "UP" << std::endl;
  _lastPosUp = this->spread(glm::vec2(0.0,1.0), _lastPosUp, _distance);
  std::cout << "LEFT" << std::endl;
  _lastPosLeft = this->spread(glm::vec2(1.0,0.0), _lastPosLeft, _distance);
  std::cout << "DOWN" << std::endl;
  _lastPosDown = this->spread(glm::vec2(0.0,-1.0), _lastPosDown, _distance);
  std::cout << "RIGHT" << std::endl;
  _lastPosRight = this->spread(glm::vec2(-1.0,0.0), _lastPosRight, _distance);

  if (_status != DESTROY && _totalDistance >= _range) {
      _status = DESTROY;
      _player->createBomb();
      SoundManager::getInstance()->manageSound(SoundManager::BOMB_EXPLOSION, SoundManager::PLAY);
    }
}

bool	Bomb::destroyEntity(int x, int y) const
{
  IEntity *entity;

  entity = _map->getEntityAt(x, y);
  if (!entity || entity == this)
    return true;
  if (entity->getType() == WALL)
    return false;
  if (entity->getType() == BOX)
    entity->setStatus(DESTROY);
  if (entity->getType() == BOMB && entity->getStatus() == OK)
    entity->setStatus(BURNING);
  return true;
}

glm::vec2	Bomb::spread(glm::vec2 dir, glm::vec2 lastPos, double distance)
{
  // to go
  glm::vec2 maxPos, newPos;

  maxPos = _vec + (_range * dir);
  newPos = lastPos;

  while (newPos != maxPos && distance > 1.0)
    {

      newPos += dir;

      if (!this->destroyEntity(newPos.x, newPos.y))
      	break;

      std::cout << "newposx : " << newPos.x << " - newPosy : " << newPos.y << std::endl;
      _fireList.push_back(new Fire(newPos));

      distance -= 1.0;
    }
  return newPos;
}

bool	Bomb::spreadTop()
{
  glm::vec2 cpy = _vec;

  if (_distance >= _range)
    _distance = _range;
  while (cpy.y < _distance + _vec.y) {
      if (!this->destroyEntity(cpy.x, cpy.y))
        return false;
      _fireList.push_back(new Fire(cpy));
      cpy.y += 1;
    }
  return true;
}

bool	Bomb::spreadLeft()
{
  glm::vec2 cpy = _vec;

  if (_distance >= _range)
    _distance = _range;
  while (cpy.x < _distance + _vec.x) {
      if (!this->destroyEntity(cpy.x, cpy.y))
        return true;
      _fireList.push_back(new Fire(cpy));
      cpy.x += 1;
    }
  return false;
}

bool	Bomb::spreadDown()
{
  glm::vec2 cpy = _vec;

  if (_distance >= _range)
    _distance = _range;
  while (cpy.y > _vec.y - _distance) {
      if (!this->destroyEntity(cpy.x, cpy.y))
        return false;
      _fireList.push_back(new Fire(cpy));
      cpy.y -= 1;
    }
  return true;
}

bool	Bomb::spreadRight()
{
  glm::vec2 cpy = _vec;

  if (_distance >= _range)
    _distance = _range;
  while (cpy.x > _vec.x - _distance) {
      if (!this->destroyEntity(cpy.x, cpy.y))
        return false;
      _fireList.push_back(new Fire(cpy));
      cpy.x -= 1;
    }
  return true;
}

void	Bomb::draw(gdl::AShader *shader, const gdl::Clock& clock)
{
  if (_status == OK)
    _obj->draw(shader, clock);

  // draw flames
  // while (_status == BURNING && !_fireList.empty()) {
  //     _fireList.front()->draw(shader, clock);
  //     delete _fireList.front();
  //     _fireList.pop_front();
  //   }
  for (std::list<Fire *>::iterator it = _fireList.begin(), end = _fireList.end(); it != end; ++it)
    {
      (*it)->draw(shader, clock);
    }
}

IEntity::Type Bomb::getType() const
{
  return IEntity::BOMB;
}

IEntity::Status Bomb::getStatus() const
{
  return _status;
}

void Bomb::setStatus(IEntity::Status status)
{
  _status = status;
}
