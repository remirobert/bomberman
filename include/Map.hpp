#ifndef MAP_HPP_
# define MAP_HPP_

# include <stdexcept>
# include <string>
# include <list>
# include <iostream>
# include <cstdlib>
# include <ctime>
# include <map>
# include <fstream>

# include "IEntity.hpp"
# include "Wall.hpp"
# include "Box.hpp"

class Map
{
public:
  typedef std::list<IEntity *> LMap;
  typedef std::list<IEntity *>::iterator iterator;
  typedef std::list<IEntity *>::const_iterator const_iterator;

private:
  glm::vec2	_dim;
  LMap		_map;
  LMap		_playerList;
  std::map<char, IEntity::Type>	_charToIEntity;

public:
  Map(const int x, const int y);
  Map(std::string const &mapFileName);
  ~Map();
  Map::LMap	&getMap();
  const glm::vec2 &getDimension() const;
  IEntity	*getEntityAt(const int x, const int y) const;
  IEntity	*getPlayerAt(const int x, const int y) const;
  bool		addEntity(IEntity *entity);
  bool		deleteEntityAt(const int x, const int y);

  IEntity::Type	getTypeAt(const int x, const int y) const;
  IEntity::Type	getTypeAt(const int x, const int y, std::vector<const IEntity *> list) const;

  Map::LMap	&getPlayerList();
  Map::iterator	begin();
  Map::const_iterator	begin() const;
  Map::iterator	playerBegin();
  Map::const_iterator	playerBegin() const;
  Map::iterator	end();
  Map::const_iterator	end() const;
  Map::iterator	playerEnd();
  Map::const_iterator	playerEnd() const;

private:
  bool		loadMapFromFile(std::string const &fileName);
  void		loadRandomMap();
  IEntity::Type	getType(const char c) const;
  IEntity	*getEntityForMap(const int x, const int y, const IEntity::Type i) const;

  //Debug methods
public:
  void	displayDebugMap() const;
};

#endif /* !MAP_HPP_ */
