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
      std::cout << "Invalid type \'" << c << "\'" << std::endl;
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
  int x = 0, y = 0;
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

IEntity *Map::getEntityForMap(const glm::ivec2& pos, const IEntity::Type i) const
{
  IEntity	*entity = NULL;

  if (i != IEntity::NONE)
    entity = EntitiesFactory::getInstance()->create(i, pos.x, pos.y);
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

const glm::ivec2 &Map::getDimension() const
{
  return _dim;
}

/*
** Return a pointer, or not, on an IEntity if there is one at coord(x, y)
*/

IEntity		*Map::getEntityAt(const glm::ivec2& pos) const
{
  for (LMap::const_iterator it = _map.begin(), end = _map.end(); it != end; ++it)
    if ((*(*it)).getPos() == pos)
      return *it;
  return NULL;
}

std::vector<APlayer *> Map::getPlayersAt(const glm::ivec2& pos) const
{
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
** Return the Type of an Entity at pos
*/

IEntity::Type	Map::getTypeAt(const glm::ivec2& pos) const
{
  IEntity::Type type = IEntity::NONE;

  for (LMap::const_iterator it = _map.begin(), end = _map.end(); it != end; ++it)
    if ((*(*it)).getPos() == pos)
      {
        type = (*it)->getType();
        if (type == IEntity::PLAYER)
          type = IEntity::NONE;
        if (type != IEntity::NONE)
          return type;
      }
  return type;
}

/*
** Add an entity in the map if it's possible, else return false
*/

bool Map::addEntity(IEntity *entity)
{
  if (entity->getType() != IEntity::PLAYER)
    _map.push_back(entity);
  if (entity->getType() == IEntity::PLAYER) {
      _playerList.push_back(entity);
    }
  else if (entity->getType() != IEntity::WALL)
    _updateList.push_back(entity);
  return true;
}

/*
** Delete an entity in the map at pos if it's possible, else return false
*/

bool	Map::deleteEntityAt(const glm::ivec2& pos)
{
  IEntity *entity;

  for (LMap::iterator it = _map.begin(); it != _map.end(); ++it)
    {
      if ((*(*it)).getPos().x == x && (*(*it)).getPos().y == y) {
          entity = *it;
          delete entity;
          _map.erase(it);
          return true;
        }
    }
  return false;
}
