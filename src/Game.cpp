#include <sstream>
#include <iomanip>

#include "EntitiesFactory.hpp"
#include "Game.hpp"
#include "AShader.hh"
#include "ArmagetroCam.hpp"
#include "FreeCam.hpp"
#include "FpsCam.hpp"
#include "TrackCam.hpp"
#include "OrthoCam.hpp"
#include "config.h"
#include <Geometry.hh>

Game::Game(const glm::ivec2& win, std::string const &saveGame)
{
  if (saveGame == "")
    throw nFault("The file name of the game is too short");
  /* TODO : load the saved game in saveGame*/
  init(win);
}

Game::Game(const glm::ivec2& win, int numberPlayer, int numberIA, std::string const & algoFileName,
           std::string const &mapName)
{

  if (numberIA < 0 || numberPlayer < 0)
    throw nFault("You need two players");

  _currentMap = new Map(mapName);
  Placement place(_currentMap);

  for (int i = 0; i < numberPlayer; ++i)
    {
      _players.push_back(new Player(place.getNewPos(), _currentMap));
    }

  for (int i = 0; i < numberIA; ++i)
    {
      _players.push_back(new Ia(_currentMap, place.getNewPos(), algoFileName));
    }

  // for (std::vector<Ia *>::iterator it = _listIA.begin() ; it != _listIA.end(); ++it)
  //   if (_currentMap->addEntity(*it) != true)
  //     throw nFault("Error in the initializiation of the map");
  // for (std::vector<Player *>::iterator it = _players.begin() ; it != _players.end(); ++it)
  //   if (_currentMap->addEntity(*it) != true)
  //     throw nFault("Error in the initializiation of the map");


  init(win);
}

void Game::init(const glm::ivec2& win)
{
  /* TODO : init game and load 3d models */
  _win = win;
  glm::vec2 playerPos = _players.front()->getPos();

  //_cam = new FreeCam;
  _cam = new BasicCam(glm::vec3(playerPos.x, playerPos.y, 0), 10, 3);
  //_cam = new TrackCam(glm::vec3(_currentMap->getDimension().x / 2, 0.0, _currentMap->getDimension().y / 2));

  _ground = new Pan(_currentMap->getDimension() / glm::vec2(4, 4));

  _ground->initialize();
  _ground->scale(glm::vec3(0.5f, 0.5f, 1.0f));
  _ground->translate(glm::vec3(-0.5f, 0, -0.5f));
  _ground->scale(glm::vec3(_currentMap->getDimension().x, _currentMap->getDimension().y, 1.0f));

  _ground->translate(glm::vec3(static_cast<float>(_currentMap->getDimension().x) / 2.0, -0.5f,
                               static_cast<float>(_currentMap->getDimension().y) / 2.0));
  _ground->rotate(glm::vec3(1, 0, 0), 90.0);

  _ortho = glm::scale(glm::translate(glm::mat4(1), glm::vec3(-1.0, -1.0, -1.0)), glm::vec3(2.0, 2.0, 2.0));

  _font = new FontText(RES_TEXTURE "font.tga");
  _ogl.init(win);
}

Game::~Game()
{
  delete _cam;
}

bool Game::updateGame(gdl::Input &input, const gdl::Clock &clock)
{
  std::list<Map::iterator> listMapToDelete;

  // update map entity
  for (Map::iterator it = _currentMap->begin(); it != _currentMap->end(); ++it) {
      (*it)->update(input, clock);

      for (std::vector<APlayer *>::iterator p = _players.begin(), pend = _players.end();
      	   p != pend; ++p)
      	{
      	  glm::vec2 posObject = (*it)->getPos();
      	  APlayer *player = *p;

      	  if ((posObject.x < player->getPos().x + 2
      	       && posObject.x > player->getPos().x - 2
      	       && posObject.y < player->getPos().y + 2
      	       && posObject.y > player->getPos().y - 2))
	    {
	      player->addEntityAround(*it);
	    }
      	}

      if ((*it)->getStatus() == IEntity::DESTROY)
        listMapToDelete.push_back(it);
    }

  for (std::vector<APlayer *>::const_iterator it = _players.begin(), end = _players.end();
       it != end; ++it)
    {
      (*it)->update(input, clock);
      (*it)->clearEntityAround();
    }
  //Delete every elements which are DESTROYs
  while (!listMapToDelete.empty()) {
      delete *listMapToDelete.front();
      _currentMap->getMap().erase(listMapToDelete.front());
      listMapToDelete.pop_front();
    }

  glm::vec2 playerPos = _players.front()->getPos();
  _cam->update(glm::vec3(playerPos.x, playerPos.y, 0));
  // _cam->update(input, clock);
  _skybox.setPosition(_cam->getPosition());
  _skybox.rotate(glm::vec3(1, 1, 0.6), 0.02f);

  return true;
}

