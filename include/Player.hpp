#ifndef PLAYER_HPP_
# define PLAYER_HPP_

# include <string>
# include <map>
# include <list>

# include "APlayer.hpp"
# include "Bomb.hpp"

class Player : public APlayer
{
  friend class boost::serialization::access;

public:
  Player() : APlayer(glm::vec2(0, 0), NULL, glm::vec4(0,0,0,0), "Player") {}
  Player(const glm::vec2& pos, Map *map, bool first, const glm::vec4& color, const std::string &name);
  ~Player();
  virtual void	update(gdl::Input &input, gdl::Clock const &clock);
private:
  template<class Archive>
  void serialize(Archive & ar, UNUSED const unsigned int version)
  {
    ar & boost::serialization::base_object<APlayer>(*this);
  }
};

#endif /* !PLAYER_HPP_ */
