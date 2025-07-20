#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
    #ifdef HOVER_API_EXPORTING
        #define HOVER_API_DLL __declspec(dllexport)
    #else
        #define HOVER_API_DLL __declspec(dllimport)
    #endif
#else
    #define HOVER_API_DLL __attribute__((visibility("default")))
#endif


class HOVER_API_DLL Hover : public CCNode {
public:
    struct HoverEntry {
        /// The function that gets called when the node is first hovered.
        /// The CCNode* passed to the function is the same node that was registered
        /// with Hover::watch(), allowing you to modify or interact with it directly.
        std::function<void(CCNode*)> onEnter;

        /// The function that gets called when the node is no longer hovered.
        /// The CCNode* passed to the function is the same node that was registered
        /// with Hover::watch(), allowing you to modify or interact with it directly.
        std::function<void(CCNode*)> onExit;
    };

    /// Get the Hover instance. The instance is destroyed once the parent node is destroyed.
    /// @param parent The parent of the tracker.
    /// @returns A Hover instance attached to the given parent node. If one already exists, it is returned; otherwise, a new instance is created.
    static Hover* get(CCNode* parent);

    /// Starts watching a node for hover events.
    /// @param node The node to watch.
    /// @param onEnter Optional callback when hovering starts.
    /// @param onExit Optional callback when hovering ends.
    void watch(CCNode* node, std::function<void(CCNode*)> onEnter = nullptr, std::function<void(CCNode*)> onExit = nullptr);

    /// Stops watching a node for hover events.
    /// @param node The node to stop watching.
    void forget(CCNode* node);

    /// Checks if a node is currently hovered.
    /// @param node The node to check.
    /// @returns Whether the node is currently hovered.
    bool isHovering(CCNode* node) const;

    /// Checks if a node is watched.
    /// @param node The node to check.
    /// @returns Whether the node is watched.
    bool isWatched(CCNode* node) const;

    /// Stops watching all nodes and clears hover states.
    void clear();

    /// @returns All currently watched nodes.
    const std::vector<CCNode*>& getWatchedNodes() const { return m_nodes; }

    /// @returns The HoverEntry (handlers) for a node, or nullptr if none registered.
    HoverEntry* getHoverHandlers(CCNode* node);

    /// Adds padding to the hover of a node, which extends the hitbox.
    /// @param node The node to add padding to.
    /// @param padding Amount of padding.
    void setHoverPadding(CCNode* node, float padding) { m_padding[node] = padding; }

    /// Offsets the hitbox of the hover of a node.
    /// @param node The node hover hitbox to offset.
    /// @param offset Offset amount.
    void setOffset(CCNode* node, CCPoint offset) { m_offset[node] = offset; }

    /// Gets the hover rect of a node.
    /// @param node The node to get the hover rect of.
    /// @returns The CCRect of the node hover.
    CCRect getHoverRect(CCNode* node) const;

    /// Sets whether nodes that are above the hover's parent node pause the hovering.
    /// For example, whether popups should pause hovers on the layer below until closed.
    bool m_respectZOrder = false;

    /// Draws the borders of hover elements.
    bool m_debugDrawEnabled = false;

    /// Color of the debug draw borders.
    ccColor4F m_debugDrawColor = { 0, 1, 1, 1 };

protected:
    void update(float dt);
    CCDrawNode* m_drawNode;

private:
    std::unordered_map<CCNode*, HoverEntry> m_handlers;
    std::vector<CCNode*> m_nodes;
    std::unordered_set<CCNode*> m_hovered;
    std::unordered_map<CCNode*, float> m_padding;
    std::unordered_map<CCNode*, CCPoint> m_offset;
};

