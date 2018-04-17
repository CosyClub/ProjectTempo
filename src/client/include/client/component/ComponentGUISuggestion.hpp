#ifndef CLIENT_COMPONENT_GUI_SUGGESTION_HPP
#define CLIENT_COMPONENT_GUI_SUGGESTION_HPP

#include <anax/Component.hpp>

namespace client
{

// This component keeps track if the local player broke the combo by performing
// an action outside the current beat
struct ComponentGUISuggestion : public anax::Component {
	bool brokeCombo;
};

}  // namespace client

#endif
