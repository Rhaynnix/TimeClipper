#ifndef DEF_TILE
#define DEF_TILE

#include <SFML/Graphics.hpp>
#include "constants.hpp"

class Tile : public sf::Drawable, public sf::Transformable
{
public:
	Tile(sf::Vector2i pos)
	{
		m_pos=pos;
		this->setPosition(m_pos.x*TILE_SIZE_PIXEL, m_pos.y*TILE_SIZE_PIXEL);
	};

	void setOffsetMap(sf::Vector2f offset)
	{
		this->setPosition(offset.x+m_pos.x*TILE_SIZE_PIXEL, offset.y+m_pos.y*TILE_SIZE_PIXEL);
	};

	bool canGoThroughRight(){return m_can_go_through_right;};
	bool canGoThroughLeft(){return m_can_go_through_left;};
	bool canGoThroughUp(){return m_can_go_through_up;};
	bool canGoThroughDown(){return m_can_go_through_down;};
	
protected:
	sf::Vector2i m_pos;
	bool m_can_go_through_right;
	bool m_can_go_through_left;
	bool m_can_go_through_up;
	bool m_can_go_through_down;
	
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const=0;
};

#endif

