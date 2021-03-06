#include "EntitiesFactory.hpp"
#include "Map.hpp"
#include "Bomb.hpp"
#include "PMutex.hpp"

/*
** constructor random map
*/

Map::Map(const int x, const int y)
  : _dim(x, y)
{
  if (x == 0 || y == 0)
    throw nFault("Map too small");
  _mut = new PMutex;
  _charToIEntity[IEntity::S_BOX] = IEntity::BOX;
  _charToIEntity[IEntity::S_WALL] = IEntity::WALL;
  _charToIEntity[IEntity::S_NONE] = IEntity::NONE;
  this->loadRandomMap();
}

/*
** constructor map with fileName
*/

Map::Map(std::string const &mapFileName)
  : _dim(0, 0)
{
  _mut = new PMutex;
  _charToIEntity[IEntity::S_BOX] = IEntity::BOX;
  _charToIEntity[IEntity::S_WALL] = IEntity::WALL;
  _charToIEntity[IEntity::S_NONE] = IEntity::NONE;
  this->loadMapFromFile(mapFileName);
}

Map::~Map()
{
  IEntity	*entity;

  {
    ScopeLock sc(*_mut);
    while (!_map.empty()) {
        entity = *(_map.begin());
        delete entity;
        _map.pop_front();
      }
  }
  delete _mut;
}

/*
** Private Methods
*/

IEntity::Type	Map::getType(const char c) const
{
  ScopeLock sl(*_mut);
  IEntity::Type type = IEntity::NONE;

  try
    {
      type = _charToIEntity.at(c);
    }
  catch (const std::out_of_range &oor)
    {
      std::cout << "Invalid type \'" << c << "\'" << std::endl;
    }
  return type;
}

bool		Map::loadMapFromFile(std::string const &fileName)
{
  std::ifstream	file(fileName.c_str());

  if (!file.is_open())
    {
      std::cerr << "Unable to load from file : " + fileName << std::endl;
      return false;
    }

  std::string	line;
  int		x = 0, y = 0;
  IEntity	*entity;

  while (std::getline(file, line)) {
      y = 0;
      for (std::string::const_iterator it = line.begin(), end = line.end();
           it != end; ++it) {
          if ((entity = this->getEntityForMap(x, y, this->getType(*it))))
            this->addEntity(entity);
          ++y;
        }
      ++x;
    }
  if (x == 0 || y == 0)
    throw nFault("Map too small");
  _dim.y = y;
  _dim.x = x;
  file.close();
  return true;
}

void		Map::loadRandomMap()
{
  IEntity	*entity;

  for (int i = 0; i < _dim.x; i++)
    for (int j = 0; j < _dim.y; j++)
      {
        if (i == 0 || j == 0 || i == _dim.x - 1 || j == _dim.y - 1 || (j % 2 == 0 && i % 2 == 0))
          entity = getEntityForMap(i, j, IEntity::WALL);
        else if (rand() % 3 == 1)
          entity = getEntityForMap(i, j, IEntity::BOX);
        else
          entity = getEntityForMap(i, j, IEntity::NONE);
        if (entity)
          addEntity(entity);
      }
}

IEntity		*Map::getEntityForMap(const int x, const int y, const IEntity::Type i) const
{
  ScopeLock sl(*_mut);
  IEntity	*entity = NULL;

  if (i != IEntity::NONE)
    entity = EntitiesFactory::getInstance()->create(i, x, y);
  return entity;
}

/*
** Debug methods
*/

void	Map::displayDebugMap() const
{
  bool	check;

  if (!_map.empty())
    {
      for (int i = 0; i < _dim.x; ++i) {
          for (int j = 0; j < _dim.y; ++j) {
              check = false;
              for (LMap::const_iterator it = _map.begin(); it != _map.end(); ++it) {
                  if ((*(*it)).getPos().x == i && (*(*it)).getPos().y == j) {
                      if (dynamic_cast<Box *>(*it) != NULL)
                        std::cerr << "o";
                      else if (dynamic_cast<Wall *>(*it) != NULL)
                        std::cerr << "#";
                      else
                        std::cerr << "*";
                      check = true;
                    }
                }
              if (check == false)
                std::cerr << " ";
            }
          std::cerr << std::endl;
        }
    }
}

/*
** Public methods
*/

