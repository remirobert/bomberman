#ifndef FIREBALL_HPP_
# define FIREBALL_HPP_

# include "SharedPointer.hpp"
# include "Texture.hpp"
# include "Map.hpp"
# include "APlayer.hpp"
# include "GameGeometry.hpp"
# include "ResourceManager.hpp"
# include "BonusFactory.hpp"

# include <boost/archive/text_oarchive.hpp>
# include <boost/archive/text_iarchive.hpp>
# include <boost/serialization/base_object.hpp>
# include <boost/serialization/export.hpp>

class FireBall : public IEntity
{
  friend class boost::serialization::access;

public:
  FireBall() {}
  FireBall(const glm::vec2 &pos, const movementCoef *mcoef, Map *map, APlayer *player);
  virtual ~FireBall();
  virtual const glm::vec2 &getPos() const;
  virtual void	setPos(const glm::vec2 &new_pos);
  virtual void update(gdl::Input &input, gdl::Clock const &clock);
  virtual void	draw(gdl::AShader *shader, const gdl::Clock& clock) const;
  virtual IEntity::Type getType() const;
  virtual IEntity::Status getStatus() const;
  virtual void setStatus(IEntity::Status status);

private:
  void	destroyEntity(IEntity *entity);
  template<class Archive>
  void serialize(Archive & ar, UNUSED const unsigned int version)
  {
    boost::serialization::void_cast_register<FireBall, IEntity>(
      static_cast<FireBall*>(NULL),
      static_cast<IEntity*>(NULL)
    );
  }

private:
  GameGeometry* _obj;
  IEntity::Status _status;
  SharedPointer<Texture> _texture;
  glm::vec2 _pos;
  const movementCoef *_mcoef;
  Map *_map;
  double _speed;
  APlayer *_player;
};

#endif /* !FIREBALL_HPP_ */
