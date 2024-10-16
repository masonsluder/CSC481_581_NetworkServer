#pragma once

#ifndef N_COMPONENT_H
#define N_COMPONENT_H

namespace N_Components {

	class N_Component {
	public:
		virtual ~N_Component() {}
		virtual void update(void) = 0;
	};

}

#endif
