#include "EntitiesFactory.hpp"
#include "Map.hpp"
#include "Bomb.hpp"

/*
** constructor random map
*/

Map::Map(const glm::ivec2& dim)
  : _dim(dim)
{
  _charToIEntity[IEntity::S_BOX] = IEntity::BOX;
  _charToIEntity[IEntity::S_WALL] = IEntity::WALL;
  _charToIEntity[IEntity::S_NONE] = IEntity::NONE;
  loadRandomMap();
}

/*
** constructor map with fileName
*/
Map::Map(std::string const &mapFileName)
  : _dim(glm::ivec2(0, 0))
{
  _charToIEntity[IEntity::S_BOX] = IEntity::BOX;
  _charToIEntity[IEntity::S_WALL] = IEntity::WALL;
  _charToIEntity[IEntity::S_NONE] = IEntity::NONE;
  loadMapFromFile(mapFileName);
}

Map::~Map()
{
  for (std::vector<IEntity*>::iterator it = _map.begin(); it != _map.end(); ++it)
    delete (*(it--));
  for (std::list<IEntity*>::iterator it = _updateList.begin(); it != _updateList.end(); ++it)
    delete (*(it--));
}

/*
** Private Methods
*/

IEntity::Type	Map::getType(char c) const
{
  IEntity::Type type = IEntity::NONE;

  try
    {
      type = _charToIEntity.at(c);
    }
  catch (const std::out_of_range &oor)
    {
      std::cout << "Invalid type \'" << c << "\' in map." << std::endl;
    }
  return type;
}

bool		Map::loadMapFromFile(std::string const &fileName)
{
  std::ifstream file(fileName.c_str());

  if (!file.is_open())
    {
      std::cerr << "Unable to load from file : " + fileName << std::endl;
      return false;
    }

  std::string line;
  glm::ivec2 pos(0, 0);
  IEntity	*entity;

  while (std::getline(file, line)) {
      pos.y = 0;
      for (std::string::const_iterator it = line.begin(), end = line.end();
           it != end; ++it) {
          if ((entity = getEntityForMap(pos, getType(*it))))
            addEntity(entity);
          ++pos.y;
        }
      ++pos.x;
    }
  _dim = pos;
  file.close();
  return true;
}

void		Map::loadRandomMap()
{
  IEntity	*entity;
  glm::ivec2 pos;

  _map.resize(_dim.x * _dim.y);
  for (pos.x = 0; i < _dim.x; ++pos.x)
    for (pos.y = 0; j < _dim.y; ++pos.y)
      {
        if (pos.x == 0 || pos.y == 0 || pos.x == _dim.x - 1 || pos.y == _dim.y - 1 || (pos.y % 2 == 0 && pos.x % 2 == 0))
          entity = getEntityForMap(pos, IEntity::WALL);
        else if (rand() % 3 == 1)
          entity = getEntityForMap(pos, IEntity::BOX);
        else
          entity = getEntityForMap(pos, IEntity::NONE);
        if (entity)
          addEntity(entity);
      }
}

IEntity *Map::getEntityForMap(const glm::ivec2& pos, const IEntity::Type i) const
{
  IEntity	*entity = NULL;

  if (i != IEntity::NONE)
    entity = EntitiesFactory::getInstance()->create(i, pos.x, pos.y);
  return entity;
}

const glm::ivec2 &Map::getDimension() const
{
  return _dim;
}

/*
** Return a pointer, or not, on an IEntity if there is one at coord(x, y)
*/

IEntity		*Map::getEntityAt(const glm::ivec2& pos) const
{
  size_t posInArray = twoDToArray(pos);
  if (posInArray >= _map.size())
    return NULL;
  return _map.at(posInArray);
}

std::vector<APlayer *> Map::getPlayersAt(const glm::ivec2& pos) const
{
  int x1, y1;
  int x, y;
  x = pos.x;
  y = pos.y;
  std::vector<APlayer *> entity;


  for (std::list<IEntity*>::const_iterator it = _updateList.begin(), end = _updateList.end();
       it != end; ++it)
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
** Return the Type of an Entity at pos
*/

IEntity::Type	Map::getTypeAt(const glm::ivec2& pos) const
{
  IEntity::Type type;
  IEntity* ent;

  size_t posInArray = twoDToArray(pos);
  if (posInArray >= _map.size())
    return IEntity::NONE;
  ent = _map.at(posInArray);
  type = ent->getType();
  return (type == IEntity::PLAYER) ? IEntity::NONE : type;
}

/*
** Add an entity in the map if it's possible, else return false
*/

bool Map::addEntity(IEntity *entity)
{
  glm::ivec2 pos = entity->getPos();

  if (entity->getType() == IEntity::WALL || entity->getType() == IEntity::BOX)
    _map[twoDToArray(pos)] = entity;
  else
    _updateList.push_back(entity);
  return true;
}

/*
** Delete an entity in the map at pos if it's possible, else return false
*/

bool	Map::deleteEntityAt(const glm::ivec2& pos)
{
  IEntity::Type type;
  IEntity* ent;

  size_t posInArray = twoDToArray(pos);
  if (posInArray >= _map.size())
    return IEntity::NONE;
  ent = _map.at(posInArray);
  delete entity;
  _map[posInArray] = NULL;
  return true;
}

size_t Map::twoDToArray(const glm::ivec2& pos) const
{
  return pos.y * _dim.x + pos.x;
}

