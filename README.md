# HoverAPI
An API for calling functions when starting or stopping to hover a node.
<cr>Intended for developers</c>, has no functionality on it's own.

---

## Usage
You can use this API as a dependency by declaring it in your ``mod.json`` file:
```
{
    "dependencies": [
		{
			"id": "natrium.hoverapi",
			"version": "1.0.0"
		}
    ]
}
```

Afterwards, include this header:
```
#include <natrium.hoverapi/include/Hover.hpp>
```

Check below for an example usage.

---

## Functions
```ccp
static Hover* get(CCNode* parent);
```
<c-EAB49A>Get a Hover instance attached to the given parent node. If one already exists, it is returned; otherwise, a new instance is created.</c> 

```ccp
void watch(CCNode* node, std::function<void(CCNode*)> onEnter = nullptr, std::function<void(CCNode*)> onExit = nullptr);
```
<c-EAB49A>Starts watching the given node for hover events. onEnter and onExit are optional callbacks for the respective event.</c> 

```ccp
void forget(CCNode* node);
```
<c-EAB49A>Stops watching the given node for hover events.</c> 

```ccp
void clear();
```
<c-EAB49A>Stops watching all nodes and clears hover states.</c> 

```ccp
bool isHovering(CCNode* node) const;
``` 
<c-EAB49A>Returns whether a given node is currently hovered.</c> 

```ccp
bool isWatched(CCNode* node) const;
``` 
<c-EAB49A>Returns whether a given node is currently watched.</c> 

```ccp
const std::vector<CCNode*>& getWatchedNodes() const;
```  
<c-EAB49A>Returns all currently watched nodes.</c> 

```ccp
HoverEntry* getHoverHandlers(CCNode* node);
``` 
<c-EAB49A>Returns the HoverEntry (handlers) for a given node, or nullptr if none registered.</c> 

```ccp
CCRect getHoverRect(CCNode* node) const;
``` 
<c-EAB49A>Returns the CCRect of a given node hover.</c> 

```ccp
void setHoverPadding(CCNode* node, float padding);
```  
<c-EAB49A>Adds padding to the hover of a given node to extend the hitbox.</c> 

```ccp
void setOffset(CCNode* node, CCPoint offset);
```  
<c-EAB49A>Offsets the hover hitbox of a given node.</c> 

---

## Fields
```ccp
bool m_respectZOrder;  
```  
<c-EAB49A>Sets whether nodes that are above the hover's parent node pause the hovering. For example, whether popups should pause hovers on the layer below until closed. False by default.</c> 

```ccp
bool m_debugDrawEnabled;
```   
<c-EAB49A>Draws borders around the hovers. False by default.</c> 

```ccp
ccColor4F m_debugDrawColor;
```  
<c-EAB49A>Color of the debug draw borders. Cyan ({ 0, 1, 1, 1}) by default.</c> 

---

## Usage Example
This example makes the profile button turn green and scale by 1.25 on hover start, and revert on hover end.
```ccp
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>

// Dependency for the HoverAPI
#include <natrium.hoverapi/include/Hover.hpp>

class $modify(MenuLayer) {
  bool init() {
    if (!MenuLayer::init()) {
	  return false;
    }

    // Get the hover instance
    auto hover = Hover::get(this);

    // Start watching the profile button
    hover->watch(m_profileButton,
	  [](CCNode* n) {

		// This gets called onEnter
		auto spr = n->getChildByType<CCSprite>(0);
		spr->setColor(ccc3(0, 255, 0));
		spr->setScale(1.25f);
	  },
	  [](CCNode* n) {

		// This gets called onExit
		auto spr = n->getChildByType<CCSprite>(0);
		spr->setColor(ccc3(255, 255, 255));
		spr->setScale(1.f);
	  });

    return true;
  }
};

```