#include <client/component/ComponentRenderButtonGroup.hpp>

namespace client
{
ComponentRenderButtonGroup::ComponentRenderButtonGroup() {
	setup = false;
}

ComponentRenderButtonGroup::~ComponentRenderButtonGroup()
{
	for (buttonRender b : buttonsRender) {
		b.button_housing->remove();
		b.button->remove();
	}
}

}  // namespace client
