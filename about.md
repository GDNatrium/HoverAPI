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
<cl>static</c> <cg>Hover</c>* <cy>get</c>(<cg>CCNode</c>* <co>parent</c>);  
<c-EAB49A>Get a Hover instance attached to the given parent node. If one already exists, it is returned; otherwise, a new instance is created.</c> 

<cl>void</c> <cy>watch</c>(<cg>CCNode</c>\* <co>node</c>, <cg>std::function</c><<cl>void</c>(<cg>CCNode</c>\*)> <co>onEnter</c>, <cg>std::function</c><<cl>void</c>(<cg>CCNode</c>\*)> <co>onExit</c>);  
<c-EAB49A>Starts watching the given node for hover events. onEnter and onExit are optional callbacks for the respective event.</c> 

<cl>void</c> <cy>forget</c>(<cg>CCNode</c>\* <co>node</c>);  
<c-EAB49A>Stops watching the given node for hover events.</c> 

<cl>void</c> <cy>clear</c>();  
<c-EAB49A>Stops watching all nodes and clears hover states.</c> 

<cl>bool</c> <cy>isHovering</c>(<cg>CCNode</c>\* <co>node</c>) <cl>const</c>;  
<c-EAB49A>Returns whether a given node is currently hovered.</c> 

<cl>bool</c> <cy>isWatched</c>(<cg>CCNode</c>\* <co>node</c>) <cl>const</c>;  
<c-EAB49A>Returns whether a given node is currently watched.</c> 

<cl>const</c> <cg>std::vector</c><<cg>CCNode</c>*>& <cy>getWatchedNodes</c>() <cl>const</c>;  
<c-EAB49A>Returns all currently watched nodes.</c> 

<cg>HoverEntry</c>* <cy>getHoverHandlers</c>(<cg>CCNode</c>\* <co>node</c>);  
<c-EAB49A>Returns the HoverEntry (handlers) for a given node, or nullptr if none registered.</c> 

<cg>CCRect</c> <cy>getHoverRect</c>(<cg>CCNode</c>\* <co>node</c>) <cl>const</c>;  
<c-EAB49A>Returns the CCRect of a given node hover.</c> 

<cl>void</c> <cy>setHoverPadding</c>(<cg>CCNode</c>\* <co>node</c>, <cl>float</c> <co>padding</c>);  
<c-EAB49A>Adds padding to the hover of a given node to extend the hitbox.</c> 

<cl>void</c> <cy>setOffset</c>(<cg>CCNode</c>\* <co>node</c>, <cg>CCPoint</c> <co>offset</c>);  
<c-EAB49A>Offsets the hover hitbox of a given node.</c> 

---

## Fields
<cl>bool</c> m_respectZOrder;  
<c-EAB49A>Sets whether nodes that are above the hover's parent node pause the hovering. For example, whether popups should pause hovers on the layer below until closed. False by default.</c> 

<cl>bool</c> m_debugDrawEnabled;  
<c-EAB49A>Draws borders around the hovers. False by default.</c> 

<cg>ccColor4F</c> m_debugDrawColor;  
<c-EAB49A>Color of the debug draw borders. Cyan ({ 0, 1, 1, 1}) by default.</c> 

---

## Usage Example
This example makes the profile button turn green and scale by 1.25 on hover start, and revert on hover end.
```
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