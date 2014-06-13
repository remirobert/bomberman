#include <sstream>
#include "PlayerManager.hpp"

#include "BasicCam.hpp"

PlayerManager::PlayerManager(const glm::vec2& pos, Map *map, bool first, const glm::vec4& color,
			     const std::string &name)
  : _player(pos, map, first, color, name)
{
  _score = 0;
  _timer = 5.0;
  _first = first;
  _win = false;
  glm::vec2 playerPos = _player.getPos();
  _cam = new BasicCam(glm::vec3(playerPos.x, playerPos.y, 0), 13, 3);
}

PlayerManager::~PlayerManager()
{
  delete _cam;
}

void PlayerManager::update(const Map& map, const gdl::Clock &clock)
{
  glm::vec2 playerPos = _player.getPos();


  if (_player.getStatus() != IEntity::DESTROY)
    _cam->update(glm::vec3(playerPos.x, playerPos.y, 0));
  else
    _timer -= clock.getElapsed();
  updateNearList(map);
}

void PlayerManager::updateNearList(const Map& map)
{
  _nearEntity.clear();

  glm::vec2 posPlayer = _player.getPos();
  glm::vec2 posObject(0, 0);
  int rayon = 17;

//  for (Map::const_iterator it = map.begin(), end = map.end();
//       it != end; ++it)
//    {
//      posObject = (*it)->getPos();
//      if ((posObject.x < posPlayer.x + rayon && posObject.x > posPlayer.x - rayon && posObject.y < posPlayer.y + rayon && posObject.y > posPlayer.y - rayon))
//        _nearEntity.push_front((*it));
//    }

  Map::UMap uList = map.getUpdateList();
  for (Map::const_iterator it = uList.begin(), end = uList.end();
       it != end; ++it)
    {
      posObject = (*it)->getPos();
      if ((posObject.x < posPlayer.x + rayon && posObject.x > posPlayer.x - rayon && posObject.y < posPlayer.y + rayon && posObject.y > posPlayer.y - rayon))
        _nearEntity.push_front((*it));
    }
}

void	PlayerManager::displayInfo(const FontText& font, const gdl::Clock &clock, gdl::AShader *shader) const
{
  const std::vector<ABonus *> bonus = _player.getBonus();
  glm::mat4 textMat;
  double line = 0.0f;
  SharedPointer<Texture> texture;

  textMat = glm::translate(glm::mat4(1), glm::vec3(0 + (_first ? 0 : 0.5), 0.97, 0));
  textMat = glm::scale(textMat, glm::vec3(0.35, 0.45, 0.0));

  std::stringstream ss("");
  ss << _player.getName() << ": " << _player.getScores() << " P";

  font.displayText(ss.str().c_str(), glm::vec4(0.0f, 1.0, 0.0f, 0.8f), textMat, shader);


  ++line;

  textMat = glm::translate(glm::mat4(1), glm::vec3(0 + (_first ? 0.0 : 0.50), 0.5, 0));
  textMat = glm::scale(textMat, glm::vec3(0.9, 1.0, 0.0));
  if (_timer < 4.5)
    font.displayText((_win ? "You Win !" : "Game Over"), glm::vec4(1.0f, 0.0, 0.0f, 1.0f), textMat, shader);

  for (std::vector<ABonus *>::const_iterator it = bonus.begin();
       it != bonus.end(); ++it)
    {
      GameGeometry *pan = new GameGeometry(ResourceManager::getInstance()->get<AGeometry>("pan.geo"));

      pan->scale(glm::vec3(0.008, 0.010, 0));
      pan->rotate(glm::vec3(0,0,1), 180);

      texture = ResourceManager::getInstance()->get<Texture>((*it)->getTexturePath());

      pan->translate(glm::vec3(0.02f + (_first ? 0 : 0.5), 1.0f - line / 45.f - 0.019, 0.0));

      texture->bind();
      pan->draw(shader, clock);

      textMat = glm::translate(glm::mat4(1), glm::vec3(0.03 + (_first ? 0 : 0.5), 1.0f - line / 45.f - 0.03, 0));
      textMat = glm::scale(textMat, glm::vec3(0.35, 0.40, 0.0));

      font.displayText((*it)->toString().c_str(), glm::vec4(0.0f, 1.0, 0.0f, 0.8f), textMat, shader);
      line++;
    }
}

bool PlayerManager::getDead(LeaderScores *scores) const
{
  if (_timer <= 0.0)
    {
      scores->pushLeader(_player.getName(), _player.getScores());
      return true;
    }
  return false;
}

void PlayerManager::setWin()
{
  if (_player.getStatus() != IEntity::DESTROY)
    _win = true;
}
