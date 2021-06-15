#include "animation.h"

Animation::Animation()
{

}

void Animation::AddLayer(const SpriteSheet* layer, const std::string& name)
{
	m_Layers[name] = layer;
}
