#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

namespace N_Components {

	class N_Component {
	public:
		virtual ~N_Component() {}
		virtual void update(void) = 0;
	};

}

#endif
