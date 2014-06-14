#ifndef _ABONUS_H_
#define _ABONUS_H_

# include <Clock.hh>
# include "config.h"

# include "IEntity.hpp"
# include "Texture.hpp"
# include "SharedPointer.hpp"
# include "ResourceManager.hpp"
# include "Timer.hpp"
# include "GameGeometry.hpp"
# include "APlayer.hpp"
# include "SoundManager.hpp"

# include <boost/archive/text_oarchive.hpp>
# include <boost/archive/text_iarchive.hpp>
# include <boost/serialization/base_object.hpp>
# include <boost/serialization/export.hpp>

struct bonusConf
{
  bonusConf(const glm::vec4 &color, int time) {
    _time = time;
    _color = color;
  }
  int		_time;
  glm::vec4	_color;
};

class ABonus : public IEntity
{
  friend class boost::serialization::access;

protected:
  enum BonusType
    {
      FASTER = 0,
      MULTI_BOMB,
      BOMB_RANGE,
      FLAMME_PASS,
      BOMB_PASS,
      COIN,
      FIREBALL,
    };

private:
  glm::vec2	_pos;
  GameGeometry	*_obj;
  double	_cube_speed;
  SharedPointer<Texture> _texture;
  std::map<BonusType, std::string> _bonusImg;
protected:
  IEntity::Status _status;
  Timer		_toDisplay, _effectTime;
  BonusType	_type;

public:
  ABonus(BonusType type, const glm::vec2 &pos, double effectTime);
  virtual ~ABonus();
  virtual const glm::vec2 &getPos() const;
  virtual void setPos(const glm::vec2 &new_pos);
  virtual void update(gdl::Input &input, gdl::Clock const &clock);
  virtual void draw(gdl::AShader *shader, const gdl::Clock& clock) const;
  virtual IEntity::Type getType() const;
  virtual IEntity::Status getStatus() const;
  virtual void setStatus(IEntity::Status status);

  virtual void update(APlayer *player, const gdl::Clock &);

  virtual void start(APlayer *player) = 0;
  virtual void takeAnother(APlayer *player) = 0;
  const std::string getTexturePath() const;
  virtual std::string toString() = 0;
  bool	  operator==(const ABonus &);

protected:
  virtual void stop(APlayer *player) = 0;

private:
  template<class Archive>
  void serialize(Archive & ar, UNUSED const unsigned int version)
  {
    boost::serialization::void_cast_register<ABonus, IEntity>(
      static_cast<ABonus*>(NULL),
      static_cast<IEntity*>(NULL)
    );
  }
};

#endif /* _ABONUS_H_ */
