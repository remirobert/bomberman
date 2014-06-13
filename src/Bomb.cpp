#include <iostream>
#include "Bomb.hpp"
#include <unistd.h>

Bomb::Bomb(APlayer *player, const glm::vec2 &pos, int range, Map *map)
  : _vec(pos), _map(map), _time(2.5), _staytime(0.25), _range(range)
{
  _obj = new GameModel(ResourceManager::getInstance()->get<Model>(RES_MODEL "bomb.fbx"));
  _player = player;
  _status = OK;
  _speed = _range * 10;

  _distance = 0;
  _distanceCovered = 0;
  _spreadTop = true;
  _spreadDown = true;
  _spreadLeft = true;
  _spreadRight = true;

  _obj->scale(glm::vec3(0.0025f, 0.0025f, 0.0025f));
  _obj->translate(glm::vec3(pos.x, 0, pos.y));
}

Bomb::~Bomb()
{
  delete _obj;
}

const glm::vec2	& Bomb::getPos() const
{
  return _vec;
}

void	Bomb::setPos(const glm::vec2 &new_pos)
{
  _vec = new_pos;
}

void	Bomb::update(UNUSED gdl::Input &input, gdl::Clock const &clock)
{
  _obj->scale(glm::vec3(1.0025f, 1.0025f, 1.0025f));

  if (_status == BURNING || _time.update(clock.getElapsed()))
    this->explode(clock);
}

void	Bomb::explode(gdl::Clock const &clock)
{
  if (_status == DESTROY)
    return ;
  _status = BURNING;
  if (!_distance)
    SoundManager::getInstance()->manageSound(SoundManager::BOMB_EXPLOSION, SoundManager::PLAY);
  _distance += clock.getElapsed() * _speed;
  if (_distance >= _range)
    _distance = _range;
  this->spread();
  if (_status != DESTROY && _distance >= _range && _staytime.update(clock.getElapsed()))
    {
      _fireList.clear();
      _status = DESTROY;
      _player->setStockBomb(_player->getStockBomb() + 1);
      for (std::vector<ABonus *>::iterator it = _generatedBonus.begin();
           it != _generatedBonus.end(); ++it)
        {
          _map->addEntity(*it);
        }
      _generatedBonus.clear();
    }
}


bool	Bomb::destroyEntity(const glm::vec2 &pos)
{
  IEntity *entity;

  std::vector<APlayer *> players = _map->getPlayersAt(glm::ivec2(pos.x, pos.y));
  for (std::vector<APlayer *>::iterator it = players.begin(); it != players.end(); ++it)
    {
      if ((*it)->getFlammePass() == false)
	{
	  if (*it != _player && (*it)->getStatus() != IEntity::DESTROY)
	    _player->increaseScores(10);
	  (*it)->setStatus(DESTROY);
	}
    }
  entity = _map->getEntityAt(glm::ivec2(pos));
  if (entity == NULL)
    return true;
  if (entity->getType() == WALL)
    return false;
  if (entity->getType() == BOMB && entity->getStatus() == OK)
    entity->setStatus(BURNING);
  if (entity->getType() == BOX)
    {
      _fireList.push_back(new Fire(entity->getPos()));
      if (rand() % 2)
	_generatedBonus.push_back(BonusFactory::getInstance()->createBonus(pos));
      entity->setStatus(DESTROY);
      _player->increaseScores(1);
      return false;
    }
  return true;
}

bool	Bomb::spread()
{
  int x = 0, y = 0;

  x = _distanceCovered;
  y = _distanceCovered;
  while ((_vec.x + x)  < _distance + _vec.x) {
      if (_spreadRight && (_spreadRight = destroyEntity(glm::vec2(_vec.x + x, _vec.y))))
	_fireList.push_back(new Fire(glm::vec2(_vec.x + x, _vec.y)));
      if (_spreadLeft && (_spreadLeft = destroyEntity(glm::vec2(_vec.x - x, _vec.y))))
	_fireList.push_back(new Fire(glm::vec2(_vec.x - x, _vec.y)));
      ++x;
      if (_spreadTop && (_spreadTop = destroyEntity(glm::vec2(_vec.x, _vec.y + y))))
	_fireList.push_back(new Fire(glm::vec2(_vec.x, _vec.y + y)));
      if (_spreadDown && (_spreadDown = destroyEntity(glm::vec2(_vec.x, _vec.y - y))))
	_fireList.push_back(new Fire(glm::vec2(_vec.x, _vec.y - y)));
      ++y;
  }
  _distanceCovered = x - 1;
  return true;
}

void	Bomb::draw(gdl::AShader *shader, const gdl::Clock& clock) const
{
  if (_status == OK)
    _obj->draw(shader, clock);

  // draw flames
  for (FireList::const_iterator it = _fireList.begin(), end = _fireList.end();
       it != end; ++it)
    if (_status == BURNING)
      (*it)->draw(shader, clock);
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
