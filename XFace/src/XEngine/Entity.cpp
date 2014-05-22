#include <XEngine/Entity.h>

namespace XEngine{

Entity::~Entity(void)
{
	release();
}

void Entity::release(bool destroyData)
{
	// clear the drawables
	if(destroyData){
		std::list<boost::shared_ptr<Drawable> >::const_iterator it = m_drawables.begin();
		while(m_drawables.end() != it){
			(*it)->destroyData();
			++it;
		}
	}
	m_drawables.clear();
}

Entity& Entity::copyFrom(const Entity& rhs, bool duplicateData)
{
	// self assignment control
	if (this == &rhs) 
		return *this;

	m_drawables.clear();
	m_transform = rhs.m_transform;
	std::list<boost::shared_ptr<Drawable> >::const_iterator it = rhs.m_drawables.begin();
	while ( it != rhs.m_drawables.end() ) {
		boost::shared_ptr<Drawable> dr((*it)->clone(duplicateData));
		addDrawable(dr);
		++it;
	}
	
	return *this;
}

void Entity::resetDeformedVertices()
{
	std::list<boost::shared_ptr<Drawable> >::const_iterator it = m_drawables.begin();
	while(m_drawables.end() != it){
		(*it)->resetDeformedVertices();
		++it;
	}
}


} // namespace XEngine