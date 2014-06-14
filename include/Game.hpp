#ifndef GAME_HPP_
# define GAME_HPP_

# include <vector>
# include <string>
# include <list>

# include <boost/archive/text_oarchive.hpp>
# include <boost/archive/text_iarchive.hpp>
# include <boost/serialization/list.hpp>
# include <boost/serialization/vector.hpp>
# include <Input.hh>
# include <Clock.hh>
# include <Texture.hh>

# include "config.h"
# include "Graphics.hpp"
# include "ArmagetroCam.hpp"
# include "BasicCam.hpp"
# include "Map.hpp"
# include "Ia.hpp"
# include "PlayerManager.hpp"
# include "Fault.hpp"
# include "FontText.hpp"
# include "Cube.hpp"
# include "Pan.hpp"
# include "Placement.hpp"
# include "SkyBox.hpp"
# include "Pause.hpp"
# include "LeaderScores.hpp"

class Game
{
  friend class boost::serialization::access;

public:
  Game(const glm::ivec2& win, const std::string &saveGame);
  Game(const glm::ivec2& win,
       int numberPlayer,
       int numberIA,
       const std::string &algoFileName,
       const std::string names[2],
       LeaderScores *leaderScores,
       const std::string &mapName = "");
  ~Game();

public:
  bool updateGame(gdl::Input &input, const gdl::Clock &clock);
  void drawGame(gdl::Clock const &clock);

private:
  void init(const glm::ivec2& win);

  void drawHud(gdl::AShader *shader, gdl::Clock const &clock) const;

public:
    template<class Archive>
    void save(Archive & ar, UNUSED unsigned int version) const
    {
      ar & _listIA;
      ar & _players;
      ar & _currentMap;
    }

    template<class Archive>
    void load(UNUSED Archive & ar, UNUSED unsigned int version)
    {
      // ar & _currentMap;
    }
  BOOST_SERIALIZATION_SPLIT_MEMBER()

private:
  std::vector<Ia *> _listIA;
  std::vector<PlayerManager*> _players;
  Pause _pause;
  Map *_currentMap;
  GameGraphics _ogl;
  LeaderScores *_leader;
  SharedPointer<Texture> _groundTex;
  bool _isPaused;
};


#endif
