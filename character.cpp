#include "character.hpp"

Character::Character(sf::Vector2f center, sf::Vector2f half_size) : MovingObject(center, half_size, sf::Color(255,255,0))
{
	m_current_state=Stand;
	m_jump_speed=CHAR_JUMP_SPEED;
	m_walk_speed=CHAR_WALK_SPEED;

	for(int i=0; i<NB_KEY_CHARACTER; i++)
	{
		m_inputs[i]=false;
		m_old_inputs[i]=false;
	}
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

void Character::update(sf::Time elapsed, bool inputs[NB_KEY_CHARACTER])
{
	this->updateInputs(inputs);
	switch(m_current_state)
	{
		case Stand:
			m_speed=sf::Vector2f(0,0);
			
			if(!m_on_ground)
				m_current_state=Jump;
			else if(keyState(GoRight) != keyState(GoLeft))
				m_current_state=Walk;
			else if(pressed(GoJump))
			{
				m_speed.y=m_jump_speed;
				m_current_state=Jump;
			}
			if(keyState(GoDown))
			{
				cout << "TEST1" << endl;
				if(m_on_drop_tile)
				{
					cout << "TEST2" << endl;
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
				if(m_pushes_right_wall)
					m_speed.x=0;
				else
					m_speed.x=m_walk_speed;
			}
			else if(keyState(GoLeft))
			{
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

			if(pressed(GoJump))
			{
				m_speed.y=m_jump_speed;
				m_current_state=Jump;
			}
			else if(!m_on_ground)
				m_current_state=Jump;

			break;
		
		case Jump:
			m_speed.y+=GRAVITY*elapsed.asSeconds();
			m_speed.y=std::min(m_speed.y, (float)MAX_FALLING_SPEED);

			if(m_on_ground)
			{
				m_speed.y=0;
				m_current_state=Stand;
			}
			else if(keyState(GoRight) == keyState(GoLeft))
				m_speed.x=0;
			else if(keyState(GoRight))
			{
				if(m_pushes_right_wall)
					m_speed.x=0;
				else
					m_speed.x=m_walk_speed;
			}
			else if(keyState(GoLeft))
			{
				if(m_pushes_left_wall)
					m_speed.x=0;
				else
					m_speed.x=-m_walk_speed;
			}
			if(!keyState(GoJump))
				m_speed.y=std::max(m_speed.y, (float)MIN_JUMP_SPEED);
			break;
	}

	this->updatePhysics(elapsed);
	this->updateOldInputs();
}

void Character::updateInputs(bool inputs[NB_KEY_CHARACTER])
{
	for(int i=0; i<NB_KEY_CHARACTER; i++)
		m_inputs[i]=inputs[i];
}

void Character::updateOldInputs()
{
	for(int i=0; i<NB_KEY_CHARACTER; i++)
		m_old_inputs[i]=m_inputs[i];
}

void Character::debug()
{
	cout << "position x=" << this->getPosition().x << " y=" << this->getPosition().y << endl;
	cout << "m_half_size x= " << m_half_size.x << " y=" << m_half_size.y << endl;
	cout << "speed x=" << m_speed.x << " y=" << m_speed.y << endl;
	cout << "current_state (Stand, Walk, Jump, GrabEdge)=" << m_current_state << endl;
	cout << "inputs ";
	for(int i=0; i<NB_KEY_CHARACTER; i++)
		cout << i << "-" << m_inputs[i] << " ";
	cout << endl;
	cout << "m_pushes_right_wall=" << m_pushes_right_wall << endl;
	cout << "m_pushes_left_wall=" << m_pushes_left_wall << endl;
	cout << "m_on_ground=" << m_on_ground << endl;
	cout << "m_on_drop_tile=" << m_on_drop_tile << endl;
	cout << "m_at_ceiling=" << m_at_ceiling << endl;

	cout << endl;
}
