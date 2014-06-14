#ifndef IENTITY_HPP_
# define IENTITY_HPP_

# include <boost/archive/text_oarchive.hpp>
# include <boost/archive/text_iarchive.hpp>
# include <boost/serialization/export.hpp>

# include <Clock.hh>
# include <Input.hh>
# include <AShader.hh>

# include "config.h"

class IEntity
{
  friend class boost::serialization::access;

public:
  enum Type
    {
      BOX = 0,
      BONUS,
      WALL,
      BOMB,
      GROUND,
      FIRE,
      MODEL,
      PLAYER,
      SKYBOX,
      LOGO,
      NONE,
      FIREBALL
    };

  enum Symbol
  {
    S_BOX = 'o',
    S_WALL = '#',
    S_NONE = ' '
  };

  enum Status
    {
      OK = 0,
      BURNING,
      DESTROY,
      REMOVE
    };

  template<class Archive>
  void serialize(UNUSED Archive & ar, UNUSED const unsigned int version)
  {}

  virtual ~IEntity() {}
  virtual const glm::vec2 &getPos() const = 0;
  virtual void setPos(const glm::vec2 &new_pos) = 0;
  virtual void update(gdl::Input &input, gdl::Clock const &clock) = 0;
  virtual void draw(gdl::AShader *shader, const gdl::Clock& clock) const = 0;
  virtual IEntity::Type getType() const = 0;
  virtual IEntity::Status getStatus() const = 0;
  virtual void setStatus(IEntity::Status status) = 0;
};

#endif
