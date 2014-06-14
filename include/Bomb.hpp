#ifndef BOMB_HPP_
# define BOMB_HPP_

# include <Texture.hh>
# include <stdlib.h>
# include <cstdlib>

# include <boost/archive/text_oarchive.hpp>
# include <boost/archive/text_iarchive.hpp>
# include <boost/serialization/export.hpp>

# include "Map.hpp"
# include "Cube.hpp"
# include "config.h"
# include "Model.hpp"
# include "Timer.hpp"
# include "Fire.hpp"
# include "APlayer.hpp"
# include "SoundManager.hpp"
# include "SharedPointer.hpp"
# include "ResourceManager.hpp"
# include "BonusFactory.hpp"

class Bomb : public IEntity
{
  friend class boost::serialization::access;

  typedef std::list<Fire *> FireList;

private:
  APlayer *_player;
  glm::vec2 _vec;
  Map *_map;
  FireList _fireList;
  Timer _time;
  Timer _staytime;
  double _range;
  GameModel *_obj;
  IEntity::Status _status;
  int _speed;
  double _distance;
  int _distanceCovered;
  bool _spreadTop;
  bool _spreadDown;
  bool _spreadLeft;
  bool _spreadRight;

  std::vector<ABonus *> _generatedBonus;
public:
  Bomb() : _time(2.5), _staytime(0.25) {}
  Bomb(APlayer *player, const glm::vec2 &pos, int range, Map *map);
  virtual ~Bomb();
  virtual const glm::vec2 &getPos() const;
  virtual void	setPos(const glm::vec2 &new_pos);
  virtual void	update(gdl::Input &input, gdl::Clock const &clock);
  virtual void draw(gdl::AShader *shader, const gdl::Clock& clock) const;
  virtual IEntity::Type getType() const;
  virtual IEntity::Status getStatus() const;
  virtual void setStatus(IEntity::Status status);
private:
  void	explode(gdl::Clock const &clock);
  bool	destroyEntity(const glm::vec2 &);
  bool	spread();

private:
  template<class Archive>
  void serialize(Archive & ar, UNUSED const unsigned int version)
  {
    boost::serialization::void_cast_register<Bomb, IEntity>(
      static_cast<Bomb*>(NULL),
      static_cast<IEntity*>(NULL)
    );
  }
};

#endif /* !BOMB_HPP_ */
