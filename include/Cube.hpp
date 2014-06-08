#ifndef _CUBE_H_
# define _CUBE_H_

# include <iostream>
# include <Texture.hh>
# include <Geometry.hh>

# include "IEntity.hpp"
# include "AObject.hpp"

class Cube : public AObject
{
public:
  Cube(gdl::Geometry* geometry);
  Cube(const Cube& c);
  const Cube& operator=(const Cube& c);

  Cube();
  virtual ~Cube();

  virtual void draw(gdl::AShader *shader, const gdl::Clock &clock);

private:
  gdl::Geometry* _geometry;
};

#endif /* _CUBE_H_ */
