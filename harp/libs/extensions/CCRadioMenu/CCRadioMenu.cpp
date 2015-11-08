//
//  CCRadioMenu.cpp
//  ZombieHero
//
//  Created by haxpor on 2/9/14.
//
//

#include "CCRadioMenu.h"

using namespace cocos2d;
void CCRadioMenu::setSelectedRadioItem(CCMenuItem * item)
{
    CCAssert(item != NULL, "Item is NULL");
    
    if(m_pSelectedItem != NULL)
        m_pSelectedItem->unselected();

    // set selected item
    item->selected();
    
    // set states appropriately
    _curHighlighted = item;
	m_pSelectedItem = item;
}
void CCRadioMenu::setSelectedRadioItemAt(int index)
{
    CCAssert(index + 1 < getChildrenCount(), "Access index out of bound.");
    CCAssert(index >= 0, "Index cannot be negative.");
    
    // get children array
    CCArray *children = this->getChildren();
    
    // get menuitem at index
    CCMenuItem *item = static_cast<CCMenuItem*>(children->objectAtIndex(index));
    CCAssert(item != NULL, "Item is null.");
    
    // unselected all items
    for (int i=0; i<children->count(); i++) {
        // get CCMenuItem
        CCMenuItem *item = static_cast<CCMenuItem*>(children->objectAtIndex(i));
        // unselected
        item->unselected();
    }
    
    // set selected item for specified index
    item->selected();
    
    // set states appropriately
    _curHighlighted = item;
    m_pSelectedItem = item;
}

void CCRadioMenu::clearSelectedForAllItems()
{
    // get children array
    CCArray *children = this->getChildren();
    
    // unselected all items
    for (int i=0; i<children->count(); i++) {
        // get CCMenuItem
        CCMenuItem *item = static_cast<CCMenuItem*>(children->objectAtIndex(i));
        // unselected
        item->unselected();
    }
    
    // set states appropriately as none is selected now
    _curHighlighted = NULL;
    m_pSelectedItem = NULL;
}

CCRadioMenu * CCRadioMenu::create(CCArray *items)
{
	CCRadioMenu *pRet = new CCRadioMenu();
	if (pRet && pRet->initWithArray(items))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCRadioMenu::ccTouchBegan(CCTouch *touch ,CCEvent *event) {
    
	if ( m_eState != kCCMenuStateWaiting ) return false;
    
	CCMenuItem *curSelection = itemForTouch(touch);
	if(!curSelection)return false;
	curSelection->selected();
	_curHighlighted = curSelection;
    
	if (_curHighlighted) {
		if (m_pSelectedItem != curSelection && m_pSelectedItem) {
			m_pSelectedItem->unselected();
		}
		m_eState = kCCMenuStateTrackingTouch;
		return true;
	}
	return false;
    
}

void CCRadioMenu::ccTouchEnded(CCTouch *touch ,CCEvent *event) {
	CCMenuItem *curSelection = itemForTouch(touch);
	if (curSelection != _curHighlighted && curSelection != NULL) {
		if(m_pSelectedItem) m_pSelectedItem->selected();
		_curHighlighted->unselected();
		_curHighlighted = NULL;
		m_eState = kCCMenuStateWaiting;
		return;
	}
    
	m_pSelectedItem = _curHighlighted;
	_curHighlighted -> activate();
	_curHighlighted = NULL;
    
	m_eState = kCCMenuStateWaiting;
    
}

void CCRadioMenu::ccTouchCancelled(CCTouch *touch ,CCEvent *event) {
	m_pSelectedItem->selected();
	_curHighlighted->unselected();
	_curHighlighted = NULL;
    
	m_eState = kCCMenuStateWaiting;
    
}

void CCRadioMenu::ccTouchMoved(CCTouch *touch, CCEvent * event)
{
	
	CCMenuItem *curSelection = itemForTouch(touch);
	if (curSelection != _curHighlighted && curSelection != NULL) {
		_curHighlighted->unselected();
		curSelection->selected();
		_curHighlighted = curSelection;
		return;
	}
    
}