#pragma once

#include "widget.hpp"

class Visual_item : public Widget
{
private:
	const sf::Texture texture;

public:
	Visual_item(const std::string& name, const sf::Texture& texture, sf::Vector2f position = { 0, 0 },
        Anchor origin = Anchor::TopLeft, Anchor anchor = Anchor::TopLeft);

	void set_texture(sf::Texture texture);


};