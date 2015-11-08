//
//  Wall2D.cpp
//  ZombieHero
//
//  Created by haxpor on 3/10/14.
//
//

#include "Wall2D.h"

using namespace cocos2d;
using namespace harp;

Wall2D::Wall2D()
{
}

Wall2D::Wall2D(const CCPoint& a, const CCPoint& b):
    m_A(a),
    m_B(b)
{
    this->calculateNormal();
}

Wall2D::Wall2D(const CCPoint& a, const CCPoint& b, const CCPoint& n):
    m_A(a),
    m_B(b),
    m_N(n)
{
}

const CCPoint& Wall2D::from() const
{
    return m_A;
}

void Wall2D::setFrom(const CCPoint& v)
{
    m_A = v;
    this->calculateNormal();
}

const CCPoint& Wall2D::to() const
{
    return m_B;
}

void Wall2D::setTo(const CCPoint& v)
{
    m_B = v;
    this->calculateNormal();
}

const CCPoint& Wall2D::normal() const
{
    return m_N;
}

void Wall2D::setNormal(const CCPoint& v)
{
    m_N = v;
}

CCPoint Wall2D::center() const
{
    return ccpMult(ccpAdd(m_A, m_B), 0.5f);
}

void Wall2D::calculateNormal()
{
    CCPoint temp = ccpNormalize(ccpSub(m_B, m_A));
    
    m_N.setPoint(-temp.y, temp.x);
}