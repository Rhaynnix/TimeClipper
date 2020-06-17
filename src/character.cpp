#include "../include/character.hpp"

Character::Character(sf::Vector2f center, sf::Vector2f half_size, bool* inputs) : MovingObject(center, half_size, sf::Color(255,255,255), "bob", false)
{
	m_current_state=Stand;
	m_jump_speed=CHAR_JUMP_SPEED;
	m_jump_count=0;
	m_walk_speed=CHAR_WALK_SPEED;

	m_inputs=inputs;

	m_texture.loadFromFile("rsc/newboi.png");
	m_sprite.setTexture(m_texture);

	for(int i=0; i<NB_KEY_CHARACTER; i++)
		m_old_inputs[i]=false;
}

bool Character::released(KeyInputChar key)
{
	return (!m_inputs[(int)key] && m_old_inputs[(int)key]);
}

bool Character::keyState(KeyInputChar key)
{
	return (m_inputs[(int)key]);
}

bool Character::pressed(KeyInputChar key)
{
	return (m_inputs[(int)key] && !m_old_inputs[(int)key]);
}

void Character::update(sf::Time elapsed)
{
	/*if(pressed(SizeUp))
		this->sizeUp();
	if(pressed(SizeDown))
		this->sizeDown();*/

	switch(m_current_state)
	{
		case Stand:
			m_speed=sf::Vector2f(0,0);

			if(!m_on_ground)
				m_current_state=Jump;
			else if(pressed(GoJump))
			{
				m_speed.y=m_jump_speed;
				m_current_state=Jump;
			}
			else if(keyState(GoRight) != keyState(GoLeft))
				m_current_state=Walk;
			if(keyState(GoDown))
			{
				if(m_on_drop_tile)
				{
					this->move(sf::Vector2f(0,DROP_TILE_THRESHOLD));
				}
			}
			break;
		
		case Walk:
			if(keyState(GoRight) == keyState(GoLeft))
			{
				m_current_state=Stand;
				m_speed=sf::Vector2f(0,0);
			}
			else if(keyState(GoRight))
			{
				this->flipSpriteRight();
				if(m_pushes_right_wall)
					m_speed.x=0;
				else
					m_speed.x=m_walk_speed;
			}
			else if(keyState(GoLeft))
			{
				this->flipSpriteLeft();
				if(m_pushes_left_wall)
					m_speed.x=0;
				else
					m_speed.x=-m_walk_speed;
			}
			
			if(keyState(GoDown))
			{
				if(m_on_drop_tile)
					this->move(sf::Vector2f(0,DROP_TILE_THRESHOLD));
			}
			else if(pressed(GoJump))
			{
				m_speed.y=m_jump_speed;
				m_current_state=Jump;
			}
			else if(!m_on_ground)
			{
				m_current_state=Jump;
				m_jump_count=CHAR_JUMP_AMOUNT;
			}

			break;
		
		case Jump:
			if(m_on_ground)
			{
				m_speed.y=0;
				m_jump_count=0;
				if(keyState(GoRight) == keyState(GoLeft))
					m_current_state=Stand;
				else
					m_current_state=Walk;
				break;
			}
			else
			{
				m_speed.y+=GRAVITY*elapsed.asSeconds();
				m_speed.y=std::min(m_speed.y, (float)MAX_FALLING_SPEED);
			}

			if(keyState(GoRight) == keyState(GoLeft))
				m_speed.x=0;
			else if(keyState(GoRight))
			{
				this->flipSpriteRight();
				if(m_pushes_right_wall)
					m_speed.x=0;
				else
					m_speed.x=m_walk_speed;
			}
			else if(keyState(GoLeft))
			{
				this->flipSpriteLeft();
				if(m_pushes_left_wall)
					m_speed.x=0;
				else
					m_speed.x=-m_walk_speed;
			}
			
			if(!keyState(GoJump))
				m_jump_count=CHAR_JUMP_AMOUNT;
			else
			{
				if(m_jump_count<CHAR_JUMP_AMOUNT)
				{
					if(this->getAtCeiling())
						m_speed.y=0;
					else
						m_speed.y=m_jump_speed;
				}
				m_jump_count+=elapsed.asSeconds();
			}
			break;

		default:
			break;
	}

	this->updatePhysics(elapsed);
	this->updateOldInputs();
}

void Character::updateOldInputs()
{
	for(int i=0; i<NB_KEY_CHARACTER; i++)
		m_old_inputs[i]=m_inputs[i];
}

void Character::sizeDown()
{
	m_half_size.x-=5;
	m_half_size.y-=5;

	if(m_half_size.x <= 0 || m_half_size.y <= 0)
	{
		m_half_size.x=5;
		m_half_size.y=5;
	}

	/*m_vertices[0]=sf::Vector2f(-m_half_size.x, -m_half_size.y);
	  m_vertices[1]=sf::Vector2f(m_half_size.x, -m_half_size.y);
	  m_vertices[2]=sf::Vector2f(m_half_size.x, m_half_size.y);
	  m_vertices[3]=sf::Vector2f(-m_half_size.x, m_half_size.y);*/

	m_hitbox.setHalfSize(m_half_size);
}

void Character::sizeUp()
{
	m_half_size.x+=5;
	m_half_size.y+=5;

	/*m_vertices[0]=sf::Vector2f(-m_half_size.x, -m_half_size.y);
	  m_vertices[1]=sf::Vector2f(m_half_size.x, -m_half_size.y);
	  m_vertices[2]=sf::Vector2f(m_half_size.x, m_half_size.y);
	  m_vertices[3]=sf::Vector2f(-m_half_size.x, m_half_size.y);*/

	m_hitbox.setHalfSize(m_half_size);
}

void Character::debug()
{
	cout << endl;
	cout << "position x=" << this->getPosition().x << " y=" << this->getPosition().y << endl;
	cout << "speed x=" << m_speed.x << " y=" << m_speed.y << endl;
	cout << "m_half_size x= " << m_half_size.x << " y=" << m_half_size.y << endl;
	cout << "current_state (Stand, Walk, Jump, GrabEdge)=" << m_current_state << endl;
	cout << "inputs ";
	for(int i=0; i<NB_KEY_CHARACTER; i++)
		cout << i << "-" << m_inputs[i] << " ";
	cout << endl;
/*	cout << "m_pushes_right_wall=" << m_pushes_right_wall << endl;
	cout << "m_pushes_left_wall=" << m_pushes_left_wall << endl;
	cout << "m_on_ground=" << m_on_ground << endl;
	cout << "m_on_drop_tile=" << m_on_drop_tile << endl;
	cout << "m_at_ceiling=" << m_at_ceiling << endl;*/
	cout << "m_pushes_right=" << m_pushes_right << endl;
	cout << "m_pushes_left=" << m_pushes_left << endl;
	cout << "m_pushes_top=" << m_pushes_top << endl;
	cout << "m_pushes_bottom=" << m_pushes_bottom << endl;
	cout << "m_pushes_right_obj=" << m_pushes_right_obj << endl;
	cout << "m_pushes_left_obj=" << m_pushes_left_obj << endl;
	cout << "m_pushes_top_obj=" << m_pushes_top_obj << endl;
	cout << "m_pushes_bottom_obj=" << m_pushes_bottom_obj << endl;
	cout << "m_jump_count=" << m_jump_count << endl;
	cout << "Collision : ";
	for(unsigned int i=0; i<m_all_colliding_objects.size(); i++)
		cout << m_all_colliding_objects[i].m_other->getName() << " ";
	cout << endl;
}