void Game::drawGraphicObject(gdl::AShader* shader, gdl::Clock const &clock) const
{
  glDisable(GL_CULL_FACE);
//Graphic objects
  AssetsManager::getInstance()->getAssets<gdl::Texture>(IEntity::GROUND)->bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  _ground->draw(shader, clock);
}

void Game::drawGame(UNUSED gdl::Input &input, gdl::Clock const &clock)
{
  gdl::AShader *shader = _ogl.getShader();
  glm::vec2 posObject(0, 0);
  int rayon = 9;

  shader->setUniform("camPos", _cam->getPosition());
  shader->setUniform("view", _cam->project());
  shader->setUniform("projection", _ogl.getPerspectiveProj());

  glEnable(GL_CULL_FACE);

 // glViewport(0, 0, _win.x / 2, _win.y);

  glDisable(GL_DEPTH_TEST);
  _skybox.draw(shader, clock);
  glEnable(GL_DEPTH_TEST);

  shader->setUniform("lightDir", glm::vec3(0.1, 0.6, -0.3));
//game entities
  glm::vec2 posPlayer = _players[0]->getPos();

  for (Map::iterator it = _currentMap->begin(); it != _currentMap->end(); ++it)
    {
      posObject = (*it)->getPos();
      if ((posObject.x < posPlayer.x + rayon && posObject.x > posPlayer.x - rayon && posObject.y < posPlayer.y + rayon && posObject.y > posPlayer.y - rayon))
        (*it)->draw(shader, clock);
   }

  // players drawing
  for (std::vector<APlayer *>::const_iterator it = _players.begin(), end = _players.end();
       it != end; ++it)
    {
      (*it)->draw(shader, clock);
    }

  drawGraphicObject(shader, clock);

 /* glViewport(_win.x / 2, 0, _win.x / 2, _win.y);

  shader->setUniform("lightDir", glm::vec3(0.0f, 0.0f, 0.0f));

  glDisable(GL_DEPTH_TEST);
  _skybox.draw(shader, clock);
  glEnable(GL_DEPTH_TEST);

  shader->setUniform("lightDir", glm::vec3(0.1, 0.6, -0.3));
//game entities
  for (Map::iterator it = _currentMap->begin(); it != _currentMap->end(); ++it)
    {
      posObject = (*it)->getPos();
      if ((posObject.x < posPlayer.x + rayon && posObject.x > posPlayer.x - rayon && posObject.y < posPlayer.y + rayon && posObject.y > posPlayer.y - rayon))
        (*it)->draw(shader, clock);
    }
  drawGraphicObject(shader, clock);

  glViewport(0, 0, _win.x, _win.y);*/

//Render object
  glEnable(GL_CULL_FACE);

//hud object
  shader->setUniform("lightDir", glm::vec3(0.0f, 0.0f, 0.0f));
  shader->setUniform("view", _ortho);
  shader->setUniform("projection", glm::mat4(1));

  glm::mat4 textMat;

  std::stringstream ss;
  static double elapsed = 0.0;
  static unsigned int k = 0;
  ++k;
  if (k % 20 == 0)
    elapsed = clock.getElapsed();
  ss << std::setprecision(2) << 1.0 / elapsed << " FPS";

  textMat = glm::translate(glm::mat4(1), glm::vec3(0.8, 0.97, 0.0));
  textMat = glm::scale(textMat, glm::vec3(0.5, 0.5, 0.0));
  _font->displayText(ss.str(), (elapsed <= 0.017) ? glm::vec4(0.0f, 1.0f, 0.0f, 0.8f) : glm::vec4(1.0f, 0.0f, 0.0f, 0.8f), textMat, shader);

}
