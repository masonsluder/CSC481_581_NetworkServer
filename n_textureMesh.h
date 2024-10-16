#pragma once

#ifndef TEXTUREMESH_H
#define TEXTUREMESH_H

#include "n_component.h"

#include <iostream>

namespace N_Components {

	class N_TextureMesh : public virtual N_Component {
	protected:
		/** The filepath to the texture to be used */
		std::string m_textureFilePath;
		/** whether or not this texture is visible */
		bool m_isVisible;
	public:
		
		/** 
		 * TextureMesh constructor. Constructs a textureMesh component 
		 * with the given textureFilePath as its default.
		 * @param textureFilePath to set during construction
		 */
		N_TextureMesh(std::string textureFilePath);

		void update() override;

		/**
		 * Returns the textureMesh's file path.
		 * @return textureFilePath
		 */
		std::string getTextureFilePath();

		/**
		 * Sets the textureFilePath to the given string.
		 * @param textureFilePath
		 */
		void setTextureFilePath(std::string textureFilePath);

		/**
		 * Gets the textureMesh's isVisible value.
		 * @return true if the textureMesh is visible
		 */
		bool getIsVisible();

		/**
		 * Sets the textureMesh's visibility to the given boolean value.
		 * @param isVisible
		 */
		void setIsVisible(bool isVisible);

	};
}

#endif