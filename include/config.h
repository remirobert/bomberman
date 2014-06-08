#ifndef _CONFIG_H_
# define _CONFIG_H_

# include "SharedPointer.hpp"
# include "Texture.hpp"
# include "Model.hpp"

# define UNUSED __attribute__((unused))

# define RES_ASSETS "./res/assets/"

# define RES_MODEL RES_ASSETS "models/"
# define RES_TEXTURE RES_ASSETS "textures/"
# define RES_SOUND RES_ASSETS "music/"

# define RES_SHADERS "./res/shaders/"

namespace bomberman
{
typedef SharedPointer<Model> SPModel;
typedef SharedPointer<Texture> SPTexture;
};

#endif /* _CONFIG_H_ */
