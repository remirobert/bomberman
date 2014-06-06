#include "EntitiesFactory.hpp"
#include "Map.hpp"
#include "Bomb.hpp"

/*
** constructor random map
*/

Map::Map(const int x, const int y) : _dim(x, y), _mutex(new PMutex)
{
  srand(time(NULL));

  _charToIEntity[IEntity::S_BOX] = IEntity::BOX;
  _charToIEntity[IEntity::S_WALL] = IEntity::WALL;
  _charToIEntity[IEntity::S_NONE] = IEntity::NONE;
  this->loadRandomMap();
}

/*
** constructor map with fileName
*/

Map::Map(std::string const &mapFileName) : _dim(0, 0), _mutex(new PMutex)
{
  _charToIEntity[IEntity::S_BOX] = IEntity::BOX;
  _charToIEntity[IEntity::S_WALL] = IEntity::WALL;
  _charToIEntity[IEntity::S_NONE] = IEntity::NONE;
  this->loadMapFromFile(mapFileName);
}

Map::~Map()
{
  IEntity	*entity;

  while (!_map.empty()) {
    entity = *(_map.begin());
    delete entity;
    _map.pop_front();
  }
  delete _mutex;
}

/*
** Private Methods
*/

IEntity::Type	Map::getType(const char c) const
{
  IEntity::Type	type;

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
	_map.push_back(entity);
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
  IEntity::Type	type;

  for (int i = 0; i < _dim.x; ++i) {
    if (i && (i != _dim.x - 1))
      for (int j = 0; j < _dim.y; ++j) {
	if (j && (j != _dim.y - 1)) {
	  type = static_cast<IEntity::Type>(((rand() % 10) - 3) % 10);
	  entity = this->getEntityForMap(i, j, type);
	  if (entity)
	    _map.push_back(entity);
	}
      }
  }
}

IEntity		*Map::getEntityForMap(const int x, const int y, const IEntity::Type i) const
{
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

  if (_map.size() > 0) {
    for (int i = 0; i < _dim.x; ++i) {
      for (int j = 0; j < _dim.y; ++j) {
  	check = false;
  	for (LMap::const_iterator it = _map.begin(); it != _map.end(); ++it) {
  	  if ((*(*it)).getPos().x == i && (*(*it)).getPos().y == j) {
  	    if (dynamic_cast<Box *>(*it) != NULL)
  	      std::cerr << "o";
  	    else if (dynamic_cast<Wall *>(*it) != NULL)
  	      std::cerr << "#";
  	    else if (dynamic_cast<Ground *>(*it) != NULL)
  	      std::cerr << " ";
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
  ScopeLock	lk(*_mutex);

  for (LMap::const_iterator it = _map.begin(), end = _map.end(); it != end; ++it)
    if ((*(*it)).getPos().x == x && (*(*it)).getPos().y == y)
      return *it;
  return NULL;
}

/*
** Return the Type of an Entity at coor(x, y)
*/

IEntity::Type	Map::getTypeAt(const int x, const int y) const
{
  IEntity::Type type = IEntity::NONE;

  for (LMap::const_iterator it = _map.begin(), end = _map.end(); it != end; ++it)
    if ((*(*it)).getPos().x == x && (*(*it)).getPos().y == y) {
      type = (*it)->getType();
      if (type == IEntity::PLAYER)
	type = IEntity::NONE;
      if (type != IEntity::NONE)
	return type;
    }
  return type;
}

IEntity::Type	Map::getTypeAt(const int x, const int y, std::vector<const IEntity *> list) const
{
  IEntity::Type type = IEntity::NONE;

  for (std::vector<const IEntity *>::const_iterator it = list.begin(), end = list.end();
       it != end; ++it)
    if ((*(*it)).getPos().x == x && (*(*it)).getPos().y == y) {
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

bool		Map::addEntity(IEntity *entity)
{
  ScopeLock	lk(*_mutex);

  /*  for (LMap::const_iterator it = _map.begin(), end = _map.end(); it != end; ++it)
    if ((*(*it)).getPos().x == (*entity).getPos().x &&
	(*(*it)).getPos().y == (*entity).getPos().y)
	return false;*/ /* We can have more than one think one the map */
  _map.push_back(entity);
  return true;
}

/*
** Delete an entity in the map at coord(x, y) if it's possible, else return false
*/

bool	Map::deleteEntityAt(const int x, const int y)
{
  ScopeLock	lk(*_mutex);
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

Map::iterator	Map::end()
{
  return _map.end();
}
