#include <client/component/ComponentKeyInput.hpp>

namespace client
{

ComponentKeyInput::ComponentKeyInput(){

}
ComponentKeyInput::Action::Action(unsigned int beat, float delta)
	: beat(beat), delta(delta) {
	// no-op
}
}
