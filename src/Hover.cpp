#include "Hover.hpp"

Hover* Hover::get(CCNode* parent) {
    static Hover* instance = nullptr;

    if (!instance || !instance->getParent()) {
        instance = new Hover();
        instance->init();
        instance->schedule(schedule_selector(Hover::update));
        instance->m_drawNode = CCDrawNode::create();
        instance->addChild(instance->m_drawNode);

        parent->addChild(instance);
    }

    return instance;
}

void Hover::update(float) {
    auto mouse = getMousePos();
    m_drawNode->clear();

    bool blocked = false;

    if (m_respectZOrder) {
        auto parent = this->getParent();
        auto parentZOrder = parent->getZOrder();

        auto grandparent = parent->getParent();
        auto children = grandparent ? grandparent->getChildren() : CCDirector::sharedDirector()->getRunningScene()->getChildren();
        CCObject* child = nullptr;

        CCARRAY_FOREACH(children, child) {
            auto node = static_cast<CCNode*>(child);
            int zOrder = node->getZOrder();
            if (zOrder > parentZOrder) blocked = true;
        }
    }

    for (auto* node : m_nodes) {
        if (!node || !node->getParent() || !node->isRunning()) continue;

        if (blocked) {
            auto it = m_handlers.find(node);
            if (it != m_handlers.end() && it->second.onExit) it->second.onExit(node);
            continue;
        }

        auto padding = 0.f;
        auto itPad = m_padding.find(node);
        if (itPad != m_padding.end()) padding = itPad->second;

        auto offset = ccp(0, 0);
        auto itOff = m_offset.find(node);
        if (itOff != m_offset.end()) offset = itOff->second;

        auto worldPos = node->getParent()->convertToWorldSpace(node->getPosition());
        auto size = node->getContentSize();
        auto anchor = node->getAnchorPoint();

        cocos2d::CCRect rect = cocos2d::CCRectMake(
            worldPos.x - size.width * anchor.x - padding + offset.x,
            worldPos.y - size.height * anchor.y - padding + offset.y,
            size.width + padding * 2, 
            size.height + padding * 2
        );

        if (m_debugDrawEnabled) {
            float minX = worldPos.x - size.width * anchor.x - padding + offset.x;
            float minY = worldPos.y - size.height * anchor.y - padding + offset.y;
            float maxX = minX + size.width + padding * 2;
            float maxY = minY + size.height + padding * 2;

            std::array<cocos2d::CCPoint, 4> points = {
                cocos2d::CCPoint(minX, minY),
                cocos2d::CCPoint(maxX, minY),
                cocos2d::CCPoint(maxX, maxY),
                cocos2d::CCPoint(minX, maxY)
            };

            m_drawNode->drawPolygon(points.data(), points.size(), { 0, 0, 0, 0 }, 0.4, m_debugDrawColor);

            // Cant use because of iOS and MacOS failing?
            // m_drawNode->drawRect(rect, { 0, 0, 0, 0 }, 0.4, m_debugDrawColor);
        }

        bool isHovering = rect.containsPoint(mouse);
        bool wasHovering = m_hovered.count(node) > 0;

        if (isHovering && !wasHovering) {
            m_hovered.insert(node);
            if (m_handlers.count(node) && m_handlers[node].onEnter) m_handlers[node].onEnter(node);
        }
        else if (wasHovering && !isHovering) {
            m_hovered.erase(node);
            if (m_handlers.count(node) && m_handlers[node].onExit) m_handlers[node].onExit(node);
        }
    }
}

void Hover::watch(CCNode* node, std::function<void(CCNode*)> onEnter, std::function<void(CCNode*)> onExit) {
    m_nodes.push_back(node);
    m_handlers[node] = HoverEntry{ onEnter, onExit };
}

void Hover::forget(CCNode* node) {
    m_nodes.erase(std::remove(m_nodes.begin(), m_nodes.end(), node), m_nodes.end());
    m_hovered.erase(node);
    m_handlers.erase(node);
}

bool Hover::isHovering(CCNode* node) const {
    return m_hovered.count(node) > 0;
}

bool Hover::isWatched(CCNode* node) const {
    if (!node) return false;
    auto it = std::find(m_nodes.begin(), m_nodes.end(), node);
    return it != m_nodes.end();
}

void Hover::clear() {
    m_nodes.clear();
    m_hovered.clear();
    m_handlers.clear();
}

Hover::HoverEntry* Hover::getHoverHandlers(CCNode* node) {
    auto it = m_handlers.find(node);
    if (it != m_handlers.end()) {
        return &it->second;
    }
    return nullptr;
}

CCRect Hover::getHoverRect(CCNode* node) const {
    if (!node || !node->getParent()) return CCRectZero;

    auto worldPos = node->getParent()->convertToWorldSpace(node->getPosition());
    auto size = node->getContentSize();
    auto anchor = node->getAnchorPoint();

    auto padding = 0.f;
    auto itPad = m_padding.find(node);
    if (itPad != m_padding.end()) padding = itPad->second;

    auto offset = CCPointZero;
    auto itOff = m_offset.find(node);
    if (itOff != m_offset.end()) offset = itOff->second;

    return CCRectMake(
        worldPos.x - size.width * anchor.x - padding + offset.x,
        worldPos.y - size.height * anchor.y - padding + offset.y,
        size.width + padding * 2,
        size.height + padding * 2
    );
}
