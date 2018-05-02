#include <client/component/ComponentKeyInput.hpp>

namespace client
{

ComponentKeyInput::ComponentKeyInput(){

}
ComponentKeyInput::Action::Action(unsigned int beat, float delta,
                                  bool outside_window)
	: beat(beat), delta(delta), outside_window(outside_window) {
	// no-op
}
}