Map::LMap	&Map::getMap()
{
  return _map;
}

Map::LMap	&Map::getUpdateMap()
{
  return _updateList;
}

// int	Map::getWidth() const
// {
//   return _x;
// }

// int	Map::getLength() const
// {
//   return _y;
// }

const glm::vec2 &Map::getDimension() const
{
  return _dim;
}

/*
** Return a pointer, or not, on an IEntity if there is one at coord(x, y)
*/

IEntity		*Map::getEntityAt(const int x, const int y) const
{
  ScopeLock sl(*_mut);
  for (LMap::const_iterator it = _map.begin(), end = _map.end(); it != end; ++it)
    if ((*(*it)).getPos().x == x && (*(*it)).getPos().y == y && (*it)->getType() != IEntity::FIREBALL)
      return *it;
  return NULL;
}

std::vector<APlayer *> const Map::getPlayersAt(const int x, const int y) const
{
  ScopeLock sl(*_mut);
  int x1, y1;
  std::vector<APlayer *> entity;

  for (LMap::const_iterator it = _playerList.begin(), end = _playerList.end(); it != end; ++it)
    {
      x1 = (*it)->getPos().x + 0.7;
      y1 = (*it)->getPos().y + 0.7;
      if (x1 == x && y1 == y)
        entity.push_back(static_cast<APlayer *>(*it));
      x1 = (*it)->getPos().x + 0.3;
      y1 = (*it)->getPos().y + 0.3;
      if (x1 == x && y1 == y)
        entity.push_back(static_cast<APlayer *>(*it));
    }
  return entity;
}

/*
** Return the Type of an Entity at coor(x, y)
*/

IEntity::Type	Map::getTypeAt(const int x, const int y) const
{
  ScopeLock sl(*_mut);
  IEntity::Type type = IEntity::NONE;

  for (LMap::const_iterator it = _map.begin(), end = _map.end(); it != end; ++it)
    {
      IEntity* ent = (*it);
      if (static_cast<int>(ent->getPos().x) == x && static_cast<int>(ent->getPos().y) == y) {
          type = ent->getType();
          if (type == IEntity::PLAYER || type == IEntity::FIREBALL)
            type = IEntity::NONE;
          if (type != IEntity::NONE)
            return type;
        }
    }
  return type;
}

Map::LMap		&Map::getPlayerList()
{
  return _playerList;
}

/*
** Add an entity in the map if it's possible, else return false
*/

bool		Map::addEntity(IEntity *entity)
{
  ScopeLock sl(*_mut);
  if (entity->getType() != IEntity::PLAYER)
    _map.push_back(entity);
  if (entity->getType() == IEntity::PLAYER) {
      _playerList.push_back(entity);
    }
  else if (entity->getType() != IEntity::WALL && entity->getType() != IEntity::BOX)
    _updateList.push_back(entity);
  return true;
}

/*
** Delete an entity in the map at coord(x, y) if it's possible, else return false
*/

bool	Map::deleteEntityAt(const int x, const int y)
{
  ScopeLock sl(*_mut);
  IEntity	*entity;

  for (LMap::iterator it = _map.begin(); it != _map.end(); ++it)
    if ((*(*it)).getPos().x == x && (*(*it)).getPos().y == y) {
        entity = *it;
        delete entity;
        _map.erase(it);
        return true;
      }
  return false;
}

Map::iterator	Map::begin()
{
  return _map.begin();
}

Map::const_iterator	Map::begin() const
{
  return _map.begin();
}

Map::iterator	Map::playerBegin()
{
  return _playerList.begin();
}

Map::const_iterator	Map::playerBegin() const
{
  return _playerList.begin();
}

Map::iterator	Map::updateBegin()
{
  return _updateList.begin();
}

Map::const_iterator	Map::updateBegin() const
{
  return _updateList.begin();
}

Map::iterator	Map::end()
{
  return _map.end();
}

Map::const_iterator	Map::end() const
{
  return _map.end();
}

Map::iterator	Map::playerEnd()
{
  return _playerList.end();
}

Map::const_iterator	Map::playerEnd() const
{
  return _playerList.end();
}

Map::iterator	Map::updateEnd()
{
  return _updateList.end();
}

Map::const_iterator	Map::updateEnd() const
{
  return _updateList.end();
}
