#ifndef WALL_HPP_
# define WALL_HPP_

# include <boost/serialization/serialization.hpp>
# include <boost/archive/text_oarchive.hpp>
# include <boost/archive/text_iarchive.hpp>
# include <boost/serialization/export.hpp>

# include "Texture.hpp"
# include "SharedPointer.hpp"
# include "ResourceManager.hpp"
# include "IEntity.hpp"
# include "AObject.hpp"
# include "GameGeometry.hpp"

class Wall : public IEntity
{
  friend class boost::serialization::access;

private:
  glm::vec2 _vec;
  GameGeometry* _obj;
  SharedPointer<Texture> _texture;

public:
  Wall() {}
  Wall(const glm::vec2 &pos);
  virtual ~Wall();
  virtual const glm::vec2 &getPos() const;
  virtual void	setPos(const glm::vec2 &new_pos);
  virtual void update(gdl::Input &input, gdl::Clock const &clock);
  virtual void	draw(gdl::AShader *shader, const gdl::Clock& clock) const;
  virtual IEntity::Type getType() const;
  virtual void setStatus(IEntity::Status status);
  virtual IEntity::Status getStatus() const;

  template<class Archive>
  void serialize(UNUSED Archive & ar, UNUSED const unsigned int version)
  {
    boost::serialization::void_cast_register<Wall, IEntity>(
      static_cast<Wall*>(NULL),
      static_cast<IEntity*>(NULL)
    );
  }
};

// BOOST_CLASS_EXPORT_KEY(Wall)

#endif /* !WALL_HPP_ */
