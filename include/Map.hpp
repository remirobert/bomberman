#ifndef MAP_HPP_
# define MAP_HPP_

# include <stdexcept>
# include <string>
# include <list>
# include <vector>
# include <iostream>
# include <cstdlib>
# include <ctime>
# include <map>
# include <fstream>

# include <boost/archive/binary_oarchive.hpp>
# include <boost/archive/binary_iarchive.hpp>
# include <boost/serialization/list.hpp>
# include <boost/serialization/vector.hpp>

# include "config.h"
# include "IEntity.hpp"
# include "Wall.hpp"
# include "Box.hpp"

class APlayer;

class Map
{
  friend class boost::serialization::access;

private:
  glm::ivec2 _dim;

  std::vector<IEntity*> _map;
  std::list<IEntity*> _updateList;

  std::map<char, IEntity::Type> _charToIEntity;

public:
  Map(const glm::ivec2& dim);
  Map(std::string const &mapFileName);
  ~Map();

  const glm::ivec2 &getDimension() const;
  IEntity *getEntityAt(const glm::ivec2& pos) const;
  std::vector<APlayer *> getPlayersAt(const glm::ivec2& pos) const;
  bool addEntity(IEntity *entity);
  bool deleteEntityAt(const glm::ivec2& pos);
  IEntity::Type getTypeAt(const glm::ivec2& pos) const;

public:
  template<class Archive>
  void serialize(Archive & ar, UNUSED const unsigned int version)
  {
    ar & _dim.x;
    ar & _dim.y;
    for (std::vector<IEntity*>::const_iterator it = _map.begin(), end = _map.end();
         it != end; it++)
      {
        int type = static_cast<int>((*it)->getType());
        int status = static_cast<int>((*it)->getStatus());
        float x = ((*it)->getPos()).x;
        float y = ((*it)->getPos()).y;

        ar & x;
        ar & y;
        ar & type;
        ar & status;
      }
    for (std::list<IEntity*>::const_iterator it = _updateList.begin(), end = _updateList.end();
         it != end; it++)
      {
        int type = static_cast<int>((*it)->getType());
        int status = static_cast<int>((*it)->getStatus());
        float x = ((*it)->getPos()).x;
        float y = ((*it)->getPos()).y;

        ar & x;
        ar & y;
        ar & type;
        ar & status;
      }
  }

private:
  bool loadMapFromFile(std::string const &fileName);
  void loadRandomMap();
  IEntity::Type getType(char c) const;
  IEntity *getEntityForMap(const glm::ivec2& pos, IEntity::Type i) const;

  size_t twoDToArray(const glm::ivec2& pos) const;

};

#endif /* !MAP_HPP_ */
