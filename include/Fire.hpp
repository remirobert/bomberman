#ifndef FIRE_HPP_
# define FIRE_HPP_

# include <boost/archive/text_oarchive.hpp>
# include <boost/archive/text_iarchive.hpp>
# include <boost/serialization/base_object.hpp>
# include <boost/serialization/export.hpp>

# include "SharedPointer.hpp"
# include "Texture.hpp"
# include "IEntity.hpp"
# include "GameGeometry.hpp"
# include "ResourceManager.hpp"

class Fire : public IEntity
{
  friend class boost::serialization::access;

private:
  glm::vec2 _vec;
  GameGeometry* _obj;
  IEntity::Status _status;
  SharedPointer<Texture> _texture;

public:
  Fire() {}
  Fire(const glm::vec2 &pos);
  virtual ~Fire();
  virtual const glm::vec2 &getPos() const;
  virtual void	setPos(const glm::vec2 &new_pos);
  virtual void update(gdl::Input &input, gdl::Clock const &clock);
  virtual void	draw(gdl::AShader *shader, const gdl::Clock& clock) const;
  virtual IEntity::Type getType() const;
  virtual IEntity::Status getStatus() const;
  virtual void setStatus(IEntity::Status status);

private:
  template<class Archive>
  void serialize(Archive & ar, UNUSED const unsigned int version)
  {
    boost::serialization::void_cast_register<Fire, IEntity>(
      static_cast<Fire*>(NULL),
      static_cast<IEntity*>(NULL)
    );
  }
};

#endif /* !FIRE_HPP_ */
