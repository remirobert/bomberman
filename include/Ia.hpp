#ifndef IA_HPP_
#define IA_HPP_

#include <string>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "Fault.hpp"
#include "PMutex.hpp"
#include "PCondVar.hpp"
#include "PThread.hpp"
#include "APlayer.hpp"

class Ia : public APlayer
{
private:

  lua_State *_L;
  int _act;
  PMutex _mutex;
  PCondVar _condAct;
  PThread _thread;
  bool _dead;
  bool _running;
  std::string _fileName;
  std::map<int, int> _actToSdlKey;

public:
  int getMap(int x, int y) const;
  const glm::vec2& getPos() const;

  void update(gdl::Input &input, gdl::Clock const &clock);
  void action(int act);
  void *init();
  bool nothing(UNUSED double const distance);
  const glm::vec2& getMapDimension() const;
public:
  Ia(Map *currentMap, glm::vec2 const &pos, std::string const &fileName);
  ~Ia();
  int exec(); /*!BLOQUANT! call it when the ia can make one action */
};

#endif
