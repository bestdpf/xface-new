#include "VisitorInfoPrinter.h"
#include <iostream>

void VisitorInfoPrinter::operator()(const boost::shared_ptr<SGGroup>& node) const
{
	std::cerr << "*****Dumping par/seq contents:*****\n";
	node->print();
	std::cerr << "******************************************\n";
	//process children
	std::for_each(node->m_children.begin(), node->m_children.end()
		,boost::apply_visitor(*this));
}

void VisitorInfoPrinter::operator()(const SMILBlock& node) const
{
	node.print();
}