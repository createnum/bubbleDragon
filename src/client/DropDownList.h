// 点击，弹出，选择，关闭。

#ifndef DROPDOWNLIST_H_INCLUDED
#define DROPDOWNLIST_H_INCLUDED

#include "cocos2d.h"

namespace azl
{

#define DROPDOWNLIST_NORMAL_COLOR       cocos2d::ccc4(128, 128, 128, 255)
#define DROPDOWNLIST_SELECTED_COLOR     cocos2d::ccc4(200, 200, 200, 255)
#define DROPDOWNLIST_HIGHLIGHT_COLOR    cocos2d::ccc4(60, 60, 200, 255)

#define DROPDOWNLIST_NORMAL_COLOR3       cocos2d::ccc3(128, 128, 128)
#define DROPDOWNLIST_SELECTED_COLOR3     cocos2d::ccc3(200, 200, 200)
#define DROPDOWNLIST_HIGHLIGHT_COLOR3    cocos2d::ccc3(60, 60, 200)

class DropDownList : public cocos2d::CCLayer
{
public:
    class TouchHelper : public cocos2d::CCObject, public cocos2d::CCTouchDelegate
    {
    protected:
        TouchHelper(DropDownList& list)
            : list_(list)
        {
            cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, cocos2d::kCCMenuHandlerPriority, true);
        }

    public:
        static TouchHelper* create(DropDownList& list)
        {
            return new TouchHelper(list);
        }

        void destroy()
        {
            cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
            autorelease();
        }

        virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
        {
            list_.onClose();
            return true;
        }

        DropDownList& list_;
    };

protected:
    DropDownList(cocos2d::CCLabelTTF* label, cocos2d::CCSize size)
        : label_(label)
        , show_(false)
        , lastSelectedIndex_(0)
        , helper_(NULL)
        , item_(NULL)
    {
        menu_ = cocos2d::CCMenu::create();
        menu_->setPosition(cocos2d::CCPoint(size.width / 2, size.height / 2));
        menu_->setVertexZ(0.1f);
        menu_->retain();

        label_->setPosition(cocos2d::CCPoint(size.width / 2, size.height / 2));
        addChild(label_);

        setContentSize(size);
    }

public:
    ~DropDownList()
    {
        menu_->release();
    }

    static DropDownList* create(cocos2d::CCLabelTTF* label, cocos2d::CCSize size)
    {
        DropDownList* list = new DropDownList(label, size);
        list->autorelease();
        return list;
    }

    std::string getString()
    {
        return label_->getString();
    }

    int getSelectedIndex()
    {
        return lastSelectedIndex_;
    }

    void setSelectedIndex(int index)
    {
        lastSelectedIndex_ = index;

        for (int i = 0, j = (int)labels_.size(); i < j; ++i)
        {
            if (i == lastSelectedIndex_)
            {
                layers_[i]->setColor(DROPDOWNLIST_HIGHLIGHT_COLOR3);
                label_->setString(labels_[i]->getString());
            }
            else
            {
                layers_[i]->setColor(DROPDOWNLIST_NORMAL_COLOR3);
            }
        }
    }

    void setDropMenuItem(cocos2d::CCMenuItem* item)
    {
        CC_ASSERT(item_ == NULL);
        item_ = item;

        addChild(item_);

        cocos2d::CCSize size = getContentSize();
        item_->setAnchorPoint(cocos2d::CCPoint(1.0f, 0.5f));
        item_->setPosition(size.width, size.height / 2.0f);

        label_->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.5f));
        label_->setPosition(cocos2d::CCPoint(4, size.height / 2.0f));
    }

public:
    void onEnter()
    {
        setTouchEnabled(true);
        cocos2d::CCLayer::onEnter();
    }

    void registerWithTouchDispatcher()
    {
        cocos2d::CCDirector* pDirector = cocos2d::CCDirector::sharedDirector();
        pDirector->getTouchDispatcher()->addTargetedDelegate(this, cocos2d::kCCMenuHandlerPriority, true);
    }

    virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
    {
        if (!show_)
        {
            cocos2d::CCRect rect;
            rect.origin = cocos2d::CCPointZero;
            rect.size = getContentSize();
            cocos2d::CCPoint position = convertTouchToNodeSpace(touch);

            if (rect.containsPoint(position))
            {
                show_ = true;
                helper_ = TouchHelper::create(*this);
                addChild(menu_);

                for (int i = 0, j = (int)labels_.size(); i < j; ++i)
                {
                    if (i == lastSelectedIndex_)
                    {
                        layers_[i]->setColor(DROPDOWNLIST_HIGHLIGHT_COLOR3);
                    }
                    else
                    {
                        layers_[i]->setColor(DROPDOWNLIST_NORMAL_COLOR3);
                    }
                }

                if (item_)
                {
                    item_->selected();
                }

                return true;
            }
        }
       
        return false;
    }

    void addLabel(cocos2d::CCLabelTTF* label)
    {
        cocos2d::CCSize size = getContentSize();

        cocos2d::CCLayerColor* normal   = cocos2d::CCLayerColor::create(DROPDOWNLIST_NORMAL_COLOR, size.width, size.height);
        cocos2d::CCLayerColor* selected = cocos2d::CCLayerColor::create(DROPDOWNLIST_SELECTED_COLOR, size.width, size.height);

        layers_.push_back(normal);
        labels_.push_back(label);
        
        cocos2d::CCMenuItem* item = cocos2d::CCMenuItemSprite::create(
                normal, 
                selected, 
                NULL,
                this,
                cocos2d::SEL_MenuHandler(&DropDownList::onSelected)
            );

        label->setPosition(cocos2d::CCPoint(size.width / 2, size.height / 2));
        item->addChild(label);
        item->setTag((int)labels_.size() - 1);
        item->setPosition(0, - (int)labels_.size() * size.height);

        menu_->addChild(item);
    }

    void onSelected(cocos2d::CCObject* sender)
    {
        cocos2d::CCMenuItem* item = dynamic_cast<cocos2d::CCMenuItem*>(sender);

        if (item)
        {
            lastSelectedIndex_ = item->getTag();
            label_->setString(labels_[item->getTag()]->getString());
        }

        onClose();
    }

    void onClose()
    {
        removeChild(menu_, true);
        
        helper_->destroy();
        helper_ = NULL;

        show_ = false;

        if (item_)
        {
            item_->unselected();
        }
    }

private:
    cocos2d::CCMenu* menu_;
    cocos2d::CCLabelTTF* label_;
    std::vector<cocos2d::CCLabelTTF*> labels_;
    std::vector<cocos2d::CCLayerColor*> layers_;
    bool show_;
    int lastSelectedIndex_;
    TouchHelper* helper_;
    cocos2d::CCMenuItem* item_;
};

}

#endif