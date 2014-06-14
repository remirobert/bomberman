#ifndef BOX_HPP_
# define BOX_HPP_

# include <boost/archive/text_oarchive.hpp>
# include <boost/archive/text_iarchive.hpp>
# include <boost/serialization/base_object.hpp>
# include <boost/serialization/export.hpp>

# include "Texture.hpp"
# include "SharedPointer.hpp"
# include "IEntity.hpp"
# include "GameGeometry.hpp"

class Box : public IEntity
{
  friend class boost::serialization::access;

private:
  glm::vec2 _vec;
  GameGeometry* _obj;
  SharedPointer<Texture> _texture;
  IEntity::Status _status;

public:
  Box() {}
  Box(const glm::vec2 &pos);
  virtual ~Box();
  virtual const glm::vec2 &getPos() const;
  virtual void setPos(const glm::vec2 &new_pos);
  virtual void update(gdl::Input &input, gdl::Clock const &clock);
  virtual void draw(gdl::AShader *shader, const gdl::Clock& clock) const;
  virtual IEntity::Type getType() const;
  virtual IEntity::Status getStatus() const;
  virtual void setStatus(IEntity::Status status);

  template<class Archive>
  void serialize(Archive & ar, UNUSED const unsigned int version)
  {
    boost::serialization::void_cast_register<Box, IEntity>(
      static_cast<Box*>(NULL),
      static_cast<IEntity*>(NULL)
    );
  }
};

#endif /* !BOX_HPP_ */
